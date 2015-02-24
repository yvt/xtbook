//
//  XTBRaxDecompressor.cpp
//  XTBook
//
//  Created by Kawada Tomoaki on 8/2/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "XTBRaxDecompressor.h"
#include "XTBException.h"
#include "XZEmbedded/xz.h"

static const uint64_t g_rawFlag=0x8000000000000000ULL;

static bool g_xzInited=false;

static void initXZ(){
	if(g_xzInited)
		return;
	xz_crc32_init();
	g_xzInited=true;
}

XTBRaxDecompressor::XTBRaxDecompressor(XTBStream *stream,
									   bool shouldClose){
	if(!stream){
		XTBThrowException(L"XTBArgumentNullException", NULL);
	}
	if(!stream->isSeekable()){
		XTBThrowException(L"XTBStreamNotSupportedException", NULL);
	}
	if(!stream->isReadable()){
		XTBThrowException(L"XTBStreamNotSupportedException", NULL);
	}
	m_baseStream=stream;
	
	m_baseOffset=stream->position();
	
	uint8_t magic[4];
	if(stream->readToBuffer(magic, 4)<4){
		stream->seekToOffset(m_baseOffset);
		XTBThrowException(L"XTBInvalidFormatException", NULL);
	}
	
	if(magic[0]!=0x08 || magic[1]!=0xde || magic[2]!=0x0f || magic[3]!=0x42){
		stream->seekToOffset(m_baseOffset);
		XTBThrowException(L"XTBInvalidFormatException", NULL);
	}
	
	m_blockSizeShift=stream->readBEInt32();
	
	if(m_blockSizeShift>22){
		// too big block!
		stream->seekToOffset(m_baseOffset);
		XTBThrowException(L"XTBRaxCorruptedException", NULL);
	}
	
	m_blockSize=1<<m_blockSizeShift;
	
	m_originalSize=((XTBOffset)stream->readBEInt32())<<32;
	m_originalSize|=((XTBOffset)stream->readBEInt32());
	
	m_indexOffset=((XTBOffset)stream->readBEInt32())<<32;
	m_indexOffset|=((XTBOffset)stream->readBEInt32());
	
	m_blockCount=(m_originalSize+m_blockSize-1)>>m_blockSizeShift;
	
	m_currentBlock=new uint8_t[(size_t)m_blockSize];
	m_currentBlockId=(XTBOffset)-1;
	
	m_inputBuffer=new uint8_t[(size_t)m_blockSize];
	
	initXZ();
	m_decoder=xz_dec_init(XZ_SINGLE, 0);
	
	m_position=0;
}

XTBRaxDecompressor::~XTBRaxDecompressor(){
	delete[] m_currentBlock;
	delete[] m_inputBuffer;
	if(m_shouldClose)
		delete m_baseStream;
	xz_dec_end(m_decoder);
}

XTBRaxDecompressor::BlockOffset XTBRaxDecompressor::offsetForBlockId(XTBOffset blockId){
	XTBOffset blockIndexOffset=m_indexOffset+8*blockId;
	blockIndexOffset+=m_baseOffset;
	
	m_baseStream->seekToOffset(blockIndexOffset);
	
	XTBOffset blockOffset, nextBlockOffset;
	
	blockOffset=((XTBOffset)m_baseStream->readBEInt32())<<32;
	blockOffset|=((XTBOffset)m_baseStream->readBEInt32());
	
	if(blockId==m_blockCount-1){
		// last block.
		nextBlockOffset=m_indexOffset;
	}else{
		nextBlockOffset=((XTBOffset)m_baseStream->readBEInt32())<<32;
		nextBlockOffset|=((XTBOffset)m_baseStream->readBEInt32());
	}
	
	nextBlockOffset&=~ g_rawFlag;
	
	BlockOffset finalOffset;
	
	if(blockOffset&g_rawFlag){
		blockOffset&=~ g_rawFlag;
		finalOffset.raw=true;
	}else{
		finalOffset.raw=false;
	}
	
	finalOffset.position=blockOffset+m_baseOffset;
	finalOffset.size=nextBlockOffset-blockOffset;
	
	return finalOffset;
}
XTBOffset XTBRaxDecompressor::sizeOfBlockId(XTBOffset blockId){
	if(blockId==m_blockCount-1){
		return m_originalSize&(m_blockSize-1);
	}else{
		return m_blockSize;
	}
}

void XTBRaxDecompressor::setCurrentBlockId(XTBOffset blockId){
	if(m_currentBlockId==blockId)
		return;
	
	BlockOffset offset=offsetForBlockId(blockId);
	
	m_baseStream->seekToOffset(offset.position);
	
	if(offset.raw){
		// not compressed.
		size_t siz=(size_t)sizeOfBlockId(blockId);
		
		// this block is raw, which means its as many as the original.
		if(siz!=(size_t)offset.size)
			XTBThrowException(L"XTBRaxCorruptedException", NULL);
		
		// read to buffer. (raw)
		if(m_baseStream->readToBuffer(m_currentBlock, siz)<siz){
			m_currentBlockId=(XTBOffset)-1;
			XTBThrowException(L"XTBRaxCorruptedException", NULL);
		}
	}else{
		// compressed.
		size_t inSize=(size_t)offset.size;
		
		// read to compressed buffer.
		if(m_baseStream->readToBuffer(m_inputBuffer, inSize)<inSize){
			XTBThrowException(L"XTBRaxCorruptedException", NULL);
		}
		
		xz_buf xzbuf;
		xzbuf.in=m_inputBuffer;
		xzbuf.in_pos=0;
		xzbuf.in_size=inSize;
		xzbuf.out=m_currentBlock;
		xzbuf.out_pos=0;
		xzbuf.out_size=(size_t)sizeOfBlockId(blockId);
		
		xz_ret result;
		result=xz_dec_run(m_decoder, &xzbuf);
		if(result!=XZ_OK && result!=XZ_STREAM_END){
			// decoder error!
			m_currentBlockId=(XTBOffset)-1;
			XTBThrowException(L"XTBRaxCorruptedException", NULL);
		}
		
		if(xzbuf.out_pos!=xzbuf.out_size){
			// data not enough!
			m_currentBlockId=(XTBOffset)-1;
			XTBThrowException(L"XTBRaxCorruptedException", NULL);
		}
		
	}
	
	m_currentBlockId=blockId;
	return;
}

size_t XTBRaxDecompressor::readToBuffer(void *buf, size_t maxLength){
	uint8_t *outBuf=(uint8_t *)buf;
	
	// beyond EOF?
	if(m_position>=m_originalSize)
		return 0;
	
	// EOF culling!
	if(m_position+maxLength>m_originalSize)
		maxLength=m_originalSize-m_position;
	
	size_t readingLength=maxLength;
	
	while(maxLength>0){
		setCurrentBlockId(m_position>>m_blockSizeShift);
		
		size_t blockRemaining=m_blockSize;
		blockRemaining-=m_position&(m_blockSize-1);
		
		size_t readSize=std::min(blockRemaining, maxLength);
		size_t readPos=m_position&(m_blockSize-1);
		
		memcpy(outBuf, m_currentBlock+readPos, readSize);
		
		m_position+=readSize;
		maxLength-=readSize;
		outBuf+=readSize;
	}
	
	return readingLength;
}



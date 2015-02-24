//
//  XTBRaxDecompressor.h
//  XTBook
//
//  Created by Kawada Tomoaki on 8/2/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#pragma once

#include "XTBStream.h"

struct xz_dec;

class XTBRaxDecompressor: public XTBStream{
	
	struct BlockOffset{
		XTBOffset position;
		XTBOffset size;
		bool raw;
	};
	
	XTBStream *m_baseStream;
	bool m_shouldClose;
	XTBOffset m_baseOffset;
	unsigned int m_blockSizeShift;
	XTBOffset m_blockSize;
	XTBOffset m_originalSize;
	XTBOffset m_blockCount;
	XTBOffset m_indexOffset;
	
	uint8_t *m_inputBuffer;
	uint8_t *m_currentBlock;
	XTBOffset m_currentBlockId;
	
	xz_dec *m_decoder;
	
	XTBOffset m_position;
	
	XTBOffset sizeOfBlockId(XTBOffset);
	BlockOffset offsetForBlockId(XTBOffset);
	
	void setCurrentBlockId(XTBOffset);
	
public:
	XTBRaxDecompressor(XTBStream *, bool shouldClose);
	virtual ~XTBRaxDecompressor();
	
	virtual size_t readToBuffer(void *, size_t maxLength);
	
	
	virtual void seekToOffset(XTBOffset p){
		m_position=p;
	}
	virtual void seekToEndOfStream(){
		m_position=m_originalSize;
	}
	
	virtual XTBOffset position() const{
		return m_position;
	}
	
	virtual bool isReadable() const{return true;}
	virtual bool isWritable() const{return false;}
	virtual bool isSeekable() const{return true;}
	
	virtual void synchronize(){}
};

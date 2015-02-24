//
//  XTBIndexDBReader.cpp
//  XTBook
//
//  Created by Kawada Tomoaki on 7/28/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "XTBIndexDBReader.h"
#include <tcw/twLock.h>
#include <tcw/twSDLSemaphore.h>
#include "XTBFileStream.h"
#include <assert.h>
#include "XTBException.h"

XTBIndexDBReader::XTBIndexDBReader(const XTBSysString& path):
m_path(path),
m_dbStream(NULL),
m_mapStream(NULL){
	try{
		m_dbStream=XTBFileStream::streamForReadingAtPath(pathForDb());
		m_mapStream=XTBFileStream::streamForReadingAtPath(pathForMap());
	}catch(...){
		if(m_dbStream)
			delete m_dbStream;
		if(m_mapStream)
			delete m_mapStream;
		throw;
	}
	
	m_dbSemaphore=new twSDLSemaphore();
	m_mapSemaphore=new twSDLSemaphore();
	
	m_entryCount=m_mapStream->readBEInt32();

	if(m_entryCount==0){
		XTBThrowException(L"XTBInvalidFormatException", NULL);
	}
}

XTBIndexDBReader::~XTBIndexDBReader(){
	// make sure I/O is done
	m_dbSemaphore->lock();
	m_mapSemaphore->lock();
	
	delete m_dbSemaphore;
	delete m_mapSemaphore;
	
	delete m_dbStream;
	delete m_mapStream;
}

XTBSysString XTBIndexDBReader::pathForDb(){
	return m_path+XTBSysText(".indexdb");
}

XTBSysString XTBIndexDBReader::pathForMap(){
	return m_path+XTBSysText(".indexmap");
}

XTBOffset XTBIndexDBReader::offsetForIndex(unsigned int index){
	twLock lock(m_mapSemaphore);
	m_mapStream->seekToOffset(4+index*4);
	return (XTBOffset)m_mapStream->readBEInt32();
}

XTBIndexDBEntry XTBIndexDBReader::entryAtIndex(unsigned int index){
	twLock lock(m_dbSemaphore);
	
	// seek to entry.
	m_dbStream->seekToOffset(offsetForIndex(index));
	
	// read sizes.
	std::string sizes=m_dbStream->readToString(2);
	size_t keyLength=(unsigned char)sizes[0];
	size_t titleLength=(unsigned char)sizes[1];
	
	// read both.
	std::string combined=m_dbStream->readToString(keyLength+titleLength);
	
	// expand.
	XTBIndexDBEntry entry;
	
	if(titleLength){
		// non-optimized.
		entry.key=combined.substr(0, keyLength);
		entry.title=combined.substr(keyLength);
	}else{
		// optimized
		entry.key.swap(combined);
		entry.title=entry.key;
	}
	
	return entry;
}

void XTBIndexDBReader::readKeyAtIndex(unsigned int index, 
									  char *buffer, 
									  size_t maxLength){
	twLock lock(m_dbSemaphore);
	
	// seek to entry.
	m_dbStream->seekToOffset(offsetForIndex(index));
	
	// read sizes.
	unsigned char sizes[2];
	m_dbStream->readToBuffer(sizes, 2);
	
	size_t keyLength=sizes[0];
	
	if(keyLength<maxLength){
		buffer[keyLength]=0;
	}else{
		keyLength=maxLength;
	}
	
	// read key.
	m_dbStream->readToBuffer(buffer, keyLength);
	
}

class XTBIndexDBBinarySearchBuffer{
	char *m_buffer;
	unsigned int m_index;
	XTBIndexDBReader *m_reader;
	bool m_valid;
	size_t m_maxLength;
	void readBuffer(){
		if(m_valid)
			return;
		m_reader->readKeyAtIndex(m_index, m_buffer, m_maxLength);
		m_valid=true;
	}
public:
	XTBIndexDBBinarySearchBuffer(size_t size, XTBIndexDBReader *r){
		m_buffer=new char[size];
		m_reader=r;
		m_valid=false;
		m_maxLength=size;
	}
	void setIndex(unsigned int i){
		if(i==m_index)
			return;
		m_index=i;
		m_valid=false;
	}
	unsigned int index() const{return m_index;}
	char *buffer(){
		readBuffer();
		return m_buffer;
	}
	~XTBIndexDBBinarySearchBuffer(){
		delete[] m_buffer;
	}
private:
	void operator =(const XTBIndexDBBinarySearchBuffer&){}
};

unsigned int XTBIndexDBReader::lowerBoundForKey(const std::string &key){
	if(key.empty())
		return 0;
	XTBIndexDBBinarySearchBuffer buf1(key.size(), this);
	XTBIndexDBBinarySearchBuffer buf2(key.size(), this);
	XTBIndexDBBinarySearchBuffer buf3(key.size(), this);
	
	buf1.setIndex(0);
	buf2.setIndex(m_entryCount-1);
	
	while(buf1.index()<buf2.index()){
		buf3.setIndex((buf1.index()+buf2.index())>>1);
		if(strncmp(buf3.buffer(), key.c_str(), key.size())<0){
			buf1.setIndex(buf3.index()+1);
		}else{
			buf2.setIndex(buf3.index());
		}
	}
	
	return buf1.index();
}

unsigned int XTBIndexDBReader::upperBoundForKey(const std::string &key){
	if(key.empty())
		return 0;
	XTBIndexDBBinarySearchBuffer buf1(key.size(), this);
	XTBIndexDBBinarySearchBuffer buf2(key.size(), this);
	XTBIndexDBBinarySearchBuffer buf3(key.size(), this);
	
	buf1.setIndex(0);
	buf2.setIndex(m_entryCount-1);
	
	while(buf1.index()<buf2.index()){
		buf3.setIndex((buf1.index()+buf2.index())>>1);
		if(strncmp(buf3.buffer(), key.c_str(), key.size())<=0){
			buf1.setIndex(buf3.index()+1);
		}else{
			buf2.setIndex(buf3.index());
		}
	}
	
	return buf1.index();
}





//
//  XTBDicDBReader.cpp
//  XTBook
//
//  Created by Kawada Tomoaki on 7/6/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "XTBDicDBReader.h"
#include <tcw/twLock.h>
#include <tcw/twSDLSemaphore.h>
#include "XTBFileStream.h"
#include <assert.h>
#include "XTBException.h"

XTBDicDBReader::XTBDicDBReader(const XTBSysString& path):
m_contentsStream(NULL),
m_keysStream(NULL),
m_keyIndexStream(NULL),
m_path(path){
	
	try{
		m_contentsStream=XTBFileStream::streamForReadingAtPath(pathForContents(), true);
		m_keysStream=XTBFileStream::streamForReadingAtPath(pathForKeys());
		m_keyIndexStream=XTBFileStream::streamForReadingAtPath(pathForKeyIndex());
	}catch(...){
		if(m_contentsStream)
			delete m_contentsStream;
		if(m_keysStream)
			delete m_keysStream;
		if(m_keyIndexStream)
			delete m_keyIndexStream;
		throw;
	}
	
	m_contentsMutex=new twSDLSemaphore();
	m_keysMutex=new twSDLSemaphore();
	m_keyIndexMutex=new twSDLSemaphore();
	
	m_keyCount=m_keyIndexStream->readBEInt32();
	
	if(m_keyCount==0){
		XTBThrowException(L"XTBInvalidFormatException", NULL);
	}
}

XTBSysString XTBDicDBReader::pathForContents() const{
	return m_path+XTBSysText(".db");
}
XTBSysString XTBDicDBReader::pathForKeys() const{
	return m_path+XTBSysText(".keys");
}
XTBSysString XTBDicDBReader::pathForKeyIndex() const{
	return m_path+XTBSysText(".indices");
}


XTBDicDBReader::~XTBDicDBReader(){
	m_contentsMutex->wait();
	m_keysMutex->wait();
	m_keyIndexMutex->wait();
	delete m_contentsStream;
	delete m_keysStream;
	delete m_keyIndexStream;
	delete m_contentsMutex;
	delete m_keysMutex;
	delete m_keyIndexMutex;
}

std::string XTBDicDBReader::contentsAtPointer(XTBDicDBPointer ptr,
											  uint32_t maxLength) const{
	twLock lock(m_contentsMutex);
	
	m_contentsStream->seekToOffset((XTBOffset)ptr);
	
	uint32_t length;
	length=m_contentsStream->readBEInt32();
	
	if(length>maxLength)
		length=maxLength;
	
	return m_contentsStream->readToString(length);
}

XTBDicDBReaderKey XTBDicDBReader::keyAtIndex(unsigned int index) const{
	
	assert(index<m_keyCount);
	
	twLock lock1(m_keysMutex);
	twLock lock2(m_keyIndexMutex);
	
	m_keyIndexStream->seekToOffset(index*4+4);
	
	// read address to key.
	uint32_t addr=m_keyIndexStream->readBEInt32();
	
	XTBDicDBReaderKey key;
	
	// read lower address.
	m_keysStream->seekToOffset(addr);
	assert(m_keysStream->position()==addr);
	key.pointer=m_keysStream->readBEInt32();
	
	// ???: is this really needed?
	if(index>0){
		assert(addr!=0);
	}else{
		assert(addr==0);
	}
	
	// read upper address, and length.
	uint32_t composite;
	size_t length;
	composite=m_keysStream->readBEInt32();
	
	key.pointer|=((uint64_t)(composite&0xffff))<<32;
	length=composite>>16;
	
	// read key.
	key.key=m_keysStream->readToString(length);
	
	return key;
}

XTBDicDBPointer XTBDicDBReader::pointerForKey(const std::string &query) const{
	
	// special case 
	if(m_keyCount==1){
		XTBDicDBReaderKey k=keyAtIndex(0);
		if(k.key==query)
			return k.pointer;
		return XTBDicDBInvalidPointer;
	}
	
	// do binary search
	
	XTBDicDBReaderKey k1, k2, k3;
	unsigned i1, i2, i3;
	
	i1=0; i2=m_keyCount-1;
	k1=keyAtIndex(i1);
	k2=keyAtIndex(i2);
	
	if(query<k1.key) // outta range?
		return XTBDicDBInvalidPointer;
	else if(query==k1.key)
		return k1.pointer;
	
	if(query>k2.key) // outta range?
		return XTBDicDBInvalidPointer;
	else if(query==k2.key)
		return k2.pointer;

	while(i1<=i2){
		
		i3=(i1+i2)/2;
		if(i3==i1)
			k3=k1;
		else if(i3==i2)
			k3=k2;
		else
			k3=keyAtIndex(i3);
		
		if(query==k3.key){
			return k3.pointer;
		}else if(query>k3.key){
			i1=i3+1;
			k1=keyAtIndex(i1);
			if(query<k1.key) // outta range?
				return XTBDicDBInvalidPointer;
			else if(query==k1.key)
				return k1.pointer;
		}else{
			i2=i3-1;
			k2=keyAtIndex(i2);
			if(query>k2.key) // outta range?
				return XTBDicDBInvalidPointer;
			else if(query==k2.key)
				return k2.pointer;
		}
		
	}
	
	return XTBDicDBInvalidPointer;
	
}


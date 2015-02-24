//
//  XTBMemoryStream.cpp
//  XTBook
//
//  Created by Kawada Tomoaki on 7/29/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "XTBMemoryStream.h"
#include <stdlib.h>
#include <algorithm>
#include <string.h>

XTBMemoryStream::XTBMemoryStream(){
	m_data=NULL;
	m_isWritable=true;
	m_shouldFree=true;
	m_pos=0;
	m_size=0;
}

XTBMemoryStream::XTBMemoryStream(char *data, size_t size,
								 bool writable){
	m_data=data;
	m_isWritable=writable;
	m_shouldFree=false;
	m_pos=0;
	m_size=size; 
}

XTBMemoryStream::XTBMemoryStream(const char *data, size_t size){
	m_data=const_cast<char *>(data);
	m_isWritable=false;
	m_shouldFree=false;
	m_pos=0;
	m_size=size; 
}

XTBMemoryStream::~XTBMemoryStream(){
	if(m_shouldFree){
		if(m_data){
			free(m_data);
		}
	}
}

size_t XTBMemoryStream::readToBuffer(void *buffer, size_t maxLength){
	if(m_pos>=m_size)
		return 0;
	maxLength=std::min(maxLength, (size_t)(m_size-m_pos));
	memcpy(buffer, m_data+(size_t)m_pos, maxLength);
	m_pos+=maxLength;
	return maxLength;
}

size_t XTBMemoryStream::writeBuffer(const void *buffer, size_t maxLength){
	if(m_pos+maxLength>m_size){
		resize(m_pos+maxLength);
	}
	if(m_pos>=m_size)
		return 0;
	maxLength=std::min(maxLength, (size_t)(m_size-m_pos));
	memcpy(m_data+(size_t)m_pos, buffer, maxLength);
	m_pos+=maxLength;
	return maxLength;
}

void XTBMemoryStream::seekToOffset(XTBOffset offset){
	m_pos=offset;
}

void XTBMemoryStream::seekToEndOfStream(){
	m_pos=m_size;
}

XTBOffset XTBMemoryStream::position() const{
	return m_pos;
}

void XTBMemoryStream::synchronize(){}

void XTBMemoryStream::resize(XTBOffset size){
	if(!isExpandable())
		return;
	if(!m_data){
		m_data=(char *)malloc(size);
	}else{
		m_data=(char *)realloc(m_data, size);
	}
	
	m_size=size;
}

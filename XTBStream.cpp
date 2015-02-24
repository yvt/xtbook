//
//  XTBStream.cpp
//  XTBook
//
//  Created by Kawada Tomoaki on 5/28/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "XTBStream.h"
#include "XTBException.h"

size_t XTBStream::readToBuffer(void *, size_t){
	XTBThrowException(L"XTBNotImplementedException", NULL);
}

std::string XTBStream::readToString(size_t maxLength){
	std::string outStr;
	char buf[XTBStreamBufferLength];
	size_t ln;
	while((ln=readToBuffer(buf, 
						   std::min((size_t)XTBStreamBufferLength,
									maxLength-outStr.size())))>0){
		outStr.append(buf, ln);
	}
	return outStr;
}

std::string XTBStream::readToString(){
	std::string outStr;
	char buf[XTBStreamBufferLength];
	size_t ln;
	while((ln=readToBuffer(buf, (size_t)XTBStreamBufferLength))>0){
		outStr.append(buf, ln);
	}
	return outStr;
}

std::string XTBStream::readToStringUntilNullChar(size_t maxLength){
	std::string outStr;
	char buf[XTBStreamBufferLength+1];
	size_t realLn;
	size_t ln;
	while((ln=readToBuffer(buf, std::min((size_t)XTBStreamBufferLength,
										 maxLength-outStr.size())))>0){
		buf[ln]=0;
		realLn=strlen(buf);
		outStr.append(buf);
		if(realLn<ln)
			break;
	}
	return outStr;
}

std::string XTBStream::readToStringUntilNullChar(){
	std::string outStr;
	char buf[XTBStreamBufferLength+1];
	size_t realLn;
	size_t ln;
	while((ln=readToBuffer(buf, (size_t)XTBStreamBufferLength))>0){
		buf[ln]=0;
		realLn=strlen(buf);
		outStr.append(buf);
		if(realLn<ln)
			break;
	}
	return outStr;
}

uint32_t XTBStream::readSysInt32(){
	uint32_t i=0;
	if(readToBuffer(&i, 4)<4)
		XTBThrowException(L"XTBInvalidStreamStateException", NULL);
	return i;
}

uint16_t XTBStream::readSysInt16(){
	uint16_t i=0;
	if(readToBuffer(&i, 2)<2)
		XTBThrowException(L"XTBInvalidStreamStateException", NULL);
	return i;
}

size_t XTBStream::writeBuffer(const void *, size_t){
	XTBThrowException(L"XTBNotImplementedException", NULL);
}

void XTBStream::writeSysInt32(uint32_t v){
	if(writeBuffer(&v, 4)<4){
		XTBThrowException(L"XTBInvalidStreamStateException", NULL);
	}
}
void XTBStream::writeSysInt16(uint16_t v){
	if(writeBuffer(&v, 2)<2){
		XTBThrowException(L"XTBInvalidStreamStateException", NULL);
	}
}

void XTBStream::writeString(std::string const &str){
	writeBuffer(str.data(), str.size());
}

void XTBStream::seekToOffset(XTBOffset){
	XTBThrowException(L"XTBNotImplementedException", NULL);
}

void XTBStream::seekToEndOfStream(){
	XTBThrowException(L"XTBNotImplementedException", NULL);
}

XTBOffset XTBStream::position() const{
	XTBThrowException(L"XTBNotImplementedException", NULL);
}

bool XTBStream::isReadable() const{
	return false;
}

bool XTBStream::isWritable() const{
	return false;
}

bool XTBStream::isSeekable() const{
	return false;
}


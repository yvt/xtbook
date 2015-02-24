//
//  XTBStringTableFileHandle.cpp
//  XTBook
//
//  Created by Nexhawks on 4/16/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "XTBStringTableFileHandle.h"
#include <tcw/twException.h>
#include <tcw/twStrConv.h>

// NOTE: Do not use XTBThrowException here. Locale may be not initialized.
// NOTE: Do not use XTBStream here.

static uint32_t readLE32(FILE *f){
	uint8_t buf[4];
	if(fread(buf, 1, 4, f)!=4){
		throw twException(L"error while reading string table.");
	}
	uint32_t value;
	value=buf[0];
	value|=(uint32_t(buf[1]))<<8;
	value|=(uint32_t(buf[2]))<<16;
	value|=(uint32_t(buf[3]))<<24;
	return value;
}

XTBStringTableFileHandle::XTBStringTableFileHandle(const XTBSysString& path){
	m_stdHandle=XTBOpen(path, "rb");
	if(!m_stdHandle)
		throw twException(L"could't open string table.");
	m_eof=false;
}
XTBStringTableFileHandle::~XTBStringTableFileHandle(){
	fclose(m_stdHandle);
}

std::wstring XTBStringTableFileHandle::readLine(){
	char buf[2048];
	uint32_t length;
	if(m_eof)
		return L"";
	length=readLE32(m_stdHandle);
	if(length==0){
		m_eof=true;
		return L"";
	}
	if(fread(buf, 1, length, m_stdHandle)!=length)
		throw twException(L"error while reading string table.");
	buf[length]=0;
	return twM2W(buf);
}

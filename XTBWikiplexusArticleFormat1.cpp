//
//  XTBWikiplexusArticleFormat1.cpp
//  XTBook
//
//  Created by Kawada Tomoaki on 7/7/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "XTBWikiplexusArticleFormat1.h"
#include <assert.h>
#include <tcw/twStrConv.h>
#include "XTBWikiplexusDictionary.h"

XTBWikiplexusArticleFormat1::XTBWikiplexusArticleFormat1
(const XTBWikiplexusDictionary *dic,
 const std::string& dbKey):
XTBWikiplexusArticle(dic, dbKey){
	// don't read bytes here.
	// it forces bytes to be read.
}


XTBWikiplexusArticleFormat1::~XTBWikiplexusArticleFormat1(){
	
}

size_t XTBWikiplexusArticleFormat1::offsetToPageTable() const{
	return BEInt32AtAddress(4);
}

size_t XTBWikiplexusArticleFormat1::offsetToIndices() const{
	return 12+2*indicesCount();
}
size_t XTBWikiplexusArticleFormat1::offsetToPages() const{
	return offsetToPageTable()+4*pagesCount();
}

int XTBWikiplexusArticleFormat1::pagesCount() const{
	return BEInt16AtAddress(8);
}

int XTBWikiplexusArticleFormat1::indicesCount() const{
	return BEInt16AtAddress(10);
}
XTBArticleIndexItem XTBWikiplexusArticleFormat1::indexAtAddress(size_t addr) const{
	const std::string& data=bytes();
	
	assert(addr<data.size()-2);
	
	XTBArticleIndexItem item;
	item.page=(uint8_t)data[addr];
	item.level=(uint8_t)data[addr+1];
	
	const char *name=data.data()+addr+2;
	item.displayText=twM2W(name);
	
	item.fragmentId=dictionary()->fragmentIdForIndexText(item.displayText);
	
	return item;
	
}
std::wstring XTBWikiplexusArticleFormat1::contentsAtAddress(size_t addr) const{
	const std::string& data=bytes();
	assert(addr<data.size()-1);
	const char *txt=data.data()+addr;
	return twM2W(txt);
}
XTBArticleIndexItem XTBWikiplexusArticleFormat1::indexAt(int i) const{
	assert(i>=0);
	assert(i<indicesCount());
	
	uint16_t addr=BEInt16AtAddress(12+i*2);
	
	return indexAtAddress(addr+offsetToIndices());
}

std::wstring XTBWikiplexusArticleFormat1::wikiText(int page) const{
	
	if(BEInt32AtAddress(0)!=XTBFormat1Magic){
		XTBThrowException(L"XTBWikiplexusInvalidMagicException",
						  XTBFormatStd(L"0x%08x", (int)BEInt32AtAddress(0)).c_str(),
						   XTBFormatStd(L"0x%08x", (int)XTBFormat1Magic).c_str(),
						  NULL);
	}
	
	assert(page>=1);
	assert(page<=pagesCount());
	
	uint32_t addr=BEInt32AtAddress(offsetToPageTable()+(page-1)*4);
	
	return contentsAtAddress(addr+offsetToPages());
}


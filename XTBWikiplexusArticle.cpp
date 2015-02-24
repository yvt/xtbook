//
//  XTBWikiplexusArticle.cpp
//  XTBook
//
//  Created by Kawada Tomoaki on 7/7/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "XTBWikiplexusArticle.h"
#include "XTBWikiplexusDictionary.h"
#include "XTBWikiplexusParser.h"
#include "commondlg.h"
#include "platform.h"

#include <tcw/twStrConv.h>

#include "TWiki/TWUtils.h"

#include <assert.h>

#include "i18n.h"

uint32_t XTBWikiplexusArticle::BEInt32AtAddress(size_t addr) const{
	union{
		uint32_t i;
		char b[4];
	} cv;
	
	const std::string& b=bytes();
	
	assert(addr<=b.size()-4);
	
#if XTBEndian==XTBBigEndian
	cv.b[0]=b[addr];
	cv.b[1]=b[addr+1];
	cv.b[2]=b[addr+2];
	cv.b[3]=b[addr+3];
#elif XTBEndian==XTBLittleEndian
	cv.b[3]=b[addr];
	cv.b[2]=b[addr+1];
	cv.b[1]=b[addr+2];
	cv.b[0]=b[addr+3];
#else
#error endian unknown.
#endif
	
	return cv.i;
}
uint16_t XTBWikiplexusArticle::BEInt16AtAddress(size_t addr) const{
	union{
		uint16_t i;
		char b[2];
	} cv;
	
	const std::string& b=bytes();
	
	assert(addr<=b.size()-2);
	
#if XTBEndian==XTBBigEndian
	cv.b[0]=b[addr];
	cv.b[1]=b[addr+1];
#elif XTBEndian==XTBLittleEndian
	cv.b[1]=b[addr];
	cv.b[0]=b[addr+1];
#else
#error endian unknown.
#endif
	
	return cv.i;
}



XTBWikiplexusArticle::XTBWikiplexusArticle
(const XTBWikiplexusDictionary * dic,
 const std::string& dbKey):
m_title(dic->displayTitleForDBKey(dbKey)),
m_dic(dic),
m_dbKey(dbKey),
m_loaded(false ){
	
}
XTBWikiplexusArticle::~XTBWikiplexusArticle(){
	
}


const std::string& XTBWikiplexusArticle::bytes() const{
	if(!m_loaded){
		m_bytes=m_dic->bytesForDBKey(m_dbKey);
		//XTBDumpBytes(m_bytes);
		m_loaded=true;
		
		assert(m_bytes.size()>0);
	}
	return m_bytes;
}


std::wstring XTBWikiplexusArticle::htmlForPage(int page) const{
	XTBWikiplexusParser *parser=m_dic->parser();
	std::wstring txt;
	
	XTBSetProgressText(XTBLocalizedString(L"XTBWikiplexusArticleReading"));
	txt=wikiText(page);
	
#if 0
	XTBLog("Here's MediaWiki...");
	XTBDumpWString(txt);
#endif
	
	XTBSetProgressText(XTBLocalizedString(L"XTBWikiplexusArticleConverting"));
	
	std::wstring m_html=parser->parse(txt, displayTitle());
#if 0
	// benchmark
	twTicks ot=twGetTicks();
	int count=0;
	while(twGetTicks()<ot+5000){ 
		
		m_html=parser->parse(txt, displayTitle());
		count++;
	}
	printf("BENCHMARK: XTBWikiplexuArticle TWiki Parser: %f iterations/sec\n", (float)count/(float)(twGetTicks()-ot)*1000.f);
#endif
	
#if 0
	XTBLog("Here's raw HTML Dump...");
	XTBDumpWString(m_html);
#endif
	//m_html=TWUtils::escapeHTMLSpecialChars(m_html);
	return m_html;
}
std::wstring XTBWikiplexusArticle::displayTitle() const{
	return m_title;
}

int XTBWikiplexusArticle::pageForFragmentId(const std::wstring& fg) const{
	int indexCount=indicesCount();
	for(int i=0;i<indexCount;i++){
		XTBArticleIndexItem idx=indexAt(i);
		if(idx.fragmentId==fg)
			return idx.page;
	}
	return 1;
}



//
//  XTBSampleDictionaryArticle.cpp
//  XTBook
//
//  Created by Kawada Tomoaki on 5/23/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "XTBSampleDictionaryArticle.h"
#include "TWiki/TWParser.h"
#include "XTBSampleDictionary.h"
#include "platform.h"
#include <tcw/twStrConv.h>
#include "commondlg.h"

XTBSampleDictionaryArticle::XTBSampleDictionaryArticle(const std::wstring& title, 
													   const XTBSysString& path, const XTBSampleDictionary *dic){
	m_title=title;
	m_path=path;
	m_dic=dic;
}

XTBSampleDictionaryArticle::~XTBSampleDictionaryArticle(){
	
}

int XTBSampleDictionaryArticle::pagesCount() const{
	return 1;
}

int XTBSampleDictionaryArticle::indicesCount() const{
	return 1;
}
													 
XTBArticleIndexItem XTBSampleDictionaryArticle::indexAt(int) const{
	XTBArticleIndexItem item;
	item.level=0;
	item.page=1;
	item.fragmentId=L"1";
	return item;
}

int XTBSampleDictionaryArticle::pageForFragmentId(const std::wstring&) const{
	return 1;
}

std::wstring XTBSampleDictionaryArticle::htmlForPage(int) const{
	if(m_html.empty()){
		TWParser *parser=m_dic->parser();
		std::wstring txt;
		
		XTBSetProgressText(L"Reading Contents");
		txt=twM2W(XTBReadAllContents(m_path));
		return txt;
		XTBSetProgressText(L"MediaWiki Now!");
		m_html=parser->parse(txt, displayTitle());
	}
	return m_html;
}

std::wstring XTBSampleDictionaryArticle::displayTitle() const{
	return m_title;
}


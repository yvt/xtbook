//
//  XTBSampleDictionaryArticle.h
//  XTBook
//
//  Created by Kawada Tomoaki on 5/23/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#pragma once

#include "platform.h"
#include "XTBArticle.h"

class XTBSampleDictionary;

class XTBSampleDictionaryArticle: public XTBArticle{
private:
	const XTBSampleDictionary *m_dic;
	std::wstring m_title;
	XTBSysString m_path;
	mutable std::wstring m_html;
public:
	XTBSampleDictionaryArticle(const std::wstring&, const XTBSysString&, const XTBSampleDictionary *dic);
	virtual ~XTBSampleDictionaryArticle();
	
	virtual int pagesCount() const;
	
	virtual int indicesCount() const;
	virtual XTBArticleIndexItem indexAt(int) const;	
	virtual int pageForFragmentId(const std::wstring&) const;
	virtual std::wstring htmlForPage(int) const;
	virtual std::wstring displayTitle() const;	
	
};

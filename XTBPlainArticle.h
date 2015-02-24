//
//  XTBPlainArticle.h
//  XTBook
//
//  Created by Kawada Tomoaki on 5/29/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#pragma once
#include "XTBArticle.h"

class XTBPlainArticle: public XTBDocument{
	std::wstring m_html;
	std::wstring m_title;
public:
	XTBPlainArticle(const std::wstring& title,
					const std::wstring& html);
	virtual ~XTBPlainArticle();
	
	virtual int pagesCount() const;
	
	virtual std::wstring htmlForPage(int) const;
	virtual std::wstring displayTitle() const;
	
};


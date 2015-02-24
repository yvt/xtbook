//
//  XTBPlainArticle.cpp
//  XTBook
//
//  Created by Kawada Tomoaki on 5/29/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "XTBPlainArticle.h"
#include "XTBException.h"

XTBPlainArticle::XTBPlainArticle(const std::wstring& title,
								 const std::wstring& html):
m_html(html), m_title(title){ }

XTBPlainArticle::~XTBPlainArticle(){}

int XTBPlainArticle::pagesCount() const{
	return 1;
}

std::wstring XTBPlainArticle::htmlForPage(int page) const{
	if(page!=1)
		XTBThrowException(L"XTBArgumentOutOfRangeException", L"page", NULL);
	return m_html;
}

std::wstring XTBPlainArticle::displayTitle() const{
	return m_title;
}

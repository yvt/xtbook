//
//  XTBArticle.h
//  XTBook
//
//  Created by Kawada Tomoaki on 5/22/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#pragma once

#include <string>
#include <stdint.h>
#include <tcw/twObject.h>
#include "XTBDocument.h"

struct XTBArticleIndexItem{
	uint16_t page;
	uint16_t level;
	std::wstring displayText;
	std::wstring fragmentId;
};

class XTBArticle: public XTBDocument{
public:
	XTBArticle();
	virtual ~XTBArticle();
	
	virtual int pagesCount() const=0;
	
	
	virtual std::wstring htmlForPage(int) const=0;
	virtual std::wstring displayTitle() const=0;
	
	virtual int indicesCount() const;
	virtual XTBArticleIndexItem indexAt(int) const;	
	virtual int pageForFragmentId(const std::wstring&) const;
};

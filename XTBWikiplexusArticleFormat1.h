//
//  XTBWikiplexusArticleFormat1.h
//  XTBook
//
//  Created by Kawada Tomoaki on 7/7/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#pragma once

#include "XTBWikiplexusArticle.h"

class XTBWikiplexusArticleFormat1: public XTBWikiplexusArticle{
	size_t offsetToPageTable() const;
	size_t offsetToIndices() const;
	size_t offsetToPages() const;
	
	XTBArticleIndexItem indexAtAddress(size_t) const;
	std::wstring contentsAtAddress(size_t) const;
	
protected:
	
	virtual std::wstring wikiText(int page) const;
	
public:
	XTBWikiplexusArticleFormat1(const XTBWikiplexusDictionary *,
								const std::string& title);
	virtual ~XTBWikiplexusArticleFormat1();
	
	virtual int pagesCount() const;
	
	virtual int indicesCount() const;
	virtual XTBArticleIndexItem indexAt(int) const;	
	
};

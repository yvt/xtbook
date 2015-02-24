//
//  XTBContentsDataSource.h
//  XTBook
//
//  Created by Kawada Tomoaki on 8/1/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#pragma once

#include "XTBListViewDataSource.h"

class XTBArticle;

class XTBContentsDataSource: public XTBListViewDataSource{
	XTBArticle *m_article;
	int m_currentIndex;
	
	bool shouldShowPageAtRow(int) const;
public:
	XTBContentsDataSource(XTBArticle *, int);
	virtual ~XTBContentsDataSource();
	
	virtual int rowCount();
	virtual std::wstring stringAtRow(int);
	virtual std::wstring detailStringAtRow(int);
	virtual XTBListViewItemState stateAtRow(int);
	
	std::wstring fragmentIdAtRow(int);
	
};

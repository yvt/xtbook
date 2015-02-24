//
//  XTBSearchViewDataSource.h
//  XTBook
//
//  Created by Kawada Tomoaki on 6/1/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#pragma once

#include "XTBListViewDataSource.h"

struct XTBSearchViewQuery;
class XTBDictionaryIterator;
struct XTBDictionaryResult;
enum XTBSearchViewRowStyle{
	XTBSearchViewRowStyleEnglish,
	XTBSearchViewRowStyleJapanese
};

class XTBSearchViewDataSource: public XTBListViewDataSource{
	std::vector<XTBDictionaryIterator *> m_iterators;
	std::vector<XTBDictionaryResult> m_results;
	int m_rowCount;
	
	XTBSearchViewRowStyle styleForRow(int row);
	void prepareRow(int row);
public:
	XTBSearchViewDataSource();
	XTBSearchViewDataSource(const XTBSearchViewQuery&);
	virtual ~XTBSearchViewDataSource();
	
	virtual int rowCount();
	virtual std::wstring stringAtRow(int);
	virtual std::wstring detailStringAtRow(int);
	virtual XTBListViewItemState stateAtRow(int);
	std::wstring titleAtRow(int);
};


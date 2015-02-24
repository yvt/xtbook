//
//  XTBStartPageBookmarkDataSource.h
//  XTBook
//
//  Created by Kawada Tomoaki on 6/25/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#pragma once

#include "XTBStartPageListDataSource.h"
#include "XTBNotificationCenter.h"
#include "XTBUserDefaults.h"

class XTBStartPageList;

class XTBStartPageBookmarkDataSource: public XTBStartPageListDataSource{
	
	typedef std::map<std::string, TPLObject *> DictionaryMap;
	
	XTBStartPageList *m_startPageList;
	XTBNotificationHandler m_handler;
	
	TPLDictionary *m_bookmark;
	
	std::vector<DictionaryMap::const_iterator> m_iterators;
	
	DictionaryMap::const_iterator iteratorAtRow(int);
public:
	XTBStartPageBookmarkDataSource(XTBStartPageList *);
	virtual ~XTBStartPageBookmarkDataSource();
	
	virtual bool isRemovable() const{return true;}
	virtual void removeItemsAtRows(const std::set<int>&);
	
	virtual int rowCount();
	virtual std::wstring stringAtRow(int);
	virtual std::wstring detailStringAtRow(int);
	
	std::wstring urlAtRow(int);
	
	virtual void reloadData();
};

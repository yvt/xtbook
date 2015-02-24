//
//  XTBStartPageHistoryDataSource.h
//  XTBook
//
//  Created by Kawada Tomoaki on 6/25/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#pragma once

#include "XTBStartPageListDataSource.h"
#include "XTBNotificationCenter.h"
#include "XTBHistory.h"

class XTBStartPageList;

class XTBStartPageHistoryDataSource: public XTBStartPageListDataSource{
	XTBStartPageList *m_startPageList;
	XTBNotificationHandler m_handler;
	XTBHistoryDB *m_history;
public:
	XTBStartPageHistoryDataSource(XTBStartPageList *);
	virtual ~XTBStartPageHistoryDataSource();
	
	virtual bool isClearable() const{return true;}
	virtual void clearAllItems();
	
	virtual int rowCount();
	virtual std::wstring stringAtRow(int);
	virtual std::wstring detailStringAtRow(int);
	
	virtual void reloadData();
};

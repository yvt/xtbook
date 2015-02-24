//
//  XTBStartPageHistoryDataSource.cpp
//  XTBook
//
//  Created by Kawada Tomoaki on 6/25/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "XTBStartPageHistoryDataSource.h"
#include "XTBNotificationCenter.h"
#include "XTBStartPageList.h"

XTBStartPageHistoryDataSource::XTBStartPageHistoryDataSource(XTBStartPageList *lst){
	m_startPageList=lst;
	
	twInvocation *invocation;
	invocation=new twNoArgumentMemberFunctionInvocation<XTBStartPageList>(lst, &XTBStartPageList::reloadData);
	
	XTBNotificationCenter *center=XTBNotificationCenter::defaultNotificationCenter();
	m_handler=center->registerHandler(XTBHistoryModifiedNotification, invocation);
	
	m_history=NULL;
	try{
		m_history=XTBHistoryDB::standardHistory();
	}catch(const std::exception& ex){
		XTBLog("exception while registering history.: %s", ex.what());
	}catch(...){
		// ignore error while opening history.
	}
	
	reloadData();
}

XTBStartPageHistoryDataSource::~XTBStartPageHistoryDataSource(){
	XTBNotificationCenter *center=XTBNotificationCenter::defaultNotificationCenter();
	center->unregisterHandler(XTBHistoryModifiedNotification, m_handler);
}


void XTBStartPageHistoryDataSource::clearAllItems(){
	m_history->clearHistory();
}

int XTBStartPageHistoryDataSource::rowCount(){
	if(!m_history)
		return 0;
	return (int)m_history->entryCount();
}

std::wstring XTBStartPageHistoryDataSource::stringAtRow(int row){
	XTBHistoryDBItem item=m_history->entryAtIndex(rowCount()-1-row);
	return item.url;
}

std::wstring XTBStartPageHistoryDataSource::detailStringAtRow(int row){
	XTBHistoryDBItem item=m_history->entryAtIndex(rowCount()-1-row);
	return XTBShortStringForDate(item.time());
}

void XTBStartPageHistoryDataSource::reloadData(){
	
	XTBStartPageListDataSource::reloadData();
}


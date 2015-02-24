//
//  XTBStartPageBookmarkDataSource.cpp
//  XTBook
//
//  Created by Kawada Tomoaki on 6/25/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "XTBStartPageBookmarkDataSource.h"
#include "XTBStartPageList.h"
#include <assert.h>
#include "TPList/TPLString.h"
#include <tcw/twStrConv.h>

XTBStartPageBookmarkDataSource::XTBStartPageBookmarkDataSource(XTBStartPageList *lst){
	m_startPageList=lst;
	
	twInvocation *invocation;
	invocation=new twNoArgumentMemberFunctionInvocation<XTBStartPageList>(lst, &XTBStartPageList::reloadData);
	
	XTBNotificationCenter *center=XTBNotificationCenter::defaultNotificationCenter();
	m_handler=center->registerHandler(XTBBookmarkModifiedNotification, invocation);
	
	m_bookmark=NULL;
	
	reloadData();
}

XTBStartPageBookmarkDataSource::~XTBStartPageBookmarkDataSource(){
	XTBNotificationCenter *center=XTBNotificationCenter::defaultNotificationCenter();
	center->unregisterHandler(XTBBookmarkModifiedNotification, m_handler);
	m_bookmark->release();
}


void XTBStartPageBookmarkDataSource::removeItemsAtRows(const std::set<int> & rows){
	std::set<std::string> urls;
	for(std::set<int>::const_iterator it=rows.begin();it!=rows.end();
		it++){
		int row=*it;
		DictionaryMap::const_iterator it2=iteratorAtRow(row);
		urls.insert(it2->first);
	}
	
	for(std::set<std::string>::const_iterator it=urls.begin();
		it!=urls.end();it++){
		m_bookmark->removeObjectForKey(it->c_str());
	}
	
	XTBNotificationCenter *center=XTBNotificationCenter::defaultNotificationCenter();
	center->sendNotification(XTBBookmarkModifiedNotification);
}

int XTBStartPageBookmarkDataSource::rowCount(){
	return m_bookmark->count();
}

std::wstring XTBStartPageBookmarkDataSource::stringAtRow(int row){
	DictionaryMap::const_iterator it=iteratorAtRow(row);
	TPLString *title=static_cast<TPLString *>(it->second);
	std::string str=title?title->UTF8String():"(null)";
	return twM2W(str);
}

std::wstring XTBStartPageBookmarkDataSource::urlAtRow(int row){
	DictionaryMap::const_iterator it=iteratorAtRow(row);
	std::string addr=it->first;
	return twM2W(addr);
}

std::wstring XTBStartPageBookmarkDataSource::detailStringAtRow(int row){
	return urlAtRow(row);
}

XTBStartPageBookmarkDataSource::DictionaryMap::const_iterator XTBStartPageBookmarkDataSource::iteratorAtRow(int row){
	assert(m_bookmark->count()>0);
	const DictionaryMap& mp=m_bookmark->STLDictionary();
	assert(row<m_bookmark->count());
	if(m_iterators.size()<row+1){
		if(m_iterators.empty()){
			m_iterators.push_back(mp.begin());
		}
		
		DictionaryMap::const_iterator it=m_iterators.back();
		while(m_iterators.size()<row+1){
			it++;
			m_iterators.push_back(it);
		}
	}
	return m_iterators[row];
}

void XTBStartPageBookmarkDataSource::reloadData(){
	
	if(m_bookmark)
		m_bookmark->release();
	
	m_bookmark=XTBUserDefaults::standardUserDefaults()->dictionaryForKey(XTBUserDefaultsKeyBookmarks);
	m_bookmark->retain();
	
	m_iterators.clear();
	
	XTBStartPageListDataSource::reloadData();
}



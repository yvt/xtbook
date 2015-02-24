//
//  XTBHistory.cpp
//  XTBook
//
//  Created by 河田 智明 on 8/3/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "XTBHistory.h"
#include "XTBFileStream.h"
#include <tcw/twLock.h>
#include <tcw/twSDLSemaphore.h>
#include <tcw/twStrConv.h>
#include <assert.h>
#include <exception>
#include "TWiki/TWUtils.h"
#include "XTBNotificationCenter.h"
#include <time.h>

XTBHistoryDB::XTBHistoryDB(const XTBSysString& path):
m_dbHandle(NULL),
m_indexHandle(NULL),
m_path(path){
	try{
		if(XTBDoesPathExist(path+XTBSysText(".historydb"))==false
		   || XTBDoesPathExist(path+XTBSysText(".historyindex"))==false){
			// create db.
			m_dbHandle=XTBFileStream::streamForWritingAtPath(m_path+XTBSysText(".historydb"));
			m_indexHandle=XTBFileStream::streamForWritingAtPath(m_path+XTBSysText(".historyindex"));
			
			delete m_dbHandle;
			delete m_indexHandle;
		}
		m_dbHandle=XTBFileStream::streamForUpdatingAtPath(m_path+XTBSysText(".historydb"));
		m_indexHandle=XTBFileStream::streamForUpdatingAtPath(m_path+XTBSysText(".historyindex"));
	
		
		
	}catch(...){
		if(m_dbHandle)
			delete m_dbHandle;
		if(m_indexHandle)
			delete m_indexHandle;
		throw;
	}
	
	m_indexHandle->seekToEndOfStream();
	m_historyCount=m_indexHandle->position()/8;
	
	XTBLog("%d history(ies) loaded", (int)m_historyCount);
	
	m_dbSemaphore=new twSDLSemaphore();
	m_indexSemaphore=new twSDLSemaphore();
}

XTBHistoryDB::~XTBHistoryDB(){
	m_dbSemaphore->lock();
	m_indexSemaphore->lock();
	delete m_dbSemaphore;
	delete m_indexSemaphore;
	
	delete m_dbHandle;
	delete m_indexHandle;
}

XTBHistoryDB::IndexItem XTBHistoryDB::indexItemAtIndex(uint64_t index)
{
	twLock lock(m_indexSemaphore);
	m_indexHandle->seekToOffset(index*8);
	
	IndexItem item;
	uint32_t buf;
	
	buf=m_indexHandle->readBEInt32();
	item.length=(uint16_t)(buf>>16);
	item.offset=((uint64_t)(buf&0xffff))<<32;
	
	buf=m_indexHandle->readBEInt32();
	item.offset|=((uint64_t)buf);
	
	return item;
}

void XTBHistoryDB::writeIndexItemAtEnd(const XTBHistoryDB::IndexItem &item){
	twLock lock(m_indexSemaphore);
	
	m_indexHandle->seekToEndOfStream();
	uint32_t buf;
	
	buf=((uint32_t)item.length)<<16;
	buf|=(uint32_t)(item.offset>>32);
	m_indexHandle->writeBEInt32(buf);
	
	buf=(uint32_t)(item.offset);
	m_indexHandle->writeBEInt32(buf);	
	
	m_indexHandle->synchronize();
}

std::string XTBHistoryDB::entryForItem(const XTBHistoryDBItem & item){
	std::string outStr;
	outStr=twW2M(item.date);
	outStr+='\t';
	outStr+=twW2M(item.url);
	outStr+='\n';
	return outStr;
}

XTBHistoryDBItem XTBHistoryDB::itemForEntry(const std::string & entry){
	XTBHistoryDBItem item;
	std::string::size_type pos;
	pos=entry.find('\t');
	if(pos==std::string::npos){
		// invalid format... heuristics.
		if(entry.empty()){
			// empty!? that's strange!
			return item;
		}
		
		// remove line-end char.
		item.url=twM2W(entry.substr(0, entry.size()-1));
	}else{
		item.date=twM2W(entry.substr(0, pos));
		item.url=twM2W(entry.substr(pos+1, entry.size()-2-pos));
	}
	return item;
}

void XTBHistoryDB::writeEntry(const XTBHistoryDBItem &item){
	if(item.url==m_lastUrl){
		// duplicating. out.
		return;
	}
	
	twLock lock(m_dbSemaphore);
	std::string entry=entryForItem(item);
	IndexItem indItem;
	
	m_dbHandle->seekToEndOfStream();
	
	indItem.offset=m_dbHandle->position();
	indItem.length=(uint16_t)entry.size();
	
	m_dbHandle->writeString(entry);
	writeIndexItemAtEnd(indItem);
	
	m_dbHandle->synchronize();
	
	m_historyCount++;
	m_lastUrl=item.url;
}

XTBHistoryDBItem XTBHistoryDB::entryAtIndex(uint64_t index){
	assert(index<entryCount());
	twLock lock(m_dbSemaphore);
	IndexItem indItem=indexItemAtIndex(index);
	
	std::string entry;
	
	m_dbHandle->seekToOffset(indItem.offset);
	entry=m_dbHandle->readToString(indItem.length);
	
	return itemForEntry(entry);
}

static XTBHistoryDB *m_standardHistory=NULL;

XTBHistoryDB *XTBHistoryDB::standardHistory(){
	if(!m_standardHistory){
		m_standardHistory=new XTBHistoryDB(XTBGetStandardHistoryPath());
	}
	return m_standardHistory;
}

void XTBHistoryDB::visitPage(const std::wstring &url){
	try{
		
		XTBHistoryDB *db=standardHistory();
		
		XTBHistoryDBItem item;
		
		char buf[64];
		wchar_t buf2[64];
		struct tm t2;
		time_t t1=time(NULL);
		t2=*(localtime(&t1));
		strftime(buf, 64, "%G%m%e%H%M%S", &t2);
		mbstowcs(buf2, buf, 64);

		item.url=url;
		item.date=buf2;
		
		db->writeEntry(item);
		
	}catch(const std::exception& ex){
		XTBLog("exception while registering history.: %s", ex.what());
	}catch(...){
		// ignore error while opening history.
	}
	
	XTBNotificationCenter::defaultNotificationCenter()->sendNotification(XTBHistoryModifiedNotification);
}

time_t XTBHistoryDBItem::time() const{
	struct tm t;
	time_t cur=::time(0);
	t=*localtime(&cur);
	t.tm_year=TWUtils::intValueFromString(date.substr(0, 4))-1900;
	t.tm_mon=TWUtils::intValueFromString(date.substr(4, 2))-1;
	t.tm_mday=TWUtils::intValueFromString(date.substr(6, 2));
	t.tm_hour=TWUtils::intValueFromString(date.substr(8, 2));
	t.tm_min=TWUtils::intValueFromString(date.substr(10, 2));
	t.tm_sec=TWUtils::intValueFromString(date.substr(12, 2));
	return mktime(&t);
}

void XTBHistoryDB::clearHistory(){
	m_historyCount=0;
	
	delete m_dbHandle;
	delete m_indexHandle;
	
	// create zero-sized db.
	
	m_dbHandle=XTBFileStream::streamForWritingAtPath(m_path+XTBSysText(".historydb"));
	m_indexHandle=XTBFileStream::streamForWritingAtPath(m_path+XTBSysText(".historyindex"));
	
	delete m_dbHandle;
	delete m_indexHandle;

	m_dbHandle=XTBFileStream::streamForUpdatingAtPath(m_path+XTBSysText(".historydb"));
	m_indexHandle=XTBFileStream::streamForUpdatingAtPath(m_path+XTBSysText(".historyindex"));
	
	m_lastUrl.clear();
}



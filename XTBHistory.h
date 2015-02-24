//
//  XTBHistory.h
//  XTBook
//
//  Created by 河田 智明 on 8/3/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#pragma once

#include <tcw/twObject.h>
#include "XTBStream.h"
#include "platform.h"
#include <tcw/twSemaphore.h>

struct XTBHistoryDBItem{
	std::wstring date;
	std::wstring url;
	
	time_t time() const;
};

class XTBHistoryDB: public twObject{
	struct IndexItem{
		uint64_t offset;
		uint16_t length;
	};
	
	XTBSysString m_path;
	
	XTBStream *m_dbHandle;
	XTBStream *m_indexHandle;
	uint64_t m_historyCount;
	
	twSemaphore *m_dbSemaphore;
	twSemaphore *m_indexSemaphore;
	
	std::wstring m_lastUrl;
	
	IndexItem indexItemAtIndex(uint64_t);
	void writeIndexItemAtEnd(const IndexItem&);
	
	std::string entryForItem(const XTBHistoryDBItem&);
	XTBHistoryDBItem itemForEntry(const std::string&);
	
public:
	
	XTBHistoryDB(const XTBSysString&);
	virtual ~XTBHistoryDB();
	
	
	void writeEntry(const XTBHistoryDBItem&);
	XTBHistoryDBItem entryAtIndex(uint64_t);
	
	uint64_t entryCount() const{return m_historyCount;}
	
	
	static XTBHistoryDB *standardHistory();
	static void visitPage(const std::wstring& url);
	
	void clearHistory();
};

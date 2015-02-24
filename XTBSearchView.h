//
//  XTBSearchView.h
//  XTBook
//
//  Created by Kawada Tomoaki on 6/1/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#pragma once

#include <tcw/twWnd.h>
#include <tcw/twSDLSemaphore.h>
#include <tcw/twTimer.h>

class XTBListView;
class XTBSearchViewDataSource;
class XTBSearchErrorView;

struct XTBSearchViewQuery{
	std::wstring key;
	bool operator !=(const XTBSearchViewQuery& query) const{
		return key!=query.key;
	}
};

class XTBSearchView: public twWnd{
	XTBListView *m_listView;
	XTBSearchErrorView *m_errorView;
	XTBSearchViewDataSource *m_dataSource;
	twSDLSemaphore m_processMutex;
	twSDLSemaphore m_queryMutex;
	volatile XTBSearchViewQuery m_query;
	volatile bool m_inProcessing;
	volatile bool m_exitRequested;
	
	int m_delaySession;
	XTBSearchViewQuery m_delayedQuery;
	
	std::wstring m_lastSelectedTitle;
	bool m_canSendCmd;
	
	XTBSearchViewDataSource *initialDataSource() const;
	XTBSearchViewDataSource *dataSourceForQuery(const XTBSearchViewQuery&) const;
	
	void setDataSource(XTBSearchViewDataSource *);
	void setError(std::wstring);
	
	void backgroundSetQuery();
	void startQuery(bool needLock=true);
	
	void delayCallback(int session);
	
	void sendCmd();
public:
	XTBSearchView();
	virtual ~XTBSearchView();
	
	virtual void setRect(const twRect&);
	
	void resetQuery();
	void setQuery(const XTBSearchViewQuery&);
	void setQueryDelayed(const XTBSearchViewQuery&);
	
	XTBListView *listView() const{return m_listView;}
	const std::wstring& lastSelectedTitle() const{return m_lastSelectedTitle;}
	
	virtual void command(int);
};


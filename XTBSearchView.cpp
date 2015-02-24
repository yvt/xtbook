//
//  XTBSearchView.cpp
//  XTBook
//
//  Created by Kawada Tomoaki on 6/1/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "XTBSearchView.h"
#include <assert.h>
#include "XTBListView.h"
#include "XTBSearchViewDataSource.h"
#include <tcw/twEvent.h>
#include <tcw/twInvocation.h>
#include <tcw/twLock.h>
#include "i18n.h"
#include "XTBSearchErrorView.h"

XTBSearchView::XTBSearchView():
m_listView(NULL),
m_dataSource(NULL),
m_query(),
m_inProcessing(false),
m_exitRequested(false){
	// setup the list view.
	m_listView=new XTBListView(XTBListViewStyleSubtitle);
	m_listView->setParent(this);
	
	// setup the error view.
	m_errorView=new XTBSearchErrorView();
	m_errorView->setParent(this);
	m_errorView->setTitle(XTBLocalizedString(L"XTBSearchViewNoResult"));
	
	// load the initial data source.
	m_dataSource=initialDataSource();
	m_listView->setDataSource(m_dataSource);
	
	// show widgets.
	m_listView->show();
	m_errorView->show();
	
	// focus the error view so that it is in the top.
	m_errorView->focus();
	
	m_canSendCmd=true;
}

XTBSearchView::~XTBSearchView(){
	// lock the mutex to make sure processing done.
	m_exitRequested=true;
	m_processMutex.lock();
	
	delete m_listView;
	delete m_errorView;
	delete m_dataSource;
}

void XTBSearchView::setRect(const twRect& rt){
	assert(m_listView!=NULL);
	
	twWnd::setRect(rt);
	
	twRect rt2(twPoint(0, 0), getClientRect().size());
	
	m_listView->setRect(rt2);
	m_errorView->setRect(rt2);
	
}

void XTBSearchView::setDataSource(XTBSearchViewDataSource *dataSource){
	if(dataSource==m_dataSource)
		return;
	delete m_dataSource;
	m_dataSource=dataSource;
	m_listView->setDataSource(dataSource);
	m_listView->setContentLocation(twPoint(0, 0));
	m_listView->invalidate();
}

void XTBSearchView::setError(std::wstring string){
	if(string.empty()){
		m_errorView->hide();
	}else{
		m_errorView->setTitle(string);
		m_errorView->invalidate();
		m_errorView->show();
		m_listView->blur();
		m_errorView->invalidate();
		
	}
}

XTBSearchViewDataSource *XTBSearchView::initialDataSource() const{
	return new XTBSearchViewDataSource();
}

XTBSearchViewDataSource *XTBSearchView::dataSourceForQuery(const XTBSearchViewQuery &query) const{
	if(query.key.empty())
		return initialDataSource();
	return new XTBSearchViewDataSource(query);
}

void XTBSearchView::resetQuery(){
	setDataSource(initialDataSource());
}

void XTBSearchView::setQuery(const XTBSearchViewQuery &query){
	{
		twLock queryLock(&m_queryMutex);
		if(const_cast<XTBSearchViewQuery&>(m_query).key==query.key)
			return;
	}
	m_exitRequested=false;
	{
		twLock queryLock(&m_queryMutex);
		const_cast<XTBSearchViewQuery&>(m_query)=query;
	}
	startQuery();
	
	m_lastSelectedTitle.clear();
	sendCmdToParent();
}

void XTBSearchView::setQueryDelayed(const XTBSearchViewQuery &d){
	{
		twLock queryLock(&m_queryMutex);
		if(const_cast<XTBSearchViewQuery&>(m_query).key==d.key)
			return;
	}
	if(m_errorView->getStyle().visible)
		m_errorView->hide();
	m_delaySession++;
	m_delayedQuery=d;
	tw_event->invokeDelayed(new twOneArgumentMemberFunctionInvocation<XTBSearchView, int>(this, &XTBSearchView::delayCallback, m_delaySession), 400);
	
}

void XTBSearchView::delayCallback(int session){
	if(session!=m_delaySession)
		return;
	setQuery(m_delayedQuery);
}

void XTBSearchView::startQuery(bool needLock){
	// if currently processing, we don't need to start
	// the new thread because the current thread will
	// start the new thread automatically.
	if(m_inProcessing && needLock)
		return;
	
	m_inProcessing=true;
	if(needLock)
		m_processMutex.lock();
	try{
		tw_event->invokeInBackground(new twNoArgumentMemberFunctionInvocation<XTBSearchView>(this, &XTBSearchView::backgroundSetQuery));
	}catch(...){
		// starting thread failed.
		// roll-back.
		if(needLock)
			m_processMutex.unlock();
		throw;
	}
}

void XTBSearchView::backgroundSetQuery(){
	try{
		// what query do we need to deal with?
		XTBSearchViewQuery query;
		{
			twLock queryLock(&m_queryMutex);
			query=const_cast<XTBSearchViewQuery&>(m_query);
		}
		
		// query taking place NOW!
		XTBSearchViewDataSource *dataSource;
		std::wstring errorMessage;
		try{
			dataSource=new XTBSearchViewDataSource(query);
		}catch(...){
			// error while the query.
			errorMessage=XTBLocalizedString(L"XTBSearchViewError");
		}
		
		// if query result is empty, show the error message.
		if(dataSource && dataSource->rowCount()==0){
			delete dataSource;
			dataSource=NULL;
			errorMessage=XTBLocalizedString(L"XTBSearchViewNoResult");
		}
		
		// now processing is completed. return the result.
		tw_event->invokeOnMainThread(new twOneArgumentMemberFunctionInvocation<XTBSearchView, XTBSearchViewDataSource *>(this, &XTBSearchView::setDataSource, dataSource));
		tw_event->invokeOnMainThread(new twOneArgumentMemberFunctionInvocation<XTBSearchView, std::wstring>(this, &XTBSearchView::setError, errorMessage));
		
		// ... but is processing really done?
		// if user inputed another query, m_query is not equal to query and 
		// we need to continue the processing...
		XTBSearchViewQuery nowQuery;
		{
			twLock queryLock(&m_queryMutex);
			nowQuery=const_cast<XTBSearchViewQuery&>(m_query);
		}
		
		// NOTE: if m_exitRequested==true, we need to complete the processing anyway.
		if(query!=nowQuery && !m_exitRequested){
			// that is, soldier.
			// start the next thread for the next curernt query.
			startQuery(false);
		}else{
			// never mind. processing is done now.
			m_inProcessing=false;
			m_processMutex.unlock(); // we hope this never throw an exception.
		}
	}catch(...){
		m_inProcessing=false;
		m_processMutex.unlock();
		
		// this is unexpected.
		// we did all we can. Just let it be as it is.
		throw;
	}	
}

void XTBSearchView::command(int){
	if(!m_canSendCmd)
		return;
	int row=m_listView->getLastActiveRow();
	
	m_lastSelectedTitle=m_dataSource->titleAtRow(row);
	m_canSendCmd=false;
	tw_event->invokeDelayed(new twNoArgumentMemberFunctionInvocation<XTBSearchView>
							(this, &XTBSearchView::sendCmd), 100);
}

void XTBSearchView::sendCmd(){
	sendCmdToParent();
	m_canSendCmd=true;
}

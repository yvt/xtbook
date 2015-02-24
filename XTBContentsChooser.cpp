//
//  XTBContentsChooser.cpp
//  XTBook
//
//  Created by Kawada Tomoaki on 8/1/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "XTBContentsChooser.h"
#include <tcw/twApp.h>
#include <tcw/twTimerWithInvocation.h>
#include <tcw/twEvent.h>
#include "XTBListView.h"
#include "XTBContentsDataSource.h"

XTBContentsChooser::XTBContentsChooser(XTBArticle *article, 
									   int currentIndex){
		
	m_listView=new XTBListView(XTBListViewStyleValue1);

	
	m_dataSource=new XTBContentsDataSource(article, currentIndex);
	m_listView->setDataSource(m_dataSource);
	
	m_listView->setRect(twRect(twPoint(0,0),startupSize()-getBorderSize()));
	m_listView->setParent(this);
	
	m_currentIndex=currentIndex;
	
	if(m_currentIndex!=-1){
		twPoint contLoc(0,m_currentIndex*24);
		twPoint maxLoc=m_listView->getMaxContentLocation();
		if(contLoc.y>maxLoc.y)
			contLoc.y=maxLoc.y;
		m_listView->setContentLocation(contLoc);
	}
		
	m_listView->show();
	
	m_selectAllowed=true;
	
	
}

twSize XTBContentsChooser::startupSize() const{
	twSize screenSize=tw_app->getScrSize();
	return twSize(screenSize.w/2,(screenSize.h*2)/3);
}

XTBContentsChooser::~XTBContentsChooser(){
	m_listView->setDataSource(NULL);
	delete m_dataSource;
	delete m_listView;
}


void XTBContentsChooser::backgroundTouched(){
	endDialog(twDR_cancel);
}

void XTBContentsChooser::selected(){
	m_lastSelectedIndex=m_listView->getLastActiveRow();
	endDialog(twDR_ok);
}

void XTBContentsChooser::command(int){
	if(!m_selectAllowed)
		return;
	m_selectAllowed=false;
	tw_event->invokeDelayed(new twNoArgumentMemberFunctionInvocation<XTBContentsChooser>(this, &XTBContentsChooser::selected), 100);
}

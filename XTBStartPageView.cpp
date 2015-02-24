//
//  XTBStartPageView.cpp
//  XTBook
//
//  Created by Kawada Tomoaki on 6/24/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "XTBStartPageView.h"
#include "XTBStartPageList.h"
#include "XTBStartPageBookmarkDataSource.h"
#include "XTBStartPageHistoryDataSource.h"
#include <tcw/twInvocation.h>
#include <tcw/twEvent.h>
#include <tcw/twSDLDC.h>
#include "xpms/LogoTexted.xpm"

static twDC *m_logoTexted=NULL;

twDC *XTBStartPageView::getLogoTexted(){
	if(!m_logoTexted){
		m_logoTexted=twSDLDC::loadFromXPM(LogoTexted);
	}
	return m_logoTexted;
}

XTBStartPageView::XTBStartPageView(){
	m_bookmarkList=new XTBStartPageList();
	m_historyList=new XTBStartPageList();
	m_bookmarkData=new XTBStartPageBookmarkDataSource(m_bookmarkList);
	m_historyData=new XTBStartPageHistoryDataSource(m_historyList);
	m_aboutButton=new twButton();
	
	m_bookmarkList->setParent(this);
	m_bookmarkList->setTitle(XTBLocalizedString(L"XTBStartPageViewBookmarks"));
	m_bookmarkList->setDataSource(m_bookmarkData);
	m_bookmarkList->setId(XTBStartPageViewBookmarkId);
	m_bookmarkList->show();
	
	m_historyList->setParent(this);
	m_historyList->setTitle(XTBLocalizedString(L"XTBStartPageViewHistories"));
	m_historyList->setDataSource(m_historyData);
	m_historyList->setId(XTBStartPageViewHistoryId);
	m_historyList->show();
	
	m_aboutButton->setParent(this);
	m_aboutButton->setTitle(XTBLocalizedString(L"XTBStartPageAbout"));
	m_aboutButton->setId(XTBStartPageViewAboutId);
	m_aboutButton->show();
	
	m_canSendCmd=true;
}

XTBStartPageView::~XTBStartPageView(){
	delete m_bookmarkList;
	delete m_historyList;
	delete m_bookmarkData;
	delete m_historyData;
	delete m_aboutButton;
}

int XTBStartPageView::logoHeight() const{
	return 40;
}

void XTBStartPageView::setRect(const twRect & rt){

	twWnd::setRect(rt);
	
	twSize clientSize=getClientRect().size();
	int bookmarkWidth=clientSize.w/2;
	
	m_bookmarkList->setRect(twRect(0, 0, 
								   bookmarkWidth,
								   clientSize.h-logoHeight()));
	m_historyList->setRect(twRect(bookmarkWidth, 0, 
								   clientSize.w-bookmarkWidth,
								   clientSize.h-logoHeight()));
	
	m_aboutButton->setRect(twRect(clientSize.w-130, clientSize.h-30,
								  120, 20));
	
}

void XTBStartPageView::command(int id){
	if(!m_canSendCmd)
		return;
	if(id==XTBStartPageViewBookmarkId){
		int row=m_bookmarkList->getLastActiveRow();
		m_lastSelectedUrl=m_bookmarkData->urlAtRow(row);
		
		m_canSendCmd=false;
		tw_event->invokeDelayed(new twNoArgumentMemberFunctionInvocation<XTBStartPageView>
								(this, &XTBStartPageView::sendCmd), 100);
	}else if(id==XTBStartPageViewHistoryId){
		int row=m_historyList->getLastActiveRow();
		m_lastSelectedUrl=m_historyData->stringAtRow(row);
		
		m_canSendCmd=false;
		tw_event->invokeDelayed(new twNoArgumentMemberFunctionInvocation<XTBStartPageView>
								(this, &XTBStartPageView::sendCmd), 100);
	}else if(id==XTBStartPageViewAboutId){
		m_lastSelectedUrl=L"about:";
		sendCmdToParent();
	}
}

void XTBStartPageView::sendCmd(){
	sendCmdToParent();
	m_canSendCmd=true;
}

void XTBStartPageView::clientPaint(const twPaintStruct &p){
	twDC *dc=p.dc;
	twRect rt;
	rt=twRect(twPoint(0,0),getClientRect().size());
	
	twRect logoRect;
	logoRect=twRect(rt.x, rt.y+rt.h-logoHeight(),
					rt.w, logoHeight());
	
	dc->fillRect(0xffffff, logoRect);
	
	twRect logoImageRect;
	logoImageRect=twRect(logoRect.x+(logoRect.w-116)/2,
						 logoRect.y+(logoRect.h-32)/2,
						 116, 32);
	
	dc->bitBlt(getLogoTexted(), logoImageRect.loc(),
			   getLogoTexted()->getClipRect());
}

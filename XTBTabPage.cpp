/*
 *  XTBTabPage.cpp
 *  XTBook
 *
 *  Created by Nexhawks on 2/21/11.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#include "XTBTabPage.h"
#include "XTBMainWindow.h"
#include "XTBBrowser.h"
#include "TWiki/TWHTMLElement.h"

class XTBTabPageBrowserDelegate: public XTBBrowserDelegate{
	XTBMainWindow *m_mainWindow;
public:
	XTBTabPageBrowserDelegate(XTBMainWindow *mw):
	m_mainWindow(mw){}
	virtual ~XTBTabPageBrowserDelegate(){}
	virtual void openLinkInNewTab(const std::wstring& url, XTBBrowser *){
		m_mainWindow->openLinkInNewTab(url);
	}
	virtual bool canOpenInNewTab(XTBBrowser *){
		return m_mainWindow->canOpenNewTab();
	}
};

XTBTabPage::XTBTabPage(XTBMainWindow * mw){
	m_mainWindow=mw;
	m_browserDelegate=new XTBTabPageBrowserDelegate(mw);
	m_browser=new XTBBrowser();
	m_browser->setParent(mw);
	m_browser->setId(XTBMainWindowBrowserId);
	m_browser->setDelegate(m_browserDelegate);
	m_animationPhase=XTBTabPageAnimationPhaseNone;
	m_lastTabPos=0;
	m_newTabPos=0;
	
	m_moveDuration=200;
}
XTBTabPage::~XTBTabPage(){
	delete m_browser;
	delete m_browserDelegate;
}

void XTBTabPage::enter(){
	m_active=true;
	layout();
	m_browser->show();
	m_browser->invalidate();
}

void XTBTabPage::leave(){
	m_browser->hide();
	m_active=false;
}

void XTBTabPage::layout(){
	if(!m_active)
		return;
	m_browser->setRect(m_mainWindow->getRectForBrowser());
	m_browser->invalidate();
}

void XTBTabPage::setTabPos(int tabPos, bool animated){
	if(m_newTabPos==tabPos)
		return;
	if(animated){
		m_lastTabPos=getTabPos();
		m_newTabPos=tabPos;
		m_animationStart=twGetTicks();
		m_animationPhase=XTBTabPageAnimationPhaseMoving;
	}else{
		m_newTabPos=tabPos;
		m_animationPhase=XTBTabPageAnimationPhaseNone;
	}
}

void XTBTabPage::moveTabPos(float fromPos, int toPos){
	m_lastTabPos=fromPos;
	m_newTabPos=toPos;
	m_animationStart=twGetTicks();
	m_animationPhase=XTBTabPageAnimationPhaseMoving;
}

float XTBTabPage::getTabPos() const{
	if(isAnimating()){
		twTicks time=twGetTicks()-m_animationStart;
		float per=(float)time/(float)m_moveDuration;
		if(per<.5f)
			per=per*per*2.f;
		else
			per=1.f-(1.f-per)*(1.f-per)*2.f;
		return m_lastTabPos+((float)m_newTabPos-m_lastTabPos)*per;
	}else{
		return (float)m_newTabPos;
	}
}

int XTBTabPage::getFinalTabPos() const{
	return m_newTabPos;
}

bool XTBTabPage::isAnimating() const{
	if(m_animationPhase==XTBTabPageAnimationPhaseMoving){
		twTicks time=twGetTicks()-m_animationStart;
		return time<m_moveDuration;
	}else{
		return false;
	}	
}

std::wstring XTBTabPage::caption() const{
	return m_browser->displayTitle();
}


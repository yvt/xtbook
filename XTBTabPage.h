/*
 *  XTBTabPage.h
 *  XTBook
 *
 *  Created by Nexhawks on 2/21/11.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#pragma once

#include <tcw/twObject.h>
#include <tcw/twApp.h>

class XTBMainWindow;
class XTBBrowser;

enum XTBTabPageAnimationPhase{
	XTBTabPageAnimationPhaseNone=0,
	XTBTabPageAnimationPhaseMoving
};

class XTBTabPageBrowserDelegate;

class XTBTabPage : twObject{
protected:
	XTBMainWindow *m_mainWindow;
	XTBBrowser *m_browser;
	bool m_active;
	float m_lastTabPos;
	int m_newTabPos;
	XTBTabPageAnimationPhase m_animationPhase;
	twTicks m_animationStart;
	twTicks m_moveDuration;
	
	XTBTabPageBrowserDelegate *m_browserDelegate;
	
public:
	XTBTabPage(XTBMainWindow *);
	virtual ~XTBTabPage();
	
	XTBMainWindow *getMainWindow() const{return m_mainWindow;}
	XTBBrowser *getBroser() const{return m_browser;}
	bool isActive() const{return m_active;}
	
	void layout();
	void enter();
	void leave();
	
	
	void setTabPos(int tabPos, bool animated=true);
	float getTabPos() const;
	void moveTabPos(float fromPos, int toPos);
	int getFinalTabPos() const;
	bool isAnimating() const;
	
	std::wstring caption() const;
};

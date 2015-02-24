/*
 *  XTBMainWindow.h
 *  XTBook
 *
 *  Created by Nexhawks on 2/12/11.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#pragma once

#include <tcw/twWnd.h>
#include "XTBTabBar.h"
#include "platform.h"

enum{
	XTBMainWindowTabBarId=1,
	XTBMainWindowBrowserId=2
};

class XTBMainWindow : public twWnd{
protected:
	XTBTabBar m_tabBar;
public:
	XTBMainWindow();
	virtual ~XTBMainWindow();
	
	virtual void clientPaint(const twPaintStruct&);
	virtual void setRect(const twRect&);
	
	twRect getRectForBrowser();
	
	virtual void command(int);
	
	bool canOpenNewTab() const;
	void openLinkInNewTab(const std::wstring& url);
	
	virtual void clientKeyDown(const std::wstring&);
};

void runMainWindow();

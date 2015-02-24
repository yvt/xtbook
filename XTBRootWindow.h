/*
 *  XTBRootWindow.h
 *  XTBook
 *
 *  Created by Nexhawks on 12/29/10.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#pragma once
#include <tcw/twWnd.h>

#include "XTBRootPanel.h"
#include "platform.h"

class XTBMainWindow;


class XTBRootWindow : public twWnd{
private:
	XTBMainWindow *mainWindow;
	XTBRootPanel rootPanel;
	twTimer *initTimer;
	
	void startPhase2();
	void initTimerFired();
	
public:
	XTBRootWindow();
	virtual ~XTBRootWindow();
	
	virtual void setRect(const twRect&);
	virtual void clientPaint(const twPaintStruct&);
	virtual bool clientHitTest(const twPoint&) const;
	virtual void command(int);
	
	void openBook(const XTBSysString&);
	void closeBook();
};

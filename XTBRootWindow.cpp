/*
 *  XTBRootWindow.cpp
 *  XTBook
 *
 *  Created by Nexhawks on 12/29/10.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#include "global.h"
#include <tcw/twApp.h>
#include "XTBRootWindow.h"
#include <tcw/twInvocation.h>
#include <tcw/twEvent.h>
#include <tcw/twMsgBox.h>
#include <tcw/twBaseSkin.h>
#include "XTBMainWindow.h"
#include "platform.h"
#include <tcw/twTimerWithInvocation.h>
#include "commondlg.h"

/** Do the initialization that doesn't interfect with the progress overlay. */
void initThreadSafeComponents();

/** Do the initialization that may interfect with the progress overlay. */
void initThreadUnsafeComponents();

enum{
	XTBControlIDRootPanel=1
};

XTBRootWindow::XTBRootWindow(){
	// setup the root panel.
	rootPanel.setParent(this);
	rootPanel.setId(XTBControlIDRootPanel);
	
	// setup this window to be the desktop.
	mainWindow=NULL;
	setRect(twRect(twPoint(0, 0), tw_app->getScrSize()));
	setTitle(L"XTBook Root Window");
	
	// show the root panel.
	rootPanel.show();
	
	// setup the timer to start the initialization after one second.
	initTimer=new twTimerWithInvocation(new twNoArgumentMemberFunctionInvocation<XTBRootWindow>
										(this,&XTBRootWindow::initTimerFired));
	initTimer->setInterval(600);
	initTimer->addToEvent(tw_event);
	
}

XTBRootWindow::~XTBRootWindow(){
	delete initTimer;
}

void XTBRootWindow::clientPaint(const twPaintStruct& p){
	twDC *dc=p.dc;
	
	// fill the background.
	if(!mainWindow)
		dc->fillRect(0xffffff, getRect());
	
	twRect rt=getRect();
}
bool XTBRootWindow::clientHitTest(const twPoint& pt) const{
	return true;
}

void XTBRootWindow::setRect(const twRect& rt){
	twWnd::setRect(rt);
	twRect rootPanelOldRect=rootPanel.getRect();
	
	// move the root panel so that it comes to the center.
	rootPanel.setRect(twRect((rt.w-rootPanelOldRect.w)/2, (rt.h-rootPanelOldRect.h)/2,
							 rootPanelOldRect.w, rootPanelOldRect.h));
	
	// DEPRECATED: this is the dead code.
	if(mainWindow)
		mainWindow->setRect(rt);
}

void XTBRootWindow::command(int ctrlId){
	if(ctrlId==XTBControlIDRootPanel){
		
	}
}

void XTBRootWindow::startPhase2(){
	// then do left initialization with the progress overlay.
	
	XTBInvokeWithProgressOverlay(new twNoArgumentGlobalFunctionInvocation(initThreadSafeComponents));
	
	// open the main window.
	runMainWindow();
}

void XTBRootWindow::initTimerFired(){
	// stop the timer.
	initTimer->invalidate();
	
	// first do some initialization that interfects with the progress overlay.
	initThreadUnsafeComponents();
	
		
	tw_event->invokeDelayed(new twNoArgumentMemberFunctionInvocation<XTBRootWindow>(this, &XTBRootWindow::startPhase2), 100);
	
	
}


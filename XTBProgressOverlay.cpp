//
//  XTBProgressOverlay.cpp
//  XTBook
//
//  Created by Nexhawks on 4/21/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "XTBProgressOverlay.h"
#include <tcw/twSDLSemaphore.h>
#include <tcw/twLock.h>
#include <tcw/twTimerWithInvocation.h>
#include <tcw/twInvocation.h>
#include <tcw/twSDLDC.h>
#include "xpms/Hourglass.xpm"

twDC *XTBProgressOverlay::m_hourglassIcon;
twDC *XTBProgressOverlay::getHourglassIcon(){
	if(!m_hourglassIcon){
		m_hourglassIcon=twSDLDC::loadFromXPM(Hourglass);
	}
	return m_hourglassIcon;
}

XTBProgressOverlay::XTBProgressOverlay(){
	m_msg=L"...";
	m_msgMutex=new twSDLSemaphore();
	m_timer=NULL;
	m_percentage=-1.f;
	m_segmentPosition=0;
	//setRect(twRect(0, 0, 171, 32));
	setRect(twRect(0, 0, 220, 48));
}

XTBProgressOverlay::~XTBProgressOverlay(){
	stopAnimation();
	delete m_msgMutex;
	
}

void XTBProgressOverlay::startAnimation(){
	if(m_timer)
		return;
	/*
	m_timer=new twTimerWithInvocation(new twNoArgumentMemberFunctionInvocation<XTBProgressOverlay>
									  (this, &XTBProgressOverlay::timerFired));
	m_timer->setInterval(60);
	m_timer->addToEvent(tw_event);*/
}

void XTBProgressOverlay::stopAnimation(){
	/*
	if(!m_timer)
		return;
	m_timer->invalidate();
	delete m_timer;
	m_timer=NULL;*/
}

void XTBProgressOverlay::timerFired(){
	/*
	{
		twLock lock(m_msgMutex);
		if(!m_modified)
			return;
	}
	m_segmentPosition++;
	invalidate();*/
}

void XTBProgressOverlay::clientPaint(const twPaintStruct& p){
	XTBOverlay::clientPaint(p);
	tw_event->sleep(60);
	std::wstring msg;
	float per;
	
	{
		twLock lock(m_msgMutex);
		msg=m_msg;
		per=m_percentage;
		m_modified=false;
	}
	
	twColor color=0xffffff;
	twDC *dc=p.dc;
	
	twRect cliRect=getClientRect();
	cliRect.x=0; cliRect.y=0;
	
	/*
	 NOTE: hourglass icon is disabled...
	 dc->bitBlt(getHourglassIcon(), twPoint(4, 4),
			   twRect(0, 0, 12, 12));
	*/
	
	
	// wrap text.
	msg=getFont()->wrapString(msg, cliRect.w-4);
	
	twPoint msgPos;
	twSize siz=getFont()->measure(msg);
	msgPos.x=4+12;
	msgPos.y=4;
	msgPos.x=(cliRect.w-siz.w)/2;
	msgPos.y=(cliRect.h-siz.h)/2;
	getFont()->render(dc, color, msgPos, msg);
	
	/*
	
	 NOTE: finally, progress bar wasn't used...
	 
	twRect barRect;
	barRect.x=4; barRect.y=cliRect.h/2+2;
	barRect.w=cliRect.w-8; barRect.h=cliRect.h-4-barRect.y;
	
	dc->drawRect(color, twRect(barRect.x, barRect.y, barRect.w-1, barRect.h-1));
	
	twRect segRect;
	segRect.y=barRect.y+2;
	segRect.h=barRect.h-4;
	
	int iPer=(int)(per*20.f);
	int segPos=(m_segmentPosition%24)-4;
	
	for(int i=0;i<20;i++){
		segRect.x=barRect.x+2+(i*8);
		segRect.w=7;
		
		if(iPer>=0){
			if(i>=iPer)
				break;
		}else{
			if(i<segPos || i>=segPos+4)
				continue;
		}
		
		dc->fillRect(color, segRect);
	}*/
	
}

void XTBProgressOverlay::setMessage(const std::wstring& str){
	twLock lock(m_msgMutex);
	m_msg=str;
	if(!m_modified)
		tw_event->invokeOnMainThread(new twNoArgumentMemberFunctionInvocation<twWnd>((twWnd *)this, (void (twWnd::*)())(&twWnd::invalidate)));
	m_modified=true;
}

void XTBProgressOverlay::setPercentage(float value){
	twLock lock(m_msgMutex);
	m_percentage=value;
	if(!m_modified)
		tw_event->invokeOnMainThread(new twNoArgumentMemberFunctionInvocation<twWnd>((twWnd *)this, (void (twWnd::*)())(&twWnd::invalidate)));
	m_modified=true;
}

void XTBProgressOverlay::show(){
	startAnimation();
	XTBOverlay::show();
}

void XTBProgressOverlay::hide(){
	stopAnimation();
	XTBOverlay::hide();
}



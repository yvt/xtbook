//
//  XTBDialogSheet.cpp
//  XTBook
//
//  Created by 河田 智明 on 8/2/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "XTBDialogSheet.h"

#include <tcw/twApp.h>
#include <tcw/twTimerWithInvocation.h>
#include <tcw/twEvent.h>

XTBDialogSheet::XTBDialogSheet(){
	m_needsDimming=true;
	m_appearAnimationTimer=NULL;
	
	twWndStyle style=getStyle();
	style.border=twBS_panel;
	setStyle(style);
	
	// height*0.2
	
	
		
}

XTBDialogSheet::~XTBDialogSheet(){
	stopAppearAnimation();
}

twRect XTBDialogSheet::finalRect() const{
	twSize screenSize=tw_app->getScrSize();
	twSize siz=startupSize();
	twRect rt;
	rt.w=siz.w;
	rt.h=siz.h;
	rt.x=(screenSize.w-rt.w)/2;
	rt.y=screenSize.h-rt.h+1;
	rt.h+=m_paddingHeight;
	return rt;
}

twPoint XTBDialogSheet::getStartupPos(const twSize&,
										  const twRect& scrRect){
	twRect fr=finalRect();
	fr.y=m_startY;
	return fr.loc();
}


void XTBDialogSheet::clientPaint(const twPaintStruct& p){
	twDC *dc=p.dc;
	
	twRect cli=p.boundRect;
	twRect rt=cli;
	
	rt.y=cli.h-m_paddingHeight;
	rt.h=m_paddingHeight;
	dc->fillRect(paddingColor(), rt);
}

#pragma mark - Appear Animations


void XTBDialogSheet::startAppearAnimation(){
	twInvocation *invocation=
	new twNoArgumentMemberFunctionInvocation<XTBDialogSheet>(this, &XTBDialogSheet::updateAppearAnimation);
	m_appearAnimationTimer=new twTimerWithInvocation(invocation);
	m_appearAnimationTimer->setInterval(10);
	m_appearStartTime=twGetTicks();
	m_appearDuration=startupSize().h*600/tw_app->getScrSize().h;
	m_appearAnimationTimer->addToEvent(tw_event);
}

void XTBDialogSheet::stopAppearAnimation(){
	if(m_appearAnimationTimer){
		delete m_appearAnimationTimer;
		m_appearAnimationTimer=NULL;
	}
}

static float transferFunction(float time){
	float v=time*time*(3.f-2.f*time);
	v=.5f+(1.f-time)*(v-.5f);
	return v*2.;
}

void XTBDialogSheet::updateAppearAnimation(){
	twTicks tm=twGetTicks()-m_appearStartTime;
	twRect rt=getRect();
	float per=(float)tm/(float)m_appearDuration;
	
	if(per>1.f){
		// animation ended.
		rt.y=m_endY;
		setRect(rt);
		stopAppearAnimation();
		return;
	}
	
	per=transferFunction(per);
	float currentY;
	currentY=(float)m_startY*(1.f-per)
	+(float)m_endY*per;
	
	rt.y=(int)currentY;
	setRect(rt);
	
}

void XTBDialogSheet::willAppear(){
	m_paddingHeight=startupSize().h/5;
	
		
	m_startY=tw_app->getScrSize().h;
	m_endY=finalRect().y;
	
	twRect rt=finalRect();
	rt.y=m_startY;
	setRect(rt);

	
	this->m_dimmer->setHidingWindow(this);
	startAppearAnimation();
	twDialog::willAppear();
	
	
}

void XTBDialogSheet::didDisappear(){
	stopAppearAnimation();
}
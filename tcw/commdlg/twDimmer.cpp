//
//  twDimmer.cpp
//  XTBook
//
//  Created by Nexhawks on 4/20/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include <tcw/twDimmer.h>
#include <tcw/twApp.h>
#include <tcw/twSDLDC.h>

twDimmer::twDimmer(twDimmerStyle style, twWnd *oldDesktop, twInvocation *invocation){
	twSize scrSize;
	scrSize=tw_app->getScrSize();
	
	setRect(twRect(0, 0, scrSize.w, scrSize.h));
	
	m_dimmedDC=new twSDLDC(scrSize);
	
	twDC *screenDC=tw_app->getDC(getRect());
	m_dimmedDC->bitBlt(screenDC, twPoint(0, 0),
					   getRect());
	if(style==twDS_dark)
		m_dimmedDC->dimRect(getRect());
	delete screenDC;
	
	m_oldDesktop=oldDesktop;
	m_invocation=invocation;
	m_hidingWindow=NULL;
}

twDimmer::~twDimmer(){
	twSetDesktop(m_oldDesktop);
	delete m_dimmedDC;
	if(m_invocation)
		delete m_invocation;
}

void twDimmer::clientMouseDown(const twPoint&, twMouseButton){
	// call cancel callback
	if(m_invocation){
		m_invocation->invoke();
	}
}

void twDimmer::clientPaint(const twPaintStruct& str){
	twDC *dc=str.dc;
	if(m_hidingWindow && m_hidingWindow->isVisible()){
		twRect rt;
		twRect wRect=m_hidingWindow->getRect();
		twRect sRect=getRect();
		wRect=wRect.inflate(-2);
		
		rt=twRect(0, 0, sRect.w, wRect.y);
		dc->bitBlt(m_dimmedDC, rt.loc(), rt);
		
		rt=twRect(0, wRect.y, wRect.x, wRect.h);
		dc->bitBlt(m_dimmedDC, rt.loc(), rt);
		
		rt=twRect(wRect.x+wRect.w, wRect.y, 
				  sRect.w-wRect.x-wRect.w, wRect.h);
		dc->bitBlt(m_dimmedDC, rt.loc(), rt);
		
		rt=twRect(0, wRect.y+wRect.h, 
				  sRect.w, sRect.h-wRect.y-wRect.h);
		dc->bitBlt(m_dimmedDC, rt.loc(), rt);
		
	}else{
		dc->bitBlt(m_dimmedDC, twPoint(0, 0), getRect());
	}
}

bool twDimmer::clientHitTest(const twPoint&) const{
	return true;
}

twDimmer *twDimmer::openDimmer(twDimmerStyle style, twInvocation *iv){
	twDimmer *dimmer=new twDimmer(style, twGetDesktop(), iv);
	twSetDesktop(dimmer);
	return dimmer;
}



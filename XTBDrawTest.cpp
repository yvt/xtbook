//
//  XTBDrawTest.cpp
//  XTBook
//
//  Created by Nexhawks on 4/25/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "XTBDrawTest.h"
#include <tcw/twSDLDC.h>

XTBDrawTest::XTBDrawTest(){
	setNeedsDimming(true);
	setRect(twRect(0, 0, 160, 160)+getBorderSize());
	setTitle(L"Draw");
	
	m_offscreen=new twSDLDC(getClientRect().size());
	m_drag=false;
	m_offscreen->fillRect(twRGB(255, 255, 255),
						  twRect(twPoint(0, 0), getClientRect().size()));
}

XTBDrawTest::~XTBDrawTest(){
	delete m_offscreen;
}

void XTBDrawTest::backgroundTouched(){
	endDialog();
}

void XTBDrawTest::clientMouseDown(const twPoint& pt, twMouseButton button){
	if(button==twMB_left){
		twSetCapture(this);
		m_drag=true;
		m_oldMousePos=pt;
	}
}
void XTBDrawTest::clientMouseMove(const twPoint& mousePos){
	if(m_drag){
		m_offscreen->drawLine(twRGB(0, 0, 0),
							  m_oldMousePos,
							  mousePos);
		invalidate();
		m_oldMousePos=mousePos;
	}
}
void XTBDrawTest::clientMouseUp(const twPoint& pt, twMouseButton button){
	if(button==twMB_left){
		twReleaseCapture();
		m_drag=false;
	}
}

void XTBDrawTest::clientPaint(const twPaintStruct& p){
	twDC *dc=p.dc;
	dc->bitBlt(m_offscreen,twPoint(0, 0), 
			   twRect(twPoint(0, 0), getClientRect().size()));
}

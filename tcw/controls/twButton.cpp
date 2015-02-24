/*
 *  twButton.cpp
 *  
 *
 *  Created by Nexhawks on 10/01/19.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#include <tcw/twButton.h>
#include <tcw/twBaseSkin.h>

using namespace std;

twButton::twButton(){
	m_keyPress=false;
	m_press=false;
	m_hover=false;
}
twButton::~twButton(){
	
}
void twButton::clientKeyDown(const wstring& str){
	if(str==L" "){
		m_keyPress=true; 
		invalidate();
	}else if(str==L"Return"){
		sendCmdToParent();
	}
}
void twButton::clientKeyUp(const wstring& str){
	if(str==L" "){
		m_keyPress=false; 
		invalidate();
		sendCmdToParent();
	}
}
void twButton::clientMouseDown(const twPoint& pt, twMouseButton button){
	m_press=true; m_hover=true;
	twSetCapture(this);
	invalidate();
}
void twButton::clientMouseMove(const twPoint& pt){
	bool ov=m_hover;
	m_hover=((getClientRect()-getClientRect().loc())&&pt);
	if(ov!=m_hover)
		invalidate();
}
void twButton::clientMouseUp(const twPoint& pt, twMouseButton button){
	if(m_press){
		m_press=false;
		
		invalidate();
		twReleaseCapture();
		if(m_hover){
			sendCmdToParent();
		}
	}
}
void twButton::clientMouseEnter(const twPoint& pt){
	m_hover=true;
	invalidate();
}
void twButton::clientMouseLeave(const twPoint& pt){
	m_hover=false;
	invalidate();
}
void twButton::clientPaint(const twPaintStruct& str){
	twRect rt(twPoint(0, 0), str.bound);
	tw_curSkin->drawButton(str.dc, rt, getStatus(),
						   (m_press&&m_hover)||m_keyPress,
						   getTitle(), getFont());
}

bool twButton::clientHitTest(const twPoint& pt) const{
	return true;
}



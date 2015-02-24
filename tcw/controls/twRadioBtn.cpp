/*
 *  twRadioBtn.cpp
 *  
 *
 *  Created by Nexhawks on 10/01/19.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#include <tcw/twRadioBtn.h>
#include <tcw/twBaseSkin.h>

using namespace std;

static void unCheckRadios(twWnd *w){
	twRadioBtn *b;
	if((b=dynamic_cast<twRadioBtn *>(w))){
		b->setCheckState(false);
	}
}

twRadioBtn::twRadioBtn(){
	m_keyPress=false;
	m_press=false;
	m_hover=false;
	m_on=false;
}
twRadioBtn::~twRadioBtn(){
	
}
void twRadioBtn::runCmd(){
	if(m_on)
		return;
	
	
	// uncheck other radiobuttons
	if(m_parent)
		m_parent->enumChildren(unCheckRadios);
		
	m_on=true;
	invalidate();
	sendCmdToParent();
}
void twRadioBtn::clientKeyDown(const wstring& str){
	if(str==L" "){
		m_keyPress=true; 
		invalidate();
	}
}
void twRadioBtn::clientKeyUp(const wstring& str){
	if(str==L" "){
		m_keyPress=false; 
		invalidate();
		runCmd();
	}
}
void twRadioBtn::clientMouseDown(const twPoint& pt, twMouseButton button){
	m_press=true; m_hover=true;
	twSetCapture(this);
	invalidate();
}
void twRadioBtn::clientMouseMove(const twPoint& pt){
	bool ov=m_hover;
	m_hover=((getClientRect()-getClientRect().loc())&&pt);
	if(ov!=m_hover)
		invalidate();
}
void twRadioBtn::clientMouseUp(const twPoint& pt, twMouseButton button){
	if(m_press){
		m_press=false;
		if(m_hover){
			runCmd();
		}
		invalidate();
		twReleaseCapture();
	}
}
void twRadioBtn::clientMouseEnter(const twPoint& pt){
	m_hover=true;
	invalidate();
}
void twRadioBtn::clientMouseLeave(const twPoint& pt){
	m_hover=false;
	invalidate();
}
void twRadioBtn::clientPaint(const twPaintStruct& str){
	twRect rt(twPoint(0, 0), str.bound);
	tw_curSkin->drawRadioBtn(str.dc, rt, getStatus(),
						   (m_press&&m_hover)||m_keyPress,
						   m_on,
						   getTitle(), getFont());
}

bool twRadioBtn::clientHitTest(const twPoint& pt) const{
	return true;
}


bool twRadioBtn::getCheckState() const{
	return m_on;
}

void twRadioBtn::setCheckState(bool v){
	if(v==m_on)
		return;
	m_on=v;
	invalidate();
}




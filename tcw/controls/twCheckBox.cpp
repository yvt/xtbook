/*
 *  twCheckBox.cpp
 *  
 *
 *  Created by Nexhawks on 10/01/19.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#include <tcw/twCheckBox.h>
#include <tcw/twBaseSkin.h>

using namespace std;

twCheckBox::twCheckBox(){
	m_keyPress=false;
	m_press=false;
	m_hover=false;
	m_on=false;
}
twCheckBox::~twCheckBox(){
	
}
void twCheckBox::runCmd(){
	m_on=!m_on;
	invalidate();
	sendCmdToParent();
}
void twCheckBox::clientKeyDown(const wstring& str){
	if(str==L" "){
		m_keyPress=true; 
		invalidate();
	}
}
void twCheckBox::clientKeyUp(const wstring& str){
	if(str==L" "){
		m_keyPress=false; 
		invalidate();
		runCmd();
	}
}
void twCheckBox::clientMouseDown(const twPoint& pt, twMouseButton button){
	m_press=true; m_hover=true;
	twSetCapture(this);
	invalidate();
}
void twCheckBox::clientMouseMove(const twPoint& pt){
	bool ov=m_hover;
	m_hover=((getClientRect()-getClientRect().loc())&&pt);
	if(ov!=m_hover)
		invalidate();
}
void twCheckBox::clientMouseUp(const twPoint& pt, twMouseButton button){
	if(m_press){
		m_press=false;
		if(m_hover){
			runCmd();
		}
		invalidate();
		twReleaseCapture();
	}
}
void twCheckBox::clientMouseEnter(const twPoint& pt){
	m_hover=true;
	invalidate();
}
void twCheckBox::clientMouseLeave(const twPoint& pt){
	m_hover=false;
	invalidate();
}
void twCheckBox::clientPaint(const twPaintStruct& str){
	twRect rt(twPoint(0, 0), str.bound);
	tw_curSkin->drawCheckBox(str.dc, rt, getStatus(),
						   (m_press&&m_hover)||m_keyPress,
						   m_on,
						   getTitle(), getFont());
}

bool twCheckBox::clientHitTest(const twPoint& pt) const{
	return true;
}


bool twCheckBox::getCheckState() const{
	return m_on;
}

void twCheckBox::setCheckState(bool v){
	if(v==m_on)
		return;
	m_on=v;
	invalidate();
}




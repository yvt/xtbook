//
//  XTBLabelMenuItem.cpp
//  XTBook
//
//  Created by Kawada Tomoaki on 5/27/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "XTBLabelMenuItem.h"


XTBLabelMenuItem::XTBLabelMenuItem(){
}

XTBLabelMenuItem::XTBLabelMenuItem(const std::wstring& str){
	m_text=str;
}


void XTBLabelMenuItem::render(twDC *dc, const twRect& rt, 
							   bool selected){
	
	XTBMenuItem::render(dc, rt, selected);
	
	twColor fgColor;
	fgColor=twRGB(255, 255, 255);
	
	m_font->render(dc, fgColor, twPoint(rt.x+1, rt.y+2), m_text);
	m_font->render(dc, fgColor, twPoint(rt.x+1, rt.y+2), m_text);
	
}

int XTBLabelMenuItem::getHeight() const{
	return m_font->measure(m_text).h+4;
}

int XTBLabelMenuItem::getMinWidth() const{
	return m_font->measure(m_text).w+2;
}

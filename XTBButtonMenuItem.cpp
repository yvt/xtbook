//
//  XTBButtonMenuItem.cpp
//  XTBook
//
//  Created by Kawada Tomoaki on 5/19/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "XTBButtonMenuItem.h"
#include <tcw/twBaseSkin.h>

XTBButtonMenuItem::XTBButtonMenuItem(){
}

XTBButtonMenuItem::XTBButtonMenuItem(const std::wstring& str){
	m_text=str;
}


void XTBButtonMenuItem::render(twDC *dc, const twRect& rt, 
						 bool selected){
	
	XTBActionMenuItem::render(dc, rt, selected);
	
	twColor fgColor;
	fgColor=twRGB(255, 255, 255);
	
	if(selected){
		fgColor=tw_curSkin->getSelectionTextColor();
	}

	if(!isEnable())
		fgColor=twRGB(32, 32, 32);
	
	m_font->render(dc, fgColor, twPoint(rt.x+5, rt.y+2), m_text);
	

	m_font->render(dc, fgColor, twPoint(rt.x+5, rt.y+2), m_text);

}

int XTBButtonMenuItem::getHeight() const{
	return m_font->measure(m_text).h+4;
}

int XTBButtonMenuItem::getMinWidth() const{
	return m_font->measure(m_text).w+7;
}

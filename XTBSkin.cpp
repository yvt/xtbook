/*
 *  XTBSkin.cpp
 *  XTBook
 *
 *  Created by Nexhawks on 2/11/11.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#include <tcw/twTypes.h>
#include "XTBSkin.h"
#include <tcw/twSDLDC.h>
#include "xpms/Glass.xpm"
#include "xpms/CheckBox.xpm"
using namespace std;

XTBSkin::XTBSkin(){
	m_glass=twSDLDC::loadFromXPM(Glass);
	m_checkBox=twSDLDC::loadFromXPM(CheckBox);
}
XTBSkin::~XTBSkin(){
	delete m_glass;
	delete m_checkBox;
}

void XTBSkin::drawSolidBorder(twDC *dc, const twRect& rt, const twWndStatus&, const wstring&){
	drawEdge(dc, rt, twES_solid);
}
bool XTBSkin::hitTestSolidBorder(const twRect& rt, const twPoint& pt){
	return true;
}
twRect XTBSkin::getSolidBorderCliRect(const twRect& rt){
	return twRect(rt.x+1, rt.y+1, rt.w-2, rt.h-2);
}

void XTBSkin::drawPanelBorder(twDC *dc, const twRect& rt, const twWndStatus& s, const wstring&){
	//twColor ctrl=getCtrlColor();
	twColor col;
	int x1=rt.x, x2=rt.x+rt.w-1;
	int y1=rt.y, y2=rt.y+rt.h-1;
	
	if(s.focused)
		col=getActiveTitleColor();
	else
		col=getInactiveTitleColor();
	
	dc->drawLine(col, twPoint(x1+1, y1), twPoint(x2-1, y1));
	dc->drawLine(col, twPoint(x1, y1+1), twPoint(x1, y2-1));
	dc->drawLine(col  , twPoint(x2, y1+1), twPoint(x2, y2-1));
	dc->drawLine(col  , twPoint(x1+1, y2), twPoint(x2-1, y2));
	dc->drawRect(getWndColor(), twRect(rt.x+1, rt.y+1, rt.w-3, rt.h-3));
}
bool XTBSkin::hitTestPanelBorder(const twRect& rt, const twPoint& pt){
	int x1=rt.x, x2=rt.x+rt.w-1;
	int y1=rt.y, y2=rt.y+rt.h-1;
	if((pt.x==x1 || pt.x==x2) && (pt.y==y1 || pt.y==y2))
		return false;
	return true;
}
twRect XTBSkin::getPanelBorderCliRect(const twRect& rt){
	return twRect(rt.x+2, rt.y+2, rt.w-4, rt.h-4);
}

void XTBSkin::drawDialogBorder(twDC *dc, const twRect& rt, const twWndStatus& s, const wstring& str){
	twRect rt2;
	
	//twColor ctrl=getCtrlColor();
	twColor dark=getDarkCtrlColor();
	//twColor dd=getDarkDarkCtrlColor();
	//twColor bright=getBrightCtrlColor();
	int x1=rt.x, x2=rt.x+rt.w-1;
	int y1=rt.y, y2=rt.y+rt.h-1;
	dc->drawLine(dark  , twPoint(x1+1, y1), twPoint(x2-1, y1));
	dc->drawLine(dark  , twPoint(x1, y1+1), twPoint(x1, y2-1));
	dc->drawLine(dark  , twPoint(x2, y1+1), twPoint(x2, y2-1));
	dc->drawLine(dark  , twPoint(x1+1, y2), twPoint(x2-1, y2));
	
	dc->drawRect(getCtrlColor(), twRect(rt.x+1, rt.y+1, rt.w-3, rt.h-3));
	
	rt2=twRect(rt.x+2, rt.y+2, rt.w-4, getTitleHeight());
	if(s.focused){
		dc->fillRect(getActiveTitleColor(), rt2);
		tw_defFont->render(dc, getActiveTitleTextColor(), twPoint(rt2.x+2, rt2.y+1), str);
	}else{
		dc->fillRect(getInactiveTitleColor(), rt2);
		tw_defFont->render(dc, getInactiveTitleTextColor(), twPoint(rt2.x+2, rt2.y+1), str);
	}
}
bool XTBSkin::hitTestDialogBorder(const twRect& rt, const twPoint& pt){
	int x1=rt.x, x2=rt.x+rt.w-1;
	int y1=rt.y, y2=rt.y+rt.h-1;
	if((pt.x==x1 || pt.x==x2) && (pt.y==y1 || pt.y==y2))
		return false;
	return true;
}
twRect XTBSkin::getDialogBorderCliRect(const twRect& rt){
	return twRect(rt.x+2, rt.y+2+getTitleHeight(), rt.w-4, rt.h-4-getTitleHeight());
}

void XTBSkin::drawOverlappedBorder(twDC * dc, const twRect& rt, const twWndStatus& s, const wstring& str){
	twRect rt2;
	
	//twColor ctrl=getCtrlColor();
	twColor dark=getDarkCtrlColor();
	//twColor dd=getDarkDarkCtrlColor();
	twColor bright=getBrightCtrlColor();
	int x1=rt.x, x2=rt.x+rt.w-1;
	int y1=rt.y, y2=rt.y+rt.h-1;
	dc->drawLine(bright, twPoint(x1+1, y1), twPoint(x2-1, y1));
	dc->drawLine(bright, twPoint(x1, y1+1), twPoint(x1, y2-1));
	dc->drawLine(dark  , twPoint(x2, y1+1), twPoint(x2, y2-1));
	dc->drawLine(dark  , twPoint(x1+1, y2), twPoint(x2-1, y2));
	
	dc->drawRect(getCtrlColor(), twRect(rt.x+1, rt.y+1, rt.w-3, rt.h-3));
	dc->drawRect(getCtrlColor(), twRect(rt.x+2, rt.y+2, rt.w-5, rt.h-5));
	dc->drawRect(getCtrlColor(), twRect(rt.x+3, rt.y+3, rt.w-7, rt.h-7));
	
	rt2=twRect(rt.x+4, rt.y+4, rt.w-8, getTitleHeight());
	if(s.focused){
		dc->fillRect(getActiveTitleColor(), rt2);
		tw_defFont->render(dc, getActiveTitleTextColor(), twPoint(rt2.x+2, rt2.y+1), str);
	}else{
		dc->fillRect(getInactiveTitleColor(), rt2);
		tw_defFont->render(dc, getInactiveTitleTextColor(), twPoint(rt2.x+2, rt2.y+1), str);
	}
}
bool XTBSkin::hitTestOverlappedBorder(const twRect& rt, const twPoint& pt){
	int x1=rt.x, x2=rt.x+rt.w-1;
	int y1=rt.y, y2=rt.y+rt.h-1;
	if((pt.x==x1 || pt.x==x2) && (pt.y==y1 || pt.y==y2))
		return false;
	return true;
}
twRect XTBSkin::getOverlappedBorderCliRect(const twRect& rt){
	return twRect(rt.x+4, rt.y+4+getTitleHeight(), rt.w-8, rt.h-8-getTitleHeight());
}

void XTBSkin::drawButton(twDC *dc, const twRect& rt, const twWndStatus& s, bool pressed, const std::wstring& str, const twFont *fnt){
	
	//twColor ctrl=getCtrlColor();
	twColor dark=twRGB(180, 180, 180);
	twColor dd=twRGB(140, 140, 140);
	//twColor bright=getBrightCtrlColor();
	
	//dc->fillRect(ctrl, rt);
	
	twRect fill(rt.x+1, rt.y+1, rt.w-2, rt.h-2);
	for(int x=0;x<fill.w;x+=32){
		twRect srcRect;
		if(pressed)
			srcRect.x=32;
		else
			srcRect.x=0;
		srcRect.y=0;
		srcRect.w=MIN(32, fill.w-x);
		srcRect.h=MIN(68, fill.h);
		dc->bitBlt(m_glass, twPoint(fill.x+x, fill.y), srcRect);
	}
	
	
	dc->drawRect(dark, twRect(rt.x, rt.y, rt.w-1, rt.h-1));
	
	if(s.focused){
		drawFocusRect(dc, twRect(rt.x+2, rt.y+2, rt.w-5, rt.h-5));
	}
	
	{
		int x1=rt.x, x2=rt.x+rt.w-1;
		int y1=rt.y, y2=rt.y+rt.h-1;
		twColor color=dd;
		dc->drawLine(color, twPoint(x1+1, y1), twPoint(x2-1, y1));
		dc->drawLine(color, twPoint(x1, y1+1), twPoint(x1, y2-1));
		dc->drawLine(color, twPoint(x2, y1+1), twPoint(x2, y2-1));
		dc->drawLine(color, twPoint(x1+1, y2), twPoint(x2-1, y2));
	}
	
	
	/*
	{
	
		int x1=rt.x+1, x2=rt.x+rt.w-2;
		int y1=rt.y+1, y2=rt.y+rt.h-2;
		
		if(pressed){
			dc->drawLine(dark, twPoint(x1  , y1), twPoint(x2  , y1));
			dc->drawLine(dark, twPoint(x1, y1  ), twPoint(x1, y2  ));
			
		}else{
			dc->drawLine(bright, twPoint(x1  , y1), twPoint(x2  , y1));
			dc->drawLine(bright, twPoint(x1, y1  ), twPoint(x1, y2  ));
			dc->drawLine(dark  , twPoint(x2, y1+1), twPoint(x2, y2  ));
			dc->drawLine(dark  , twPoint(x1  , y2), twPoint(x2  , y2));
		}
		
	}*/
	/*if(s.focused){
		drawFocusRect(dc, twRect(rt.x+3, rt.y+3, rt.w-7, rt.h-7));
	}*/
	twSize sz;
	sz=fnt->measure(str);
	if(s.enable)
		fnt->render(dc, getCtrlTextColor(), twPoint(rt.x+(rt.w-sz.w)/2, rt.y+(rt.h-sz.h)/2)+
					(pressed?twPoint(1, 1):twPoint(0, 0)), str);
	else{
		fnt->render(dc, getBrightCtrlColor(), twPoint(rt.x+(rt.w-sz.w)/2, rt.y+(rt.h-sz.h)/2)+
					twPoint(1, 1), str);
		fnt->render(dc, getDarkCtrlColor(), twPoint(rt.x+(rt.w-sz.w)/2, rt.y+(rt.h-sz.h)/2)+
					twPoint(0, 0), str);
	}
}

void XTBSkin::drawFocusRect(twDC *dc, const twRect& rt){
	dc->drawRect(getDarkCtrlColor(), rt);
}

twColor XTBSkin::getActiveTitleColor(){
	return twRGB(89, 123, 194);
}
twColor XTBSkin::getActiveTitleTextColor(){
	return twRGB(255, 255, 255);
}
twColor XTBSkin::getInactiveTitleColor(){
	return twRGB(128, 128, 128);
}
twColor XTBSkin::getInactiveTitleTextColor(){
	return twRGB(192, 192, 192);
}
twColor XTBSkin::getCtrlColor(){
	return twRGB(238, 238, 238);
}
twColor XTBSkin::getSelectionColor(){
	return twRGB(0, 110, 186);
}
twColor XTBSkin::getSelectionTextColor(){
	return twRGB(255, 255, 255);
}

void XTBSkin::drawCheckBox(twDC *dc, const twRect& rt, const twWndStatus& s, bool pressed, bool on, const std::wstring& str, const twFont *fnt){
	twRect rt2;
	int siz=12;
	rt2=twRect(rt.x, rt.y+(rt.h-siz)/2, siz, siz);
	
	twRect srcRect;
	srcRect.x=(pressed || !s.enable)?12:0;
	srcRect.y=on?12:0;
	srcRect.w=siz;
	srcRect.h=siz;
	
	dc->bitBlt(m_checkBox, rt2.loc(), srcRect);
	
	siz+=3;
	
	twSize sz;
	sz=fnt->measure(str);
	if(s.enable)
		fnt->render(dc, getCtrlTextColor(), twPoint(rt.x+siz, rt.y+(rt.h-sz.h)/2)+
					(twPoint(0, 0)), str);
	else{
		fnt->render(dc, getBrightCtrlColor(), twPoint(rt.x+siz, rt.y+(rt.h-sz.h)/2)+
					twPoint(1, 1), str);
		fnt->render(dc, getDarkCtrlColor(), twPoint(rt.x+siz, rt.y+(rt.h-sz.h)/2)+
					twPoint(0, 0), str);
	}
	if(s.focused){
		drawFocusRect(dc, twRect(rt.x+siz, rt.y+(rt.h-sz.h)/2, sz.w, sz.h));
	}
	
}




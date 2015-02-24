/*
 *  twBasicSkin.cpp
 *  
 *
 *  Created by Nexhawks on 10/01/16.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#include <cmath>
#include <tcw/twBaseSkin.h>
#include <tcw/twWnd.h>

using namespace std;

twBaseSkin *tw_curSkin;

void twInitSkin();
void twExitSkin();

void twInitSkin(){
	tw_curSkin=new twBaseSkin();
}

void twExitSkin(){
	delete tw_curSkin;
}

int twBaseSkin::getTitleHeight(){
	return tw_defFont->measure(L"").h;
}

twBaseSkin::twBaseSkin(){
	
}

twBaseSkin::~twBaseSkin(){
	
}

void twBaseSkin::drawEdge(twDC *dc, const twRect& rt, twEdgeStyle es){
	twColor ctrl=getCtrlColor();
	twColor dark=getDarkCtrlColor();
	twColor dd=getDarkDarkCtrlColor();
	twColor bright=getBrightCtrlColor();
	twColor bd=getBorderColor();
	int x1=rt.x, x2=rt.x+rt.w-1;
	int y1=rt.y, y2=rt.y+rt.h-1;
	if(es==twES_raised){
		dc->drawLine(ctrl,  twPoint(x1, y1), twPoint(x2, y1));
		dc->drawLine(ctrl,  twPoint(x1, y1), twPoint(x1, y2));
		dc->drawLine(dd,    twPoint(x2, y2), twPoint(x2, y1));
		dc->drawLine(dd,    twPoint(x2, y2), twPoint(x1, y2));
		dc->drawLine(dark,  twPoint(x2-1, y2-1), twPoint(x2-1, y1+1));
		dc->drawLine(dark,  twPoint(x2-1, y2-1), twPoint(x1+1, y2-1));
		dc->drawLine(bright,twPoint(x1+1, y1+1), twPoint(x2-1, y1+1));
		dc->drawLine(bright,twPoint(x1+1, y1+1), twPoint(x1+1, y2-1));
	}else if(es==twES_sunken){
		dc->drawLine(dd,    twPoint(x1, y1), twPoint(x2, y1));
		dc->drawLine(dd,    twPoint(x1, y1), twPoint(x1, y2));
		dc->drawLine(ctrl,  twPoint(x2, y2), twPoint(x2, y1));
		dc->drawLine(ctrl,  twPoint(x2, y2), twPoint(x1, y2));
		dc->drawLine(bright,twPoint(x2-1, y2-1), twPoint(x2-1, y1+1));
		dc->drawLine(bright,twPoint(x2-1, y2-1), twPoint(x1+1, y2-1));
		dc->drawLine(dark  ,twPoint(x1+1, y1+1), twPoint(x2-1, y1+1));
		dc->drawLine(dark  ,twPoint(x1+1, y1+1), twPoint(x1+1, y2-1));
	}else if(es==twES_bumped){
		dc->drawLine(dark,  twPoint(x2, y2), twPoint(x2, y1));
		dc->drawLine(dark,  twPoint(x2, y2), twPoint(x1, y2));
		dc->drawLine(bright,twPoint(x1, y1), twPoint(x2, y1));
		dc->drawLine(bright,twPoint(x1, y1), twPoint(x1, y2));
		dc->drawLine(bright,twPoint(x2-1, y2-1), twPoint(x2-1, y1+1));
		dc->drawLine(bright,twPoint(x2-1, y2-1), twPoint(x1+1, y2-1));
		dc->drawLine(dark  ,twPoint(x1+1, y1+1), twPoint(x2-1, y1+1));
		dc->drawLine(dark  ,twPoint(x1+1, y1+1), twPoint(x1+1, y2-1));
	}else if(es==twES_relief){
		dc->drawLine(dark  ,twPoint(x1, y1), twPoint(x2, y1));
		dc->drawLine(dark  ,twPoint(x1, y1), twPoint(x1, y2));
		dc->drawLine(bright,  twPoint(x2, y2), twPoint(x2, y1));
		dc->drawLine(bright,  twPoint(x2, y2), twPoint(x1, y2));
		dc->drawLine(bright,twPoint(x1+1, y1+1), twPoint(x2-1, y1+1));
		dc->drawLine(bright,twPoint(x1+1, y1+1), twPoint(x1+1, y2-1));
		dc->drawLine(dark  ,twPoint(x2-1, y2-1), twPoint(x2-1, y1+1));
		dc->drawLine(dark  ,twPoint(x2-1, y2-1), twPoint(x1+1, y2-1));
	}else if(es==twES_solid){
		dc->drawLine(bd,    twPoint(x1, y1), twPoint(x2, y1));
		dc->drawLine(bd,    twPoint(x1, y1), twPoint(x1, y2));
		dc->drawLine(bd,    twPoint(x2, y2), twPoint(x2, y1));
		dc->drawLine(bd,    twPoint(x2, y2), twPoint(x1, y2));
	}else if(es==twES_double){
		dc->drawLine(bd,    twPoint(x1, y1), twPoint(x2, y1));
		dc->drawLine(bd,    twPoint(x1, y1), twPoint(x1, y2));
		dc->drawLine(bd,    twPoint(x2, y2), twPoint(x2, y1));
		dc->drawLine(bd,    twPoint(x2, y2), twPoint(x1, y2));
		dc->drawLine(bd    ,twPoint(x1+1, y1+1), twPoint(x2-1, y1+1));
		dc->drawLine(bd    ,twPoint(x1+1, y1+1), twPoint(x1+1, y2-1));
		dc->drawLine(bd    ,twPoint(x2-1, y2-1), twPoint(x2-1, y1+1));
		dc->drawLine(bd    ,twPoint(x2-1, y2-1), twPoint(x1+1, y2-1));
	}else if(es==twES_dotted){
		int x, y;
		for(x=x1;x<=x2;x+=2){
			dc->drawPoint(bd, twPoint(x, y1));
			dc->drawPoint(bd, twPoint(x, y2));
		}
		for(y=y1;y<=y2;y+=2){
			dc->drawPoint(bd, twPoint(x1, y));
			dc->drawPoint(bd, twPoint(x2, y));
		}
	}
}

void twBaseSkin::drawFocusRect(twDC *dc, const twRect& rt){
	int x, y;
	twColor col=twRGB(0, 0, 0);
	for(x=rt.x;x<=rt.x+rt.w;x+=2){
		dc->drawPoint(col, twPoint(x, rt.y));
		dc->drawPoint(col, twPoint(x, rt.y+rt.h));
	}
	for(y=rt.y;y<=rt.y+rt.h;y+=2){
		dc->drawPoint(col, twPoint(rt.x, y));
		dc->drawPoint(col, twPoint(rt.x+rt.w, y));
	}
}

void twBaseSkin::drawSolidBorder(twDC *dc, const twRect& rt, const twWndStatus&, const wstring&){
	drawEdge(dc, rt, twES_solid);
}
bool twBaseSkin::hitTestSolidBorder(const twRect& rt, const twPoint& pt){
	return true;
}
twRect twBaseSkin::getSolidBorderCliRect(const twRect& rt){
	return twRect(rt.x+1, rt.y+1, rt.w-2, rt.h-2);
}

void twBaseSkin::drawPanelBorder(twDC *dc, const twRect& rt, const twWndStatus&, const wstring&){
	drawEdge(dc, rt, twES_sunken);
}
bool twBaseSkin::hitTestPanelBorder(const twRect&, const twPoint&){
	return true;
}
twRect twBaseSkin::getPanelBorderCliRect(const twRect& rt){
	return twRect(rt.x+2, rt.y+2, rt.w-4, rt.h-4);
}

void twBaseSkin::drawDialogBorder(twDC *dc, const twRect& rt, const twWndStatus& s, const wstring& str){
	twRect rt2;
	
	drawEdge(dc, rt, twES_raised);
	dc->drawRect(getCtrlColor(), twRect(rt.x+2, rt.y+2, rt.w-5, rt.h-5));
	
	rt2=twRect(rt.x+3, rt.y+3, rt.w-6, getTitleHeight());
	if(s.focused){
		dc->fillRect(getActiveTitleColor(), rt2);
		tw_defFont->render(dc, getActiveTitleTextColor(), twPoint(rt2.x+2, rt2.y+1), str);
	}else{
		dc->fillRect(getInactiveTitleColor(), rt2);
		tw_defFont->render(dc, getInactiveTitleTextColor(), twPoint(rt2.x+2, rt2.y+1), str);
	}
}
bool twBaseSkin::hitTestDialogBorder(const twRect&, const twPoint&){
	return true;
}
twRect twBaseSkin::getDialogBorderCliRect(const twRect& rt){
	return twRect(rt.x+3, rt.y+3+getTitleHeight(), rt.w-6, rt.h-6-getTitleHeight());
}

void twBaseSkin::drawOverlappedBorder(twDC * dc, const twRect& rt, const twWndStatus& s, const wstring& str){
	twRect rt2;
	
	drawEdge(dc, rt, twES_raised);
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
bool twBaseSkin::hitTestOverlappedBorder(const twRect&, const twPoint&){
	return true;
}
twRect twBaseSkin::getOverlappedBorderCliRect(const twRect& rt){
	return twRect(rt.x+4, rt.y+4+getTitleHeight(), rt.w-8, rt.h-8-getTitleHeight());
}

void twBaseSkin::drawButton(twDC *dc, const twRect& rt, const twWndStatus& s, bool pressed, const std::wstring& str, const twFont *fnt){
	dc->fillRect(getCtrlColor(), rt);
	if(pressed){
		drawEdge(dc, rt, twES_sunken);
	}else{
		drawEdge(dc, rt, twES_raised);
	}
	if(s.focused){
		drawFocusRect(dc, twRect(rt.x+3, rt.y+3, rt.w-7, rt.h-7));
	}
	twSize sz;
	sz=fnt->measure(str);
	if(s.enable)
		fnt->render(dc, getCtrlTextColor(), twPoint(rt.x+(rt.w-sz.w)/2, rt.y+(rt.h-sz.h)/2)+
					(pressed?twPoint(2, 2):twPoint(0, 0)), str);
	else{
		fnt->render(dc, getBrightCtrlColor(), twPoint(rt.x+(rt.w-sz.w)/2, rt.y+(rt.h-sz.h)/2)+
					twPoint(1, 1), str);
		fnt->render(dc, getDarkCtrlColor(), twPoint(rt.x+(rt.w-sz.w)/2, rt.y+(rt.h-sz.h)/2)+
					twPoint(0, 0), str);
	}
}

void twBaseSkin::drawRadioBtn(twDC *dc, const twRect& rt, const twWndStatus& s, bool pressed, bool on, const std::wstring& str, const twFont *fnt){
	twRect rt2, rt3, rt4;
	int siz=fnt->measure(L"A").h;
	rt2=twRect(rt.x, rt.y+(rt.h-siz)/2, siz, siz);
	rt3=twRect(rt2.x+1, rt2.y+1, rt2.w-2, rt2.h-2);
	rt4=twRect(rt2.x+2, rt2.y+2, rt2.w-4, rt2.h-4);
	
	
	twRect oclip=dc->getClipRect();
	int siz2=(int)((float)siz*(sqrtf(2.f)/2.f+1.f)/2.f);
	int siz3=(int)((float)(siz-2)*(sqrtf(2.f)/2.f+1.f)/2.f);
	twRect clp2=twRect(rt2.x, rt2.y, siz2, siz2);
	twRect clp3=twRect(rt3.x, rt3.y, siz3, siz3);
	
	dc->fillEllipse(getCtrlColor(), rt2);
	dc->addClipRect(clp2);
	dc->fillEllipse(getDarkCtrlColor(), rt2);
	
	dc->setClipRect(oclip);
	dc->fillEllipse(getBrightCtrlColor(), rt3);
	dc->addClipRect(clp3);
	dc->fillEllipse(getDarkDarkCtrlColor(), rt3);
	dc->setClipRect(oclip);
	
	if(pressed || !s.enable){
		dc->fillEllipse(getCtrlColor(), rt4);
	}else{
		dc->fillEllipse(getWndColor(), rt4);
	}
	
	twColor col=s.enable?getCtrlTextColor():getDarkCtrlColor();
	
	int bsiz=siz/2;
	twRect rt5;
	rt5=twRect(rt2.x+(rt2.w-bsiz)/2, rt2.y+(rt2.w-bsiz)/2, bsiz, bsiz);
	
	if(on)
		dc->fillEllipse(col, rt5);
	
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

void twBaseSkin::drawCheckBox(twDC *dc, const twRect& rt, const twWndStatus& s, bool pressed, bool on, const std::wstring& str, const twFont *fnt){
	twRect rt2;
	int siz=fnt->measure(L"A").h;
	rt2=twRect(rt.x, rt.y+(rt.h-siz)/2, siz, siz);
	
	if(pressed || !s.enable){
		dc->fillRect(getCtrlColor(), rt2);
	}else{
		dc->fillRect(getWndColor(), rt2);
	}
	
	drawEdge(dc, rt2, twES_sunken);
	
	twColor col=s.enable?getCtrlTextColor():getDarkCtrlColor();
	
	int bsiz=siz/2;
	twRect rt3;
	rt3=twRect(rt2.x+(rt2.w-bsiz)/2, rt2.y+(rt2.w-bsiz)/2, bsiz, bsiz);
	
	if(on)
		dc->fillRect(col, rt3);
	
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

twColor twBaseSkin::getBorderColor(){
	return twRGB(0, 0, 0);
}
twColor twBaseSkin::getWndColor(){
	return twRGB(255, 255, 255);
}
twColor twBaseSkin::getCtrlColor(){
	return twRGB(192, 192, 192);
}
twColor twBaseSkin::getWndTextColor(){
	twColor c=getWndColor();
	if(twGetR(c)+twGetG(c)*2+twGetB(c)>512)
		return twRGB(0, 0, 0);
	else
		return twRGB(255, 255, 255);
}
twColor twBaseSkin::getCtrlTextColor(){
	twColor c=getCtrlColor();
	if(twGetR(c)+twGetG(c)*2+twGetB(c)>512)
		return twRGB(0, 0, 0);
	else
		return twRGB(255, 255, 255);
}
twColor twBaseSkin::getSelectionColor(){
	return twRGB(0, 0, 128);
}
twColor twBaseSkin::getSelectionTextColor(){
	return twRGB(255, 255, 255);
}
twColor twBaseSkin::getDisabledWndTextColor(){
	return twRGB(128, 128, 128);
}
twColor twBaseSkin::getDisabledCtrlTextColor(){
	return twRGB(128, 128, 128);
}
twColor twBaseSkin::getDarkCtrlColor(){
	twColor c=getCtrlColor();
	return (c&0xfefefe)>>1;
}
twColor twBaseSkin::getDarkDarkCtrlColor(){
	twColor c=getDarkCtrlColor();
	return (c&0xfcfcfc)>>2;
}
twColor twBaseSkin::getBrightCtrlColor(){
	twColor c=getCtrlColor();
	return 0xffffff-(((0xffffff-c)&0xfcfcfc)>>2);
}

twColor twBaseSkin::getActiveTitleColor(){
	return twRGB(0, 0, 128);
}
twColor twBaseSkin::getActiveTitleTextColor(){
	return twRGB(255, 255, 255);
}
twColor twBaseSkin::getInactiveTitleColor(){
	return twRGB(128, 128, 128);
}
twColor twBaseSkin::getInactiveTitleTextColor(){
	return twRGB(192, 192, 192);
}





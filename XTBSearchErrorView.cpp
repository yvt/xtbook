//
//  XTBSearchErrorView.cpp
//  XTBook
//
//  Created by Kawada Tomoaki on 6/1/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "XTBSearchErrorView.h"
#include <tcw/twDC.h>
#include <tcw/twBaseSkin.h>

XTBSearchErrorView::XTBSearchErrorView(){}

XTBSearchErrorView::~XTBSearchErrorView(){
}

void XTBSearchErrorView::clientPaint(const twPaintStruct& p){
	std::wstring text=getTitle();
	twRect rt(twPoint(0, 0), getClientRect().size());
	const twFont *font=getFont();
	
	if(text.empty())
		return;
	
	// wrap the text, if needed.
	if(font->measure(text).w>rt.w){
		text=font->wrapString(text, rt.w);
	}
	
	twPoint pt;
	twSize siz=font->measure(text);
	
	// center the text.
	pt.x=(rt.w-siz.w)/2;
	pt.y=(rt.h-siz.h)/2;
	
	// draw the message.
	twColor col=tw_curSkin->getDisabledWndTextColor();
	font->render(p.dc, col, pt, text);
}

bool XTBSearchErrorView::clientHitTest(const twPoint& pt) const{
	return true;
}

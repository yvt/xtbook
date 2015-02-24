//
//  XTBOverlay.cpp
//  XTBook
//
//  Created by Nexhawks on 4/21/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "XTBOverlay.h"

XTBOverlay::XTBOverlay(){
	setNeedsDimming(true);
	setDimmerStyle(twDS_normal);
	{
		twWndStyle style=getStyle();
		style.border=twBS_none;
		setStyle(style);
	}
}

XTBOverlay::~XTBOverlay(){
	
}

void XTBOverlay::clientPaint(const twPaintStruct& p) {
	twDC *dc=p.dc;
	twRect rt=getClientRect();
	rt.x=0; rt.y=0;
	dc->dimRect(twRect(0, 1, rt.w, rt.h-2));
	dc->dimRect(twRect(1, 0, rt.w-2, 1));
	dc->dimRect(twRect(1, rt.h-1, rt.w-2, 1));
	
}

bool XTBOverlay::clientHitTest(const twPoint& pt) const{
	twRect rt=getClientRect();
	if((pt.x==0 || pt.x==rt.w-1) && 
	   (pt.y==0 || pt.y==rt.h-1))
		return false;
	return true;
}


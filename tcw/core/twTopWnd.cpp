/*
 *  twTopWnd.cpp
 *  
 *
 *  Created by Nexhawks on 10/01/30.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#include <tcw/twTopWnd.h>
#include <tcw/twBaseSkin.h>

twTopWnd::twTopWnd(){
	setParent(twGetDesktop());
	twWndStyle style=getStyle();
	style.border=twBS_overlapped;
	setStyle(style);
}

void twTopWnd::clientPaint(const twPaintStruct& str){
	twDC *dc=str.dc;
	dc->clear(tw_curSkin->getCtrlColor());
}

bool twTopWnd::clientHitTest(const twPoint&) const{
	return true;
}


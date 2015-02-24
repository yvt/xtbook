/*
 *  twLabel.cpp
 *  
 *
 *  Created by Nexhawks on 10/01/19.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#include <tcw/twLabel.h>
#include <tcw/twBaseSkin.h>

twLabel::twLabel(){
}

twLabel::~twLabel(){
}

void twLabel::clientPaint(const twPaintStruct& s){
	m_font->render(s.dc, tw_curSkin->getCtrlTextColor(), twPoint(0, 0),
				   getTitle());
}

bool twLabel::clientHitTest(const twPoint& pt) const{
	return true;
}

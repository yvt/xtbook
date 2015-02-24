//
//  XTBMenuItem.cpp
//  XTBook
//
//  Created by Kawada Tomoaki on 5/19/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include <tcw/twWnd.h>
#include <tcw/twBaseSkin.h>

#include "XTBMenuItem.h"

XTBMenuItem::XTBMenuItem(){
	m_font=tw_defFont;
}

XTBMenuItem::~XTBMenuItem(){
}



bool XTBMenuItem::isSelectable() const{
	return false;
}

void XTBMenuItem::render(twDC *dc, const twRect& rt, 
						 bool selected){
	
	if(selected){
		dc->fillRect(tw_curSkin->getSelectionColor(),
					 rt);
	}
	
}


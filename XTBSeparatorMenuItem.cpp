//
//  XTBSeparatorMenuItem.cpp
//  XTBook
//
//  Created by Kawada Tomoaki on 5/20/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "XTBSeparatorMenuItem.h"

XTBSeparatorMenuItem::XTBSeparatorMenuItem(){
	
}

int XTBSeparatorMenuItem::getHeight() const{
	return 5;
}
int XTBSeparatorMenuItem::getMinWidth() const{
	return 3;
}

void XTBSeparatorMenuItem::render(twDC *dc, const twRect& rt, bool){
	//twColor color=twRGB(0, 0, 0);
	dc->dimRect(twRect(1, 2, rt.w-2, 1)+rt.loc());
}

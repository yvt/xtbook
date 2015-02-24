/*
 *  XTBRootPanel.cpp
 *  XTBook
 *
 *  Created by Nexhawks on 2/12/11.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#include "XTBRootPanel.h"
#include "XTBAboutDialog.h"
#include <tcw/twSDLDC.h>
#include "XTBTestWindow.h"
#include "xpms/HomeScreen.xpm"
#include "i18n.h"

enum{
	XTBControlIDOpen=1,
	XTBControlIDAbout=2,
	XTBControlIDDebug=3
};


XTBRootPanel::XTBRootPanel(){
	
	// set the default size.
	setRect(twRect(0,0,320, 240));
	
	// load the icon of XTBook.
	homeDC=twSDLDC::loadFromXPM(HomeScreen);
	
}

XTBRootPanel::~XTBRootPanel(){
	// release the memory for the icon.
	delete homeDC;
}

void XTBRootPanel::clientPaint(const twPaintStruct& p){
	twDC *dc=p.dc;
	
	// draw the icon of XTBook.
	dc->bitBlt(homeDC, twPoint((320-135)/2, (240-161)/2), twRect(0,0,320,240));
}




void XTBRootPanel::command(int){
	// no-op.
}

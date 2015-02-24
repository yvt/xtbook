/*
 *  XTBAboutDialog.cpp
 *  XTBook
 *
 *  Created by Nexhawks on 2/12/11.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#include "XTBAboutDialog.h"
#include <tcw/twSDLDC.h>
#include "xpms/Nexhawks.xpm"

enum{
	XTBControlIDClose=1
};

XTBAboutDialog::XTBAboutDialog(){
	// setup the message label.
	messageLabel.setParent(this);
	messageLabel.setTitle(L"XTBook development version\n"
						  "Copyright 2011 Nexhawks.\n"
						  "Visit http://www.nexhawks.net/");
	messageLabel.setRect(twRect(84, 13, 160, 50));
	
	// setup the close button.
	closeButton.setParent(this);
	closeButton.setTitle(L"OK");
	closeButton.setRect(twRect(170, 70, 80, 20));
	
	// setup itself.
	setRect(twRect(0, 0, 260, 100)+getBorderSize());
	setTitle(L"About XTBook");
	
	// set the close button as the default button.
	setDefault(&closeButton);
	
	// load the icon of Nexhawks.
	iconDC=twSDLDC::loadFromXPM(Nexhawks);
	
	// show the widgets.
	messageLabel.show();
	closeButton.show();
}
XTBAboutDialog::~XTBAboutDialog(){
	// release the icon of Nexhawks.
	delete iconDC;
}
void XTBAboutDialog::command(int){
	// clock button was clicked; close the dialog.
	endDialog(twDR_ok);
}

void XTBAboutDialog::clientPaint(const twPaintStruct& p){
	// fill the background.
	twDialog::clientPaint(p);
	twDC *dc=p.dc;
	
	// and then draw the icon.
	dc->bitBlt(iconDC, twPoint(10, 10), twRect(0,0,64,64));
}

void XTBShowAboutDialog(twWnd *wnd){
	XTBAboutDialog dlg;
	dlg.showModal(wnd);
}

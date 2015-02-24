/*
 *  XTBAboutDialog.h
 *  XTBook
 *
 *  Created by Nexhawks on 2/12/11.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#include <tcw/twDialog.h>
#include <tcw/twButton.h>
#include <tcw/twLabel.h>

class XTBAboutDialog : public twDialog{
protected:
	twButton closeButton;
	twLabel messageLabel;
	twDC *iconDC;

public:
	XTBAboutDialog();
	virtual ~XTBAboutDialog();
	virtual void clientPaint(const twPaintStruct&);
	virtual void command(int);
};

void XTBShowAboutDialog(twWnd *);

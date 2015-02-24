/*
 *  XTBRootPanel.h
 *  XTBook
 *
 *  Created by Nexhawks on 2/12/11.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#pragma once

#include <tcw/twWnd.h>
#include <tcw/twButton.h>
#include <tcw/twLabel.h>

class XTBRootPanel : public twWnd{
protected:
	/** icon of XTBook. */
	twDC *homeDC;
	
public:
	/** initializes the root panel. */
	XTBRootPanel();
	
	/** finalizes the root panel. */
	virtual ~XTBRootPanel();
	
	/** renders the contents of the root panel. */
	virtual void clientPaint(const twPaintStruct&);
	
	/** no-op. */
	virtual void command(int);
};

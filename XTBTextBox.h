/*
 *  XTBTextBox.h
 *  XTBook
 *
 *  Created by Nexhawks on 2/15/11.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#pragma once
#include <tcw/twWnd.h>

class XTBTextField;

class XTBTextBox : public twWnd{
protected:
	XTBTextField *m_textField;
	
public:
	XTBTextBox();
	virtual ~XTBTextBox();
	XTBTextField *getTextField() const;
	
	virtual void setRect(const twRect&);
	virtual void clientPaint(const twPaintStruct&);
	virtual bool clientHitTest(const twPoint&) const;
	virtual void command(int);
};

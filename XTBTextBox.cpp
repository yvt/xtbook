/*
 *  XTBTextBox.cpp
 *  XTBook
 *
 *  Created by Nexhawks on 2/15/11.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#include "XTBTextBox.h"
#include "XTBTextField.h"
#include <tcw/twBaseSkin.h>

XTBTextBox::XTBTextBox(){
	m_textField=new XTBTextField();
	m_textField->setParent(this);
	m_textField->show();
}
XTBTextBox::~XTBTextBox(){
	delete m_textField;
}
XTBTextField *XTBTextBox::getTextField() const{
	return m_textField;
}

void XTBTextBox::setRect(const twRect& _rt){
	twRect rt=_rt;
	rt.h=16;
	twWnd::setRect(rt);
	m_textField->setRect(twRect(2, 2, rt.w-4, rt.h-4));
}
void XTBTextBox::clientPaint(const twPaintStruct& p){
	twDC *dc=p.dc;
	twRect rt(twPoint(0, 0), p.bound);
	
	int x1=rt.x, x2=rt.x+rt.w-1;
	int y1=rt.y, y2=rt.y+rt.h-2;
	twColor col=tw_curSkin->getDarkCtrlColor();
	twColor col2=tw_curSkin->getBrightCtrlColor();
	
	dc->drawRect(tw_curSkin->getWndColor(), twRect(rt.x+1, rt.y+1, rt.w-3, rt.h-3));
	
	
	dc->drawLine(col  , twPoint(x1+1, y1), twPoint(x2-1, y1));
	dc->drawLine(col  , twPoint(x1, y1+1), twPoint(x1, y2-1));
	dc->drawLine(col  , twPoint(x2, y1+1), twPoint(x2, y2-1));
	dc->drawLine(col  , twPoint(x1+1, y2), twPoint(x2-1, y2));
	dc->drawLine(col2 , twPoint(x1+1, y2+1), twPoint(x2-1, y2+1));
	dc->fillRect(col2, twRect(x1, y2, 1, 1));
	dc->fillRect(col2, twRect(x2, y2, 1, 1));
	
	
}
bool XTBTextBox::clientHitTest(const twPoint& pt) const{
	twRect rt=getClientRect();
	if((pt.x==0 || pt.x==rt.h-1) && (pt.y==0 || pt.y==rt.h-1))
		return false;
	return true;
}

void XTBTextBox::command(int){
	sendCmdToParent();
}


/*
 *  XTBSearchBar.cpp
 *  XTBook
 *
 *  Created by Nexhawks on 2/17/11.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#include "XTBSearchBar.h"
#include "XTBTextField.h"
#include <tcw/twBaseSkin.h>
#include "xpms/SearchIcon.xpm"
#include "xpms/RomajiInput.xpm"
#include "xpms/DirectInput.xpm"
#include "xpms/RomajiInputPress.xpm"
#include "xpms/DirectInputPress.xpm"
#include "xpms/ClearIcon.xpm"
#include "xpms/HistoryIcon.xpm"
#include "xpms/ClearIconPress.xpm"
#include "xpms/HistoryIconPress.xpm"
#include <tcw/twSDLDC.h>
#include <tcw/twEvent.h>
#include <tcw/twInvocation.h>

twDC *XTBSearchBar::m_searchIcon=NULL;

twDC *XTBSearchBar::getSearchIcon(){
	if(!m_searchIcon){
		m_searchIcon=twSDLDC::loadFromXPM(SearchIcon);
	}
	return m_searchIcon;
}

twDC *XTBSearchBar::m_directInput=NULL;

twDC *XTBSearchBar::getDirectInputImage(){
	if(!m_directInput){
		m_directInput=twSDLDC::loadFromXPM(DirectInput);
	}
	return m_directInput;
}

twDC *XTBSearchBar::m_romajiInput=NULL;

twDC *XTBSearchBar::getRomajiInputImage(){
	if(!m_romajiInput){
		m_romajiInput=twSDLDC::loadFromXPM(RomajiInput);
	}
	return m_romajiInput;
}

twDC *XTBSearchBar::m_directInputPress=NULL;

twDC *XTBSearchBar::getDirectInputPressImage(){
	if(!m_directInputPress){
		m_directInputPress=twSDLDC::loadFromXPM(DirectInputPress);
	}
	return m_directInputPress;
}

twDC *XTBSearchBar::m_romajiInputPress=NULL;

twDC *XTBSearchBar::getRomajiInputPressImage(){
	if(!m_romajiInputPress){
		m_romajiInputPress=twSDLDC::loadFromXPM(RomajiInputPress);
	}
	return m_romajiInputPress;
}

twDC *XTBSearchBar::m_clearIcon=NULL;

twDC *XTBSearchBar::getClearIcon(){
	if(!m_clearIcon){
		m_clearIcon=twSDLDC::loadFromXPM(ClearIcon);
	}
	return m_clearIcon;
}

twDC *XTBSearchBar::m_historyIcon=NULL;

twDC *XTBSearchBar::getHistoryIcon(){
	if(!m_historyIcon){
		m_historyIcon=twSDLDC::loadFromXPM(HistoryIcon);
	}
	return m_historyIcon;
}

twDC *XTBSearchBar::m_clearIconPress=NULL;

twDC *XTBSearchBar::getClearIconPress(){
	if(!m_clearIconPress){
		m_clearIconPress=twSDLDC::loadFromXPM(ClearIconPress);
	}
	return m_clearIconPress;
}

twDC *XTBSearchBar::m_historyIconPress=NULL;

twDC *XTBSearchBar::getHistoryIconPress(){
	if(!m_historyIconPress){
		m_historyIconPress=twSDLDC::loadFromXPM(HistoryIconPress);
	}
	return m_historyIconPress;
}

XTBSearchBar::XTBSearchBar(){
	m_textField=new XTBTextField();
	m_textField->setParent(this);
	m_textField->show();
	m_dragLeftButton=false;
	m_dragRightButton=false;
	m_hoverLeftButton=false;
	m_hoverRightButton=false;
}
XTBSearchBar::~XTBSearchBar(){
	delete m_textField;
	m_textField=NULL;
}
XTBTextField *XTBSearchBar::getTextField() const{
	return m_textField;
}

void XTBSearchBar::setRect(const twRect& _rt){
	twRect rt=_rt;
	rt.h=14;
	twWnd::setRect(rt);
	m_textField->setRect(twRect(2+12, 1, rt.w-4-12-12+1, rt.h-2));
}
void XTBSearchBar::clientPaint(const twPaintStruct& p){
	twDC *dc=p.dc;
	twRect rt(twPoint(0, 0), p.bound);
	
	
	twColor col=tw_curSkin->getDarkCtrlColor();
	if(getStatus().focused)
		col=tw_curSkin->getActiveTitleColor();
	else
		col=tw_curSkin->getInactiveTitleColor();/*
	dc->drawLine(col  , twPoint(x1+1, y1), twPoint(x2-1, y1));
	dc->drawLine(col  , twPoint(x1, y1+1), twPoint(x1, y2-1));
	dc->drawLine(col  , twPoint(x2, y1+1), twPoint(x2, y2-1));
	dc->drawLine(col  , twPoint(x1+1, y2), twPoint(x2-1, y2));*/
	//dc->drawRect(col, twRect(rt.x, rt.y, rt.w-1, rt.h-1));
	dc->drawRect(tw_curSkin->getWndColor(), twRect(rt.x, rt.y, rt.w-1, rt.h-1));
	
	if(getStatus().focused){
		if(m_dragLeftButton && m_hoverLeftButton){
			switch(m_textField->getInputMode()){
				case XTBTextFieldInputModeDirect:
					dc->bitBlt(getDirectInputPressImage(), twPoint(1, 1), twRect(0, 0, 12, 12));
					break;
				case XTBTextFieldInputModeHiragana:
					dc->bitBlt(getRomajiInputPressImage(), twPoint(1, 1), twRect(0, 0, 12, 12));
					break;
			}
		}else{
			switch(m_textField->getInputMode()){
				case XTBTextFieldInputModeDirect:
					dc->bitBlt(getDirectInputImage(), twPoint(1, 1), twRect(0, 0, 12, 12));
					break;
				case XTBTextFieldInputModeHiragana:
					dc->bitBlt(getRomajiInputImage(), twPoint(1, 1), twRect(0, 0, 12, 12));
					break;
			}
		}
	}else{
		dc->bitBlt(getSearchIcon(), twPoint(1, 1), twRect(0, 0, 12, 12));
	}
	
	if(m_textField->getTitle().size()){
		if(m_dragRightButton && m_hoverRightButton){
			dc->bitBlt(getClearIconPress(), twPoint(rt.w-13, 1), twRect(0, 0, 12, 12));
		}else{
			dc->bitBlt(getClearIcon(), twPoint(rt.w-13, 1), twRect(0, 0, 12, 12));
		}
	}else{
		// Hide history icon because it is not longer needed.
		/*
		if(m_dragRightButton && m_hoverRightButton){
			dc->bitBlt(getHistoryIconPress(), twPoint(rt.w-13, 1), twRect(0, 0, 12, 12));
		}else{
			dc->bitBlt(getHistoryIcon(), twPoint(rt.w-13, 1), twRect(0, 0, 12, 12));
		}*/
	}
	
	dc->drawLine(tw_curSkin->getWndColor(), twPoint(13, 1), twPoint(13, 13));
}
bool XTBSearchBar::clientHitTest(const twPoint& pt) const{
	return true;
}

void XTBSearchBar::command(int){
	sendCmdToParent();
	invalidate();
}

void XTBSearchBar::clientMouseDown(const twPoint& pt, twMouseButton button){
	if(button==twMB_left){
		if(hitTestLeftButton(pt)){
			m_dragLeftButton=true;
			m_hoverLeftButton=true;
			invalidate();
		}
		if(hitTestRightButton(pt)){
			m_dragRightButton=true;
			m_hoverRightButton=true;
			invalidate();
		}
	}
	twSetCapture(this);
}
void XTBSearchBar::clientMouseMove(const twPoint& pt){
	if(m_dragLeftButton){
		bool newHover=hitTestLeftButton(pt);
		if(newHover!=m_hoverLeftButton){
			m_hoverLeftButton=newHover;
			invalidate();
		}
	}
	if(m_dragRightButton){
		bool newHover=hitTestRightButton(pt);
		if(newHover!=m_hoverRightButton){
			m_hoverRightButton=newHover;
			invalidate();
		}
	}
}
void XTBSearchBar::clientMouseUp(const twPoint& pt, twMouseButton button){
	if(button==twMB_left){
		if(m_dragLeftButton){
			m_dragLeftButton=false;
			if(m_hoverLeftButton){
				XTBTextFieldInputMode currentMode=m_textField->getInputMode();
				if(currentMode==XTBTextFieldInputModeDirect)
					m_textField->setInputMode(XTBTextFieldInputModeHiragana);
				else
					m_textField->setInputMode(XTBTextFieldInputModeDirect);
				invalidate();
			}
		}
		if(m_dragRightButton){
			m_dragRightButton=false;
			if(m_hoverRightButton){
				if(m_textField->getTitle().size()){
					// clear text
					m_textField->removeAllContents();
					sendCmdToParent();
				}else{
					//history no longer exists.
				}
				invalidate();
			}
		}
	}
	twReleaseCapture();
}

void XTBSearchBar::clientKeyDown(const std::wstring& name){
	if(name==L"F1"){
		
		// switch input mode
		XTBTextFieldInputMode currentMode=m_textField->getInputMode();
		if(currentMode==XTBTextFieldInputModeDirect)
			m_textField->setInputMode(XTBTextFieldInputModeHiragana);
		else
			m_textField->setInputMode(XTBTextFieldInputModeDirect);
		invalidate();
		
		clientKeyEventHandled();
	}else if(name==L"KeyPad1"){
		// clear!
		m_textField->removeAllContents();
		sendCmdToParent();
	}
}

void XTBSearchBar::clientEnter(){/*
	tw_event->invokeOnMainThread(new twNoArgumentMemberFunctionInvocation<XTBTextField>(m_textField,
																						&XTBTextField::selectAll));*/
	//m_textField->selectAll();
}

bool XTBSearchBar::hitTestLeftButton(const twPoint& pt)const{
	if(pt.x<0 || pt.y<0 || pt.y>=14)
		return false;
	return pt.x<13;
}

bool XTBSearchBar::hitTestRightButton(const twPoint& pt)const{
	twRect rt=getClientRect();
	if(pt.x>=rt.w || pt.y<0 || pt.y>=14)
		return false;
	return pt.x>=rt.w-13;
}

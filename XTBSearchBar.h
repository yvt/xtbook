/*
 *  XTBSearchBar.h
 *  XTBook
 *
 *  Created by Nexhawks on 2/17/11.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#pragma once
#include <tcw/twWnd.h>
#include <tcw/twDC.h>
class XTBTextField;

class XTBSearchBar : public twWnd{
protected:
	static twDC *m_searchIcon;
	static twDC *getSearchIcon();
	static twDC *m_romajiInput;
	static twDC *getRomajiInputImage();
	static twDC *m_directInput;
	static twDC *getDirectInputImage();
	static twDC *m_romajiInputPress;
	static twDC *getRomajiInputPressImage();
	static twDC *m_directInputPress;
	static twDC *getDirectInputPressImage();
	static twDC *m_clearIcon;
	static twDC *getClearIcon();
	static twDC *m_historyIcon;
	static twDC *getHistoryIcon();
	static twDC *m_clearIconPress;
	static twDC *getClearIconPress();
	static twDC *m_historyIconPress;
	static twDC *getHistoryIconPress();
	
	bool m_dragLeftButton;
	bool m_hoverLeftButton;
	bool m_dragRightButton;
	bool m_hoverRightButton;
	XTBTextField *m_textField;
	
	bool hitTestLeftButton(const twPoint&) const;
	bool hitTestRightButton(const twPoint&) const;
	
public:
	XTBSearchBar();
	virtual ~XTBSearchBar();
	XTBTextField *getTextField() const;
	
	virtual void setRect(const twRect&);
	virtual void clientPaint(const twPaintStruct&);
	virtual bool clientHitTest(const twPoint&) const;
	virtual void command(int);
	virtual void clientMouseDown(const twPoint&, twMouseButton);
	virtual void clientMouseMove(const twPoint&);
	virtual void clientMouseUp(const twPoint&, twMouseButton);
	
	virtual void clientEnter();
	
	virtual void clientKeyDown(const std::wstring&);
};
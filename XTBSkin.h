/*
 *  XTBSkin.h
 *  XTBook
 *
 *  Created by Nexhawks on 2/11/11.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#pragma once
#include <tcw/twBaseSkin.h>

class XTBSkin : public twBaseSkin{
protected:
	twDC *m_glass;
	twDC *m_checkBox;
public:
	
	XTBSkin();
	virtual ~XTBSkin();
	
	virtual void drawFocusRect(twDC *, const twRect&);
	virtual void drawSolidBorder(twDC *, const twRect&, const twWndStatus&, const std::wstring&);
	virtual void drawPanelBorder(twDC *, const twRect&, const twWndStatus&, const std::wstring&);
	virtual void drawDialogBorder(twDC *, const twRect&, const twWndStatus&, const std::wstring&);
	virtual void drawOverlappedBorder(twDC *, const twRect&, const twWndStatus&, const std::wstring&);
	virtual bool hitTestSolidBorder(const twRect&, const twPoint&);
	virtual bool hitTestPanelBorder(const twRect&, const twPoint&);
	virtual bool hitTestDialogBorder(const twRect&, const twPoint&);
	virtual bool hitTestOverlappedBorder(const twRect&, const twPoint&);
	virtual twRect getSolidBorderCliRect(const twRect&);
	virtual twRect getPanelBorderCliRect(const twRect&);
	virtual twRect getDialogBorderCliRect(const twRect&);
	virtual twRect getOverlappedBorderCliRect(const twRect&);
	
	virtual void drawButton(twDC *, const twRect&, const twWndStatus&, bool pressed, const std::wstring&, const twFont *);
	
	virtual twColor getActiveTitleColor();
	virtual twColor getActiveTitleTextColor();
	virtual twColor getInactiveTitleColor();
	virtual twColor getInactiveTitleTextColor();
	virtual twColor getCtrlColor();
	
	virtual twColor getSelectionColor();
	virtual twColor getSelectionTextColor();
	
	virtual void drawCheckBox(twDC *dc, const twRect& rt, const twWndStatus& s, bool pressed, bool on, const std::wstring& str, const twFont *fnt);

	
};

/*
 *  twBasicSkin.h
 *  
 *
 *  Created by Nexhawks on 10/01/16.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#include <tcw/twDC.h>
#include <tcw/twTypes.h>
#include <tcw/twWnd.h>
#include <wchar.h>
#include <string>

enum twEdgeStyle{
	twES_none=0,
	twES_raised,
	twES_sunken,
	twES_bumped,
	twES_relief,
	twES_solid,
	twES_double,
	twES_dotted
};	

class twBaseSkin{
protected:
	
	int getTitleHeight();
	
public:
	
	twBaseSkin();
	virtual ~twBaseSkin();
	
	virtual void drawEdge(twDC *, const twRect&, twEdgeStyle);
	
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
	virtual void drawRadioBtn(twDC *, const twRect&, const twWndStatus&, bool pressed, bool on, const std::wstring&, const twFont *);
	virtual void drawCheckBox(twDC *, const twRect&, const twWndStatus&, bool pressed, bool on, const std::wstring&, const twFont *);
	
	virtual twColor getBorderColor();
	virtual twColor getWndColor();
	virtual twColor getCtrlColor();
	virtual twColor getWndTextColor();
	virtual twColor getCtrlTextColor();
	virtual twColor getSelectionColor();
	virtual twColor getSelectionTextColor();
	virtual twColor getDisabledWndTextColor();
	virtual twColor getDisabledCtrlTextColor();
	virtual twColor getDarkCtrlColor();
	virtual twColor getDarkDarkCtrlColor();
	virtual twColor getBrightCtrlColor();
	
	
	virtual twColor getActiveTitleColor();
	virtual twColor getActiveTitleTextColor();
	virtual twColor getInactiveTitleColor();
	virtual twColor getInactiveTitleTextColor();
	
	
};

extern twBaseSkin *tw_curSkin;

//
//  XTBPreferenceWindow.h
//  XTBook
//
//  Created by Kawada Tomoaki on 8/2/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#pragma once

#include "XTBDialogSheet.h"
#include <tcw/twCheckBox.h>
#include <tcw/twLabel.h>
#include "XTBScrollView.h"

class XTBListView;
class XTBPreferenceLocaleDataSource;

class XTBPreferenceWindow: public XTBDialogSheet{
	XTBScrollView *m_scrollView;
	
	twLabel *m_localesLabel;
	XTBListView *m_localesView;
	XTBPreferenceLocaleDataSource *m_localesDataSource;
	twCheckBox *m_singleTouchLinkCheckBox;
	twCheckBox *m_fullScreenCheckBox;
	twCheckBox *m_singlePageCheckBox;
	twCheckBox *m_ignoreTemplateCheckBox;
	
	bool m_fullScreenModified;
	
	twLabel *m_label;
	
	void localeSelected();
protected:
	virtual twSize startupSize() const;
	virtual twColor paddingColor() const;
public:
	XTBPreferenceWindow();
	virtual ~XTBPreferenceWindow();
	
	virtual void clientPaint(const twPaintStruct&);
	
	virtual void backgroundTouched();
	
	virtual void command(int);
	
	virtual void clientKeyDown(const std::wstring&);
};


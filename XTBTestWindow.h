/*
 *  XTBTestWindow.h
 *  XTBook
 *
 *  Created by Nexhawks on 2/20/11.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#pragma once

#include "XTBScrollView.h"
#include <tcw/twWnd.h>
#include <tcw/twButton.h>
#include <tcw/twDialog.h>
#include "XTBListView.h"
#include "XTBListViewDataSource.h"

class XTBTestWindow : public twDialog{
protected:
	XTBListView m_listView;
	XTBListViewDataSource *m_dataSource;
	twButton m_buttons[5];
	void doFontRenderPerformanceTest();
public:
	XTBTestWindow();
	virtual ~XTBTestWindow();
	virtual void command(int);
	
	virtual void backgroundTouched();
};

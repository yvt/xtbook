/*
 *  XTBScrollViewTest.h
 *  XTBook
 *
 *  Created by Nexhawks on 2/20/11.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#pragma once
#include <tcw/twDialog.h>
#include "XTBScrollView.h"
#include <tcw/twButton.h>


class XTBScrollViewTest : public twDialog {
protected:
	XTBScrollView m_scrollView;
	twButton m_buttons[100];
	twButton m_button;
public:
	XTBScrollViewTest();
	virtual void command(int);
};


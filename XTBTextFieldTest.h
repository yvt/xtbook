/*
 *  XTBTextFieldTest.h
 *  XTBook
 *
 *  Created by Nexhawks on 2/20/11.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#pragma once
#include <tcw/twDialog.h>
#include "XTBTextBox.h"
#include "XTBSearchBar.h"
#include <tcw/twButton.h>

class XTBTextFieldTest : public twDialog {
protected:
	XTBTextBox m_textBox;
	XTBSearchBar m_searchBar;
	twButton m_button;
public:
	XTBTextFieldTest();
	virtual void command(int);
};

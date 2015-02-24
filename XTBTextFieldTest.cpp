/*
 *  XTBTextFieldTest.cpp
 *  XTBook
 *
 *  Created by Nexhawks on 2/20/11.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#include "XTBTextFieldTest.h"

XTBTextFieldTest::XTBTextFieldTest(){
	setRect(twRect(0, 0, 150, 68)+getBorderSize());
	setTitle(L"Text Fields");
	
	m_textBox.setRect(twRect(4, 4, 142, 16));
	m_textBox.setParent(this);
	m_searchBar.setRect(twRect(4, 24, 142, 16));
	m_searchBar.setParent(this);
	
	m_button.setRect(twRect(4, 48, 142, 16));
	m_button.setTitle(L"Dismiss");
	m_button.setId(1);
	m_button.setParent(this);
	
	m_textBox.show();
	m_searchBar.show();
	m_button.show();
}

void XTBTextFieldTest::command(int index){
	if(index==1)
		endDialog();
}

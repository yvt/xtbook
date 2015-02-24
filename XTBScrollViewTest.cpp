/*
 *  XTBScrollViewTest.cpp
 *  XTBook
 *
 *  Created by Nexhawks on 2/20/11.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#include "XTBScrollViewTest.h"


XTBScrollViewTest::XTBScrollViewTest(){
	setRect(twRect(0, 0, 150, 150)+getBorderSize());
	setTitle(L"Scroll View");
	
	for(int i=0;i<100;i++){
		m_buttons[i].setRect(twRect(rand()%232, rand()%990, 10, 10));
		m_buttons[i].setParent(&m_scrollView);
		m_buttons[i].show();
	}
	
	m_button.setRect(twRect(4, 132, 142, 16));
	m_button.setTitle(L"Dismiss");
	m_button.setId(1);
	m_button.setParent(this);

	m_scrollView.setRect(twRect(4, 4, 142, 124));
	m_scrollView.setParent(this);
	m_scrollView.setContentSize(twSize(242, 1000));
	{
		twWndStyle style=m_scrollView.getStyle();
		style.border=twBS_panel;
		m_scrollView.setStyle(style);
	}
	
	m_button.show();
	m_scrollView.show();
}

void XTBScrollViewTest::command(int index){
	if(index==1)
	endDialog();
}


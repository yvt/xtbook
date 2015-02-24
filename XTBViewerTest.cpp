/*
 *  XTBViewerTest.cpp
 *  XTBook
 *
 *  Created by Nexhawks on 2/20/11.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#include "XTBViewerTest.h"
#include <tcw/twEvent.h>
XTBViewerTest::XTBViewerTest(){
	setRect(twRect(0, 0, 150, 150)+getBorderSize());
	setTitle(L"Viewer");
	

	m_button.setRect(twRect(4, 132, 142, 16));
	m_button.setTitle(L"Dismiss");
	m_button.setId(1);
	m_button.setParent(this);
	
	m_viewer.setRect(twRect(4, 4, 142, 124));
	m_viewer.setParent(this);
	m_viewer.setContentSize(twSize(242, 1000));
	
	{
		twWndStyle style=m_viewer.getStyle();
		style.border=twBS_panel;
		m_viewer.setStyle(style);
	}
	
	m_button.show();
	m_viewer.show();
}

void XTBViewerTest::command(int index){
	if(index==1){
		endDialog();
		tw_event->doEvents(10);
	}
}

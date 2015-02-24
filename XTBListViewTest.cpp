//
//  XTBListViewTest.cpp
//  XTBook
//
//  Created by Nexhawks on 4/16/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "XTBListViewTest.h"

#include "XTBListViewDataSource.h"
#include <tcw/twMsgBox.h>

class XTBListViewTestDataSource: public XTBListViewDataSource{
public:
	int m_selection;
	XTBListViewTestDataSource(){
		m_selection=0;
	}
	virtual int rowCount(){
		return 1000;
	}
	virtual std::wstring stringAtRow(int row){
		wchar_t buf[64];
#ifdef EV_UNSAFE_SWPRINTF
		swprintf(buf, L"%d", row);
#else
		swprintf(buf, 64, L"%d", row);
#endif
		return buf;
	}
	virtual std::wstring detailStringAtRow(int){
		return L"Detail";
	}
	virtual XTBListViewItemState stateAtRow(int row){
		if(row==m_selection)
			return XTBListViewItemStateChecked;
		else
			return XTBListViewItemStateNormal;
	}
	
};

XTBListViewTest::XTBListViewTest():
m_listView(XTBListViewStyleValue2){
	setRect(twRect(0, 0, 150, 150)+getBorderSize());
	setTitle(L"List View");
	
	
	m_button.setRect(twRect(4, 132, 142, 16));
	m_button.setTitle(L"Dismiss");
	m_button.setId(1);
	m_button.setParent(this);
	
	m_listView.setRect(twRect(4, 4, 142, 124));
	m_listView.setParent(this);
	
	{
		twWndStyle style=m_listView.getStyle();
		style.border=twBS_panel;
		m_listView.setStyle(style);
	}
	m_dataSource=new XTBListViewTestDataSource();
	m_listView.setDataSource(m_dataSource);
	m_listView.setId(2);
	
	m_button.show();
	m_listView.show();
}

XTBListViewTest::~XTBListViewTest(){
	XTBListViewDataSource *source=m_listView.getDataSource();
	m_listView.setDataSource(NULL);
	delete source;
}

void XTBListViewTest::command(int index){
	if(index==1){
		endDialog();
		tw_event->doEvents(10);
	}else if(index==2){
		int row=m_listView.getLastActiveRow();
		int oldsel=m_dataSource->m_selection;
		m_dataSource->m_selection=row;
		m_listView.invalidateItem(oldsel);
		m_listView.invalidateItem(row);
		
	}
}

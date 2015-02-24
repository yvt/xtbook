//
//  XTBListViewTest.h
//  XTBook
//
//  Created by Nexhawks on 4/16/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#pragma once
#include <tcw/twDialog.h>
#include "XTBListView.h"
#include <tcw/twButton.h>

class XTBListViewTestDataSource;
class XTBListViewTest : public twDialog {
protected:
	XTBListView m_listView;
	twButton m_button;
	XTBListViewTestDataSource *m_dataSource;
	
public:
	XTBListViewTest();
	virtual ~XTBListViewTest();
	virtual void command(int);
};


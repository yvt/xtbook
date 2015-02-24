//
//  XTBListViewDataSource.h
//  XTBook
//
//  Created by Nexhawks on 4/16/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#pragma once

#include <tcw/twObject.h>
#include <string>

class XTBListView;

enum XTBListViewItemState{
	XTBListViewItemStateNormal=0,
	XTBListViewItemStateChecked
};

class XTBListViewDataSource: public twObject{
public:
	
	XTBListViewDataSource();
	virtual ~XTBListViewDataSource();
	
	virtual int rowCount();
	virtual std::wstring stringAtRow(int);
	virtual std::wstring detailStringAtRow(int);
	virtual XTBListViewItemState stateAtRow(int);
};

//
//  XTBStartPageListDataSource.h
//  XTBook
//
//  Created by Kawada Tomoaki on 6/24/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#pragma once


#include "XTBListViewDataSource.h"

class XTBStartPageListDataSource: public XTBListViewDataSource{

	std::set<int> m_checkedItems;
	
public:
	
	XTBStartPageListDataSource(){}
	virtual ~XTBStartPageListDataSource(){}
	
	virtual bool isRemovable() const{return false;}
	virtual bool isClearable() const{
		return false;
	}
	virtual void removeItemsAtRows(const std::set<int>&);
	virtual void clearAllItems();
	
	virtual XTBListViewItemState stateAtRow(int);
	
	virtual void reloadData();
	void clearChecks();
	void toggleCheckAtRow(int);
	bool isRowChecked(int) const; 
};

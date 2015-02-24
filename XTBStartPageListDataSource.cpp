//
//  XTBStartPageListDataSource.cpp
//  XTBook
//
//  Created by Kawada Tomoaki on 6/24/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "XTBStartPageListDataSource.h"
#include "XTBException.h"
#include <assert.h>

void XTBStartPageListDataSource::removeItemsAtRows(const std::set<int> &){
	XTBThrowException(L"XTBNotImplementedException", NULL);
}

void XTBStartPageListDataSource::clearAllItems(){
	XTBThrowException(L"XTBNotImplementedException", NULL);
}

XTBListViewItemState XTBStartPageListDataSource::stateAtRow(int row){
	if(isRowChecked(row))
		return XTBListViewItemStateChecked;
	else
		return XTBListViewItemStateNormal;
}

void XTBStartPageListDataSource::reloadData(){
	clearChecks();
}

void XTBStartPageListDataSource::clearChecks(){
	m_checkedItems.clear();
}

void XTBStartPageListDataSource::toggleCheckAtRow(int row){
	if(isRowChecked(row)){
		// uncheck it.
		std::set<int>::iterator it;
		it=m_checkedItems.find(row);
		assert(it!=m_checkedItems.end());
		m_checkedItems.erase(it);
	}else{
		// check it.
		m_checkedItems.insert(row);
	}
}

bool XTBStartPageListDataSource::isRowChecked(int row)const{
	std::set<int>::iterator it;
	it=m_checkedItems.find(row);
	return (it!=m_checkedItems.end());
}

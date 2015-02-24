//
//  XTBListViewDataSource.cpp
//  XTBook
//
//  Created by Nexhawks on 4/16/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "XTBListViewDataSource.h"

XTBListViewDataSource::XTBListViewDataSource(){
}

XTBListViewDataSource::~XTBListViewDataSource(){
	
}

int XTBListViewDataSource::rowCount(){
	return 0;
}

std::wstring XTBListViewDataSource::stringAtRow(int){
	return L"";
}

std::wstring XTBListViewDataSource::detailStringAtRow(int){
	return L"";
}

XTBListViewItemState XTBListViewDataSource::stateAtRow(int){
	return XTBListViewItemStateNormal;
}



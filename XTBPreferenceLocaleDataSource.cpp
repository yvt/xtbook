//
//  XTBPreferenceLocaleDataSource.cpp
//  XTBook
//
//  Created by Kawada Tomoaki on 8/2/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "XTBPreferenceLocaleDataSource.h"

XTBPreferenceLocaleDataSource::XTBPreferenceLocaleDataSource(){
	m_locales=XTBScanStringTables();
	m_selectedIndex=indexForLocale(XTBGetLocale());
}

XTBPreferenceLocaleDataSource::~XTBPreferenceLocaleDataSource(){
	
}

int XTBPreferenceLocaleDataSource::rowCount(){
	return m_locales.size();
}


std::wstring XTBPreferenceLocaleDataSource::stringAtRow(int row){
	const XTBStringTableInfo& info=m_locales[row];
	return info.description;
}

std::wstring XTBPreferenceLocaleDataSource::detailStringAtRow(int row){
	const XTBStringTableInfo& info=m_locales[row];
	return info.identifier;
}

XTBListViewItemState XTBPreferenceLocaleDataSource::stateAtRow(int row){
	if(m_selectedIndex==row)
		return XTBListViewItemStateChecked;
	else
		return XTBListViewItemStateNormal;
}

int XTBPreferenceLocaleDataSource::indexForLocale(const std::wstring &locale){
	for(size_t i=0;i<m_locales.size();i++)
		if(m_locales[i].identifier==locale)
			return i;
	return -1;
}

const std::wstring& XTBPreferenceLocaleDataSource::localeForIndex(int row){
	return m_locales[row].identifier;
}

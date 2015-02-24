//
//  XTBPreferenceLocaleDataSource.h
//  XTBook
//
//  Created by Kawada Tomoaki on 8/2/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#pragma once

#include "XTBListViewDataSource.h"
#include "i18n.h"

class XTBPreferenceLocaleDataSource: public XTBListViewDataSource{
	std::vector<XTBStringTableInfo> m_locales;
	int m_selectedIndex;
public:
	XTBPreferenceLocaleDataSource();
	virtual ~XTBPreferenceLocaleDataSource();
	
	virtual int rowCount();
	virtual std::wstring stringAtRow(int);
	virtual std::wstring detailStringAtRow(int);
	virtual XTBListViewItemState stateAtRow(int);
	
	int indexForLocale(const std::wstring&);
	const std::wstring& localeForIndex(int);
	
	int selectedIndex() const{return m_selectedIndex;}
	void setSelectedIndex(int i){m_selectedIndex=i;}
};

//
//  XTBContentsDataSource.cpp
//  XTBook
//
//  Created by Kawada Tomoaki on 8/1/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "XTBContentsDataSource.h"
#include "XTBArticle.h"

XTBContentsDataSource::XTBContentsDataSource(XTBArticle *article, int i):
m_article(article),
m_currentIndex(i){
}

XTBContentsDataSource::~XTBContentsDataSource(){
	
}

int XTBContentsDataSource::rowCount(){
	return m_article->indicesCount();
}

bool XTBContentsDataSource::shouldShowPageAtRow(int row) const{
	if(row==0)
		return true;
	XTBArticleIndexItem ind1, ind2;
	ind1=m_article->indexAt(row-1);
	ind2=m_article->indexAt(row);
	return ind1.page!=ind2.page;
}

std::wstring XTBContentsDataSource::stringAtRow(int row){
	if(shouldShowPageAtRow(row)){
		XTBArticleIndexItem ind=m_article->indexAt(row);
		std::wstring pageStr=XTBFormatStd(L"%d", (int)ind.page);
		return XTBFormat(XTBLocalizedString(L"XTBBrowserIndexPage"), pageStr.c_str(), NULL);
	}else{
		return std::wstring();
	}
}

std::wstring XTBContentsDataSource::detailStringAtRow(int row){
	std::wstring text;
	XTBArticleIndexItem ind=m_article->indexAt(row);
	int level=ind.level;
	for(int i=0;i<level;i++){
		text+=L"  ";
	}
	
	const std::wstring& inText=ind.displayText;
	for(size_t i=0;i<inText.size();i++){
		if(inText[i]==L'[' || inText[i]==L']')
			continue;
		text+=inText[i];
	}
	return text;
}

XTBListViewItemState XTBContentsDataSource::stateAtRow(int row){
	if(row==m_currentIndex)
		return XTBListViewItemStateChecked;
	return XTBListViewItemStateNormal;
}

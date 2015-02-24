//
//  XTBSearchViewDataSource.cpp
//  XTBook
//
//  Created by Kawada Tomoaki on 6/1/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "XTBSearchViewDataSource.h"
#include "XTBSearchView.h"
#include "XTBDictionary.h"
#include "XTBDictionaryIterator.h"
#include <assert.h>
#include "i18n.h"

static const unsigned long long g_maxRowCount=8713;

XTBSearchViewDataSource::XTBSearchViewDataSource(){
	m_rowCount=0;
}

XTBSearchViewDataSource::XTBSearchViewDataSource(const XTBSearchViewQuery& query){
	if(query.key.empty()){
		m_rowCount=0;
		return;
	}
	std::map<std::wstring, XTBDictionary *>& allDics=XTBDictionary::allDictionaries();
	
	unsigned long long rowCount=0;
	
	for(std::map<std::wstring, XTBDictionary *>::iterator it=allDics.begin();
		it!=allDics.end();it++){
		XTBDictionary *dic=it->second;
		XTBDictionaryIterator *iter=NULL;
		
		try{
			iter=dic->searchWithKey(query.key);
		}catch(...){
			iter=NULL;
		}
		
		if(iter){
			m_iterators.push_back(iter);
			rowCount+=iter->resultCount();
		}
	}
	
	if(rowCount>g_maxRowCount)
		rowCount=g_maxRowCount;
	
	m_rowCount=rowCount;
	
}

XTBSearchViewDataSource::~XTBSearchViewDataSource(){
	for(std::vector<XTBDictionaryIterator *>::iterator it
		=m_iterators.begin();it!=m_iterators.end();it++){
		delete *it;
	}
}

int XTBSearchViewDataSource::rowCount(){
	return m_rowCount;
}

std::wstring XTBSearchViewDataSource::stringAtRow(int row){
	prepareRow(row);
	XTBDictionaryResult& result=m_results[row];
	
	switch(styleForRow(row)){
		case XTBSearchViewRowStyleJapanese:
			return XTBFormat(XTBLocalizedString(L"XTBSearchViewJapaneseResult"),
							 result.key.c_str(), result.title.c_str(), NULL);
		case XTBSearchViewRowStyleEnglish:
			return result.title;
	}
	
}

std::wstring XTBSearchViewDataSource::detailStringAtRow(int row){
	prepareRow(row);
	XTBDictionaryResult& result=m_results[row];
	const XTBDictionary *dic=result.dictionary;
	return dic->displayName();
}

std::wstring XTBSearchViewDataSource::titleAtRow(int row){
	prepareRow(row);
	XTBDictionaryResult& result=m_results[row];
	std::wstring scheme=result.dictionary->scheme();
	return scheme+L':'+result.title;
}

XTBListViewItemState XTBSearchViewDataSource::stateAtRow(int row){
	return XTBListViewItemStateNormal;
}

static bool containingNonASCII(const std::wstring& str){
	for(size_t i=0;i<str.size();i++){
		if(str[i]<0 || str[i]>255){
			return true;
		}
	}
	return false;
}

XTBSearchViewRowStyle XTBSearchViewDataSource::styleForRow(int row){
	prepareRow(row);
	
	XTBDictionaryResult& result=m_results[row];
	
	// if any non-ascii are included, japanese.
	if(containingNonASCII(result.key))
		return XTBSearchViewRowStyleJapanese;
	if(containingNonASCII(result.title))
		return XTBSearchViewRowStyleJapanese;
	
	return XTBSearchViewRowStyleEnglish;
}

void XTBSearchViewDataSource::prepareRow(int row){
	// over range?
	if(row>=m_rowCount || row<0)
		return;
	
	// is already prepared?
	if(m_results.size()>row)
		return;
	
	// we can prepare only the next row.
	// so if later row was requested, first
	// make sure rows before that are prepared.
	while(row>m_results.size()){
		prepareRow(m_results.size());
	}
	
	XTBDictionaryIterator *foundIter=NULL;
	XTBDictionaryResult result;
	bool found=false;
	
	for(std::vector<XTBDictionaryIterator *>::iterator it
		=m_iterators.begin();it!=m_iterators.end();it++){
		XTBDictionaryIterator *iter=*it;
		if(iter->isEOF())
			continue;
		
		if(!found){
			result=iter->currentResult();
			foundIter=iter;
			found=true;
		}else{
			XTBDictionaryResult r=iter->currentResult();
			if(r.key<result.key){
				// replace.
				result=r;
				foundIter=iter;
			}
		}
	}
	
	assert(found);
	
	m_results.push_back(result);
	foundIter->next();
	
}


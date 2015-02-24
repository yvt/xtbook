//
//  XTBSampleDictionaryIterator.cpp
//  XTBook
//
//  Created by Kawada Tomoaki on 5/23/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "platform.h"
#include "XTBSampleDictionaryIterator.h"
#include "XTBSampleDictionary.h"
#include <vector>
#include <algorithm>

static wchar_t toLowerer(wchar_t c){
	if(c>=L'A' && c<=L'Z')
		c-='A'-'a';
	return c;
}

XTBSampleDictionaryIterator::XTBSampleDictionaryIterator(const std::wstring& key, const XTBSampleDictionary *dic){
	std::vector<XTBSysString> pathes=XTBScanDirectory(dic->path());
	for(std::vector<XTBSysString>::iterator it=pathes.begin();it!=pathes.end();it++){
		XTBSysString ss=*it;
		ss=ss.substr(ss.rfind(XTBPathSeparator)+1);
		std::wstring name=XTBWStringFromSysString(ss);
		name=name.substr(0,name.rfind(L'.'));
		std::wstring lowerName=name;
		std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), toLowerer);
		if(lowerName.find(key)==0){
			m_results.push_back(name);
		}
	}
	std::stable_sort(m_results.begin(), m_results.end());
	m_currentPosition=0;
	m_dic=dic;
}
XTBSampleDictionaryIterator::~XTBSampleDictionaryIterator(){
	
}
int XTBSampleDictionaryIterator::resultCount() const{
	return m_results.size();
}
XTBDictionaryResult XTBSampleDictionaryIterator::currentResult() const{
	XTBDictionaryResult result;
	const std::wstring& text=m_results[m_currentPosition];
	result.key=text;
	result.title=text;
	std::transform(result.key.begin(), result.key.end(), result.key.begin(), toLowerer);
	result.dictionary=m_dic;
	return result;
}
void XTBSampleDictionaryIterator::next(){
	m_currentPosition++;
}
bool XTBSampleDictionaryIterator::isEOF(){
	return m_currentPosition>=m_results.size();
}


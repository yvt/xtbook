//
//  XTBSampleDictionaryIterator.h
//  XTBook
//
//  Created by Kawada Tomoaki on 5/23/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#pragma once
#include "XTBDictionaryIterator.h"
#include <vector>

class XTBSampleDictionary;

class XTBSampleDictionaryIterator: public XTBDictionaryIterator{
private:
	const XTBSampleDictionary *m_dic;
	std::vector<std::wstring> m_results;
	int m_currentPosition;
public:
	XTBSampleDictionaryIterator(const std::wstring&, const XTBSampleDictionary *);
	~XTBSampleDictionaryIterator();
	virtual int resultCount() const;
	virtual XTBDictionaryResult currentResult() const;
	virtual void next();
	virtual bool isEOF();
};

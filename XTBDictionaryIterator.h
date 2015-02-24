//
//  XTBDictionaryIterator.h
//  XTBook
//
//  Created by Kawada Tomoaki on 5/23/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#pragma once

#include <string>
#include <tcw/twObject.h>

class XTBDictionary;

struct XTBDictionaryResult{
	std::wstring key;
	std::wstring title; // without schema
	const XTBDictionary *dictionary;
	bool operator <(const XTBDictionaryResult& res) const{
		return key<res.key;
	}
};

class XTBDictionaryIterator: public twObject{
public:	
	virtual int resultCount() const=0;
	virtual XTBDictionaryResult currentResult() const=0;
	virtual void next()=0;
	virtual bool isEOF()=0;
};

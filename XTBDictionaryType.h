//
//  XTBDictionaryType.h
//  XTBook
//
//  Created by Kawada Tomoaki on 5/22/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#pragma once

#include <string>
#include "platform.h"
#include "TPList/TPLDictionary.h"

class XTBDictionary;

// maybe singleton...
class XTBDictionaryType: public twObject{
public:
	static void registerDictionaryType(XTBDictionaryType *);
	static const XTBDictionaryType *dictionaryTypeWithIdentifier(const std::wstring&);
	
	virtual std::wstring name() const=0;
	virtual std::wstring identifier() const=0;
	virtual XTBDictionary *dictionaryForPath(const XTBSysString&, const TPLDictionary *options) const=0;
};


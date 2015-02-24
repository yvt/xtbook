//
//  XTBWikiplexus.h
//  XTBook
//
//  Created by Kawada Tomoaki on 7/6/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#pragma once

#include "XTBDictionaryType.h"

class XTBWikiplexus: public XTBDictionaryType{
public:
	virtual std::wstring name() const;
	virtual std::wstring identifier() const;
	virtual XTBDictionary *dictionaryForPath(const XTBSysString&, const TPLDictionary *options) const;
};

//
//  XTBImageComplex.h
//  XTBook
//
//  Created by Kawada Tomoaki on 8/1/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "XTBDictionaryType.h"

class XTBImageComplex: public XTBDictionaryType{
public:
	virtual std::wstring name() const;
	virtual std::wstring identifier() const;
	virtual XTBDictionary *dictionaryForPath(const XTBSysString&, const TPLDictionary *options) const;
};

//
//  XTBSampleDictionaryType.cpp
//  XTBook
//
//  Created by Kawada Tomoaki on 5/23/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "XTBSampleDictionaryType.h"
#include "XTBSampleDictionary.h"

std::wstring XTBSampleDictionaryType::name() const{
	return L"Sample Dictionary";
}

std::wstring XTBSampleDictionaryType::identifier() const{
	return L"com.nexhawks.XTBook.SampleDictionaryType";
}

XTBDictionary *XTBSampleDictionaryType::dictionaryForPath(const XTBSysString &path, const TPLDictionary *options) const{
	return new XTBSampleDictionary(path, options);
}

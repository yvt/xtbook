//
//  XTBImageComplex.cpp
//  XTBook
//
//  Created by Kawada Tomoaki on 8/1/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "XTBImageComplex.h"
#include "XTBImageComplexDictionary.h"

std::wstring XTBImageComplex::name() const{
	return L"XTBook ImageComplex";
}
std::wstring XTBImageComplex::identifier() const{
	return L"com.nexhawks.XTBook.ImageComplex";
}
XTBDictionary *XTBImageComplex::dictionaryForPath(const XTBSysString& path, const TPLDictionary *options) const{
	return new XTBImageComplexDictionary(path, options);
}


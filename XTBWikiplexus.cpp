//
//  XTBWikiplexus.cpp
//  XTBook
//
//  Created by Kawada Tomoaki on 7/6/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "XTBWikiplexus.h"
#include "XTBWikiplexusDictionary.h"

std::wstring XTBWikiplexus::name() const{
	return L"XTBook Wikiplexus";
}
std::wstring XTBWikiplexus::identifier() const{
	return L"com.nexhawks.XTBook.Wikiplexus";
}
XTBDictionary *XTBWikiplexus::dictionaryForPath(const XTBSysString& path, const TPLDictionary *options) const{
	return new XTBWikiplexusDictionary(path, options);
}

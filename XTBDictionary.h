//
//  XTBDictionary.h
//  XTBook
//
//  Created by Kawada Tomoaki on 5/22/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#pragma once

#include <string>
#include <tcw/twObject.h>
#include "TPList/TPLDictionary.h"

class XTBDocument;
class XTBDictionaryIterator;

#define XTBDictionaryIdentifierKey			"XTBDictionaryIdentifier"
#define XTBDictionarySchemeKey				"XTBDictionaryScheme"
#define XTBDictionaryTypeIdentifierKey		"XTBDictionaryTypeIdentifier"
#define XTBDictionaryDisplayNameKey		"XTBDictionaryDisplayName"

class XTBDictionary: public twObject{
protected:
	std::wstring m_scheme;
	std::wstring m_identifier;
	std::wstring m_displayName;
public:
	XTBDictionary(const TPLDictionary *options);
	virtual ~XTBDictionary();
	virtual bool hasDocumentForName(const std::wstring&) const=0;
	virtual XTBDocument *documentForName(const std::wstring&) const=0;
	virtual XTBDictionaryIterator *searchWithKey(const std::wstring&) const{return NULL;}
	
	static XTBDictionary *dictionaryWithName(const std::wstring&);
	static void addDictionaryAtPath(const XTBSysString&);
	static void scanDictionaries();
	static std::map<std::wstring, XTBDictionary *>& allDictionaries();
	
	const std::wstring& scheme() const{return m_scheme;}
	const std::wstring& identifier() const{return m_identifier;}
	const std::wstring& displayName() const{return m_displayName;}
};



//
//  XTBSampleDictionary.h
//  XTBook
//
//  Created by Kawada Tomoaki on 5/22/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#pragma once

#include "platform.h"
#include "XTBDictionary.h"

class XTBSampleDictionaryArticle;
class TWParser;
class TWParserConfig;

#define XTBSampleDictionaryDefaultPageKey	"XTBSampleDictionaryDefaultPage"

class XTBSampleDictionary: public XTBDictionary{
	friend class XTBSampleDictionaryArticle;
	friend class XTBSampleDictionaryIterator;
private:
	XTBSysString m_path;
	std::wstring m_defaultPage;
	TWParserConfig *m_config;
	TWParser *m_parser;
	const XTBSysString& path() const{return m_path;}
	XTBSysString pathForArticleWithName(const std::wstring&) const;
	XTBSysString pathForImageArticleWithName(const std::wstring&) const;
protected:
	TWParser *parser() const{return m_parser;}
public:
	XTBSampleDictionary(const XTBSysString& path, const TPLDictionary *options);
	virtual ~XTBSampleDictionary();
	virtual bool hasDocumentForName(const std::wstring&) const;
	virtual XTBDocument *documentForName(const std::wstring&) const;
	
	virtual XTBDictionaryIterator *searchWithKey(const std::wstring&) const;
};




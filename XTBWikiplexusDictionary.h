//
//  XTBWikiplexusDictionary.h
//  XTBook
//
//  Created by Kawada Tomoaki on 7/6/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#pragma once

#include "XTBDictionary.h"

class XTBDicDBReader;
class XTBDicDBMemoryCache;
class XTBIndexDBReader;

const uint32_t XTBFormat1Magic=0xf1c0ffee;

#define XTBWikiplexusArticlesFileKey	"XTBWikiplexusArticlesFile"

#define XTBWikiplexusTemplatesFileKey	"XTBWikiplexusTemplatesFile"

#define XTBWikiplexusSiteInfoFileKey	"XTBWikiplexusSiteInfoFile"

#define XTBWikiplexusSearchFileKey		"XTBWikiplexusSearchFile"

#define XTBWikiplexusSchemeForImagesKey		"XTBWikiplexusSchemeForImages"

class XTBWikiplexusParser;
class XTBWikiplexusParserConfig;
class XTBWikiplexusArticle;
struct XTBWikiplexusSiteInfo;

class XTBWikiplexusDictionary: public XTBDictionary{
	friend class XTBWikiplexusArticle;
	friend class XTBWikiplexusArticleFormat1;
	friend class XTBWikiplexusParser;
	friend class XTBWikiplexusParserConfig;	
	friend class XTBWikiplexusIterator;
	
	XTBWikiplexusSiteInfo *m_siteInfo;
	
	std::wstring m_schemeForImages;
	
	XTBDicDBReader *m_articles;
	XTBDicDBReader *m_templates;
	XTBIndexDBReader *m_searchIndex;
	
	// don't use cache for articles.
	/* XTBDicDBMemoryCache *m_articleCache; */
	XTBDicDBMemoryCache *m_templateCache;
	
	XTBWikiplexusParser *m_parser;
	XTBWikiplexusParserConfig *m_parserConfig;
	
	std::string dbKeyForDocumentName(const std::wstring&) const;
	
	XTBWikiplexusArticle *articleForDbKey(const std::string&) const;
	
protected:
	std::string bytesForDBKey(const std::string&) const;
	std::wstring displayTitleForDBKey(const std::string&) const;
	std::wstring fragmentIdForIndexText(const std::wstring&) const;
	
	XTBWikiplexusParser *parser() const{return m_parser;}
	XTBWikiplexusParserConfig *parserConfig()const{return m_parserConfig;}
	
	std::wstring templateWikiForName(const std::wstring&) const;
	
public:
	XTBWikiplexusDictionary(const XTBSysString&, const TPLDictionary *options);
	virtual ~XTBWikiplexusDictionary();
	
	virtual bool hasDocumentForName(const std::wstring&) const;
	virtual XTBDocument *documentForName(const std::wstring&) const;
	virtual XTBDictionaryIterator *searchWithKey(const std::wstring&) const;
	
	XTBWikiplexusSiteInfo *siteInfo() const{
		return m_siteInfo;
	}
	
	const std::wstring &schemeForImages() const{
		return m_schemeForImages;
	}
};

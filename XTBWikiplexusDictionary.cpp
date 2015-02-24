//
//  XTBWikiplexusDictionary.cpp
//  XTBook
//
//  Created by Kawada Tomoaki on 7/6/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "XTBWikiplexusDictionary.h"
#include "XTBDicDBReader.h"
#include "XTBDicDBMemoryCache.h"
#include "XTBIndexDBReader.h"
#include "XTBWikiplexusParser.h"
#include "XTBWikiplexusParserConfig.h"
#include "XTBWikiplexusArticleFormat1.h"
#include "XTBWikiplexusSiteInfo.h"
#include "XTBWikiplexusIterator.h"

#include "TWiki/TWPreprocessorFrame.h"
#include "TWiki/TWUtils.h"

#include "TPList/TPLDictionary.h"
#include "TPList/TPLString.h"

#include <tcw/twStrConv.h>

#include "commondlg.h"
#include "i18n.h"
#include <assert.h>

XTBWikiplexusDictionary::XTBWikiplexusDictionary(const XTBSysString& path, const TPLDictionary *options):
XTBDictionary(options),
m_articles(NULL),
m_templates(NULL),
m_templateCache(NULL),
m_searchIndex(NULL){
	
	std::wstring articlesFile=L"Articles";
	std::wstring templatesFile=L"Templates";
	std::wstring siteInfoFile=L"SiteInfo.plist";
	std::wstring searchFile;
	
	if(TPLString *value=dynamic_cast<TPLString *>(options->objectForKey(XTBWikiplexusArticlesFileKey))){
		articlesFile=twM2W(value->UTF8String());
	}
	
	if(TPLString *value=dynamic_cast<TPLString *>(options->objectForKey(XTBWikiplexusTemplatesFileKey))){
		templatesFile=twM2W(value->UTF8String());
	}
	
	if(TPLString *value=dynamic_cast<TPLString *>(options->objectForKey(XTBWikiplexusSiteInfoFileKey))){
		siteInfoFile=twM2W(value->UTF8String());
	}
	
	if(TPLString *value=dynamic_cast<TPLString *>(options->objectForKey(XTBWikiplexusSearchFileKey))){
		searchFile=twM2W(value->UTF8String());
	}
	
	if(TPLString *value=dynamic_cast<TPLString *>(options->objectForKey(XTBWikiplexusSchemeForImagesKey))){
		m_schemeForImages=twM2W(value->UTF8String());
	}

	XTBSysString articlesPath;
	XTBSysString templatesPath;
	XTBSysString siteInfoPath;
	XTBSysString searchPath;
	
	articlesPath=path+XTBPathSeparator+XTBSysStringFromWString(articlesFile);
	templatesPath=path+XTBPathSeparator+XTBSysStringFromWString(templatesFile);
	siteInfoPath=path+XTBPathSeparator+XTBSysStringFromWString(siteInfoFile);
	if(!searchFile.empty())
		searchPath=path+XTBPathSeparator+XTBSysStringFromWString(searchFile);
	
	
	try{
		
		// load articles. (required)
		m_articles=new XTBDicDBReader(articlesPath);
		
		// load templates, if it exists.
		if(XTBDoesPathExist(templatesPath+XTBSysText(".db"))){
			m_templates=new XTBDicDBReader(templatesPath);
		}
		
		// load search index.
		if(XTBDoesPathExist(searchPath+XTBSysText(".indexdb"))){
			m_searchIndex=new XTBIndexDBReader(searchPath);
		}
		
	}catch(...){
		if(m_articles)
			delete m_articles;
		if(m_templates)
			delete m_templates;
		if(m_searchIndex)
			delete m_searchIndex;
		throw;
	}
	
	if(m_templates){
		m_templateCache=new XTBDicDBMemoryCache
		(m_templates, 8, 100000);
	}
	
	// load siteinfo. (required)
	m_siteInfo=new XTBWikiplexusSiteInfo(siteInfoPath);
	
	m_parserConfig=new XTBWikiplexusParserConfig(this);
	m_parser=new XTBWikiplexusParser(m_parserConfig, this);
	
}
XTBWikiplexusDictionary::~XTBWikiplexusDictionary(){
	if(m_templateCache)
		delete m_templateCache;
	if(m_articles)
		delete m_articles;
	if(m_templates)
		delete m_templates;
	if(m_searchIndex)
		delete m_searchIndex;
	delete m_siteInfo;
}

std::string XTBWikiplexusDictionary::dbKeyForDocumentName(const std::wstring& name) const{
	return twW2M(TWUtils::dbKeyFor(name));
}

bool XTBWikiplexusDictionary::hasDocumentForName(const std::wstring& name) const{
	std::string dbKey=dbKeyForDocumentName(name);
	return m_articles->pointerForKey(dbKey)!=XTBDicDBInvalidPointer;
}
XTBWikiplexusArticle *XTBWikiplexusDictionary::articleForDbKey(const std::string& dbKey) const{
	return new XTBWikiplexusArticleFormat1(this, dbKey);
}

XTBDocument *XTBWikiplexusDictionary::documentForName(const std::wstring&name) const{
	std::string dbKey=dbKeyForDocumentName(name);
	
	if(m_articles->pointerForKey(dbKey)!=XTBDicDBInvalidPointer){
		return articleForDbKey(dbKey);
	}
	
	return NULL;
}
XTBDictionaryIterator *XTBWikiplexusDictionary::searchWithKey(const std::wstring&name) const{
	if(!m_searchIndex)
		return NULL;
	
	std::string key=twW2M(name);
	unsigned int lowerBound=m_searchIndex->lowerBoundForKey(key);
	unsigned int upperBound=m_searchIndex->upperBoundForKey(key);
	
	return new XTBWikiplexusIterator(this, m_searchIndex, 
									 lowerBound, upperBound);
}

std::string XTBWikiplexusDictionary::bytesForDBKey(const std::string& dbKey) const{
	XTBDicDBPointer ptr=m_articles->pointerForKey(dbKey);
	if(ptr==XTBDicDBInvalidPointer){
		XTBThrowException(L"XTBIOErrorException", NULL);
	}
	return m_articles->contentsAtPointer(ptr);
}

static wchar_t convertToDisplayTitle(wchar_t c){
	if(c==L'_')
		return L' ';
	return c;
}

std::wstring XTBWikiplexusDictionary::displayTitleForDBKey(const std::string& dbKey) const{
	std::wstring title=twM2W(dbKey);
	std::transform(title.begin(), title.end(), title.begin(), convertToDisplayTitle);
	return title;
}

std::wstring XTBWikiplexusDictionary::fragmentIdForIndexText(const std::wstring&text) const{
	return TWPreprocessorFrame::idForHeading(text);
}

std::wstring XTBWikiplexusDictionary::templateWikiForName(const std::wstring &name) const{
	if(name==L"!")
		return L"|";
	if(m_templates){
		std::string dbKey=dbKeyForDocumentName(name);
		XTBDicDBPointer ptr;
		
		XTBSetProgressText(XTBFormat(XTBLocalizedString(L"XTBWikiplexusArticleReadingPage"),
									 name.c_str(), NULL));
		
		if(m_templateCache){
			if(m_templateCache->doesKeyExist(dbKey)){
				std::string text;
				text=m_templateCache->contentsForKey(dbKey);
				
				return twM2W(text);
			}
		}else{
			ptr=m_templates->pointerForKey(dbKey);
			if(ptr!=XTBDicDBInvalidPointer){
				std::string text;
				text=m_templates->contentsAtPointer(ptr);
				return twM2W(text);
			}
		}
	}
	throw std::wstring(L"target not found: "+name);
}

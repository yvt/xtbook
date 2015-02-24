//
//  XTBSampleDictionary.cpp
//  XTBook
//
//  Created by Kawada Tomoaki on 5/22/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "XTBSampleDictionary.h"
#include "TWiki/TWParser.h"
#include "TWiki/TWParserConfig.h"
#include "TPList/TPLString.h"
#include <tcw/twStrConv.h>
#include "XTBSampleDictionaryArticle.h"
#include "XTBSampleDictionaryIterator.h"
#include "XTBSampleDictionaryImageArticle.h"

XTBSampleDictionary::XTBSampleDictionary(const XTBSysString& path, const TPLDictionary *options):
XTBDictionary(options){
	m_path=path;
	m_defaultPage=L"Index";
	
	if(TPLString *str=dynamic_cast<TPLString *>(options->objectForKey(XTBSampleDictionaryDefaultPageKey))){
		m_defaultPage=twM2W(str->UTF8String());
	}
	
	m_config=new TWParserConfig();
	m_parser=new TWParser(m_config);
}
XTBSampleDictionary::~XTBSampleDictionary(){
	delete m_parser;
	delete m_config;
}
bool XTBSampleDictionary::hasDocumentForName(const std::wstring& name) const{
	return XTBDoesPathExist(pathForArticleWithName(name))
	|| XTBDoesPathExist(pathForImageArticleWithName(name));
}
XTBDocument *XTBSampleDictionary::documentForName(const std::wstring& name) const{
	if(XTBDoesPathExist(pathForImageArticleWithName(name)))
		return new XTBSampleDictionaryImageArticle(pathForImageArticleWithName(name));
	else
		return new XTBSampleDictionaryArticle(name, pathForArticleWithName(name), this);
}

XTBSysString XTBSampleDictionary::pathForArticleWithName(const std::wstring& name) const{
	if(name.empty())
		return pathForArticleWithName(m_defaultPage);
	XTBSysString sysName=XTBSysStringFromWString(name+L".txt");
	return m_path+XTBPathSeparator+sysName;
}

XTBSysString XTBSampleDictionary::pathForImageArticleWithName(const std::wstring& name) const{
	if(name.empty())
		return pathForImageArticleWithName(m_defaultPage);
	XTBSysString sysName=XTBSysStringFromWString(name+L".jpg");
	return m_path+XTBPathSeparator+sysName;
}

XTBDictionaryIterator *XTBSampleDictionary::searchWithKey(const std::wstring& name) const{
	return new XTBSampleDictionaryIterator(name, this);
}

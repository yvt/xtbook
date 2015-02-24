//
//  XTBImageComplexDictionary.cpp
//  XTBook
//
//  Created by Kawada Tomoaki on 8/1/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "XTBImageComplexDictionary.h"
#include "XTBDicDBReader.h"
#include "platform.h"
#include "TPList/TPLString.h"
#include "TPList/TPLDictionary.h"
#include <tcw/twStrConv.h>
#include "XTBIndexDBReader.h"
#include "TWiki/TWUtils.h"
#include "XTBImageComplexArticle.h"
#include "XTBImageComplexIterator.h"

XTBImageComplexDictionary::XTBImageComplexDictionary(const XTBSysString& path, const TPLDictionary *options):
XTBDictionary(options),
m_images(NULL){
	
	std::wstring imagesFile=L"Images";
	std::wstring searchFile;
	
	if(TPLString *value=dynamic_cast<TPLString *>(options->objectForKey(XTBImageComplexImagesFileKey))){
		imagesFile=twM2W(value->UTF8String());
	}

	if(TPLString *value=dynamic_cast<TPLString *>(options->objectForKey(XTBImageComplexSearchFileKey))){
		searchFile=twM2W(value->UTF8String());
	}
	
	XTBSysString imagesPath;
	XTBSysString searchPath;
	
	imagesPath=path+XTBPathSeparator+XTBSysStringFromWString(imagesFile);
	if(!searchFile.empty())
		searchPath=path+XTBPathSeparator+XTBSysStringFromWString(searchFile);
	
	m_searchIndex=NULL;
	
	try{
		
		// load articles. (required)
		m_images=new XTBDicDBReader(imagesPath);
		
		// load search index.
		if(XTBDoesPathExist(searchPath+XTBSysText(".indexdb"))){
			m_searchIndex=new XTBIndexDBReader(searchPath);
		}
		
	}catch(...){
		if(m_images)
			delete m_images;
		if(m_searchIndex)
			delete m_searchIndex;
		throw;
	}
	
}
XTBImageComplexDictionary::~XTBImageComplexDictionary(){
	if(m_images)
		delete m_images;
	if(m_searchIndex)
		delete m_searchIndex;
}

std::string XTBImageComplexDictionary::dbKeyForDocumentName(const std::wstring& name) const{
	return twW2M(TWUtils::dbKeyFor(name));
}

bool XTBImageComplexDictionary::hasDocumentForName(const std::wstring& name) const{
	std::string dbKey=dbKeyForDocumentName(name);
	return m_images->pointerForKey(dbKey)!=XTBDicDBInvalidPointer;
}
XTBImageComplexArticle *XTBImageComplexDictionary::articleForDbKey(const std::string& dbKey) const{
	return new XTBImageComplexArticle(this, dbKey);
}

XTBDocument *XTBImageComplexDictionary::documentForName(const std::wstring&name) const{
	std::string dbKey=dbKeyForDocumentName(name);
	
	if(m_images->pointerForKey(dbKey)!=XTBDicDBInvalidPointer){
		return articleForDbKey(dbKey);
	}
	
	return NULL;
}
XTBDictionaryIterator *XTBImageComplexDictionary::searchWithKey(const std::wstring&name) const{
	if(!m_searchIndex)
		return NULL;
	
	std::string key=twW2M(name);
	unsigned int lowerBound=m_searchIndex->lowerBoundForKey(key);
	unsigned int upperBound=m_searchIndex->upperBoundForKey(key);
	
	return new XTBImageComplexIterator(this, m_searchIndex, 
									 lowerBound, upperBound);
}

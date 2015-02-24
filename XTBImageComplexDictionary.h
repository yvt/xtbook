//
//  XTBImageComplexDictionary.h
//  XTBook
//
//  Created by Kawada Tomoaki on 8/1/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#pragma once

#include "XTBDictionary.h"

class XTBDicDBReader;
class XTBIndexDBReader;
class XTBImageComplexArticle;

#define XTBImageComplexImagesFileKey	"XTBImageComplexImagesFile"

#define XTBImageComplexSearchFileKey		"XTBImageComplexSearchFile"



class XTBImageComplexDictionary: public XTBDictionary{
	friend class XTBImageComplexArticle;
	XTBDicDBReader *m_images;
	XTBIndexDBReader *m_searchIndex;
	
	std::string dbKeyForDocumentName(const std::wstring& name) const;
	XTBImageComplexArticle *articleForDbKey(const std::string& dbKey) const;
public:
	XTBImageComplexDictionary(const XTBSysString&, const TPLDictionary *options);
	virtual ~XTBImageComplexDictionary();
	
	virtual bool hasDocumentForName(const std::wstring&) const;
	virtual XTBDocument *documentForName(const std::wstring&) const;
	virtual XTBDictionaryIterator *searchWithKey(const std::wstring&) const;
};

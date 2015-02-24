//
//  XTBImageComplexArticle.h
//  XTBook
//
//  Created by Kawada Tomoaki on 8/1/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#pragma once

#include "XTBImageArticle.h"
#include "XTBDicDBReader.h"

class XTBImageComplexDictionary;

class XTBImageComplexArticle: public XTBImageArticle{

	mutable bool m_pointerLoaded;
	mutable XTBDicDBPointer m_pointer;
	
	mutable bool m_imageLoaded;
	mutable XTBImage *m_image;
	mutable bool m_dimensionLoaded;
	mutable twSize m_dimension;
	
	
	std::string m_dbKey;
	const XTBImageComplexDictionary *m_dic;
	
	XTBDicDBPointer pointer() const;
public:
	XTBImageComplexArticle(const XTBImageComplexDictionary *,
						 const std::string& dbKey);
	virtual ~XTBImageComplexArticle();
	
	virtual twSize dimension() const;
	virtual XTBImage *image() const;
	
	const XTBImageComplexDictionary *dictionary() const{return m_dic;}
};
//
//  XTBSampleDictionaryImageArticle.h
//  XTBook
//
//  Created by Kawada Tomoaki on 7/29/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#pragma once

#include "platform.h"
#include "XTBImageArticle.h"

class XTBSampleDictionaryImageArticle: public XTBImageArticle{
	XTBSysString m_path;
	mutable XTBImage *m_image;
public:
	XTBSampleDictionaryImageArticle(const XTBSysString& path);
	virtual ~XTBSampleDictionaryImageArticle();
	
	virtual twSize dimension() const;
	virtual XTBImage *image() const;
};


//
//  XTBImageArticle.h
//  XTBook
//
//  Created by Kawada Tomoaki on 5/27/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#pragma once

#include <tcw/twTypes.h>
#include <tcw/twDC.h>
#include "XTBDocument.h"
#include "XTBImage.h"

class XTBImageArticle: public XTBDocument{
public:
	XTBImageArticle(){}
	virtual ~XTBImageArticle(){}
	
	virtual twSize dimension() const=0;
	
	/** XTBImageArticle must hold image. */
	virtual XTBImage *image() const=0;
};

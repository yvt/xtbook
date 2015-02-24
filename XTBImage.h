//
//  XTBImage.h
//  XTBook
//
//  Created by Kawada Tomoaki on 7/29/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#pragma once

#include <tcw/twObject.h>
#include <tcw/twDC.h>
#include "XTBStream.h"

class XTBImage: twObject{
	twDC *m_dc;
	twSize m_dimension;
public:
	XTBImage(twDC *,
			 twSize);
	virtual ~XTBImage();
	
	static XTBImage *imageForStream(XTBStream *);
	
	twDC *dc() const{return m_dc;}
	twSize dimension() const{return m_dimension;}
};

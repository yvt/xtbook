//
//  XTBImage.cpp
//  XTBook
//
//  Created by Kawada Tomoaki on 7/29/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "XTBImage.h"
#include "XTBImageLoader.h"

XTBImage::XTBImage(twDC *dc, twSize dim):
m_dc(dc), m_dimension(dim){}

XTBImage::~XTBImage(){
	delete m_dc;
}

XTBImage *XTBImage::imageForStream(XTBStream *str){
	twDC *dc=XTBImageLoader::dcForStream(str);
	return new XTBImage(dc, dc->getClipRect().size());
}


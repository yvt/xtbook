//
//  XTBSampleDictionaryImageArticle.cpp
//  XTBook
//
//  Created by Kawada Tomoaki on 7/29/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "XTBSampleDictionaryImageArticle.h"
#include "XTBFileStream.h"
#include <memory>
#include "XTBImageLoader.h"

XTBSampleDictionaryImageArticle::XTBSampleDictionaryImageArticle(const XTBSysString& path):
m_path(path),
m_image(NULL){
	
}

XTBSampleDictionaryImageArticle::~XTBSampleDictionaryImageArticle(){
	delete m_image;
}

twSize XTBSampleDictionaryImageArticle::dimension() const{
	return image()->dimension();
}

XTBImage *XTBSampleDictionaryImageArticle::image() const{
	if(!m_image){
		std::auto_ptr<XTBFileStream> stream(XTBFileStream::streamForReadingAtPath(m_path));
		m_image=XTBImage::imageForStream(stream.get());
	}
	return m_image;
}


//
//  XTBImageComplexArticle.cpp
//  XTBook
//
//  Created by Kawada Tomoaki on 8/1/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "XTBImageComplexArticle.h"
#include "XTBImageComplexDictionary.h"
#include "XTBException.h"
#include "XTBMemoryStream.h"

XTBImageComplexArticle::XTBImageComplexArticle(const XTBImageComplexDictionary *dic,
											   const std::string& dbKey):
m_dic(dic),
m_imageLoaded(false),
m_dbKey(dbKey),
m_dimensionLoaded(false),
m_image(NULL),
m_pointerLoaded(false){
}

XTBImageComplexArticle::~XTBImageComplexArticle(){
	if(m_image)
		delete m_image;
}

twSize XTBImageComplexArticle::dimension() const{
	if(!m_dimensionLoaded){
		XTBDicDBReader *reader=m_dic->m_images;
		XTBDicDBPointer ptr=pointer();
		std::string header=reader->contentsAtPointer(ptr, 4);
		
		m_dimension.w=((unsigned char)header[1]);
		m_dimension.w|=((int)(unsigned char)header[0])<<8;
		m_dimension.h=((unsigned char)header[3]);
		m_dimension.h|=((int)(unsigned char)header[2])<<8;
		
		m_dimensionLoaded=true;
	}
	return m_dimension;
}

XTBImage *XTBImageComplexArticle::image() const{
	if(!m_imageLoaded){
		XTBDicDBReader *reader=m_dic->m_images;
		XTBDicDBPointer ptr=pointer();
		std::string bytes=reader->contentsAtPointer(ptr);
		XTBMemoryStream memoryStream(bytes.data(), bytes.size());
		memoryStream.seekToOffset(4);
		
		m_image=XTBImage::imageForStream(&memoryStream);
		
		m_imageLoaded=true;
	}
	return m_image;
}

XTBDicDBPointer XTBImageComplexArticle::pointer() const{
	if(!m_pointerLoaded){
		XTBDicDBReader *reader=m_dic->m_images;
		m_pointer=reader->pointerForKey(m_dbKey);
		if(m_pointer==XTBDicDBInvalidPointer){
			XTBThrowException(L"XTBIOErrorException", NULL);
		}
		m_pointerLoaded=true;
	}
	return m_pointer;
}

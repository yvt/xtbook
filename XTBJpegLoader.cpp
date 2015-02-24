//
//  XTBJpegLoader.cpp
//  XTBook
//
//  Created by Kawada Tomoaki on 7/29/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "XTBJpegLoader.h"
#include "XTBRichgelJpegStream.h"
#include "XTBException.h"
#include <tcw/twSDLDC.h>
#include <assert.h>

bool XTBJpegLoader::probe(XTBStream *steam){
	unsigned char buf[2];
	if(steam->readToBuffer(buf, 2)<2)
		return false;
	if(buf[0]==0xff && buf[1]==0xd8)
		return true;
	return false;
}

#pragma mark - Decoder

XTBJpegLoader::XTBJpegLoader(XTBStream *stream):
m_stream(stream){
	m_stream2=new XTBRichgelJpegStream(m_stream);
	m_decoder=new jpeg_decoder(m_stream2, false);
	if(m_decoder->get_error_code()){
		int errorCode=m_decoder->get_error_code();
		delete m_decoder;
		delete m_stream2;
		
		XTBThrowException(L"XTBRichgelJpegException", 
						  XTBFormatStd(L"0x%08x", errorCode).c_str(), NULL);
	}
	
	
	
}

XTBJpegLoader::~XTBJpegLoader(){
	delete m_decoder;
	delete m_stream2;
}

twDC *XTBJpegLoader::image(){
	if(m_decoder->begin()){
		int errorCode=m_decoder->get_error_code();
		delete m_decoder;
		delete m_stream2;
		
		XTBThrowException(L"XTBRichgelJpegException", 
						  XTBFormatStd(L"0x%08x", errorCode).c_str(), NULL);
	}
	
	uint8_t *lineBuffer; // don't need to malloc/free
	twSDLDC *image;
	SDL_Surface *surf;
	uint lineLen;
	
	
	int width=m_decoder->get_width();
	int height=m_decoder->get_height();
	
	image=new twSDLDC(twSize(width, height));
	surf=image->getSurface();
	
	uint16_t *dest=(uint16_t *)surf->pixels;
	
	assert(surf->format->BitsPerPixel==16);
	
	int y=0;
	while(!m_decoder->decode((void **)&lineBuffer, &lineLen)){
		
		if(m_decoder->get_num_components()==1){
			convert8To565(dest, lineBuffer, m_decoder->get_width(), y);
		}else if(m_decoder->get_num_components()==3){
			convert888To565(dest, lineBuffer, m_decoder->get_width(), y);
		}
		
		dest+=m_decoder->get_width();
		
		y++;
	}
	
	return image;
	
}

#pragma mark - Converter

static const uint8_t g_ditherMap[]={
	0, 4, 1, 2,
	6, 2, 7, 3,
	2, 6, 1, 5,
	8, 4, 7, 3
};

void XTBJpegLoader::convert888To565(uint16_t *dest, 
									const uint8_t *src, 
									size_t pixels,
									int y){
	register const uint8_t *ditherMap=g_ditherMap;
	register int ditherPos=(y&3)*4;
	register int ditherLimit=ditherPos+4;
	while(pixels--){
		register unsigned int out;
		register uint8_t dither=ditherMap[ditherPos];
		register unsigned int tmp;
		
		tmp=*(src++);
		if(tmp<(255-7))
			tmp+=dither;
		out=(tmp>>3);
		
		out<<=6;
		
		tmp=*(src++);
		if(tmp<(255-3))
			tmp+=(dither>>1);
		out|=(tmp>>2);
		
		out<<=5;
		
		tmp=*(src++);
		if(tmp<(255-7))
			tmp+=dither;
		out|=(tmp>>3);
		*(dest++)=out;
		src++;
		ditherPos++;
		if(ditherPos==ditherLimit)
			ditherPos-=4;
	}
}

void XTBJpegLoader::convert8To565(uint16_t *dest, 
									const uint8_t *src, 
									size_t pixels,
								  int y){
	while(pixels--){
		register unsigned int out, v;
		v=*(src++)>>3;
		out=v;
		out<<=5;
		out|=v;
		out<<=6;
		out|=v;
		*(dest++)=out;
	}
}


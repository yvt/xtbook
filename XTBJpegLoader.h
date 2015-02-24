//
//  XTBJpegLoader.h
//  XTBook
//
//  Created by Kawada Tomoaki on 7/29/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#pragma once

#include "XTBStream.h"

class jpeg_decoder;
class XTBRichgelJpegStream;

class XTBJpegLoader{
	XTBStream *m_stream;
	XTBRichgelJpegStream *m_stream2;
	jpeg_decoder *m_decoder;
	
	void convert888To565(uint16_t *dest,
						 const uint8_t *src,
						 size_t pixels,
						 int y);
	void convert8To565(uint16_t *dest,
						 const uint8_t *src,
						 size_t pixels,
					   int y);
	
public:
	
	static bool probe(XTBStream *);
	
	XTBJpegLoader(XTBStream *);
	~XTBJpegLoader();
	
	twDC *image();
};

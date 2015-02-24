//
//  XTBRichgelJpegStream.h
//  XTBook
//
//  Created by Kawada Tomoaki on 7/29/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#pragma once

#include "RichgelJpeg/jpegdecoder.h"
#include "XTBStream.h"

class XTBRichgelJpegStream: public jpeg_decoder_stream{
	XTBStream *m_stream;
public:
	XTBRichgelJpegStream(XTBStream *);
	virtual ~XTBRichgelJpegStream();
	
	virtual int read(uchar *Pbuf, int max_bytes_to_read, bool *Peof_flag);
	
};

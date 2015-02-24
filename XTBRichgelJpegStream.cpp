//
//  XTBRichgelJpegStream.cpp
//  XTBook
//
//  Created by Kawada Tomoaki on 7/29/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "XTBRichgelJpegStream.h"

XTBRichgelJpegStream::XTBRichgelJpegStream(XTBStream *stream):
m_stream(stream){}

XTBRichgelJpegStream::~XTBRichgelJpegStream(){}

int XTBRichgelJpegStream::read(uchar *Pbuf, int max_bytes_to_read,
							   bool *Peof_flag){
	size_t readBytes;
	readBytes=m_stream->readToBuffer(Pbuf, max_bytes_to_read);
	if(readBytes<max_bytes_to_read)
		*Peof_flag=true;
	return readBytes;
}

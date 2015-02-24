//
//  XTBImageLoader.cpp
//  XTBook
//
//  Created by Kawada Tomoaki on 5/28/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "XTBImageLoader.h"
#include "XTBJpegLoader.h"
#include "i18n.h"

twDC *XTBImageLoader::dcForStream(XTBStream *stream){
	XTBOffset offset=stream->position();
	bool found;
	
	found=XTBJpegLoader::probe(stream);
	stream->seekToOffset(offset);
	if(found){
		XTBJpegLoader loader(stream);
		return loader.image();
	}
	
	XTBThrowException(L"XTBInvalidFormatException", NULL);
}

//
//  XTBWikiplexusGalleryExtension.h
//  XTBook
//
//  Created by Kawada Tomoaki on 8/4/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#pragma once

#include "TWiki/TWParserExtension.h"

class XTBWikiplexusGalleryExtension: public TWParserExtension{
public:
	virtual std::wstring runHook(TWParser *parser,
								 TWPreprocessorFrame *frame,
								 const std::wstring& attr,
								 const std::wstring& contents);
};


//
//  XTBWikiplexusFootnoteExtension.h
//  XTBook
//
//  Created by 河田 智明 on 8/3/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#pragma once

#include "TWiki/TWParserExtension.h"

class XTBWikiplexusFootnoteExtension: public TWParserExtension{
public:
	virtual std::wstring runHook(TWParser *parser,
								 TWPreprocessorFrame *frame,
								 const std::wstring& attr,
								 const std::wstring& contents);
};

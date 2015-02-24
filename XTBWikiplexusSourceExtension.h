//
//  XTBWikiplexusSourceExtension.h
//  XTBook
//
//  Created by 河田 智明 on 8/4/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#pragma once

#include "TWiki/TWParserExtension.h"

class XTBWikiplexusSourceExtension: public TWParserExtension{
public:
	virtual std::wstring runHook(TWParser *parser,
								 TWPreprocessorFrame *frame,
								 const std::wstring& attr,
								 const std::wstring& contents);
};




//
//  XTBWikiplexusMathCodizeExtension.h
//  XTBook
//
//  Created by 河田 智明 on 9/19/11.
//  Copyright (c) 2011 Nexhawks. All rights reserved.
//

#pragma once


#include "TWiki/TWParserExtension.h"

class XTBWikiplexusMathCodizeExtension: public TWParserExtension{
public:
	virtual std::wstring runHook(TWParser *parser,
								 TWPreprocessorFrame *frame,
								 const std::wstring& attr,
								 const std::wstring& contents);
};


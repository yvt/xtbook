//
//  TWRandomSelectionExtension.h
//  XTBook
//
//  Created by Kawada Tomoaki on 8/1/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#pragma once

#include "TWParserExtension.h"

class TWRandomSelectionExtension: public TWParserExtension{
	double randomFloat(double) const;
public:
	virtual std::wstring runHook(TWParser *parser,
								 TWPreprocessorFrame *frame,
								 const std::wstring& attr,
								 const std::wstring& contents);
};
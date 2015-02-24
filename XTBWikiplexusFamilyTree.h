//
//  XTBWikiplexusFamilyTree.h
//  XTBook
//
//  Created by 河田 智明 on 9/25/11.
//  Copyright (c) 2011 Nexhawks. All rights reserved.
//


#pragma once

#include "TWiki/TWPreparsedParserFunction.h"

class XTBWikiplexusFamilyTree: public TWPreparsedParserFunction{
	
	
public:
	virtual ~XTBWikiplexusFamilyTree(){}
	virtual std::wstring execute(const std::wstring& arg0, const std::vector<std::wstring>& args, TWPreprocessorFrame *);
};



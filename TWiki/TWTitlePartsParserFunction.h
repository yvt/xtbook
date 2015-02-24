//
//  TWTitlePartsParserFunction.h
//  XTBook
//
//  Created by 河田 智明 on 9/23/11.
//  Copyright (c) 2011 Nexhawks. All rights reserved.
//

#pragma once

#include "TWPreparsedParserFunction.h"

class TWTitlePartsParserFunction: public TWPreparsedParserFunction{
public:	
	virtual std::wstring execute(const std::wstring& arg0, const std::vector<std::wstring>& args, TWPreprocessorFrame *);
};



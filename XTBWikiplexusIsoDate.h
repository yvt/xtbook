//
//  XTBWikiplexusIsoDate.h
//  XTBook
//
//  Created by 河田 智明 on 8/4/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#pragma once

#include "TWiki/TWPreparsedParserFunction.h"

class XTBWikiplexusIsoDate: public TWPreparsedParserFunction{
	
	
public:
	virtual ~XTBWikiplexusIsoDate(){}
	virtual std::wstring execute(const std::wstring& arg0, const std::vector<std::wstring>& args, TWPreprocessorFrame *);
};



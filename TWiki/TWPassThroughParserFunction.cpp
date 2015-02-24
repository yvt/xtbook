//
//  TWPassThroughParserFunction.cpp
//  XTBook
//
//  Created by Kawada Tomoaki on 7/26/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "TWPassThroughParserFunction.h"
#include <algorithm>


std::wstring TWPassThroughParserFunction::execute(const std::wstring &arg0, TWPreprocessorFrame *){
	return arg0;
}

//
//  XTBWikiplexusIsoDate.cpp
//  XTBook
//
//  Created by 河田 智明 on 8/4/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "XTBWikiplexusIsoDate.h"

std::wstring XTBWikiplexusIsoDate::execute
(const std::wstring &arg0, 
 const std::vector<std::wstring> &args, 
 TWPreprocessorFrame *){
	// no need! performance precedence!
	if(args.empty())
		return std::wstring();
	return args[0];
}


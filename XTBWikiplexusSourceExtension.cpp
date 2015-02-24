//
//  XTBWikiplexusSourceExtension.cpp
//  XTBook
//
//  Created by 河田 智明 on 8/4/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "XTBWikiplexusSourceExtension.h"
#include "TWiki/TWPreprocessorFrame.h"
#include "TWiki/TWParser.h"
#include "TWiki/TWUtils.h"

std::wstring XTBWikiplexusSourceExtension::runHook
(TWParser *parser, 
 TWPreprocessorFrame *frame, 
 const std::wstring &attr, 
 const std::wstring &contents){
	
	return parser->recursiveTagParse
	(L"<pre "+attr+L">"+TWUtils::escapeSpecialChars(contents)+L"</pre>",frame);
	
}


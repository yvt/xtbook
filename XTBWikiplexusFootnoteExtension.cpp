//
//  XTBWikiplexusFootnoteExtension.cpp
//  XTBook
//
//  Created by 河田 智明 on 8/3/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "XTBWikiplexusFootnoteExtension.h"
#include "TWiki/TWUtils.h"
#include "TWiki/TWParser.h"

static int g_footNodeId=0;

std::wstring XTBWikiplexusFootnoteExtension::runHook
(TWParser *parser, TWPreprocessorFrame *frame,
 const std::wstring &attr, const std::wstring &contents){
	
	if(contents.empty())
		return std::wstring();
	
	std::wstring outHtml;
	std::wstring footnoteId=L"footnote"+TWUtils::stringWithIntValue(g_footNodeId);
	
	
	
	outHtml=L"<a href=\"xtbook:footnote#";
	outHtml+=footnoteId;
	outHtml+=L"\" id=\"";
	outHtml+=footnoteId;
	outHtml+=L"\">";
	
	// write footnote body.
	outHtml+=L"<div style=\"display: none;\">";
	outHtml+=parser->recursiveTagParse(contents, frame);
	outHtml+=L"</div>";
	
	// write footnote link.
	outHtml+=L"<sup>[*]</sup>";
	outHtml+=L"</a>";
	
	g_footNodeId++;
	return outHtml;
}


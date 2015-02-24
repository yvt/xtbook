//
//  XTBWikiplexusGalleryExtension.cpp
//  XTBook
//
//  Created by Kawada Tomoaki on 8/4/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "XTBWikiplexusGalleryExtension.h"
#include "TWiki/TWUtils.h"
#include "TWiki/TWParser.h"

std::wstring XTBWikiplexusGalleryExtension::runHook
(TWParser *parser, 
 TWPreprocessorFrame *frame, 
 const std::wstring &attr, 
 const std::wstring &contents){
	std::vector<std::wstring> lines;
	lines=TWUtils::split(contents, L"\n");
	
	std::wstring outHtml;
	
	int imagesInLastRow=0;
	
	outHtml+=L"<table><tr>";
	
	for(size_t i=0;i<lines.size();i++){
		std::wstring line=TWUtils::trimString(lines[i]);
		if(line.empty())
			continue;
		
		if(imagesInLastRow>=3){
			outHtml+=L"</tr><tr>";
			imagesInLastRow=0;
		}
		
		outHtml+=L"<td>[[";
		outHtml+=line;
		outHtml+=L"|thumb|140px";
		outHtml+=L"]]</td>";
		
		imagesInLastRow++;
	}
	
	outHtml+=L"</tr></table>";
	
	return parser->recursiveTagParse(outHtml, frame);
}


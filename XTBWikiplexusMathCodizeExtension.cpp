//
//  XTBWikiplexusMathCodizeExtension.cpp
//  XTBook
//
//  Created by 河田 智明 on 9/19/11.
//  Copyright (c) 2011 Nexhawks. All rights reserved.
//


#include "XTBWikiplexusMathCodizeExtension.h"
#include "TWiki/TWUtils.h"
#include "TWiki/TWParser.h"


std::wstring XTBWikiplexusMathCodizeExtension::runHook
(TWParser *parser, TWPreprocessorFrame *frame,
 const std::wstring &attr, const std::wstring &contents){
	
	if(contents.empty())
		return std::wstring();
	
	std::wstring outHtml;
	std::wstring s;
	s=TWUtils::escapeSpecialChars(contents);
	
	const wchar_t *search=L"\n";
	std::wstring::size_type pos=0, nextPos;
	while(pos<s.size()){
		nextPos=s.find_first_of(search, pos);
		if(nextPos==std::wstring::npos){
			outHtml.append(s, pos, s.size()-pos);
			break;
		}
		
		outHtml.append(s, pos, nextPos-pos);
		outHtml+=L"<br />";
		pos=nextPos+1;
	}
	
	
	return outHtml;
}

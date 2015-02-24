/*
 *  twFont.cpp
 *  
 *
 *  Created by Nexhawks on 10/01/16.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#include <tcw/twFont.h>

twFont::twFont(){
	
}

twFont::~twFont(){
	
}

std::wstring twFont::wrapString(const std::wstring& str, int maxWidth) const{
	std::wstring outStr;
	outStr.reserve(str.size());
	
	std::wstring::size_type pos=0;
	int width=0;
	std::wstring buf=L" ";
	
	while(pos<str.size()){
		if(str[pos]==L'\n'){
			// force-wrap.
			outStr+=L'\n';
			width=0;
			pos++;
			continue;
		}
		buf[0]=str[pos];
		int charWidth=measure(buf).w;
		if(width+charWidth>maxWidth){
			// wrap.
			outStr+=L'\n';
			width=0;
		}
		outStr+=str[pos];
		width+=charWidth;
		pos++;
	}
	return outStr;
}



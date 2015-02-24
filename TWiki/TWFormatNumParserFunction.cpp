//
//  TWFormatNumParserFunction.cpp
//  XTBook
//
//  Created by Kawada Tomoaki on 7/26/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//
/*
 * This file is part of XTBook.
 *
 * XTBook is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * XTBook is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with XTBook.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "TWFormatNumParserFunction.h"

std::wstring TWFormatNumParserFunction::execute(const std::wstring& arg0, const std::vector<std::wstring>& args, TWPreprocessorFrame *){
	bool reversed=false;
	if(args.size()>=1){
		const std::wstring& s=args[0];
		if(s.find(L'R')!=std::wstring::npos)
			reversed=true;
	}
	
	std::wstring out;
	std::wstring inStr;
	if(arg0.empty())
		return inStr;
	if(arg0[0]==L'-'){
		inStr=arg0.substr(1);
		out+=L'-';
	}else{
		inStr=arg0;
	}
	
	if(reversed){
		for(size_t i=0;i<inStr.size();i++){
			if(inStr[i]==L',')
				continue;
			out+=inStr[i];
		}
	}else{
		std::wstring::size_type pos=inStr.find(L'.');
		std::wstring::size_type interval=3;
		if(pos==std::wstring::npos)
			pos=inStr.size();
		out+=inStr.substr(0, pos%interval);
		for(size_t i=pos%interval;i<pos;i+=interval){
			out+=L',';
			out+=inStr.substr(i, 3);
		}
		out+=inStr.substr(pos);
	}
	
	return out;
}





//
//  TWSwitchParserFunction.cpp
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

#include "TWSwitchParserFunction.h"
#include "TWPreprocessorFrame.h"
#include "TWUtils.h"

static const std::wstring TWSwitchParserFunctionDefault=L"#default";

TWPreprocessorNode *TWSwitchParserFunction::execute(const std::wstring& arg0, const std::vector<TWPreprocessorNode *> args, TWPreprocessorFrame *frame){
	bool found=false;
	bool defaultFound=false;
	TWPreprocessorNode *defaultArg=NULL;
	bool lastItemHadNoEquals=false;
	TWPreprocessorNode *lastValueNode=NULL;
	std::wstring test;
	std::wstring inArg=TWUtils::trimString(arg0);
	for(size_t i=0;i<args.size();i++){
		TWPreprocessorNode *arg=args[i];
		TWPreprocessorNodePart bits;
		bits=arg->splitArg();
		if(bits.index.empty()){
			// found "="!
			lastItemHadNoEquals=false;
			if(found){
				return bits.valueNode;
			}else{
				test=frame->expand(bits.nameNode, 0);
				test=TWUtils::trimString(test);
				if(TWUtils::isEqual(test, inArg)){
					return bits.valueNode;
				}else if(defaultFound || TWUtils::matchStart(test, TWSwitchParserFunctionDefault)){
					defaultArg=bits.valueNode;
				}
			}
		}else{
			lastItemHadNoEquals=true;
			test=frame->expand(bits.valueNode, 0);
			test=TWUtils::trimString(test);
			lastValueNode=bits.valueNode;
			if(TWUtils::isEqual(test, inArg)){
				found=true;
			}else if(TWUtils::matchStart(test, TWSwitchParserFunctionDefault)){
				defaultFound=true;
			}
		}
	}
	
	if(lastItemHadNoEquals){
		return lastValueNode;
	}else if(defaultArg){
		return defaultArg;
	}else{
		return NULL;
	}
}

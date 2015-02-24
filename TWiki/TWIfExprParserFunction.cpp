//
//  TWIfExprParserFunction.cpp
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

#include "TWIfExprParserFunction.h"
#include "TWUtils.h"
#include "TWNumber.h"
#include "TWExpressionParserFunction.h"

TWPreprocessorNode *TWIfExprParserFunction::execute(const std::wstring& arg0, const std::vector<TWPreprocessorNode *> args, TWPreprocessorFrame *){
	
	bool isTrue=false;
	try{
		TWNumber num;
		num=TWUtils::evaluateExpression(arg0);
		if(!num.isZero())
			isTrue=true;
	}catch(const std::wstring& str){
		std:wstring outs;
		outs=L"<strong class=\"error\">"+TWUtils::escapeHTMLSpecialChars(str)+L"</strong>";
		throw outs;
	}
	
	if(!isTrue){
		// empty.
		if(args.size()<2)
			return NULL;
		else
			return args[1];
	}else{
		// true.
		if(args.size()<1)
			return NULL;
		else
			return args[0];
	}
}

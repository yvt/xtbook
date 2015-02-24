//
//  TWExpressionParserFunction.cpp
//  XTBook
//
//  Created by Kawada Tomoaki on 7/20/11.
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

#include "TWExpressionParserFunction.h"
#include "TWUtils.h"

//void XTBDumpWString(const std::wstring&);

std::wstring TWExpressionParserFunction::execute(const std::wstring& arg0, TWPreprocessorFrame *){
	try{
		return TWUtils::evaluateExpression(arg0);
	}catch(const std::wstring& str){
		//XTBDumpWString(arg0);
		std:wstring outs;
		outs=L"<strong class=\"error\">"+TWUtils::escapeHTMLSpecialChars(str)+L"</strong>";
		return outs;
	}
}

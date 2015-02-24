//
//  TWBasePageNameVariable.cpp
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

#include "TWBasePageNameVariable.h"
#include "TWParser.h"
#include "TWUtils.h"

TWBasePageNameVariable::TWBasePageNameVariable(TWParser *parser):
m_parser(parser){
	
}

std::wstring TWBasePageNameVariable::execute(const std::wstring& arg0, const std::vector<std::wstring>& args, TWPreprocessorFrame *){
	std::wstring s=TWUtils::displayTitleFor(args.empty()?(m_parser->parsedTitle().dbKey):
											m_parser->parseTitle(args[0]).dbKey);
	std::wstring::size_type pos;
	pos=s.rfind(L'/');
	if(pos==std::wstring::npos)
		return std::wstring();
	else
		return s.substr(0,pos);
}

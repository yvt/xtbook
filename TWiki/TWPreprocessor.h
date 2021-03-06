/*
 *  TWPreprocessor.h
 *  XTBook
 *
 *  Created by Nexhawks on 2/23/11.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */
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

#pragma once

#include "TWPreprocessorStack.h"

class TWParser;
class TWPreprocessorNode;

#include <map>
#include <string>

enum {
	TWPreprocessorFlagsInclusion   = 1,
	TWPreprocessorFlagsNoArgs	   = 2,
	TWPreprocessorFlagsNoTemplates = 4,
	TWPreprocessorFlagsNoIgnore    = 8
};

struct TWPreprocessorRule{
	std::wstring start;
	std::wstring end;
	std::map<int, std::wstring> names;
	int minCount, maxCount;
};

class TWPreprocessor{
protected:
	TWParser *m_parser;
	std::map<std::wstring, TWPreprocessorRule> m_rules;
	std::wstring m_title;
	TWPreprocessorStackArena m_stackArena;
	
public:
	TWPreprocessor(TWParser *);
	
	TWParser *parser()const{return m_parser;}
	
	TWPreprocessorNode *nodeFromText(const std::wstring& text, int flags);
	
};
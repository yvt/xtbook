//
//  TWTransformParserFunction.h
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

#pragma once
#include "TWSimpleParserFunction.h"

class TWPreprocessorFrame;

typedef wchar_t (*TWTransformParserFunctionCallback)(wchar_t);

class TWTransformParserFunction: public TWSimpleParserFunction{
	TWTransformParserFunctionCallback m_callback;
	bool m_first;
public:
	TWTransformParserFunction(TWTransformParserFunctionCallback, bool first);
	virtual std::wstring execute(const std::wstring& arg0, TWPreprocessorFrame *);
};



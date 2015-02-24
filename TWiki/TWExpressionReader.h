//
//  TWExpressionReader.h
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

#pragma once

#include <string>

enum TWExpressionTokenType{
	TWExpressionTokenTypeInvalid=0,
	TWExpressionTokenTypeNumber,	// numeric value + '.'
	TWExpressionTokenTypeOperator,	// not numeric nor alphabet
	TWExpressionTokenTypeSymbol,	// alphabet only (no number!)
	TWExpressionTokenTypeEOS
};

class TWExpressionReader{
	const std::wstring& m_inputString;
	std::wstring::size_type m_pos;
	bool m_isCurrentTokenRead;
	TWExpressionTokenType m_currentTokenType;
	std::wstring::size_type m_currentTokenLength;
	std::wstring::size_type m_currentTokenPos;
	
	void readCurrentToken();
	
	void skipWhitespaces();
	
	TWExpressionTokenType tokenTypeforChar(wchar_t) const;
	
	bool canTokenCharContinued(wchar_t) const;
public:
	TWExpressionReader(const std::wstring&);
	~TWExpressionReader();
	
	TWExpressionTokenType currentTokenType();
	std::wstring::size_type currentTokenPos();
	std::wstring::size_type currentTokenLength();
	
	wchar_t currentTokenChar();
	std::wstring currentToken();
	
	void forwardPointer();
};


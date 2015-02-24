//
//  TWExpressionReader.cpp
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

#include "TWExpressionReader.h"
#include <assert.h>


TWExpressionReader::TWExpressionReader(const std::wstring& inputString):
m_inputString(inputString),
m_pos(0),
m_isCurrentTokenRead(false),
m_currentTokenType(TWExpressionTokenTypeInvalid),
m_currentTokenLength(0),
m_currentTokenPos(0){
	
}

TWExpressionReader::~TWExpressionReader(){
	
}

bool TWExpressionReader::canTokenCharContinued(wchar_t c) const{
	if(c==L'+' || c==L'-' || c==L'(' || c==L')' || c=='*' || c=='/' || c=='^')
		return false;
	return true;
}

void TWExpressionReader::readCurrentToken(){
	if(m_isCurrentTokenRead)
		return;
	
	m_isCurrentTokenRead=true;
	
	skipWhitespaces();
	
	if(m_pos==std::wstring::npos || m_pos>=m_inputString.size()){
		m_currentTokenType=TWExpressionTokenTypeEOS;
		m_currentTokenLength=0;
		m_currentTokenPos=m_inputString.size();
		return;
	}
	
	
	m_currentTokenPos=m_pos;
	wchar_t c=m_inputString[m_pos++];
	m_currentTokenType=tokenTypeforChar(c);
	m_currentTokenLength=1;
	
	if(canTokenCharContinued(c)){
		while(m_pos<m_inputString.size()){
			if(tokenTypeforChar(m_inputString[m_pos])!=m_currentTokenType)
				break;
			m_pos++;
			m_currentTokenLength++;
		}
	}
}


void TWExpressionReader::forwardPointer(){
	if(m_pos==std::wstring::npos)
		return;
	
	m_pos=currentTokenPos();
	m_pos+=currentTokenLength();
	m_isCurrentTokenRead=false;
}

void TWExpressionReader::skipWhitespaces(){
	if(m_pos==std::wstring::npos)
		return;
	m_pos=m_inputString.find_first_not_of(L" \n\r\t", m_pos);
	
}

TWExpressionTokenType TWExpressionReader::tokenTypeforChar(wchar_t c) const{
	if((c>=L'0' && c<=L'9') || (c==L'.'))
		return TWExpressionTokenTypeNumber;
	else if((c>=L'a' && c<=L'z') || (c>=L'A' && c<=L'Z'))
		return TWExpressionTokenTypeSymbol;
	else if(c==0)
		return TWExpressionTokenTypeEOS;
	else
		return TWExpressionTokenTypeOperator;
}

TWExpressionTokenType TWExpressionReader::currentTokenType(){
	readCurrentToken();
	return m_currentTokenType;
}

std::wstring::size_type TWExpressionReader::currentTokenPos(){
	readCurrentToken();
	return m_currentTokenPos;
}

std::wstring::size_type TWExpressionReader::currentTokenLength(){
	readCurrentToken();
	return m_currentTokenLength;
}

wchar_t TWExpressionReader::currentTokenChar(){
	return m_inputString[currentTokenPos()];
}

std::wstring TWExpressionReader::currentToken(){
	return m_inputString.substr(currentTokenPos(), currentTokenLength());
}



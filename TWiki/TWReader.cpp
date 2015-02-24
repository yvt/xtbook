//
//  TWReader.cpp
//  XTBook
//
//  Created by Nexhawks on 4/17/11.
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

#include "TWReader.h"
#include <tcw/twStrConv.h>

TWReader::TWReader(const std::wstring& str):
m_nullReplaced(false){
	m_string=str;
	m_pos=0;
}

TWReader::~TWReader(){
	
}

wchar_t TWReader::peek(){
	restoreNullReplace();
	if(m_pos>=m_string.size())
		return 0;
	else
		return m_string[m_pos];
}

wchar_t TWReader::read(){
	wchar_t r=peek();
	m_pos++;
	return r;
}

std::wstring TWReader::readUntilFirstOf(const wchar_t *key){
	restoreNullReplace();
	if(m_pos>=m_string.size())
		return L"";
	std::wstring::size_type newPos;
	newPos=m_string.find_first_of(key, m_pos);
	if(newPos==std::wstring::npos)
		newPos=m_string.size();
	
	std::wstring ret=m_string.substr(m_pos, newPos-m_pos);
	m_pos=newPos;
	return ret;
	
}
std::wstring TWReader::readUntil(const wchar_t *key){
	restoreNullReplace();
	if(m_pos>=m_string.size())
		return L"";
	std::wstring::size_type newPos;
	newPos=m_string.find(key, m_pos);
	if(newPos==std::wstring::npos)
		newPos=m_string.size();
	
	std::wstring ret=m_string.substr(m_pos, newPos-m_pos);
	m_pos=newPos;
	return ret;
	
}

void TWReader::nullReplace(){
	restoreNullReplace();
	if(m_pos>=m_string.size())
		return;
	m_nullReplaced=true;
	m_nullReplaceOldChar=m_string[m_pos];
	m_nullReplacePos=m_pos;
	m_string[m_pos]=0;
}
void TWReader::restoreNullReplace(){
	if(!m_nullReplaced)
		return;
	m_string[m_nullReplacePos]=m_nullReplaceOldChar;
	m_nullReplaced=false;
}

void TWReader::dumpUntil(size_t ln) const{
	puts(twW2M(m_string.substr(0, ln)).c_str());
}


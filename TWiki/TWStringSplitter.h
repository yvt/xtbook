//
//  TWStringSplitter.h
//  XTBook
//
//  Created by Kawada Tomoaki on 5/1/11.
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
#include <stdlib.h>
#include <exception>
#include <stdexcept>
#include <wchar.h>
#include "TWUtils.h"
#include <assert.h>

template<int MaxElements>
class TWStringSplitter{
private:
	std::wstring& m_baseString;
	wchar_t *m_elements[MaxElements];
	size_t m_elementCount;
public:
	TWStringSplitter():
	m_baseString(const_cast<std::wstring&>(TWEmptyString)),
	m_elementCount(0){
	}
	TWStringSplitter(std::wstring& baseString, const wchar_t *delimiter, 
					 std::wstring::size_type startPos=0, std::wstring::size_type length=std::wstring::npos):
	m_baseString(baseString),
	m_elementCount(0){
		wchar_t *str=const_cast<wchar_t *>(baseString.c_str());
		wchar_t *pos, *newPos;
		size_t delimiterLength=wcslen(delimiter);
		
		if(length==std::wstring::npos){
			if(startPos>baseString.size())
				startPos=baseString.size();
			str+=startPos;
		}else{
			length+=startPos;
			if(startPos>baseString.size())
				startPos=baseString.size();
			if(length>baseString.size())
				length=baseString.size();
			str[length]=0;
			str+=startPos;
		}
		
		pos=str;
		while(newPos=wcsstr(pos, delimiter), newPos){
			if(m_elementCount==MaxElements)
				return;
			if(m_elementCount<MaxElements-1)
			*newPos=0;
			
			m_elements[m_elementCount++]=pos;
			pos=newPos+delimiterLength;
		}
		if(m_elementCount==MaxElements)
			return;
		m_elements[m_elementCount++]=pos;
	}
	static TWStringSplitter<MaxElements> splitterWithTrimmedString(std::wstring& str, const wchar_t *delimiter){
		std::wstring::size_type index1, index2;
		index1=str.find_first_not_of(L' ');
		if(index1==std::wstring::npos)
			return TWStringSplitter<MaxElements>();
		index2=str.find_last_not_of(L' ');
		return TWStringSplitter<MaxElements>(str, delimiter,
											 index1, index2-index1+1);
	}
	int elementCount() const{return m_elementCount;}
	wchar_t *elementAtIndex(size_t i) const{
		if(i>=m_elementCount)
			throw std::out_of_range("i");
		return m_elements[i];
	}
	wchar_t *operator [](size_t i) const{
		assert(i<m_elementCount);
		return m_elements[i];
	}
	
};

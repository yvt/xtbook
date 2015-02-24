//
//  TWReader.h
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

#pragma once

#include <string>

class TWReader{
public:
	typedef std::wstring::size_type PositionType;
private:
	std::wstring m_string;
	PositionType m_pos;
	bool m_nullReplaced;
	wchar_t m_nullReplaceOldChar;
	PositionType m_nullReplacePos;
	void restoreNullReplace();
	void nullReplace();
public:
	
	TWReader(const std::wstring&);
	virtual ~TWReader();
	wchar_t peek();
	wchar_t read();
	std::wstring readUntilFirstOf(const wchar_t *);
	std::wstring readUntil(const wchar_t *);
	
	PositionType position() const{
		return m_pos;
	}
	void setPosition(PositionType pos){
		m_pos=pos;
	}
	bool eof() const{return m_pos>=m_string.size();}
	
	void dumpUntil(size_t) const; // for debugging.
};


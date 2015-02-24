//
//  TPLString.cpp
//  XTBook
//
//  Created by Kawada Tomoaki on 5/21/11.
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

#include "TPLString.h"
#include <string.h>

TPLString::TPLString(){}

TPLString::TPLString(const char *utf8String):
m_string(utf8String?utf8String:""){}

TPLString::TPLString(const TPLString *str):
m_string(str->UTF8String()){
}

TPLString::~TPLString(){
}

const char *TPLString::UTF8String() const{
	return m_string.c_str();
}

bool TPLString::isEqualToString(TPLString *str) const{
	return !strcmp(UTF8String(), str->UTF8String());
}

bool TPLString::isEqualToUTF8String(const char *str) const{
	return !strcmp(UTF8String(), str);
}

TPLUInteger TPLString::length() const{
	return m_string.size();
}

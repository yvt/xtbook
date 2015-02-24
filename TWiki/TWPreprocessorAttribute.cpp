/*
 *  TWPreprocessorAttribute.cpp
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

#include "TWPreprocessorAttribute.h"
#include "TWUtils.h"

TWPreprocessorAttribute::TWPreprocessorAttribute(const std::wstring& name,
												 const std::wstring& value):
TWPreprocessorNode(name),
m_value(value),
m_intValue(TWUtils::intValueFromString(value)){
}
TWPreprocessorAttribute::TWPreprocessorAttribute(const std::wstring& name,
												 int value):
TWPreprocessorNode(name),
m_value(TWUtils::stringWithIntValue(value)),
m_intValue(value){
}

std::wstring TWPreprocessorAttribute::toString() const{
	return L"<"+m_name+L">"+TWUtils::escapeHTMLSpecialChars(m_value)+L"</"+m_name+L">";
}



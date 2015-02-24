/*
 *  TWElement.cpp
 *  XTBook
 *
 *  Created by Nexhawks on 2/24/11.
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

#include "TWElement.h"

TWElement::TWElement(const std::wstring& str):
TWNode(str){
}
void TWElement::setAttribute(const std::wstring& name, 
							 const std::wstring& value){
	m_attributes[name]=value;
}
static const std::wstring g_emptyString;
std::wstring TWElement::getAttribute(const std::wstring& name) const{
	std::map<std::wstring, std::wstring>::const_iterator it=m_attributes.find(name);
	if(it==m_attributes.end()){
		return g_emptyString;
	}else{
		return it->second;
	}	
}
bool TWElement::hasAttribute(const std::wstring& name) const{
	return m_attributes.find(name)!=m_attributes.end();
}
void TWElement::removeAttribute(const std::wstring& name){
	if(hasAttribute(name))
		m_attributes.erase(m_attributes.find(name));
}


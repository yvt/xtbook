//
//  TWHTMLText.cpp
//  XTBook
//
//  Created by Nexhawks on 4/10/11.
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

#include "TWHTMLText.h"
#include <assert.h>
#include "TWUtils.h"

const std::wstring g_textElementName=L"#txt";

TWHTMLText::TWHTMLText(const std::wstring& newInnerText):
TWHTMLElement(g_textElementName),
m_innerText(newInnerText){
	m_type=TWHTMLElementTypeText;
}

void TWHTMLText::addChild(TWNode *newChild){
	assert("TWHTMLText cannot have any children."==0);
}

std::wstring TWHTMLText::innerText() const{
	return m_innerText;
}

std::wstring TWHTMLText::innerHTML() const{
	return TWUtils::escapeHTMLSpecialChars(m_innerText);
}

std::wstring TWHTMLText::outerHTML() const{
	return innerHTML();
}

void TWHTMLText::appendText(std::wstring const & str){
	m_innerText+=str;
}

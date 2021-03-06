/*
 *  TWHTMLBodyElement.cpp
 *  XTBook
 *
 *  Created by Nexhawks on 2/26/11.
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

#include "TWHTMLBodyElement.h"
#include "TWHTMLStylesheet.h"
#include "TWVectorStyle.h"
#include "TWStandardStyle.h"

TWHTMLBodyElement::TWHTMLBodyElement():
TWHTMLElement(L"body"){
	m_stylesheets.push_back(new TWStandardStyle());
	m_stylesheets.push_back(new TWVectorStyle());
}
TWHTMLBodyElement::~TWHTMLBodyElement(){
	for(std::vector<TWHTMLStylesheet *>::iterator it=m_stylesheets.begin();
		it!=m_stylesheets.end();it++)
		delete *it;
}

void TWHTMLBodyElement::applyStylesheet(TWHTMLElement *elm, TWHTMLStyle *style){
	for(std::vector<TWHTMLStylesheet *>::iterator it=m_stylesheets.begin();
		it!=m_stylesheets.end();it++)
		(*it)->apply(elm, style);
}

void TWHTMLBodyElement::applyCustomStyle(){
	m_style.setDisplay(TWHTMLStyleDisplayBlock);
}

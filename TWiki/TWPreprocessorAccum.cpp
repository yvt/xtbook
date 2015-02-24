/*
 *  TWPreprocessorAccum.cpp
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

#include "TWPreprocessorAccum.h"
#include "TWPreprocessorText.h"

TWPreprocessorAccum::TWPreprocessorAccum(const std::wstring& name):
TWPreprocessorNode(name){
	
}

void TWPreprocessorAccum::addLiteral(const std::wstring& str){
	addChild(new TWPreprocessorText(str));
}

void TWPreprocessorAccum::addNodeWithText(const std::wstring& name, const std::wstring& value){
	addChild(TWPreprocessorNode::nodeWithText(name, value));
}
void TWPreprocessorAccum::addAccum(TWPreprocessorAccum *accum){
	TWNode *child=accum->firstChild();
	while(child){
		TWNode *next=child->nextSibling();
		addChild(child);
		child=next;
	}
}

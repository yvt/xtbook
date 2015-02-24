//
//  OBSelector.cpp
//  XTBook
//
//  Created by Nexhawks on 4/27/11.
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

#include "OBSelector.h"
#include <string.h>
#include <wchar.h>

OBSelector::OBSelector(const OBNode *nodes, const OBIndex *indices):
m_nodes(nodes),
m_indices(indices){
}

uint16_t OBSelector::searchNodeForKey(uint16_t nodeIndex, const wchar_t *key, uint16_t keyLength) const{
	if(nodeIndex&OBNodeRecursiveFlag){
		const OBNode& node=m_nodes[nodeIndex&~OBNodeRecursiveFlag];
		if(node.charPos>=keyLength){
			return searchNodeForKey(node.left, key, keyLength);
		}
		if((uint16_t)key[node.charPos]>=node.border)
			return searchNodeForKey(node.right, key, keyLength);
		else
			return searchNodeForKey(node.left, key, keyLength);
	}else{
		const OBIndex& index=m_indices[nodeIndex];
		if(wcscmp(index.key, key))
			return OBNodeNotFound;
		else
			return nodeIndex;
	}
}

uint16_t OBSelector::leafIndexForKey(const wchar_t *key) const{
	size_t keyLength=wcslen(key);
	if(keyLength>0xffff)
		keyLength=0xffff;
	return searchNodeForKey(OBNodeRecursiveFlag|0x0000,
							key, (uint16_t)keyLength);
}


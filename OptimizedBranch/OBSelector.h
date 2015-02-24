//
//  OBSelector.h
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

#pragma once

#include <stdint.h>

static const uint16_t OBNodeRecursiveFlag=0x8000;
static const uint16_t OBNodeNotFound=0xffff;

struct OBNode{
	uint16_t charPos;
	uint16_t border;
	uint16_t left;
	uint16_t right;
};

struct OBIndex{
	const wchar_t *key;	
};

class OBSelector{
private:
	const OBNode *m_nodes;
	const OBIndex *m_indices;
	uint16_t searchNodeForKey(uint16_t nodeIndex, const wchar_t *key, uint16_t keyLength) const;
public:
	OBSelector(const OBNode *,const OBIndex *);
	bool containsKey(const wchar_t *) const;
	uint16_t leafIndexForKey(const wchar_t *) const;
};

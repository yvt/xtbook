//
//  BlockElements.cpp
//  XTBook
//
//  Created by Kawada Tomoaki on 5/15/11.
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

#include "BlockElements.h"
#include "../OptimizedBranch/OBSelector.h"

static const OBNode BlockElementsNodes[]={
	{0x0000, 0x006b, 0x8001, 0x8007}, {0x0000, 0x0065, 0x800f, 0x8002}, 
	{0x0001, 0x0032, 0x0002, 0x8003}, {0x0001, 0x0033, 0x0003, 0x8004}, 
	{0x0001, 0x0034, 0x0004, 0x8005}, {0x0001, 0x0035, 0x0005, 0x8006}, 
	{0x0001, 0x0036, 0x0006, 0x0007}, {0x0000, 0x0072, 0x800b, 0x8008}, 
	{0x0001, 0x006a, 0x8009, 0x800c}, {0x0001, 0x0063, 0x0000, 0x800a}, 
	{0x0001, 0x0066, 0x000a, 0x000b}, {0x0001, 0x0072, 0x800d, 0x0008}, 
	{0x0000, 0x0075, 0x0009, 0x000d}, {0x0000, 0x0070, 0x800e, 0x000c}, 
	{0x0000, 0x006e, 0x000f, 0x000e}, {0x0000, 0x0063, 0x0001, 0x8010}, 
	{0x0000, 0x0064, 0x0010, 0x0011}, };
static const OBIndex BlockElementsIndices[]={
	{L"table"}, 
	{L"blockquote"}, 
	{L"h1"}, 
	{L"h2"}, 
	{L"h3"}, 
	{L"h4"}, 
	{L"h5"}, 
	{L"h6"}, 
	{L"pre"}, 
	{L"tr"}, 
	{L"td"}, 
	{L"th"}, 
	{L"p"}, 
	{L"ul"}, 
	{L"ol"}, 
	{L"li"}, 
	{L"center"}, 
	{L"div"}, };


static OBSelector BlockElementsSelector(BlockElementsNodes, BlockElementsIndices);

bool isBlockElement(const wchar_t *name){
	uint16_t i=BlockElementsSelector.leafIndexForKey(name);
	return i!=OBNodeNotFound;
}

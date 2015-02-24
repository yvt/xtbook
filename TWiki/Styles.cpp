//
//  Styles.cpp
//  XTBook
//
//  Created by Kawada Tomoaki on 4/30/11.
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

#include "Styles.h"
#include "../OptimizedBranch/OBSelector.h"

static const OBNode StylesNodes[]={
	{0x000a, 0x002d, 0x8001, 0x8006}, {0x0000, 0x0063, 0x800f, 0x8002}, 
	{0x0000, 0x0064, 0x8024, 0x8003}, {0x0000, 0x006e, 0x8004, 0x800a}, 
	{0x0000, 0x0066, 0x0003, 0x8005}, {0x0000, 0x006b, 0x8023, 0x8008}, 
	{0x0000, 0x0068, 0x8010, 0x8007}, {0x0007, 0x006f, 0x8009, 0x8028}, 
	{0x0006, 0x002d, 0x0006, 0x0009}, {0x0007, 0x0067, 0x800b, 0x0007}, 
	{0x0000, 0x0074, 0x8027, 0x8026}, {0x0000, 0x006f, 0x000a, 0x800c}, 
	{0x0008, 0x006f, 0x800e, 0x800d}, {0x0008, 0x0073, 0x802a, 0x000e}, 
	{0x0008, 0x0067, 0x000f, 0x000c}, {0x0001, 0x0068, 0x0000, 0x8012}, 
	{0x0001, 0x0068, 0x0001, 0x8011}, {0x0007, 0x006f, 0x8013, 0x8014}, 
	{0x0006, 0x002d, 0x0010, 0x0013}, {0x0007, 0x0067, 0x8018, 0x8015}, 
	{0x0007, 0x0075, 0x8016, 0x0015}, {0x000b, 0x002d, 0x8025, 0x801a}, 
	{0x000c, 0x002d, 0x8019, 0x8017}, {0x0007, 0x0073, 0x801b, 0x801c}, 
	{0x000d, 0x002d, 0x801e, 0x801d}, {0x0007, 0x0073, 0x0012, 0x001a}, 
	{0x000c, 0x0075, 0x801f, 0x0016}, {0x000d, 0x0075, 0x8020, 0x0017}, 
	{0x000b, 0x0075, 0x8021, 0x0018}, {0x000e, 0x0075, 0x8022, 0x0019}, 
	{0x0007, 0x0063, 0x0014, 0x001f}, {0x000c, 0x006b, 0x0020, 0x001b}, 
	{0x000d, 0x006b, 0x0021, 0x001c}, {0x000b, 0x006b, 0x0022, 0x001d}, 
	{0x000e, 0x006b, 0x0023, 0x001e}, {0x0000, 0x0067, 0x8029, 0x0005}, 
	{0x0001, 0x006e, 0x0025, 0x0002}, {0x0000, 0x0064, 0x0011, 0x0026}, 
	{0x0000, 0x0076, 0x0027, 0x0004}, {0x0001, 0x0068, 0x000b, 0x0028}, 
	{0x0000, 0x0072, 0x0008, 0x0029}, {0x0001, 0x006e, 0x0024, 0x002a}, 
	{0x0000, 0x0072, 0x000d, 0x002b}, };
static const OBIndex StylesIndices[]={
	{L"background"}, 
	{L"background-color"}, 
	{L"color"}, 
	{L"display"}, 
	{L"width"}, 
	{L"height"}, 
	{L"margin"}, 
	{L"margin-left"}, 
	{L"margin-right"}, 
	{L"margin-top"}, 
	{L"margin-bottom"}, 
	{L"padding"}, 
	{L"padding-left"}, 
	{L"padding-right"}, 
	{L"padding-top"}, 
	{L"padding-bottom"}, 
	{L"border"}, 
	{L"border-left"}, 
	{L"border-right"}, 
	{L"border-top"}, 
	{L"border-bottom"}, 
	{L"border-width"}, 
	{L"border-left-width"}, 
	{L"border-right-width"}, 
	{L"border-top-width"}, 
	{L"border-bottom-width"}, 
	{L"border-style"}, 
	{L"border-left-style"}, 
	{L"border-right-style"}, 
	{L"border-top-style"}, 
	{L"border-bottom-style"}, 
	{L"border-color"}, 
	{L"border-left-color"}, 
	{L"border-right-color"}, 
	{L"border-top-color"}, 
	{L"border-bottom-color"}, 
	{L"float"}, 
	{L"clear"}, 
	{L"font-weight"}, 
	{L"text-align"}, 
	{L"position"}, 
	{L"white-space"}, 
	{L"font-size"}, 
	{L"text-decoration"}, };

static OBSelector StylesSelector(StylesNodes, StylesIndices);


StyleType styleTypeWithName(const wchar_t *name){
	uint16_t i=StylesSelector.leafIndexForKey(name);
	if(i==OBNodeNotFound)
		return StyleTypeUnknown;
	return (StyleType)(i+1);
}


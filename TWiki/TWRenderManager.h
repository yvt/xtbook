//
//  TWRenderManager.h
//  XTBook
//
//  Created by Nexhawks on 3/13/11.
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

#include <string>
#include "TWTypes.h"

class TWRenderDC;
class TWHTMLStyleProxy;

class TWRenderManager {
public:
    TWRenderManager(){}
    virtual ~TWRenderManager(){}
    virtual void setStyle(const TWHTMLStyleProxy&)=0;
	virtual TWSize sizeForString(const std::wstring&)=0;
	virtual void drawString(const std::wstring&, const TWPoint&,
							TWRenderDC *)=0;
	
	virtual TWSize sizeForImage(const std::wstring&)=0;
	virtual void drawImage(const std::wstring&, const TWRect&, TWRenderDC *)=0;
	
	virtual TWSize sizeForListMarker()=0;
	virtual void drawListMarker(const TWRect&, TWRenderDC *)=0;
};

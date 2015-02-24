/*
 *  TWRenderDC.h
 *  XTBook
 *
 *  Created by Nexhawks on 2/28/11.
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

#pragma once

#include <string>
#include "TWTypes.h"

class TWHTMLStyleProxy;
class TWRenderManager;
class TWRenderObject;

class TWRenderDC {
public:
	TWRenderDC(){}
	virtual ~TWRenderDC(){}
	
    virtual void setStyle(const TWHTMLStyleProxy&)=0;
	virtual void fillBackground(const TWRect&)=0;
	virtual void drawBorder(const TWRect&)=0;
	virtual void drawPlaceHolder(const TWRect&)=0;
	virtual bool isRectVisible(const TWRect&)=0;
	virtual void fillSelection(const TWRect&)=0;
	
	virtual void debugRenderObject(const TWRect&, TWRenderObject *){}
	
	virtual TWRenderManager *fontManager()=0;
};

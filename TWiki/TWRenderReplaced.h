//
//  TWRenderReplaced.h
//  XTBook
//
//  Created by Nexhawks on 3/12/11.
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
#include "TWRenderBox.h"

class TWRenderFlowContent;

class TWRenderReplaced: public TWRenderBox {
protected:
   
	
public:
    TWRenderReplaced(TWHTMLElement *);
    
	virtual void render(TWRenderDC *);
	virtual void render(TWRenderFlowContent *, TWRenderDC *);
    
	virtual void layout(TWRenderManager *);
	
	virtual int replacedDefaultWidth() const;
	virtual int replacedDefaultHeight() const;
	
	virtual int contentHeight() const;
	virtual int contentWidth() const;
	virtual int contentWidth(int segment) const;
	virtual int segments() const;
	
	virtual wchar_t firstChar(int segment) const;
	virtual wchar_t lastChar(int segment) const;
	wchar_t firstChar() const;
	wchar_t lastChar() const;
	
	virtual bool isEmpty() const{return false;}
	
	virtual void handleMaximumContentWidth(TWRenderManager *manager);
	virtual void handleMinimumContentWidth(TWRenderManager *manager);
};

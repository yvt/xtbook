/*
 *  TWRenderBoxModelObject.h
 *  XTBook
 *
 *  Created by Nexhawks on 2/27/11.
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

#include "TWRenderObject.h"
#include "TWTypes.h"
#include <vector>

class TWRenderBoxModelObject : public TWRenderObject{
protected:
	TWRect m_boundaryRect;
    TWRect m_screenRect;
	TWRect m_rect;
public:
	TWRenderBoxModelObject(TWHTMLElement *node, const std::wstring&);
	
	const TWRect& rect() const{return m_rect;}
	void setRect(const TWRect& rt){m_rect=rt;};
    
    const TWRect& screenRect() const{return m_screenRect;}
	const TWRect& boundaryRect() const{return m_boundaryRect;}
    void calcScreenRect();
    
	int marginLeft() const;
	int marginTop() const;
	int marginRight() const;
	int marginBottom() const;
    
    int paddingLeft() const;
	int paddingTop() const;
	int paddingRight() const;
	int paddingBottom() const;
	
	virtual int borderSizeLeft() const;
	virtual int borderSizeTop() const;
	virtual int borderSizeRight() const;
	virtual int borderSizeBottom() const;
	
	virtual void render(TWRenderDC *);
	
    
	void accumulateRenderObjectsAtPoint(const TWPoint&, TWNodeIterateCallback callback, void *) const;
};

/*
 *  TWRenderBoxModelObject.cpp
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

#include "TWRenderBoxModelObject.h"
#include "TWHTMLElement.h"
#include "TWRenderDC.h"
#include <assert.h>

TWRenderBoxModelObject::TWRenderBoxModelObject(TWHTMLElement *node, 
											   const std::wstring& name):
TWRenderObject(node, name){
	m_isBoxModelObject=true;
	m_rect=TWRect(0xdeadbeef,0xdeadbeef,100,0);
	m_screenRect=TWRect(0,0,100,0);
	m_boundaryRect=m_screenRect;
}

int TWRenderBoxModelObject::marginLeft() const{
	return style().marginLeft().toPixels(0, style());
}
int TWRenderBoxModelObject::marginTop() const{
	return style().marginTop().toPixels(0, style());
}
int TWRenderBoxModelObject::marginRight() const{
	return style().marginRight().toPixels(0, style());
}
int TWRenderBoxModelObject::marginBottom() const{
	return style().marginBottom().toPixels(0, style());
}

int TWRenderBoxModelObject::paddingLeft() const{
	return style().paddingLeft().toPixels(0, style());
}
int TWRenderBoxModelObject::paddingTop() const{
	return style().paddingTop().toPixels(0, style());
}
int TWRenderBoxModelObject::paddingRight() const{
	return style().paddingRight().toPixels(0, style());
}
int TWRenderBoxModelObject::paddingBottom() const{
	return style().paddingBottom().toPixels(0, style());
}

int TWRenderBoxModelObject::borderSizeLeft() const{
	const TWHTMLStyleBorder& border=style().borderLeft();
	if(border.style==TWHTMLStyleBorderStyleNone)
		return 0;
	return border.width.toPixels(0, style());
}
int TWRenderBoxModelObject::borderSizeTop() const{
	const TWHTMLStyleBorder& border=style().borderTop();
	if(border.style==TWHTMLStyleBorderStyleNone)
		return 0;
	return border.width.toPixels(0, style());
}
int TWRenderBoxModelObject::borderSizeRight() const{
	const TWHTMLStyleBorder& border=style().borderRight();
	if(border.style==TWHTMLStyleBorderStyleNone)
		return 0;
	return border.width.toPixels(0, style());
}
int TWRenderBoxModelObject::borderSizeBottom() const{
	const TWHTMLStyleBorder& border=style().borderBottom();
	if(border.style==TWHTMLStyleBorderStyleNone)
		return 0;
	return border.width.toPixels(0, style());
}

void TWRenderBoxModelObject::render(TWRenderDC * dc){
	dc->setStyle(style());
	dc->fillBackground(m_rect);
	dc->drawBorder(m_rect);
	dc->debugRenderObject(m_screenRect, this);
	
	TWRenderObject::render(dc);
}

void TWRenderBoxModelObject::calcScreenRect(){
    TWRenderBoxModelObject *parentBox=static_cast<TWRenderBoxModelObject *>(parent());
    if(parentBox){
        m_screenRect=m_rect;
        TWRect parentScreenRect=parentBox->screenRect();
        m_screenRect.x+=parentScreenRect.x;
        m_screenRect.y+=parentScreenRect.y;
    }else{
        m_screenRect=m_rect;
    }
	
	m_boundaryRect=m_screenRect.sanitize();
	
	assert((unsigned int)m_boundaryRect.w<0x100000U);
	assert((unsigned int)m_boundaryRect.h<0x10000000U);
	
    
    for(TWNode *child=firstChild();child;child=child->nextSibling()){
		if(static_cast<TWRenderObject *>(child)->isInline())
			continue;
		if(static_cast<TWRenderObject *>(child)->isReplaced())
			continue;
        if(TWRenderBoxModelObject *childBox=dynamic_cast<TWRenderBoxModelObject *>(child)){
            childBox->calcScreenRect();
			m_boundaryRect=m_boundaryRect|childBox->boundaryRect();
		}
    }
	
	assert((unsigned int)m_boundaryRect.w<0x100000U);
	assert((unsigned int)m_boundaryRect.h<0x10000000U);
	

}

void TWRenderBoxModelObject::accumulateRenderObjectsAtPoint(const TWPoint& pt, TWNodeIterateCallback callback,
															void *data) const{
	if(!(m_boundaryRect&&pt)){
		return;
	}
	if(callback)
		(*callback)(data, const_cast<TWRenderBoxModelObject *> (this));
	
    for(TWNode *child=firstChild();child;child=child->nextSibling()){
		TWRenderObject *renderer=static_cast<TWRenderObject *>(child);
		if(renderer->isInline())
			continue;
		if(renderer->isReplaced())
			continue;
		
        if(renderer->isBoxModelObject()){
			TWRenderBoxModelObject *childBox=static_cast<TWRenderBoxModelObject *>(child);
            childBox->accumulateRenderObjectsAtPoint(pt, callback, data);
		}
    }
}
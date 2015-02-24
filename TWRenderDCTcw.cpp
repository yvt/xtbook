//
//  TWRenderDCTcw.cpp
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

#include "TWRenderDCTcw.h"
#include <tcw/twDC.h>
#include "TWRenderManagerTcw.h"
#include "TWiki/TWRenderObject.h"
#include <typeinfo>
#include <tcw/twStrConv.h>
#include "TWiki/TWHTMLElement.h"

TWRenderDCTcw::TWRenderDCTcw(twDC *dc, TWRenderManager *manager):
m_dc(dc),
m_fontManager(manager){
    
}

TWRenderDCTcw::~TWRenderDCTcw(){
}

static twColor twColorFromStyleColor(const TWHTMLStyleColor& styleColor){
    return twRGB(styleColor.red, styleColor.green, styleColor.blue);
}

static twRect twRectFromTWRect(const TWRect& rt){
    return twRect(rt.x, rt.y, rt.w, rt.h);
}

void TWRenderDCTcw::fillBackground(const TWRect &rt){
    const TWHTMLStyleColor& color=m_style.backgroundColor();
    if(color.alpha==0)
        return;
   
    m_dc->fillRect(twColorFromStyleColor(color), 
                   twRectFromTWRect(rt));
}

void TWRenderDCTcw::drawBorder(const TWRect &rt){
	twColor color;
	int width;
	// TODO: support for non-1px-border
	
	color=twColorFromStyleColor(m_style.borderLeft().color);
	width=m_style.borderLeft().width.toPixels(0, m_style);
	switch(m_style.borderLeft().style){
		case TWHTMLStyleBorderStyleNone:
			break;
		case TWHTMLStyleBorderStyleSolid:
		case TWHTMLStyleBorderStyleDouble:
		case TWHTMLStyleBorderStyleGroove:
		case TWHTMLStyleBorderStyleInset:
		case TWHTMLStyleBorderStyleOutset:
		case TWHTMLStyleBorderStyleRidge:
			m_dc->fillRect(color, twRect(rt.x, rt.y,
										 width, rt.h));
			break;
		case TWHTMLStyleBorderStyleDotted:
			for(int y=rt.y;y<rt.y+rt.h;y+=2)
				m_dc->fillRect(color, twRect(rt.x, y, width, 1));
			break;
		case TWHTMLStyleBorderStyleDashed:
			for(int y=rt.y;y<rt.y+rt.h;y+=4)
				m_dc->fillRect(color, twRect(rt.x, y,
											 width, MIN(2, rt.y+rt.h-y)));
			break;
	}
	
	color=twColorFromStyleColor(m_style.borderRight().color);
	width=m_style.borderRight().width.toPixels(0, m_style);
	switch(m_style.borderRight().style){
		case TWHTMLStyleBorderStyleNone:
			break;
		case TWHTMLStyleBorderStyleSolid:
		case TWHTMLStyleBorderStyleDouble:
		case TWHTMLStyleBorderStyleGroove:
		case TWHTMLStyleBorderStyleInset:
		case TWHTMLStyleBorderStyleOutset:
		case TWHTMLStyleBorderStyleRidge:
			m_dc->fillRect(color, twRect(rt.x+rt.w-width, rt.y,
										 width, rt.h));
			break;
		case TWHTMLStyleBorderStyleDotted:
			for(int y=rt.y;y<rt.y+rt.h;y+=2)
				m_dc->fillRect(color, twRect(rt.x+rt.w-width, y,
											 width, 1));
			break;
		case TWHTMLStyleBorderStyleDashed:
			for(int y=rt.y;y<rt.y+rt.h;y+=4)
				m_dc->fillRect(color, twRect(rt.x+rt.w-width, y,
											 width, MIN(2, rt.y+rt.h-y)));
			break;
	}
	
	color=twColorFromStyleColor(m_style.borderTop().color);
	width=m_style.borderTop().width.toPixels(0, m_style);
	switch(m_style.borderTop().style){
		case TWHTMLStyleBorderStyleNone:
			break;
		case TWHTMLStyleBorderStyleSolid:
		case TWHTMLStyleBorderStyleDouble:
		case TWHTMLStyleBorderStyleGroove:
		case TWHTMLStyleBorderStyleInset:
		case TWHTMLStyleBorderStyleOutset:
		case TWHTMLStyleBorderStyleRidge:
			m_dc->fillRect(color, twRect(rt.x, rt.y,
										 rt.w, width));
			break;
		case TWHTMLStyleBorderStyleDotted:
			for(int x=rt.x;x<rt.x+rt.w;x+=2)
				m_dc->fillRect(color, twRect(x, rt.y,
											 1, width));
			break;
		case TWHTMLStyleBorderStyleDashed:
			for(int x=rt.x;x<rt.x+rt.w;x+=4)
				m_dc->fillRect(color, twRect(x, rt.y,
											 MIN(2, rt.x+rt.w-x), width));
			break;
	}
	
	color=twColorFromStyleColor(m_style.borderBottom().color);
	width=m_style.borderBottom().width.toPixels(0, m_style);
	switch(m_style.borderBottom().style){
		case TWHTMLStyleBorderStyleNone:
			break;
		case TWHTMLStyleBorderStyleSolid:
		case TWHTMLStyleBorderStyleDouble:
		case TWHTMLStyleBorderStyleGroove:
		case TWHTMLStyleBorderStyleInset:
		case TWHTMLStyleBorderStyleOutset:
		case TWHTMLStyleBorderStyleRidge:
			m_dc->fillRect(color, twRect(rt.x, rt.y+rt.h-width,
										 rt.w, width));
			break;
		case TWHTMLStyleBorderStyleDotted:
			for(int x=rt.x;x<rt.x+rt.w;x+=2)
				m_dc->fillRect(color, twRect(x, rt.y+rt.h-width,
											 1, width));
			break;
		case TWHTMLStyleBorderStyleDashed:
			for(int x=rt.x;x<rt.x+rt.w;x+=4)
				m_dc->fillRect(color, twRect(x, rt.y+rt.h-width,
											 MIN(2, rt.x+rt.w-x), width));
			break;
	}
	
	//m_dc->drawRect(rand(), twRect(rt.x, rt.y, rt.w-1, rt.h-1));
}

void TWRenderDCTcw::debugRenderObject(const TWRect& rt, TWRenderObject *obj){
#if 0
	if(obj->isRenderFlowObject())
		return;
	twRect r;
	r=twRectFromTWRect(rt.sanitize());
	
	m_dc->drawRect(0, twRect(r.x, r.y, r.w-1, r.h-1).inflate(3));
	m_dc->drawRect(0xff, twRect(r.x, r.y, r.w-1, r.h-1).inflate(2));
	
	std::wstring str;
	
	TWHTMLElement *element=obj->node();
	if(!element)
		str=L"*";
	else
		str=element->name();
	
	//std::wstring str=obj->name();
	/*
	str+=L'(';
	str+=twM2W(typeid(*obj).name());
	str+=L')';*/
	
	twPoint pt(r.x+3, r.y+3);
	twSize sz=tw_defFont->measure(str);
	
	//pt.x-=sz.w+6;
	
	tw_defFont->render(m_dc, 0, twPoint(pt.x-1, pt.y), str);
	tw_defFont->render(m_dc, 0, twPoint(pt.x+1, pt.y), str);
	tw_defFont->render(m_dc, 0, twPoint(pt.x, pt.y-1), str);
	tw_defFont->render(m_dc, 0, twPoint(pt.x, pt.y+1), str);
	
	tw_defFont->render(m_dc, 0, twPoint(pt.x-1, pt.y-1), str);
	tw_defFont->render(m_dc, 0, twPoint(pt.x+1, pt.y+1), str);
	tw_defFont->render(m_dc, 0, twPoint(pt.x-1, pt.y-1), str);
	tw_defFont->render(m_dc, 0, twPoint(pt.x+1, pt.y+1), str);
	
	tw_defFont->render(m_dc, 0x00ffff, 
					   twPoint(pt.x, pt.y), str);
#endif
}

void TWRenderDCTcw::drawPlaceHolder(const TWRect &rt){
	m_dc->fillRect(0xffffff, twRectFromTWRect(rt));
	m_dc->drawLine(0, twPoint(rt.x, rt.y), twPoint(rt.x+rt.w, rt.y+rt.h));
	m_dc->drawLine(0, twPoint(rt.x+rt.w, rt.y), twPoint(rt.x, rt.y+rt.h));
	
}

bool TWRenderDCTcw::isRectVisible(const TWRect & rt){
	twRect rt2=twRectFromTWRect(rt);
	twRect clipRect=m_dc->getClipRect();
	return rt2 && clipRect;
}

void TWRenderDCTcw::fillSelection(const TWRect& rt){
	twRect rt2=twRectFromTWRect(rt.sanitize());
	rt2-=m_selectionShift;
	
	// round rect.
	m_dc->dimRect(twRect(rt2.x-1, rt2.y, rt2.w+2, rt2.h));
	m_dc->dimRect(twRect(rt2.x, rt2.y-1, rt2.w, 1));
	m_dc->dimRect(twRect(rt2.x, rt2.y+rt2.h, rt2.w, 1));
	
	
}

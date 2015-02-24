//
//  TWRenderFlowInline.cpp
//  XTBook
//
//  Created by Nexhawks on 3/20/11.
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

#include "TWRenderFlowInline.h"
#include "TWRenderInline.h"
#include "TWRenderDC.h"

TWRenderFlowInline::TWRenderFlowInline(TWRenderInline *renderer):
TWRenderFlowObject(NULL),
m_renderer(renderer),
m_hasLeftBorder(true),
m_hasRightBorder(true){
	m_isRenderFlowInline=true;
}

void TWRenderFlowInline::render(TWRenderDC *dc){
	if(!dc->isRectVisible(m_boundaryRect.sanitize()))
		return;
	TWHTMLStyle drawStyle=m_renderer->style();
	if(!m_hasLeftBorder){
		drawStyle.setMarginLeft(TWHTMLStyleValueZero);
		drawStyle.setPaddingLeft(TWHTMLStyleValueZero);
		drawStyle.setBorderLeft(TWHTMLStyleBorder());
	}
	if(!m_hasRightBorder){
		drawStyle.setMarginRight(TWHTMLStyleValueZero);
		drawStyle.setPaddingRight(TWHTMLStyleValueZero);
		drawStyle.setBorderRight(TWHTMLStyleBorder());
	}
	
	dc->setStyle(drawStyle);
	dc->fillBackground(screenRect().sanitize());
	dc->drawBorder(screenRect().sanitize());
	dc->debugRenderObject(m_screenRect, this);
	
	TWRenderObject::render(dc);
}

void TWRenderFlowInline::removeLeftborder(){
	m_hasLeftBorder=false;
}
void TWRenderFlowInline::removeRightBorder(){
	m_hasRightBorder=false;
}

void TWRenderFlowInline::calcStyle(){
	m_style=m_renderer->style();
	m_styleValid=true;
	
	
}

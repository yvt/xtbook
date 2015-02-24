//
//  TWRenderListMarker.cpp
//  XTBook
//
//  Created by Kawada Tomoaki on 8/4/11.
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

#include "TWRenderListMarker.h"
#include "TWRenderListItem.h"
#include "TWRenderManager.h"
#include "TWUtils.h"
#include "TWRenderDC.h"

TWRenderListMarker::TWRenderListMarker(TWHTMLElement *element,
									   bool numbered):
TWRenderBlock(element),
m_isNumbered(numbered),
m_number(0){
	m_isListMarker=true;
}

TWRenderListMarker::~TWRenderListMarker(){
	
}

TWPoint TWRenderListMarker::markerPosition() const{
	int lineHeight=style().lineHeight().toPixels(0, style());
	int fontHeight=style().fontSize().toPixels(style().inheritedFontSize(), style());
	int position=-5;
	if(isNumbered())
	return TWPoint(position, lineHeight/2);
	else
		return TWPoint(position, lineHeight-fontHeight/2-2);
}

void TWRenderListMarker::layout(TWRenderManager *manager){
	TWPoint pos=markerPosition();
	TWSize size=markerSize(manager);
	
	m_rect.x=pos.x-size.w;
	m_rect.y=pos.y-size.h/2;
	m_rect.w=size.w;
	m_rect.h=size.h;
	
	m_isLayouted=true;
}

std::wstring TWRenderListMarker::textForNumberedMarker()const {
	return TWUtils::stringWithIntValue(m_number)+L". ";
}

TWSize TWRenderListMarker::markerSize(TWRenderManager *manager) const{
	TWRenderListItem *listItem=static_cast<TWRenderListItem *>(parent());
	manager->setStyle(listItem->style());
	
	if(m_isNumbered){
		return manager->sizeForString(textForNumberedMarker());
	}else{
		return manager->sizeForListMarker();
	}
}

void TWRenderListMarker::render(TWRenderDC *dc){
	TWRenderManager *manager=dc->fontManager();
	TWRenderListItem *listItem=static_cast<TWRenderListItem *>(parent());
	manager->setStyle(listItem->style());
	
	if(m_isNumbered){
		manager->drawString(textForNumberedMarker(), 
							m_screenRect.loc(), 
							dc);
	}else{
		manager->drawListMarker(m_screenRect, dc);
	}
}

void TWRenderListMarker::calcChildrenFlowIndex(){
	// no flow!
	setFlowIndexEnd(flowIndexStart());
}

unsigned int TWRenderListMarker::flowIndexAtPoint(const TWPoint &pt) const{
	return flowIndexStart();
}

TWRect TWRenderListMarker::drawSelection(TWRenderDC *, int begin, int end){
	return TWRect(0,0,0,0);
}


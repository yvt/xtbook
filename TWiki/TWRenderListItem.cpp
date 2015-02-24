//
//  TWRenderListItem.cpp
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

#include "TWRenderListItem.h"
#include "TWRenderListMarker.h"
#include "TWHTMLElement.h"
#include <assert.h>

TWRenderListItem::TWRenderListItem(TWHTMLElement *element):
TWRenderBlock(element),
m_marker(NULL){
	m_isListItem=true;
}

void TWRenderListItem::layout(TWRenderManager *manager){
	if(!m_marker){
		// create marker.
		if(needsMarker()){
			m_marker=new TWRenderListMarker(NULL, isNumbered());
			addChild(m_marker);
			m_marker->calcStyle();
		}
	}
	
	if(isNumbered() && needsMarker())
		calcNumber();
	
	if(m_marker)
		m_marker->layout(manager);
	
	TWRenderBlock::layout(manager);
	
}

bool TWRenderListItem::isNumbered() const{
	TWRenderObject *obj=static_cast<TWRenderObject *>(parent());
	while(obj!=NULL && obj->isAnonymous())
		obj=static_cast<TWRenderObject *>(obj->parent());
	if(!obj)
		return false;
	return obj->node()->type()==TWHTMLElementTypeOl;
}

void TWRenderListItem::calcNumber(){
	assert(isNumbered());
	
	if(!m_marker)
		return;
	
	TWRenderObject *obj=static_cast<TWRenderObject *>(prevSibling());
	while(obj!=NULL && obj->isListItem()==false){
		obj=static_cast<TWRenderObject *>(obj->prevSibling());
	}
	
	int number=1;
	if(obj!=NULL){
		TWRenderListItem *prevItem=static_cast<TWRenderListItem *>(obj);
		number=prevItem->number()+1;
	}
	
	m_marker->setNumber(number);
}
int TWRenderListItem::number() const{
	if(!m_marker)
		return 0;
	assert(isNumbered());
	return m_marker->number();
}

TWRenderListItem::~TWRenderListItem(){
	
}

bool TWRenderListItem::needsMarker() const{
	TWRenderObject *obj=static_cast<TWRenderObject *>(firstChild());
	if(!obj)
		return true;
	if(obj->node()){
		TWHTMLElement *elm=obj->node();
		if(elm->type()==TWHTMLElementTypeUl || elm->type()==TWHTMLElementTypeOl){
			return false;
		}
	}
	return true;
}

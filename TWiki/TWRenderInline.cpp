/*
 *  TWRenderInline.cpp
 *  XTBook
 *
 *  Created by Nexhawks on 3/1/11.
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

#include "TWRenderInline.h"
#include "TWHTMLElement.h"
#include "TWRenderBlock.h"
#include "TWRenderManager.h"
#include "TWRenderReplaced.h"
#include <assert.h>

TWRenderInline::TWRenderInline(TWHTMLElement *element):
TWRenderBoxModelObject(element, element?(element->name()):L""){
    m_isInline=true;
}


void TWRenderInline::layout(TWRenderManager *manager){
	m_manager=manager;
	TWRenderBlock *cb=containingBlock();
	
	TWRenderBlock::FlowTag tag;
	
	tag.renderer=this;
	tag.type=TWRenderBlock::FlowTagTypeStart;
	
	cb->addFlowTag(tag);
	
	for(TWNode *child=firstChild();child;child=child->nextSibling()){
		TWRenderObject *childBox=static_cast<TWRenderObject *>(child);
		if(childBox->isFloating()){
			childBox->layout(manager);
			cb->addFloatingObject(static_cast<TWRenderBox *>(childBox));
		}else
			childBox->layout(manager);
			
	}
	
	tag.type=TWRenderBlock::FlowTagTypeEnd;
	
	cb->addFlowTag(tag);
	
	m_isLayouted=true;
}
int TWRenderInline::contentHeight() const{
	assert(m_manager);
	m_manager->setStyle(style());
	return m_manager->sizeForString(L"A").h;
}
int TWRenderInline::contentWidth() const{
	int width=0;
	for(TWNode *child=firstChild();child;child=child->nextSibling()){
		TWRenderObject *childBox=static_cast<TWRenderObject *>(child);
		if(!childBox->isFloating()){
			if(childBox->isInline()){
				TWRenderInline *childInline=static_cast<TWRenderInline *>(childBox);
				width+=childInline->marginLeft();
				width+=childInline->paddingLeft();
				width+=childInline->borderSizeLeft();
				width+=childInline->contentWidth();
				width+=childInline->borderSizeRight();
				width+=childInline->paddingRight();
				width+=childInline->marginRight();
			}else if(childBox->isReplaced()){
				TWRenderReplaced *childInline=static_cast<TWRenderReplaced *>(childBox);
				width+=childInline->marginLeft();
				width+=childInline->paddingLeft();
				width+=childInline->borderSizeLeft();
				width+=childInline->contentWidth();
				width+=childInline->borderSizeRight();
				width+=childInline->paddingRight();
				width+=childInline->marginRight();
			}
		}
	}
	return width;
}
void TWRenderInline::handleMaximumContentWidth(TWRenderManager *manager){
	TWRenderBlock *cb=containingBlock();
	cb->addMaximumContentWidth(marginLeft());
	cb->addMaximumContentWidth(paddingLeft());
	cb->addMaximumContentWidth(borderSizeLeft());
	for(TWNode *child=firstChild();child;child=child->nextSibling()){
		TWRenderObject *childBox=static_cast<TWRenderObject *>(child);
		if(!childBox->isFloating()){
			if(childBox->isInline()){
				TWRenderInline *childInline=static_cast<TWRenderInline *>(childBox);
				childInline->handleMaximumContentWidth(manager);
			}else if(childBox->isReplaced()){
				TWRenderReplaced *childInline=static_cast<TWRenderReplaced *>(childBox);
				childInline->handleMaximumContentWidth(manager);
			}
		}
	}
	cb->addMaximumContentWidth(borderSizeRight());
	cb->addMaximumContentWidth(paddingRight());
	cb->addMaximumContentWidth(marginRight());
}

void TWRenderInline::handleMinimumContentWidth(TWRenderManager *manager){
	TWRenderBlock *cb=containingBlock();
	cb->addMinimumContentWidth(marginLeft()+
							   paddingLeft()+
							   borderSizeLeft());
	for(TWNode *child=firstChild();child;child=child->nextSibling()){
		TWRenderObject *childBox=static_cast<TWRenderObject *>(child);
		if(!childBox->isFloating()){
			if(childBox->isInline()){
				TWRenderInline *childInline=static_cast<TWRenderInline *>(childBox);
				childInline->handleMinimumContentWidth(manager);
			}else if(childBox->isReplaced()){
				TWRenderReplaced *childInline=static_cast<TWRenderReplaced *>(childBox);
				childInline->handleMinimumContentWidth(manager);
			}
		}else if(childBox->isBlock()){
			TWRenderBlock *o=static_cast<TWRenderBlock *>(child);
			int w=o->maximumContentWidth(manager);
			
			w+=o->marginLeft();
			w+=o->marginRight();
			w+=o->paddingLeft();
			w+=o->paddingRight();
			w+=o->borderSizeLeft();
			w+=o->borderSizeRight();
			
			cb->addMinimumContentWidth(w);
			
		}
	}
	cb->addMinimumContentWidth(marginRight()+
							   paddingRight()+
							   borderSizeRight());
}


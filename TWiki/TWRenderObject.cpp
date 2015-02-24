/*
 *  TWRenderObject.cpp
 *  XTBook
 *
 *  Created by Nexhawks on 2/26/11.
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

#include "TWRenderObject.h"
#include "TWHTMLElement.h"
#include "TWRenderTableRow.h"
#include "TWRenderTableCol.h"
#include "TWRenderTableCell.h"
#include "TWRenderBlock.h"
#include "TWRenderInline.h"
#include "TWRenderImage.h"
#include "TWRenderTable.h"
#include "TWRenderView.h"
#include "TWRenderText.h"
#include "TWRenderListItem.h"
#include "TWRenderBR.h"
#include "TWRenderBoxModelObject.h"
#include <assert.h>

TWRenderObject::TWRenderObject(TWHTMLElement *node, const std::wstring& name):
TWNode(name),
m_node(node),
m_childrenInline(true),
m_styleValid(false),
m_isBR(false),
m_isBlockFlow(false),
m_isBoxModelObject(false),
m_isImage(false),
m_isBlock(false),
m_isInline(false),
m_isView(false),
m_isTable(false),
m_isTableCell(false),
m_isTableRow(false),
m_isTableCol(false),
m_isTableSection(false),
m_isReplaced(false),
m_isRenderFlowObject(false),
m_isRenderFlowContent(false),
m_isRenderFlowInline(false),
m_isRenderFlowLine(false),
m_isText(false),
m_isListMarker(false),
m_isListItem(false),
m_isLayouted(false){
	if(node){
        m_style=node->style();
        m_styleValid=true;
    }
}
TWRenderObject::~TWRenderObject(){
	m_node=(TWHTMLElement *)0xdeadbeef;
}
TWRenderObject *TWRenderObject::objectWithNode(TWHTMLElement *node, TWRenderObject *parent){
	bool replaced=false;
	TWHTMLElementType type=node->type();
	
	if(parent && parent->isBR())
		return NULL;
	
	
	
	if(type==TWHTMLElementTypeText){
		return new TWRenderText(node);
	}
	if(type==TWHTMLElementTypeBr){
		return new TWRenderBR(node);
	}
	
	if(node->style().position()==TWHTMLStylePositionAbsolute){
		// not supported.
		return NULL;
	}
	
    TWRenderReplaced *replacedObject=NULL;
    
    if(type==TWHTMLElementTypeImg){
        replaced=true;
        replacedObject=new TWRenderImage(node);
    }
    
	// TODO: support for splitting inline elements when creating block in inline.
	if(parent && parent->isInline() && node->style().display()!=TWHTMLStyleDisplayInline &&
	   !replacedObject){
		return NULL;
	}
    
    TWHTMLStyleDisplay displayStyle=node->style().display();
    TWRenderObject *obj;
    TWHTMLElement *baseNode=node;
    if(replaced)
        baseNode=NULL;
    
    if(type==TWHTMLElementTypeBody && parent==NULL){
        obj=new TWRenderView(baseNode);
    }else{
    
        switch(displayStyle){  
            case TWHTMLStyleDisplayNone:
                obj=NULL;
                break;
            case TWHTMLStyleDisplayInline:
                if(replaced)
                    obj=replacedObject;
                else
                    obj=new TWRenderInline(baseNode);
                break;
            case TWHTMLStyleDisplayBlock:
            case TWHTMLStyleDisplayTableCaption:
                obj=new TWRenderBlock(baseNode);
                break;
            case TWHTMLStyleDisplayTable:
                obj=new TWRenderTable(baseNode);
                break;
            case TWHTMLStyleDisplayTableCol:
            case TWHTMLStyleDisplayTableColGroup:
				if(parent && ((!parent->isTableCol()) && (!parent->isTable())))
					obj=new TWRenderInline(baseNode);
				else
					obj=new TWRenderTableCol(baseNode);
                break;
            case TWHTMLStyleDisplayTableRow:
            case TWHTMLStyleDisplayTableRowGroup:
            case TWHTMLStyleDisplayTableHeaderGroup:
            case TWHTMLStyleDisplayTableFooterGroup:
				if(parent && ((!(!parent->isAnonymous() && 
								 (parent->style().display()==TWHTMLStyleDisplayTableRowGroup ||
								  parent->style().display()==TWHTMLStyleDisplayTableHeaderGroup ||
								  parent->style().display()==TWHTMLStyleDisplayTableFooterGroup))) && (!parent->isTable())))
					obj=new TWRenderBlock(baseNode);
				else if(!parent->isAnonymous() && 
						(parent->style().display()==TWHTMLStyleDisplayTableRowGroup ||
						 parent->style().display()==TWHTMLStyleDisplayTableHeaderGroup ||
						 parent->style().display()==TWHTMLStyleDisplayTableFooterGroup)){
					obj=new TWRenderTableRow(baseNode, static_cast<TWRenderTableRow *>(parent)->table());
				}else{
					assert(dynamic_cast<TWRenderTable *>(parent));
					obj=new TWRenderTableRow(baseNode, static_cast<TWRenderTable *>(parent));
				}
                break;
            case TWHTMLStyleDisplayTableCell:
                obj=new TWRenderTableCell(baseNode);
                break;
			case TWHTMLStyleDisplayListItem:
				obj=new TWRenderListItem(baseNode);
				break;
			default:
				if(replaced)
                    obj=replacedObject;
                else
                    obj=new TWRenderInline(baseNode);
                break;
        }
        
    }
	
	if(!obj){
		return NULL;
	}
    
    if(replaced){
        if(obj!=replacedObject){
            obj->addChild(replacedObject);
        }
    }
    
    // TODO: better support for non-inline replaced objects.
    
    for(TWNode *child=node->firstChild();child;child=child->nextSibling()){
        TWHTMLElement *childElement=static_cast<TWHTMLElement *>(child);
		TWRenderObject *obj2=objectWithNode(childElement, obj);
		if(obj2)
			obj->addChild(obj2);
    }
    
    return obj;
    
}

TWRenderBlock *TWRenderObject::containingBlock() const{
    // TODO: deal with TableCell
    // TODO: deal with RenderView
    TWRenderObject *o=static_cast<TWRenderObject *>(parent());
    while(o && ((o->isInline() && !o->isReplaced()) || o->isTableRow() ||
                o->isTableCol()))
        o=static_cast<TWRenderObject *>(o->parent());
    if(!o || !o->isBlock())
        o=NULL;
    return static_cast<TWRenderBlock *>(o);
}

void TWRenderObject::layout(TWRenderManager *manager){
    m_isLayouted=true;
}
void TWRenderObject::render(TWRenderDC * dc){
	for(TWNode *node=firstChild();node;node=node->nextSibling()){
		TWRenderObject *renderObject=static_cast<TWRenderObject *> (node);
		renderObject->render(dc);
	}
}

bool TWRenderObject::isFloating() const{
	if(m_node)
		return m_node->style()._float()!=TWHTMLStyleFloatNone;
    return false;
}

const TWHTMLStyleProxy& TWRenderObject::style() const{
    assert(m_styleValid);
    return m_style;
}

void TWRenderObject::calcStyle(){
    const TWRenderObject *obj=this;
    
    if(obj->node()){
        m_style=obj->node()->style();
    }else{
        while(obj->node()==NULL)
            obj=static_cast<const TWRenderObject *>(obj->parent());
        m_style.inheritFrom(obj->node()->style());
    }
    
    m_styleValid=true;
	
	for(TWNode *node=firstChild();node;node=node->nextSibling()){
		TWRenderObject *renderObject=static_cast<TWRenderObject *> (node);
		renderObject->calcStyle();
	}
	
}

void TWRenderObject::validateLayout(){
#ifndef NDEBUG
	assert(m_isLayouted);
	if(TWRenderBoxModelObject *box=dynamic_cast<TWRenderBoxModelObject *>(this)){
		if((!this->isInline()) && (!this->isReplaced())){
			assert(box->rect().x!=(int)0xdeadbeef);
			assert(box->rect().y!=(int)0xdeadbeef);
			assert(box->rect().x<0x10000000
				   || box->rect().x>-0x10000000);
			assert(box->rect().y<0x10000000
				   || box->rect().y>-0x10000000);
			assert(box->rect().h<0x10000000);
			assert(box->rect().w<0x10000000);
			
		}
	}
	for(TWNode *node=firstChild();node;node=node->nextSibling()){
		TWRenderObject *obj=dynamic_cast<TWRenderObject *>(node);
		obj->validateLayout();
	}
#endif
}

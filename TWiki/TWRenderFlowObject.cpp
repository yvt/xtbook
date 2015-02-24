//
//  TWRenderFlowObject.cpp
//  XTBook
//
//  Created by Nexhawks on 4/10/11.
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

#include "TWRenderFlowObject.h"
#include <assert.h>
#include "TWRenderFlowInline.h"
#include "TWRenderFlowLine.h"
#include "TWRenderFlowContent.h"
#include "TWRenderBlock.h"
#include "TWRenderReplaced.h"
#include "TWRenderText.h"

TWRenderFlowObject::TWRenderFlowObject(TWHTMLElement *node):
TWRenderBox(node){
	m_isRenderFlowObject=true;
	m_isLayouted=true;
}

void TWRenderFlowObject::calcChildrenFlowIndex(){
	assert(!isRenderFlowContent());
	
	int index;
	if(isRenderFlowLine()){
		TWRenderFlowLine *thisLine=static_cast<TWRenderFlowLine *>(this);
		index=thisLine->flowIndexStart();
		
		// deal with floating objects.
		std::list<TWRenderObject *>& objs=thisLine->m_floats;
		
		for(std::list<TWRenderObject *>::iterator it=objs.begin();
			it!=objs.end();it++){
			TWRenderBlock *floatObject=static_cast<TWRenderBlock *>(*it);
			floatObject->setFlowIndexStart(index);
			floatObject->calcChildrenFlowIndex();
			index=floatObject->flowIndexEnd();
		}
		
	}else
		index=static_cast<TWRenderFlowInline *>(this)->flowIndexStart();
	
	for(TWNode *child=firstChild();child;child=child->nextSibling()){
		TWRenderObject *obj=static_cast<TWRenderObject *>(child);
		if(obj->isRenderFlowInline()){
			assert(dynamic_cast<TWRenderFlowInline *>(obj));
			TWRenderFlowInline *o=static_cast<TWRenderFlowInline *>(obj);
			o->setFlowIndexStart(index);
			o->calcChildrenFlowIndex();
			index=o->flowIndexEnd();
		}else if(obj->isRenderFlowContent()){
			assert(dynamic_cast<TWRenderFlowContent *>(obj));
			TWRenderFlowContent *o=static_cast<TWRenderFlowContent *>(obj);
			o->setFlowIndexStart(index);
			// don't calcChildrenFlowIndex for contents
			index=o->flowIndexEnd();
		}
	}
	
	if(isRenderFlowLine())
		static_cast<TWRenderFlowLine *>(this)->setFlowIndexEnd(index);
	else
		static_cast<TWRenderFlowInline *>(this)->setFlowIndexEnd(index);
	
}

unsigned int TWRenderFlowObject::flowIndexAtPoint(const TWPoint & pt) const{
	
	if(isRenderFlowContent()){
		// special routine for flow contents.
		
		const TWRenderFlowContent *content=static_cast<const TWRenderFlowContent *>(this);
		
		int left=content->paddingLeft()+
		content->borderSizeLeft();
		
		int right=content->paddingRight()+
		content->borderSizeRight();
		
		TWRect rt=content->screenRect();
		if(pt.x<rt.x+left)
			return content->flowIndexStart();
		
		if(pt.x>=rt.x+rt.w-right)
			return content->flowIndexEnd();
		
		int minSeg=content->minSegment();
		int maxSeg=content->maxSegment();
		int x=rt.x+left;
		
		for(int seg=minSeg;seg<maxSeg;seg++){
			int ox=x;
			int ww=content->renderer()->contentWidth(seg);
			x+=ww;
			if(x>pt.x){
				// if pt is right than center of the segment,
				// pt points the next segment.
				if(pt.x>=ox+ww/2)
					seg++;
				
				return content->flowIndexStart()+seg-minSeg;
			}
		}
		
		// shouldn't come here...
		assert(false);
		return content->flowIndexEnd();
		
	}
	
	if(pt.x<screenRect().x){
		unsigned int index;
		if(isRenderFlowLine())
			index=static_cast<const TWRenderFlowLine *>(this)->flowIndexStart();
		else
			index=static_cast<const TWRenderFlowInline *>(this)->flowIndexStart();
		return index;
	}
	if(pt.x>screenRect().x+screenRect().w){
		unsigned int index;
		if(isRenderFlowLine())
			index=static_cast<const TWRenderFlowLine *>(this)->flowIndexEnd();
		else
			index=static_cast<const TWRenderFlowInline *>(this)->flowIndexEnd();
		return index;
	}
	
	for(TWNode *child=firstChild();child;child=child->nextSibling()){
		TWRenderFlowObject *obj=static_cast<TWRenderFlowObject *>(child);
		TWRect rt=obj->screenRect().sanitize();
		if(rt.x+rt.w>pt.x){
			return obj->flowIndexAtPoint(pt);
		}
	}
	
	// fall back.
	{
		unsigned int index;
		if(isRenderFlowLine())
			index=static_cast<const TWRenderFlowLine *>(this)->flowIndexEnd();
		else
			index=static_cast<const TWRenderFlowInline *>(this)->flowIndexEnd();
		return index;
	}

}

int TWRenderFlowObject::horizonalPositionForFlowIndex(unsigned int index) const{
	if(isRenderFlowContent()){
		const TWRenderFlowContent *content=static_cast<const TWRenderFlowContent *>(this);
		TWRect rt=content->screenRect();
		int left=content->paddingLeft()+
		content->borderSizeLeft();
		
		int right=content->paddingRight()+
		content->borderSizeRight();
		
		if(index<=content->flowIndexStart())
			return rt.x+left;
		if(index>=content->flowIndexEnd()){
			return rt.x+rt.w-right;
		}
		
		index-=content->flowIndexStart()-content->minSegment();
		
		int x=rt.x+left;
		int i=content->minSegment();
		while(i<index){
			x+=content->renderer()->contentWidth(i);
			i++;
		}
		
		return x;
	}
	
	TWRect rt=screenRect();
	
	// scan boundary.
	{
		unsigned int start;
		if(isRenderFlowLine())
			start=static_cast<const TWRenderFlowLine *>(this)->flowIndexStart();
		else
			start=static_cast<const TWRenderFlowInline *>(this)->flowIndexStart();
		if(index<=start){
			return rt.x;
		}
		
	}
	
	{
		unsigned int end;
		if(isRenderFlowLine())
			end=static_cast<const TWRenderFlowLine *>(this)->flowIndexEnd();
		else
			end=static_cast<const TWRenderFlowInline *>(this)->flowIndexEnd();
		if(index>=end)
			return rt.x+rt.w;
	}
	
	// go through children.
	
	for(TWNode *child=firstChild();child;
		child=child->nextSibling()){
		TWRenderFlowObject *obj=static_cast<TWRenderFlowObject *>(child);
		unsigned int end;
		if(obj->isRenderFlowContent())
			end=static_cast<const TWRenderFlowContent *>(obj)->flowIndexEnd();
		else
			end=static_cast<const TWRenderFlowInline *>(obj)->flowIndexEnd();
		if(end>index)
			return obj->horizonalPositionForFlowIndex(index);
	}
	
	
	// shouldn't reach here...
	assert(false);
	return -1;
	
}

void TWRenderFlowObject::captureSelectionString(std::wstring &out, int begin, int end){
	
	// scan boundary.
	{
		unsigned int start;
		if(isRenderFlowLine())
			start=static_cast<const TWRenderFlowLine *>(this)->flowIndexStart();
		else if(isRenderFlowContent())
			start=static_cast<const TWRenderFlowContent *>(this)->flowIndexStart();
		else
			start=static_cast<const TWRenderFlowInline *>(this)->flowIndexStart();
		if(end<=start){
			return;
		}
	}
	{
		unsigned int last;
		if(isRenderFlowLine())
			last=static_cast<const TWRenderFlowLine *>(this)->flowIndexEnd();
		else if(isRenderFlowContent())
			last=static_cast<const TWRenderFlowContent *>(this)->flowIndexEnd();
		else
			last=static_cast<const TWRenderFlowInline *>(this)->flowIndexEnd();
		if(begin>=end)
			return;
	}
	
	if(isRenderFlowContent()){
		TWRenderFlowContent *content=static_cast<TWRenderFlowContent *>(this);
		TWRenderReplaced *renderer=content->renderer();
		if(renderer->isText()){
			TWRenderText *text=static_cast<TWRenderText *>(renderer);
			if(begin<content->flowIndexStart())
				begin=content->flowIndexStart();
			if(end>content->flowIndexEnd())
				end=content->flowIndexEnd();
			if(begin>=end)
				return;
			int minSeg=begin;
			int maxSeg=end;
			minSeg-=content->flowIndexStart()-content->minSegment();
			maxSeg-=content->flowIndexStart()-content->minSegment();
			out+=text->substringForSegmentRange(minSeg, maxSeg);
		}else if(renderer->isBR()){
			out+=L'\n';
		}
		return;
	}
	
	if(isRenderFlowLine()){
		TWRenderFlowLine *thisLine=static_cast<TWRenderFlowLine *>(this);
		
		// deal with floating objects.
		std::list<TWRenderObject *>& objs=thisLine->m_floats;
		
		for(std::list<TWRenderObject *>::iterator it=objs.begin();
			it!=objs.end();it++){
			TWRenderBlock *floatObject=static_cast<TWRenderBlock *>(*it);
			floatObject->captureSelectionString(out, begin, end);
		}
		
	}
	
	for(TWNode *child=firstChild();child;child=child->nextSibling()){
		TWRenderFlowObject *obj=static_cast<TWRenderFlowObject *>(child);
		obj->captureSelectionString(out, begin, end);
	}
}


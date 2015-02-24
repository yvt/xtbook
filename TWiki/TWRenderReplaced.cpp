//
//  TWRenderReplaced.cpp
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

#include "TWRenderReplaced.h"
#include "TWRenderBlock.h"
#include "TWRenderFlowContent.h"
#include "TWRenderDC.h"
#include "TWHTMLElement.h"
#include "TWUtils.h"

TWRenderReplaced::TWRenderReplaced(TWHTMLElement *element):
TWRenderBox(element){
    m_isReplaced=true;
}

int TWRenderReplaced::replacedDefaultWidth() const{
	return 20;
}

int TWRenderReplaced::replacedDefaultHeight() const{
	return 20;
}

void TWRenderReplaced::render(TWRenderDC *dc){
	return;
}
void TWRenderReplaced::render(TWRenderFlowContent *content, TWRenderDC *dc){
	TWRect screenRect=content->screenRect();
	
	dc->setStyle(content->style());
	
	dc->drawBorder(screenRect);
	
	dc->debugRenderObject(m_screenRect, this);
	
}

const std::wstring g_widthAttrName(L"width");
const std::wstring g_heightAttrName(L"height");
const std::wstring g_noMagnifyAttrName(L"twiki:nomagnify");


void TWRenderReplaced::layout(TWRenderManager *manager){
	TWRenderBox::layout(manager);
	TWRenderBlock *cb=containingBlock();
	calcWidth();
	m_rect.h=style().height().toPixels(replacedDefaultHeight(), style());
	
	if(!isText() && !isBR()){
		if(node()){
			int specifiedWidth=-1;
			int specifiedHeight=-1;
			bool noMagnify=false;
			
			if(node()->hasAttribute(g_widthAttrName)){
				specifiedWidth=TWUtils::intValueFromString(node()->getAttribute(g_widthAttrName));
			}
			
			if(node()->hasAttribute(g_heightAttrName)){
				specifiedHeight=TWUtils::intValueFromString(node()->getAttribute(g_heightAttrName));
			}
			
			if(node()->hasAttribute(g_noMagnifyAttrName)){
				noMagnify=true;
			}
			
			// invalid size?
			if(specifiedWidth<0)
				specifiedWidth=-1;
			if(specifiedHeight<0)
				specifiedHeight=-1;
			
			if(specifiedWidth!=-1 && specifiedHeight!=-1){
				// forced set
				m_rect.w=specifiedWidth;
				m_rect.h=specifiedHeight;
			}else if(specifiedWidth!=-1 && m_rect.w!=0){
				if(specifiedWidth<m_rect.w || !noMagnify){
					m_rect.h=(m_rect.h*specifiedWidth)/m_rect.w;
					m_rect.w=specifiedWidth;
				}
			}else if(specifiedHeight!=-1 && m_rect.h!=0){
				if(specifiedHeight<m_rect.h || !noMagnify){
					m_rect.w=(m_rect.w*specifiedHeight)/m_rect.h;
					m_rect.h=specifiedHeight;
				}
			}
			
			m_rect.w+=paddingLeft();
			m_rect.w+=paddingRight();
			m_rect.w+=borderSizeLeft();
			m_rect.w+=borderSizeRight();
			m_rect.h+=paddingTop();
			m_rect.h+=paddingBottom();
			m_rect.h+=borderSizeTop();
			m_rect.h+=borderSizeBottom();
		}
	}
	//XTBLog("%dx%d", m_rect.w, m_rect.h);
	if(cb->isBuildingFlow()){
		int segmentCount=segments();
		for(int i=0;i<segmentCount;i++){
			TWRenderBlock::FlowTag tag;
			tag.type=TWRenderBlock::FlowTagTypeContent;
			tag.renderer=this;
			tag.segment=i;
			cb->addFlowTag(tag);
		}
	}
	
	m_isLayouted=true;
}

int TWRenderReplaced::contentWidth() const{
	return m_rect.w;
}
int TWRenderReplaced::contentHeight() const{
	return m_rect.h;
}
int TWRenderReplaced::segments() const{
	return 1;
}
int TWRenderReplaced::contentWidth(int segment) const{
	return contentWidth();
}

wchar_t TWRenderReplaced::firstChar(int segment) const{
	return 0;
}
wchar_t TWRenderReplaced::lastChar(int segment) const{
	return 0;
}

wchar_t TWRenderReplaced::firstChar() const{
	return firstChar(0);
}
wchar_t TWRenderReplaced::lastChar() const{
	return lastChar(segments()-1);
}

void TWRenderReplaced::handleMaximumContentWidth(TWRenderManager *manager){
	TWRenderBlock *cb=containingBlock();
	layout(manager);
	cb->addMaximumContentWidth(marginLeft());
	cb->addMaximumContentWidth(paddingLeft());
	cb->addMaximumContentWidth(borderSizeLeft());
	cb->addMaximumContentWidth(contentWidth());
	cb->addMaximumContentWidth(borderSizeRight());
	cb->addMaximumContentWidth(paddingRight());
	cb->addMaximumContentWidth(marginRight());
	
}

void TWRenderReplaced::handleMinimumContentWidth(TWRenderManager *manager){
	TWRenderBlock *cb=containingBlock();
	layout(manager);
	if(segments()==1){
		cb->addMinimumContentWidth(marginLeft()+
								   paddingLeft()+
								   borderSizeLeft()+
								   contentWidth()+
								   borderSizeRight()+
								   paddingRight()+
								   marginRight());
	}else{
		int cnt=segments();
		for(int i=0;i<cnt;i++){
			cb->addMinimumContentWidth(contentWidth(i));
		}
	}
	
}


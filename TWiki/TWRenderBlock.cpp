/*
 *  TWRenderBlock.cpp
 *  XTBook
 *
 *  Created by Nexhawks on 2/28/11.
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

#include "TWRenderBlock.h"
#include "TWHTMLStyle.h"
#include "TWHTMLElement.h"
#include <assert.h>
#include "TWRenderFlowLine.h"
#include "TWRenderBox.h"
#include "TWRenderInline.h"
#include "TWRenderFlowInline.h"
#include "TWRenderText.h"
#include "TWRenderFlowContent.h"

TWRenderBlock::TWRenderBlock(TWHTMLElement *node):
TWRenderBox(node),
m_firstLine(NULL),
m_flowInfo(0),
m_maximumContentWidth(-1),
m_minimumContentWidth(-1),
m_flowIndexStart(0),
m_minY(0),
m_maxY(0),
m_lineMinY(0),
m_lineMaxY(0),
m_lastMaxY(0){
	m_isBlock=true;
}

void TWRenderBlock::layout(TWRenderManager *manager){
    if(isInline())
        return;
    calcWidth();
    clearFloats();
    
    m_rect.h=0;
    
	TWPoint cbOffset;
	
	// get floating objects from the containing block.
	if(parent() && !isFloating()){
		TWRenderBlock *cb=containingBlock();
		cbOffset=offsetToContainingBlock();
		swapFloatingObjects(cb, cbOffset);
	}
	
	if(!firstChild())
		layoutEmpty(manager);
	else{
		if(isChildrenInline())
			layoutInlineChildren(manager);
		else
			layoutBlockChildren(manager);
	}

	
	// return floating objects to the containing block.
	if(parent() && !isFloating()){
		TWRenderBlock *cb=containingBlock();
		swapFloatingObjects(cb, cbOffset);
	}else{
		clearFloats();
	}
    
    if(style().height().unit!=TWHTMLStyleUnitAuto){
        TWRenderBox *cb=containingBlock();
		int newHeight=style().height().toPixels(cb->contentHeight(), style());
		// we don't have css "overflow". use safe option.
		
         /* m_rect.h=style().height().toPixels(cb->contentHeight(), style()); */
		if(newHeight>m_rect.h)
			m_rect.h=newHeight;
    }
	
	m_isLayouted=true;
}

void TWRenderBlock::swapFloatingObjects(TWRenderBlock *block,
										TWPoint offset){
	if(m_floatingObjects.empty() && block->m_floatingObjects.empty())
		return;
	
	m_floatingObjects.swap(block->m_floatingObjects);
	
	std::list<FloatingObject>::iterator it;
	
	if(m_floatingObjects.empty()){
		// moved to parent.
		for(it=block->m_floatingObjects.begin();
			it!=block->m_floatingObjects.end();
			it++){
			FloatingObject& obj=*it;
			obj.rect.x+=offset.x;
			obj.rect.y+=offset.y;
			
			// ???: assert(obj.placed);
		}
	}else{
		// moved to this.
		for(it=m_floatingObjects.begin();
			it!=m_floatingObjects.end();
			it++){
			FloatingObject& obj=*it;
			obj.rect.x-=offset.x;
			obj.rect.y-=offset.y;
			
			// ???: assert(obj.placed);
		}
	}
	
}

TWPoint TWRenderBlock::offsetToContainingBlock() const{
	assert(parent());
	TWRenderBlock *cb=containingBlock();
	const TWRenderObject *obj=this;
	TWPoint offset(0,0);
	while(obj!=cb){
		if(obj->isBoxModelObject()){
			const TWRenderBoxModelObject *box=static_cast<const TWRenderBoxModelObject *>(obj);
			offset+=box->rect().loc();
		}
		obj=static_cast<TWRenderObject *>(obj->parent());
	}
	return offset;
}

void TWRenderBlock::layoutEmpty(TWRenderManager *){
	int beforeEdge=paddingTop()+borderSizeTop();
    int afterEdge=paddingBottom()+borderSizeBottom();
    
    m_rect.h=beforeEdge;
	m_rect.h+=afterEdge;
	
}

void TWRenderBlock::layoutBlockChildren(TWRenderManager * manager){
    int beforeEdge=paddingTop()+borderSizeTop();
    int afterEdge=paddingBottom()+borderSizeBottom();
    int lastMargin=0;
    
    m_rect.h=beforeEdge;
	
    
    for(TWNode *node=firstChild();node;node=node->nextSibling()){
        TWRenderBox *childBox=static_cast<TWRenderBox *>(node);
        
        if(handleFloatingChild(childBox, manager))
            continue;
        
		if(childBox->isListMarker()) // skip now.
			continue;
		
		if(childBox->style().clear()!=TWHTMLStyleFloatNone){
			int newY;
			switch (childBox->style().clear()) {
				case TWHTMLStyleFloatNone:
				case TWHTMLStyleFloatLeft:
					newY=lowestFloatLogicalBottom(FloatingObjectTypeLeft);
					if(newY>m_rect.h)
						m_rect.h=newY;
					break;
				case TWHTMLStyleFloatRight:
					newY=lowestFloatLogicalBottom(FloatingObjectTypeRight);
					if(newY>m_rect.h)
						m_rect.h=newY;
					break;
				case TWHTMLStyleFloatBoth:
					newY=lowestFloatLogicalBottom(FloatingObjectTypeBoth);
					if(newY>m_rect.h)
						m_rect.h=newY;
					clearFloats();
					break;
			}
		}
		
        int top=m_rect.h;
        childBox->calcWidth();
        TWRect rt=childBox->rect();
        rt.y=top;
        
        if(childBox->style().marginLeft().unit==TWHTMLStyleUnitAuto){
            if(childBox->style().marginRight().unit==TWHTMLStyleUnitAuto){
                // centering
                rt.x=(availableLogicalWidth()-rt.w)/2;
            }else{
                rt.x=availableLogicalWidth()-rt.w-childBox->marginRight();
            }
        }else{
            rt.x=childBox->marginLeft();
        }
        rt.x+=logicalLeftOffsetForContent();
        
        int beforeMargin=childBox->marginTop();
		
		rt.y+=beforeMargin;
		
        int collapseHeight=lastMargin;
        if(beforeMargin<collapseHeight)
            collapseHeight=beforeMargin;
        
        // collapse margin
        rt.y-=collapseHeight;
		
		int firstY=rt.y;
        
        // resolve style::clear
        rt.y+=clearDelta(childBox, rt.y);
		
		TWPoint offset=rt.loc();
			
        
        childBox->setRect(rt);
        childBox->layout(manager);
		
        
        rt=childBox->rect();
		
		// table avoids float elements.
		if(childBox->isTable()){
			if(rt.y<lowestFloatLogicalBottom(FloatingObjectTypeBoth)){
				if(rt.w>availableLogicalWidthForLine(rt.y, false)){
					clearFloats();
					rt.y=m_rect.h;
				}
			}
		}
        
        m_rect.h=rt.y+rt.h+childBox->marginBottom();
		
		if(childBox->isTable()){
			if(childBox->style().marginLeft().unit==TWHTMLStyleUnitAuto){
				if(childBox->style().marginRight().unit==TWHTMLStyleUnitAuto){
					// centering
					rt.x=(availableLogicalWidth()-rt.w)/2;
				}else{
					rt.x=availableLogicalWidth()-rt.w-childBox->marginRight();
				}
			}else{
				rt.x=childBox->marginLeft();
			}
			 rt.x+=logicalLeftOffsetForContent();
			childBox->setRect(rt);
		}
		
		assert(rt.y>=firstY);
        
    }
	
	positionNewFloats();
	
	if(isTableCell() || isFloating()){
		// we cannot return float objects to parent.
		// we have to deal with them right now.
		clearFloats();
	}
    
    m_rect.h+=afterEdge;
}

enum CharGroup{
	CharGroupAlphabet=0,
	CharGroupNumeric,
	CharGroupSymbol,
	CharGroupKinsokuBegin,
	CharGroupKinsokuEnd,
	CharGroupKinsokuBoth,
	CharGroupOther
};

static inline CharGroup charGroupForChar(wchar_t c){
	if((c>=L'a' && c<=L'z') || (c>=L'A' && c<=L'Z'))
		return CharGroupAlphabet;
	if(c>=L'0' && c<=L'9')
		return CharGroupNumeric;
//&#x30FB;&#x3001;:;,
	switch((uint16_t)c){
		case L']': case L')': case L':': case L';': case L',':
		case 0x30fb: case 0x3001: case 0x3002: case L'.':
		case 0x203c: case 0x2047: case 0x2048: case 0x2049:
		case 0xff5d: case 0x3015: case 0x3009: case 0x300b: case 0x300d:
		case 0x300f: case 0x3011: case 0x3019: case 0x3017: case 0x301f:
		case 0x2019: case 0x201d: case 0xff60: case 0x30fd: case 0x30fe:
		case 0x30fc: case 0x30a1: case 0x30a3: case 0x30a5: case 0x30a7:
		case 0x30a9: case 0x30c3: case 0x30e3: case 0x30e5: case 0x30e7:
		case 0x30ee: case 0x30f5: case 0x30f6: case 0x3041: case 0x3043:
		case 0x3045: case 0x3047: case 0x3049: case 0x3063: case 0x3083:
		case 0x3085: case 0x3087: case 0x308e: case 0x3095: case 0x3096:
		case 0x31f0: case 0x31f1: case 0x31f2: case 0x31f3: case 0x31f4:
		case 0x31f5: case 0x31f6: case 0x31f7: case 0x31f8: case 0x31f9:
		case 0x31fa: case 0x31fb: case 0x31fc: case 0x31fd: case 0x31fe:
		case 0x31ff: case 0x303b:
		case 0x203a: case 0x003e: case 0xff09: case 0x2769: case 0x276f:
		case 0x007d: 
		case 0xbb:
			return CharGroupKinsokuBegin;
			
		case L'[':
		case L'(':
		case 0xff5b: case 0x3014: case 0x3008: case 0x300a: case 0x300c:
		case 0x300e: case 0x3010: case 0x3018: case 0x3016: case 0x301d:
		case 0x2018: case 0x201c: case 0xff5f: case 0xab:
		case 0x2039: case 0x003c: case 0xff08: case 0x2768: case 0x276e:
		case 0x007b:
			return CharGroupKinsokuEnd;
	}
	if(c>=1 && c<=255)
		return CharGroupSymbol;
	
	return CharGroupOther;
}

static bool isCharsBreakable(wchar_t c1, wchar_t c2){
	
	CharGroup g1=charGroupForChar(c1);
	CharGroup g2=charGroupForChar(c2);
	
	// special cases.
	
	// number like -5,000,000 mustn't be separated.
	if(c1==L',' && g2==CharGroupNumeric)
		return false;
	if(g1==CharGroupNumeric && c2==L',')
		return false;
	if((c1==L'-' || c1==L'+') && g2==CharGroupNumeric)
		return false;
	
	if(g1==CharGroupKinsokuEnd)
		return false;
	if(g2==CharGroupKinsokuBegin)
		return false;
	if(g1==CharGroupKinsokuBoth || g2==CharGroupKinsokuBoth)
		return false;
	
	if(g1!=g2)
		return true;
	if(g1==CharGroupOther ||
	   g2==CharGroupOther)
		return true;
	if(g1==CharGroupSymbol ||
	   g2==CharGroupSymbol)
		return true;
	return false;
}

void TWRenderBlock::clearLines(){
	for(std::list<TWRenderFlowLine *>::iterator it=m_lines.begin();it!=m_lines.end();it++)
		delete *it;
	m_lines.clear();
	m_firstLine=NULL;
	m_minY=0;
	m_maxY=0;
	m_lineMinY=0;
	m_lineMaxY=0;
	m_lastMaxY=0;
	m_lastChar=0;
	m_flowInfo->flowTagsWidth=0;
	assert(m_flowInfo->flowTags.empty());
	assert(m_flowInfo->inlineStack.empty());
	assert(m_flowInfo->flowStack.empty());
}

int TWRenderBlock::currentLineHeight() const{
	assert(m_flowInfo);
	int lineHeight=m_flowInfo->specifiedLineHeight;
	
	if(m_lineMaxY-m_lineMinY+m_lastMaxY>lineHeight)
		lineHeight=m_lineMaxY-m_lineMinY+m_lastMaxY;
	
	return lineHeight;
}

int TWRenderBlock::currentLineHeightAfterFlushingFlowTags() const{
	assert(m_flowInfo);
	int lineHeight=m_flowInfo->specifiedLineHeight;
	
	int maxY=std::max(m_maxY, m_lineMaxY);
	int minY=std::min(m_minY, m_lineMinY);
	if(maxY-minY+m_lastMaxY>lineHeight)
		lineHeight=maxY-minY+m_lastMaxY;
	
	return lineHeight;
}

void TWRenderBlock::newLine(TWHTMLStyleFloat clear){
	
	assert(m_flowInfo);
	
	// break flow elements
	for(std::list<TWRenderInline *>::iterator it=
		m_flowInfo->inlineStack.begin();it!=m_flowInfo->inlineStack.end();it++){
		
	}
	for(std::list<TWRenderFlowInline *>::iterator it=
		m_flowInfo->flowStack.begin();it!=m_flowInfo->flowStack.end();it++){
		// remove right border
		TWRenderFlowInline *content=*it;
		content->removeRightBorder();
	}
	m_flowInfo->flowStack.clear();
	
	// finalize the last line.
	if(!m_lines.empty()){
		TWRenderFlowLine *lastLine=m_lines.back();
		
		TWRect rt=lastLine->rect();
		int lineHeight=currentLineHeight();
		
		
		
		rt.y+=lineHeight-m_lineMaxY;
		m_rect.h=rt.y+m_lineMaxY;
		
		//assert(rt.sanitize().y>=0);		
		
		lastLine->setRect(rt);
		
	}
	
	m_lastChar=0;
	m_lastMaxY=m_lineMaxY;
	m_lineMinY=0;
	m_lineMaxY=0;
	
	{
		TWRenderFlowLine *line=new TWRenderFlowLine();
		TWRect rt=line->rect();
		rt.y=m_rect.h;
		rt.x=logicalLeftOffsetForLine(rt.y, m_lines.empty(), currentLineHeight());
		assert(rt.w==0);
		line->setRect(rt);
		
		m_lines.push_back(line);
		
		if(m_firstLine==NULL)
			m_firstLine=line;
		
		TWRenderBox::addChild(line);
		line->calcStyle();
		
		// restart flow elements
		TWRenderFlowObject *parent=line;
		for(std::list<TWRenderInline *>::iterator it=
			m_flowInfo->inlineStack.begin();it!=m_flowInfo->inlineStack.end();it++){
			TWRenderFlowInline *flowInline;
			flowInline=new TWRenderFlowInline(*it);
			
			
			
			parent->addChild(flowInline);
			
			
			flowInline->calcStyle();
			
			TWRenderInline *renderer=flowInline->renderer();
			int height=renderer->contentHeight();
			height+=renderer->paddingTop();
			height+=renderer->paddingBottom();
			height+=renderer->borderSizeTop();
			height+=renderer->borderSizeBottom();
			flowInline->setRect(TWRect(0, renderer->paddingBottom()+renderer->borderSizeBottom(), 0, -height));
			flowInline->removeLeftborder();
			
			int minY, maxY;
			minY=-renderer->contentHeight();
			maxY=0;
			minY-=renderer->borderSizeTop();
			minY-=renderer->paddingTop();
			minY-=renderer->marginTop();
			maxY+=renderer->borderSizeBottom();
			maxY+=renderer->paddingBottom();
			maxY+=renderer->marginBottom();
			
			
			if(minY<m_lineMinY)
				m_lineMinY=minY;
			if(maxY>m_lineMaxY)
				m_lineMaxY=maxY;
			
			parent=flowInline;
			
			m_flowInfo->flowStack.push_back(flowInline);
		}
	}
	
	m_flowInfo->currentAvailableWidthValid=false;
	m_flowInfo->maxAvailableWidth=maxAvailableWidthForContents();
	
	positionNewFloats();
	
}

int TWRenderBlock::maxAvailableWidthForContents()const{
	int logicalLeftFixedOffset=logicalLeftOffsetForContent();
	int logicalRightFixedOffset=logicalRightOffsetForContent();
	int logicalLeftFixedOffsetIndented=logicalLeftFixedOffset;
	int logicalRightFixedOffsetIndented=logicalRightFixedOffset;
	
	if(m_firstLine==m_lines.back()){
		int cw;
        if(style().textIndent().unit==TWHTMLStyleUnitPercent)
            cw=containingBlock()->availableLogicalWidth();
        logicalLeftFixedOffsetIndented+=style().textIndent().toPixels(cw, node()->style());
	}
	
	return logicalRightFixedOffsetIndented-logicalLeftFixedOffsetIndented;
}

int TWRenderBlock::widthOfFlowTag(const TWRenderBlock::FlowTag & tag) const{
	assert(m_flowInfo);
	if(tag.type==FlowTagTypeContent){
		TWRenderReplaced *box=static_cast<TWRenderReplaced *>(tag.renderer);
		return box->contentWidth(tag.segment)+
		box->paddingLeft()+
		box->marginLeft()+box->borderSizeLeft()+
		box->paddingRight()+
		box->marginRight()+box->borderSizeRight();
	}else if(tag.type==FlowTagTypeStart){
		TWRenderInline *childInline=static_cast<TWRenderInline *>(tag.renderer);
		return childInline->paddingLeft()+
		childInline->marginLeft()+childInline->borderSizeLeft();
	}else if(tag.type==FlowTagTypeEnd){
		TWRenderInline *childInline=static_cast<TWRenderInline *>(tag.renderer);
		return childInline->paddingRight()+
		childInline->marginRight()+childInline->borderSizeRight();
	}
	assert(false);
	return 0;
}

void TWRenderBlock::addFlowTag(FlowTag tag){
	assert(m_flowInfo);
	int minY, maxY;
	
	if(tag.type==FlowTagTypeContent){
		TWRenderBox *content=static_cast<TWRenderBox *>(tag.renderer);
		wchar_t firstChar;
		
		if(!content->isText())
			firstChar=0;
		else
			firstChar=static_cast<TWRenderText *>(content)->firstChar(tag.segment);
		if(isCharsBreakable(m_lastChar, firstChar)){
			// can break.
			flushFlowTags();
		}
		if(!content->isText())
			m_lastChar=0;
		else
			m_lastChar=static_cast<TWRenderText *>(content)->lastChar(tag.segment);
		
		minY=-content->contentHeight();
		maxY=0;
		
		
		
	}else if(tag.type==FlowTagTypeStart){
		TWRenderInline *content=static_cast<TWRenderInline *>(tag.renderer);
		minY=-content->contentHeight();
		maxY=0;
		minY-=content->borderSizeTop();
		minY-=content->paddingTop();
		minY-=content->marginTop();
		maxY+=content->borderSizeBottom();
		maxY+=content->paddingBottom();
		maxY+=content->marginBottom();
	}else if(tag.type==FlowTagTypeEnd){
		TWRenderInline *content=static_cast<TWRenderInline *>(tag.renderer);
		minY=-content->contentHeight();
		maxY=0;
		minY-=content->borderSizeTop();
		minY-=content->paddingTop();
		minY-=content->marginTop();
		maxY+=content->borderSizeBottom();
		maxY+=content->paddingBottom();
		maxY+=content->marginBottom();
	}
	m_flowInfo->flowTagsWidth+=widthOfFlowTag(tag);
	m_flowInfo->flowTags.push_back(tag);
	if(minY<m_minY)
		m_minY=minY;
	if(maxY>m_maxY)
		m_maxY=maxY;
	
}

int TWRenderBlock::currentAvailableWidthForFlow(){
	assert(m_flowInfo);
	int lineHeight=currentLineHeightAfterFlushingFlowTags();
	
	if((!m_flowInfo->currentAvailableWidthValid) || 
	   (lineHeight!=m_flowInfo->currentAvailableWidthLineHeight)){
		m_flowInfo->currentAvailableWidthLineHeight=lineHeight;
		bool firstLine= &(*(m_lines.begin()))==&(*(m_lines.end()));
		m_flowInfo->currentAvailableWidth=availableLogicalWidthForLine(m_rect.h, firstLine,
																	   lineHeight);
		
		m_flowInfo->currentAvailableWidthValid=true;
	}
	return m_flowInfo->currentAvailableWidth;
}

void TWRenderBlock::flushFlowTags(){
	assert(m_flowInfo);
	assert(!m_lines.empty());
	TWRenderFlowLine *lastLine=m_lines.back();
	
	int newWidth=lastLine->rect().w;
	newWidth+=m_flowInfo->flowTagsWidth;
	
	bool firstLine= &(*(m_lines.begin()))==&(*(m_lines.end()));
	int availableWidth=currentAvailableWidthForFlow();
	
	if(newWidth>availableWidth && 
	   (availableWidth<m_flowInfo->maxAvailableWidth || lastLine->rect().w>0)){
		// the line becoming too long to fit; break the line.
		
		newLine(TWHTMLStyleFloatNone);
		lastLine=m_lines.back();
		newWidth=m_flowInfo->flowTagsWidth;
		
		// avoid floats enough.
		int delta=fittingLineDelta(m_rect.h, currentLineHeightAfterFlushingFlowTags(),
								   newWidth, firstLine);
		m_rect.h+=delta;
		availableWidth=currentAvailableWidthForFlow();
		
		TWRect r=lastLine->rect();
		r.y+=delta;
		r.x=logicalLeftOffsetForLine(m_rect.h, logicalLeftOffsetForContent(),
									 firstLine, NULL, currentLineHeight());
		
		lastLine->setRect(r);
		
		
	}
	
	// process flow tags.
	
	for(std::vector<FlowTag>::iterator it=m_flowInfo->flowTags.begin();it!=m_flowInfo->flowTags.end();it++){
		const FlowTag& tag=*it;
		int width=widthOfFlowTag(tag);
		int leftPos=lastLine->rect().w;
	
		TWRenderBox *parent=lastLine;
		if(!m_flowInfo->flowStack.empty())
			parent=m_flowInfo->flowStack.back();
		
		int minY, maxY;
		
		if(tag.type==FlowTagTypeStart){
			TWRenderFlowInline *flowInline;
			TWRenderInline *content=static_cast<TWRenderInline *>(tag.renderer);
			flowInline=new TWRenderFlowInline(content);
			
			int beforeOffset=0;
			if(parent!=lastLine){
				TWRenderInline *containingInline=m_flowInfo->inlineStack.back();
				beforeOffset=containingInline->paddingLeft();
				beforeOffset+=containingInline->marginLeft();
			}
			
			beforeOffset+=content->marginLeft();
		
			
			int height=content->contentHeight();
			height+=content->paddingTop();
			height+=content->paddingBottom();
			height+=content->borderSizeTop();
			height+=content->borderSizeBottom();
			
			int vertOffset=0;
			if(content->node()->type()==TWHTMLElementTypeSup)
				vertOffset-=content->style().fontSize().toPixels(content->style().inheritedFontSize(), content->style())/3;
			if(content->node()->type()==TWHTMLElementTypeSub)
				vertOffset+=content->style().fontSize().toPixels(content->style().inheritedFontSize(), content->style())/3;
			
			flowInline->setRect(TWRect(parent->rect().w+beforeOffset, 
									   content->paddingBottom()+content->borderSizeBottom()+vertOffset, 
									   width-beforeOffset, 
									   -height));
			
			parent->addChild(flowInline);
			flowInline->calcStyle();
			
			m_flowInfo->flowStack.push_back(flowInline);
			m_flowInfo->inlineStack.push_back(content);
			
			
			maxY=content->paddingBottom()+content->borderSizeBottom()+content->marginBottom();
			minY=maxY-height-content->marginBottom()-content->marginTop();
			
		}else if(tag.type==FlowTagTypeEnd){
			TWRenderInline *content=static_cast<TWRenderInline *>(tag.renderer);
			
			TWRenderFlowInline *flowInline;
			assert(!m_flowInfo->flowStack.empty());
			assert(!m_flowInfo->inlineStack.empty());
			flowInline=m_flowInfo->flowStack.back();
			
			TWRect rt=flowInline->rect();
			rt.w+=flowInline->paddingRight();
			rt.w+=flowInline->borderSizeRight();
			flowInline->setRect(rt);
			
			assert(content==m_flowInfo->inlineStack.back());
			m_flowInfo->inlineStack.pop_back();
			m_flowInfo->flowStack.pop_back();
			
			parent=lastLine;
			if(!m_flowInfo->flowStack.empty())
				parent=m_flowInfo->flowStack.back();
		}else if(tag.type==FlowTagTypeContent){
			TWRenderFlowContent *flowContent;
			TWRenderReplaced *content=static_cast<TWRenderReplaced *>(tag.renderer);
			
			//int beforeOffset=content->marginLeft();
			int height=content->contentHeight();
			height+=content->paddingTop();
			height+=content->paddingBottom();
			height+=content->borderSizeTop();
			height+=content->borderSizeBottom();
			
			TWRenderFlowContent *lastContent=NULL;
			
			if(!parent->isEmpty()){
				TWRenderFlowObject *lastObj=static_cast<TWRenderFlowObject *>(parent->lastChild());
				if(lastObj->isRenderFlowContent()){
					lastContent=static_cast<TWRenderFlowContent *>(lastObj);
				}
			}
			
			if(lastContent && lastContent->isAppendable(content, tag.segment)){
				
				lastContent->append(content, tag.segment);
				
				TWRect oldRect=lastContent->rect();
				oldRect.w+=width;
				lastContent->setRect(oldRect);
				
			}else{
			
				flowContent=new TWRenderFlowContent(content, tag.segment);
				flowContent->setRect(TWRect(parent->rect().w,
											-parent->paddingBottom()-parent->borderSizeBottom()-height, width, height));
				parent->addChild(flowContent);
				flowContent->calcStyle();
				
			}
			
			maxY=-parent->paddingBottom()-parent->borderSizeBottom();
			minY=maxY-height;
			
		}
		
		if(tag.type!=FlowTagTypeEnd){
			TWRenderBox *node=parent;
			
			while(node!=lastLine){
				int y=node->rect().y;
				minY+=y;
				maxY+=y;
				
				node=static_cast<TWRenderBox *>(node->parent());
			}
		
			if(minY<m_lineMinY)
				m_lineMinY=minY;
			if(maxY>m_lineMaxY)
				m_lineMaxY=maxY;
		}
		
		{
			TWRenderBox *node=parent;
			while(node!=lastLine){
				TWRect rt=node->rect();
				rt.w+=width;
				node->setRect(rt);
				node=static_cast<TWRenderBox *>(node->parent());
			}
		}
		
		lastLine->addedNodeWithRect(TWRect(leftPos,m_minY, width, m_maxY-m_minY));
	}
	
	m_flowInfo->flowTags.clear();
	
	
	assert(lastLine->rect().w==newWidth);
	
	
	m_flowInfo->flowTagsWidth=0;
	m_minY=m_maxY=0;
	
	//positionNewFloats();
	
}

static TWRenderBlock::FlowInfo g_staticFlowInfo;
static bool g_staticFlowInfoBeingUsed=false;

void TWRenderBlock::alignLines(){
	assert(isChildrenInline());
	TWHTMLStyleTextAlign align=style().textAlign();
	
	if(align==TWHTMLStyleTextAlignLeft)
		return;
	
	for(std::list<TWRenderFlowLine *>::iterator it=m_lines.begin();
		it!=m_lines.end();it++){
		TWRenderFlowLine *line=*it;
		//int leftOffset;
		int availableWidth;
		TWRect rt=line->rect();
		
		//leftOffset=logicalLeftOffsetForLine(rt.y, line==m_firstLine);
		availableWidth=availableLogicalWidthForLine(rt.y, line==m_firstLine);
		
		int offset=0;
		if(align==TWHTMLStyleTextAlignCenter)
			offset=(availableWidth-rt.w)/2;
		else if(align==TWHTMLStyleTextAlignRight)
			offset=availableWidth-rt.w;
		if(offset<0)
			offset=0;
		rt.x+=offset;
		
		line->setRect(rt);
		
	}
}

void TWRenderBlock::layoutInlineChildren(TWRenderManager * manager){
	int beforeEdge=paddingTop()+borderSizeTop();
    int afterEdge=paddingBottom()+borderSizeBottom();
    //int lastMargin=0;
	
	assert(beforeEdge>=0);
	assert(beforeEdge<0x10000000);
    
	if(g_staticFlowInfoBeingUsed){
		m_flowInfo=new FlowInfo();
	}else{
		m_flowInfo=&g_staticFlowInfo;
		g_staticFlowInfoBeingUsed=true;
	}
	m_flowInfo->initialize();
	m_flowInfo->specifiedLineHeight=style().lineHeight().toPixels(0, style());
	
    m_rect.h=beforeEdge;
	
	clearLines();
	newLine(TWHTMLStyleFloatNone);
	
	for(TWNode *node=firstChild();node && node!=m_firstLine;node=node->nextSibling()){
		assert(node->parent()==this);
        TWRenderBox *childBox=static_cast<TWRenderBox *>(node);
        if(childBox->isFloating()){
			childBox->layout(manager);
			addFloatingObject(childBox);
			continue;
		}
		
		if(childBox->isListMarker()){
			// skip now.
			continue;
		}
		
		assert(childBox->isInline() || childBox->isReplaced());
		childBox->layout(manager);
	}
	
	
	flushFlowTags();
	newLine(TWHTMLStyleFloatNone);
	
	positionNewFloats();
	
	if(isTableCell() || isFloating()){
		// we cannot return float objects to parent.
		// we have to deal with them right now.
		clearFloats();
	}
	
	assert(m_flowInfo->flowTags.empty());
	assert(m_flowInfo->inlineStack.empty());
	assert(m_flowInfo->flowStack.empty());
	
	m_rect.h+=afterEdge+m_lastMaxY;
	
	if(m_flowInfo==&g_staticFlowInfo){
		g_staticFlowInfoBeingUsed=false;
	}else{
		delete m_flowInfo;
	}
	m_flowInfo=NULL;
	
	alignLines();
}

void TWRenderBlock::clearFloats(){
	if(m_floatingObjects.size()){
		int y=lowestFloatLogicalBottom(FloatingObjectTypeBoth);
		if(y>m_rect.h){
			m_rect.h=y;
			
			if(isChildrenInline()){
				if(firstLine()){
					TWRenderFlowLine *lastLine=m_lines.back();
					TWRect r=lastLine->rect();
					r.y=m_rect.h;
					r.x=logicalLeftOffsetForLine(r.y, lastLine==firstLine());
					lastLine->setRect(r);
				}
			}
		}
	}
    m_floatingObjects.clear();
}

int TWRenderBlock::availableLogicalWidth() const{
    // note that columns are not supported.
    return TWRenderBox::availableLogicalWidth();
}

int TWRenderBlock::logicalRightOffsetForLine(int position, int fixedOffset, bool applyTextIndent, int* logicalHeightRemaining, int lineHeight) const{
    
    int right=fixedOffset;
    
    if(logicalHeightRemaining)
        *logicalHeightRemaining=1;
    for(std::list<FloatingObject>::const_iterator it=m_floatingObjects.begin();
        it!=m_floatingObjects.end();it++){
        TWRect rt=it->rect;
        if(it->placed && position+lineHeight>rt.y && position<rt.y+rt.h){
            if(it->type==FloatingObjectTypeRight){
                int newHPos=rt.x-it->renderer->marginLeft();
                if(newHPos<right){
                    right=newHPos;
                    if(logicalHeightRemaining)
                        *logicalHeightRemaining=rt.y+rt.h-position;
                }
            }
        }
    }
    
    /*
    if(applyTextIndent){
        int cw;
        if(style().textIndent().unit==TWHTMLStyleUnitPercent)
            cw=containingBlock()->availableLogicalWidth();
        right+=style().textIndent().toPixels(cw, style());
    }*/
    return right;
}
int TWRenderBlock::logicalLeftOffsetForLine(int position, int fixedOffset, bool applyTextIndent, int* logicalHeightRemaining, int lineHeight) const{
    int left=fixedOffset;
    
    if(logicalHeightRemaining)
        *logicalHeightRemaining=1;
    for(std::list<FloatingObject>::const_iterator it=m_floatingObjects.begin();
        it!=m_floatingObjects.end();it++){
        TWRect rt=it->rect;
        if(it->placed && position+lineHeight>rt.y && position<rt.y+rt.h){
            if(it->type==FloatingObjectTypeLeft){
                int newHPos=rt.x+rt.w+it->renderer->marginRight();
                if(newHPos>left){
                    left=newHPos;
                    if(logicalHeightRemaining)
                        *logicalHeightRemaining=rt.y+rt.h-position;
                }
            }
        }
    }
    
    if(applyTextIndent){
        int cw;
        if(style().textIndent().unit==TWHTMLStyleUnitPercent)
            cw=containingBlock()->availableLogicalWidth();
        left+=style().textIndent().toPixels(cw, node()->style());
    }
    return left;
}

int TWRenderBlock::availableLogicalWidthForLine(int position, bool firstLine,
												int lineHeight) const{
    int result = logicalRightOffsetForLine(position, logicalRightOffsetForContent(), 
										   firstLine, NULL, lineHeight) - 
	logicalLeftOffsetForLine(position, logicalLeftOffsetForContent(),
							 firstLine, NULL, lineHeight);
    return (result < 0) ? 0 : result;
}

int TWRenderBlock::fittingLineDelta(int position, int lineHeight, 
									int lineWidth, bool firstLine) const{
	int logicalLeftFixedOffset=logicalLeftOffsetForContent();
	int logicalRightFixedOffset=logicalRightOffsetForContent();
	int logicalLeftFixedOffsetIndented=logicalLeftFixedOffset;
	int logicalRightFixedOffsetIndented=logicalRightFixedOffset;
	int delta=0;
	
	if(firstLine){
		int cw;
        if(style().textIndent().unit==TWHTMLStyleUnitPercent)
            cw=containingBlock()->availableLogicalWidth();
        logicalLeftFixedOffsetIndented+=style().textIndent().toPixels(cw, node()->style());
	}
	
	while(true){
		int lhRemaining1, lhRemaining2;
		int leftOffset=logicalLeftOffsetForLine(position, logicalLeftFixedOffset,
												firstLine, &lhRemaining1,
												lineHeight);
		int rightOffset=logicalRightOffsetForLine(position, logicalRightFixedOffset,
												firstLine, &lhRemaining2,
												lineHeight);
		
		int lhRemaining=-1;
		
		if(leftOffset>logicalLeftFixedOffsetIndented){
			lhRemaining=lhRemaining1;
		}
		
		if(rightOffset<logicalRightFixedOffsetIndented && 
		   (lhRemaining2<lhRemaining || lhRemaining==-1)){
			lhRemaining=lhRemaining2;
		}
		
		int availableWidth=rightOffset-leftOffset;
		if(availableWidth>=lineWidth){
			// enough wide.
			return delta;
		}
		
		if(lhRemaining<=0){
			// no float left.
			return delta;
		}
		
		delta+=lhRemaining;
		position+=delta;
		
	}
	assert(false);
	return 0;
}

int TWRenderBlock::logicalRightOffsetForContent() const{
    return borderSizeLeft()+paddingLeft()+availableLogicalWidth();
}
int TWRenderBlock::logicalLeftOffsetForContent() const{
    return borderSizeLeft()+paddingLeft();
}

void TWRenderBlock::addFloatingObject(TWRenderBox *box){
    FloatingObject obj;
    obj.renderer=box;
 
    TWHTMLStyleFloat floatStyle=box->style()._float();
    assert(floatStyle!=TWHTMLStyleFloatNone);
    
    obj.type=((floatStyle==TWHTMLStyleFloatLeft)?
              FloatingObjectTypeLeft:
              FloatingObjectTypeRight);
    obj.placed=false;
    obj.rect=box->rect();
    
    m_floatingObjects.push_back(obj);
}

void TWRenderBlock::removeFloatingObject(TWRenderBox *box){
    for(std::list<FloatingObject>::iterator it=m_floatingObjects.begin();
        it!=m_floatingObjects.end();it++){
        if(it->renderer==box){
            m_floatingObjects.erase(it);
            return;
        }
    }
    assert("FloatingObject being removed not found."==0);
}

void TWRenderBlock::positionNewFloats(){
    if(m_floatingObjects.empty())
        return;
    
    if(m_floatingObjects.back().placed)
        return;
    
    std::list<FloatingObject>::iterator it=m_floatingObjects.end();
    do{
        if(it==m_floatingObjects.begin())
            break;
        it--;
    }while(!it->placed);
    if(it->placed)
        it++;
    
    int top=rect().h;
    if(it!=m_floatingObjects.begin()){
        std::list<FloatingObject>::iterator lastFloat=it;
        lastFloat--;
        if(lastFloat->rect.y>top)
            top=lastFloat->rect.y;
    }
    
    while(it!=m_floatingObjects.end()){
        TWRenderBox *childBox=it->renderer;
        
        if(childBox->containingBlock()!=this){
            it++;
            continue;
        }
		
		FloatingObject& fobj=*it;
        
        int leftOffset=logicalLeftOffsetForContent();
        int rightOffset=logicalRightOffsetForContent();
        
        //childBox->calcWidth(); already calculated.
		fobj.rect=childBox->rect();
        int floatLogicalWidth=childBox->rect().w;
        
        if(rightOffset-leftOffset<floatLogicalWidth)
            floatLogicalWidth=rightOffset-leftOffset;
        
        TWRect oldRect=childBox->rect();
        
        if(childBox->style().clear()==TWHTMLStyleFloatLeft
           || childBox->style().clear()==TWHTMLStyleFloatBoth){
            int newTop=lowestFloatLogicalBottom(FloatingObjectTypeLeft);
            if(newTop>top)
                top=newTop;
			
			assert(logicalLeftOffsetForLine(top, leftOffset)==leftOffset);
        }
        
        if(childBox->style().clear()==TWHTMLStyleFloatRight
           || childBox->style().clear()==TWHTMLStyleFloatBoth){
            int newTop=lowestFloatLogicalBottom(FloatingObjectTypeRight);
            if(newTop>top)
                top=newTop;
			
			assert(logicalRightOffsetForLine(top, rightOffset)==rightOffset);
        }
        
        int floatLogicalLeft;
        if(fobj.type==FloatingObjectTypeLeft){
            int heightRemainingLeft=1;
            int heightRemainingRight=1;
            floatLogicalLeft=logicalLeftOffsetForLine(top, leftOffset,
                                                      false, &heightRemainingLeft);
            while(logicalRightOffsetForLine(top, rightOffset, false, &heightRemainingRight)
                  -floatLogicalLeft < floatLogicalWidth){
                if(heightRemainingLeft<heightRemainingRight)
                    top+=heightRemainingLeft;
                else
                    top+=heightRemainingRight;
                floatLogicalLeft=logicalLeftOffsetForLine(top, leftOffset,
                                                          false, &heightRemainingLeft);
            }
            
            if(floatLogicalLeft<0)
                floatLogicalLeft=0;
        }else{
            int heightRemainingLeft=1;
            int heightRemainingRight=1;
            floatLogicalLeft=logicalRightOffsetForLine(top, rightOffset,
                                                      false, &heightRemainingRight);
            while(floatLogicalLeft-
                  logicalLeftOffsetForLine(top, leftOffset, false, &heightRemainingLeft)
                   < floatLogicalWidth){
                if(heightRemainingLeft<heightRemainingRight)
                    top+=heightRemainingLeft;
                else
                    top+=heightRemainingRight;
                floatLogicalLeft=logicalRightOffsetForLine(top, rightOffset,
                                                           false, &heightRemainingRight);
            }
            
            floatLogicalLeft-=childBox->rect().w;
        }
		top+=childBox->marginTop();
		
        childBox->setRect(TWRect(floatLogicalLeft, top,
                                 childBox->rect().w,
                                 childBox->rect().h));
    
        
        fobj.rect=childBox->rect();
        fobj.placed=true;
		
		if(isChildrenInline()){
			// add floating object to flow line so that
			// we can count flow index.
			TWRenderObject *obj=static_cast<TWRenderObject *>(lastChild());
			if(obj->isRenderFlowLine()){
				TWRenderFlowLine *lastLine=static_cast<TWRenderFlowLine *>(obj);
				lastLine->addFloatingObject(fobj.renderer);
			}
		}
        
        it++;
    }
}

int TWRenderBlock::lowestFloatLogicalBottom(TWRenderBlock::FloatingObjectType type) const{
	int lowestFloatBottom=0;
	
    for(std::list<FloatingObject>::const_iterator it=m_floatingObjects.begin();
        it!=m_floatingObjects.end();it++){
        if(it->placed && ((it->type==type) || type==FloatingObjectTypeBoth)){
            int floatBottom=it->rect.y+it->rect.h+it->renderer->marginBottom();
            if(floatBottom>lowestFloatBottom)
                lowestFloatBottom=floatBottom;
        }
    }
    return lowestFloatBottom;
}

bool TWRenderBlock::handleFloatingChild(TWRenderBox *child, TWRenderManager *fontManager){
    if(child->isFloating()){
		child->layout(fontManager);
        addFloatingObject(child);
        positionNewFloats();
		
		return true;
    }
    return false;
}



int TWRenderBlock::clearDelta(TWRenderBox *child, int yPos) const{
    TWHTMLStyleFloat clearStyle=child->style().clear();
    int lowestFloatBottom=0;
    if(clearStyle==TWHTMLStyleFloatLeft)
        lowestFloatBottom=lowestFloatLogicalBottom(FloatingObjectTypeLeft);
    else if(clearStyle==TWHTMLStyleFloatRight)
        lowestFloatBottom=lowestFloatLogicalBottom(FloatingObjectTypeRight);
    else if(clearStyle==TWHTMLStyleFloatBoth)
        lowestFloatBottom=lowestFloatLogicalBottom(FloatingObjectTypeBoth);
	else if(clearStyle==TWHTMLStyleFloatNone); // done.
	else
		assert(false);
    
    lowestFloatBottom-=yPos;
    if(lowestFloatBottom<0)
        lowestFloatBottom=0;
    
    return lowestFloatBottom;
}

TWRenderBlock *TWRenderBlock::createAnonymousBlock() const{
    TWRenderBlock *block=new TWRenderBlock(NULL);
    return block;
}

void TWRenderBlock::makeChildrenNonInline(){
    if(isChildrenInline()==false)
        return;
    setChildrenInline(false);
    if(firstChild()==NULL)
        return;
    
    TWRenderBlock *block=createAnonymousBlock();
    TWNode *next=firstChild();
    
    while(next){
        TWNode *node=next;
        next=next->nextSibling();
        block->addChild(node);
    }
    
    TWRenderBox::addChild(block);
    block->calcStyle();
}

void TWRenderBlock::addChild(TWNode *node){
    TWRenderBox *newChild=static_cast<TWRenderBox *>(node);
	if(!newChild)
		return;
	
	assert(!newChild->isTableRow());
	
	if(newChild->isListMarker()){
		TWRenderBox::addChild(newChild);
		return;
	}
	
    if(isChildrenInline() && !(newChild->isInline() || newChild->isFloating() || newChild->isReplaced())){
        makeChildrenNonInline();
    }else if(!isChildrenInline() && (newChild->isInline() || newChild->isFloating() || newChild->isReplaced())){
        TWRenderObject *obj=static_cast<TWRenderObject *>(lastChild());
        if(obj->isAnonymous()){
            obj->addChild(newChild);
            return;
        }else{
            obj=createAnonymousBlock();
            obj->addChild(newChild);
            addChild(obj);
            return;
        }
    }
    
    
    assert(isChildrenInline()==(newChild->isInline() || newChild->isFloating() || newChild->isReplaced()));
    
    
    TWRenderBox::addChild(newChild);
}

int TWRenderBlock::maximumContentWidth(TWRenderManager *manager){
	if(m_maximumContentWidth!=-1)
		return m_maximumContentWidth;
	
	m_maximumContentWidth=0;
	
	if(style().width().unit!=TWHTMLStyleUnitAuto &&
	   style().width().unit!=TWHTMLStyleUnitPercent){
		
		m_maximumContentWidth=style().width().toPixels(0, style());
		
		return m_maximumContentWidth;
	}
	
	if(isChildrenInline() && firstChild()){
		m_currentMaximumContentWidth=0;
		for(TWNode *node=firstChild();node && node!=m_firstLine;node=node->nextSibling()){
			assert(node->parent()==this);
			TWRenderObject *childBox=static_cast<TWRenderObject *>(node);
			if(!childBox->isFloating() && !childBox->isRenderFlowObject()){
				if(childBox->isReplaced()){
					TWRenderReplaced *o=static_cast<TWRenderReplaced *>(childBox);
					o->handleMaximumContentWidth(manager);
				}else if(childBox->isInline()){
					TWRenderInline *o=static_cast<TWRenderInline *>(childBox);
					o->handleMaximumContentWidth(manager);
				}
			}else if(childBox->isFloating() && childBox->isBlock()){
				TWRenderBlock *o=static_cast<TWRenderBlock *>(node);
				int w=o->maximumContentWidth(manager);
				
				w+=o->marginLeft();
				w+=o->marginRight();
				w+=o->paddingLeft();
				w+=o->paddingRight();
				w+=o->borderSizeLeft();
				w+=o->borderSizeRight();
				
				if(w>m_maximumContentWidth)
					m_maximumContentWidth=w;
			}
			
		}
	}else{
		for(TWNode *node=firstChild();node && node!=m_firstLine;node=node->nextSibling()){
			assert(node->parent()==this);
			TWRenderBlock *childBox=static_cast<TWRenderBlock *>(node);
			int w=childBox->maximumContentWidth(manager);
			
			w+=childBox->marginLeft();
			w+=childBox->marginRight();
			w+=childBox->paddingLeft();
			w+=childBox->paddingRight();
			w+=childBox->borderSizeLeft();
			w+=childBox->borderSizeRight();
			
			if(w>m_maximumContentWidth)
				m_maximumContentWidth=w;
			
		}
	}
	
	return m_maximumContentWidth;
}

void TWRenderBlock::maximumContentWidthNewLine(){
	m_currentMaximumContentWidth=0;
}

void TWRenderBlock::addMaximumContentWidth(int w){
	m_currentMaximumContentWidth+=w;
	if(m_currentMaximumContentWidth>m_maximumContentWidth)
		m_maximumContentWidth=m_currentMaximumContentWidth;
}

int TWRenderBlock::minimumContentWidth(TWRenderManager *manager){
	if(m_minimumContentWidth!=-1)
		return m_minimumContentWidth;
	
	m_minimumContentWidth=0;
	
	if(isChildrenInline() && firstChild()){
		if(style().width().unit!=TWHTMLStyleUnitAuto &&
		   style().width().unit!=TWHTMLStyleUnitPercent){
			
			m_minimumContentWidth=style().width().toPixels(0, style());
		}
		
		for(TWNode *node=firstChild();node && node!=m_firstLine;node=node->nextSibling()){
			assert(node->parent()==this);
			TWRenderObject *childBox=static_cast<TWRenderObject *>(node);
			if(!childBox->isFloating() && !childBox->isRenderFlowObject()){
				if(childBox->isReplaced()){
					TWRenderReplaced *o=static_cast<TWRenderReplaced *>(childBox);
					o->handleMinimumContentWidth(manager);
				}else if(childBox->isInline()){
					TWRenderInline *o=static_cast<TWRenderInline *>(childBox);
					o->handleMinimumContentWidth(manager);
				}
			}
			
		}
	}else{
		if(style().width().unit!=TWHTMLStyleUnitAuto &&
		   style().width().unit!=TWHTMLStyleUnitPercent){
			
			m_minimumContentWidth=style().width().toPixels(0, style());
		}
		
		for(TWNode *node=firstChild();node && node!=m_firstLine;node=node->nextSibling()){
			assert(node->parent()==this);
			TWRenderBlock *childBox=static_cast<TWRenderBlock *>(node);
			int w=childBox->minimumContentWidth(manager);
			
			w+=childBox->marginLeft();
			w+=childBox->marginRight();
			w+=childBox->paddingLeft();
			w+=childBox->paddingRight();
			w+=childBox->borderSizeLeft();
			w+=childBox->borderSizeRight();
			
			if(w>m_minimumContentWidth)
				m_minimumContentWidth=w;
			
		}
	}
	
	return m_minimumContentWidth;

}

void TWRenderBlock::addMinimumContentWidth(int w){
	if(w>m_minimumContentWidth)
		m_minimumContentWidth=w;
}

void TWRenderBlock::calcChildrenFlowIndex(){
	unsigned int index=flowIndexStart();
	
	if(!firstChild()){
		// empty block.
		setFlowIndexEnd(index);
		return;
	}
	if(isChildrenInline() && (!isTable()) && (!isTableRow())){
		assert(m_firstLine);
		for(TWNode *child=m_firstLine;child;child=child->nextSibling()){
			TWRenderFlowLine *line=static_cast<TWRenderFlowLine *>(child);
			assert(line->isRenderFlowLine());
			line->setFlowIndexStart(index);
			line->calcChildrenFlowIndex();
			index=line->flowIndexEnd();
		}
	}else{
		for(TWNode *child=firstChild();child;child=child->nextSibling()){
			TWRenderBlock *line=static_cast<TWRenderBlock *>(child);
			assert(line->isBlock());
			if(line->isListMarker())
				continue;
			line->setFlowIndexStart(index);
			line->calcChildrenFlowIndex();
			index=line->flowIndexEnd();
		}
	}
	setFlowIndexEnd(index);
}

unsigned int TWRenderBlock::flowIndexAtPoint(const TWPoint & pt) const{
	if(isChildrenInline() && (!isTable()) && (!isTableRow()) && (firstChild())){
		// check floating objects.
		assert(m_firstLine);
		for(TWNode *child=m_firstLine;child;child=child->nextSibling()){
			TWRenderFlowLine *line=static_cast<TWRenderFlowLine *>(child);
			const std::list<TWRenderObject *>& floats=line->floats();
			for(std::list<TWRenderObject *>::const_iterator it=floats.begin();it!=floats.end();it++){
				TWRenderBlock *floatBlock=static_cast<TWRenderBlock *>(*it);
				if(floatBlock->boundaryRect() && pt){
					unsigned int i=floatBlock->flowIndexAtPoint(pt);
					if(i!=(unsigned int)-1)
						return i;
				}
			}
		}
	
		// if this contains floating block, given point
		// may go lower than lowerest line.
		if(!(screenRect().sanitize()&&pt))
			return (unsigned int)-1;
	
		// top/bottom overflow check.
		{
			// higher than first line?
			TWRenderFlowLine *line=m_firstLine;
			TWRect rt=line->screenRect().sanitize();
			if(pt.y<rt.y){
				return flowIndexStart();
			}
		}
		
		{
			// lower than last line?
			TWRenderFlowLine *line=static_cast<TWRenderFlowLine *>(lastChild());
			TWRect rt=line->screenRect().sanitize();
			if(pt.y>=rt.y+rt.h){
				return flowIndexEnd();
			}
		}
		
		// now go through lines.
		for(TWNode *child=m_firstLine;child;child=child->nextSibling()){
			TWRenderFlowLine *line=static_cast<TWRenderFlowLine *>(child);
			TWRect rt=line->screenRect().sanitize();
			if(rt.y+rt.h>pt.y){
				return line->flowIndexAtPoint(pt);
			}
		}
		
		// oops?
		return flowIndexEnd();
	}else{
		
		
		if(isFloating())
			if(!(screenRect().sanitize()&&pt))
				return (unsigned int)-1;
		
		if(!firstChild()){
			// empty.
			return flowIndexStart();
		}
		
		// note: there must be at least one non-floating block child.
		// because it there is no one, this whould be "inlined-children-block".
		
		// top/bottom overflow check.
		if(screenRect().sanitize()&&pt){
			// higher than first block?
			TWRenderBlock *block;
			
			// skip floating objects.
			for(block=static_cast<TWRenderBlock *>(firstChild());
				block->isFloating()||block->isListMarker();
				block=static_cast<TWRenderBlock *>(block->nextSibling()));
			
			TWRect rt=block->screenRect().sanitize();
			if(pt.y<rt.y){
				return flowIndexStart();
			}
		}
		
		
		
		// now go through blocks, and floating ones.
		for(TWNode *child=firstChild();child;child=child->nextSibling()){
			TWRenderBlock *line=static_cast<TWRenderBlock *>(child);
			if(line->isListMarker())
				continue;
			
			TWRect rt=line->boundaryRect().sanitize();
			if(rt&&pt){
				unsigned int i=line->flowIndexAtPoint(pt);
				if(i!=(unsigned int)-1)
					return i;
			}
		}
		
		if(screenRect().sanitize()&&pt){
		
			// must be between blocks...
			for(TWNode *child=firstChild();child;child=child->nextSibling()){
				TWRenderBlock *line=static_cast<TWRenderBlock *>(child);
				if(line->isFloating())
					continue;
				if(line->isListMarker())
					continue;
				
				TWRect rt=line->screenRect().sanitize();
				if(rt.y+rt.h>pt.y){
					return line->flowIndexStart();
				}
			}
		
		
			// lower than last block?
			
			return flowIndexEnd();
			
		}
		
		// oops?
		return (unsigned int)-1;
	}
}

void TWRenderBlock::captureSelectionString
(std::wstring &out, int begin, int end){
	TWRenderBoxModelObject *first;
	
	if(!firstChild()){
		// empty block.
		return;
	}
	
	if(isChildrenInline()){
		first=m_firstLine;
	}else
		first=static_cast<TWRenderBoxModelObject *>(firstChild());
	
	TWRect filledRect(0,0,0,0);
	
	// outta range?
	
	if(end<=flowIndexStart())
		return ;
	if(begin>=flowIndexEnd())
		return;
	
	for(TWNode *child=first;child;
		child=child->nextSibling()){
		if(isChildrenInline()){
			TWRenderFlowLine *line=static_cast<TWRenderFlowLine *>(child);
			line->captureSelectionString(out, begin, end);
		}else{
			TWRenderBlock *block=static_cast<TWRenderBlock *>(child);
			block->captureSelectionString(out, begin, end);
		}
	}
}

TWRect TWRenderBlock::drawSelection
(TWRenderDC *dc, int begin, int end){
	TWRenderBoxModelObject *first;
	
	// don't fill block elements.
	// fill flow line, or flow objects.
	
	if(!firstChild()){
		// empty block.
		return TWRect(0,0,0,0);
	}
	
	if(isChildrenInline()){
		first=m_firstLine;
	}else
		first=static_cast<TWRenderBoxModelObject *>(firstChild());

	TWRect filledRect(0,0,0,0);
	
	// outta range?
	
	if(end<=flowIndexStart())
		return filledRect;
	if(begin>=flowIndexEnd())
		return filledRect;
	
	for(TWNode *child=first;child;
		child=child->nextSibling()){
		if(isChildrenInline()){
			TWRenderFlowLine *line=static_cast<TWRenderFlowLine *>(child);
			TWRect r=line->drawSelection(dc, begin, end);
			if(r.w && r.h){
				if(filledRect.w==0 || filledRect.h==0){
					filledRect=r;
				}else{
					filledRect=filledRect|r;
				}
			}
		}else{
			TWRenderBlock *block=static_cast<TWRenderBlock *>(child);
			TWRect r=block->drawSelection(dc, begin, end);
			if(r.w && r.h){
				if(filledRect.w==0 || filledRect.h==0){
					filledRect=r;
				}else{
					filledRect=filledRect|r;
				}
			}
		}
	}
	return filledRect;
}


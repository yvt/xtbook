//
//  TWRenderBox.cpp
//  XTBook
//
//  Created by Nexhawks on 3/8/11.
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

#include "TWRenderBox.h"
#include "TWHTMLElement.h"
#include "TWHTMLStyle.h"
#include "TWRenderBlock.h"
#include "TWRenderDC.h"
#include "TWRenderReplaced.h"

TWRenderBox::TWRenderBox(TWHTMLElement *node):
TWRenderBoxModelObject(node, node?(node->name()):std::wstring()){

}


void TWRenderBox::calcWidth(){
	//bool stretching=true;
	
	// table row's width is set by its table.
	if(isTableRow())
		return;
	
	// table cell's width is set by its row.
	if(isTableCell())
		return;
    
	bool treatAsReplaced=shouldComputeSizeAsReplaced();
    TWHTMLStyleValue widthValue=treatAsReplaced ? 
        TWHTMLStyleValue(TWHTMLStyleUnitPixels, 
                         computeReplacedLogicalWidth()) :
    style().width();

    //TWRenderBlock *cb=containingBlock();
    int containerWidth=containingBlockLogicalWidthForContent();
    if(containerWidth<0)
        containerWidth=0;
	
	
    
    if(isInline()){
        if(treatAsReplaced){
            m_rect.w=widthValue.toPixels(containerWidth,style());
            m_rect.w+=borderSizeLeft();
            m_rect.w+=borderSizeRight();
            m_rect.w+=paddingLeft();
            m_rect.w+=paddingRight();
            // TODO: setPreferredLogicalWidthsDirty in account
        }
        return;
    }
    
    if(treatAsReplaced){
        m_rect.w=widthValue.toPixels(containerWidth,style());
        //m_rect.w+=borderSizeLeft();
        //m_rect.w+=borderSizeRight();
       // m_rect.w+=paddingLeft();
       // m_rect.w+=paddingRight();
    }else{
        TWHTMLStyleValue width=style().width();
        int marginStart=marginLeft();
        int marginEnd=marginRight();
        m_rect.w=containerWidth-marginStart-marginEnd;
		m_rect.w-=borderSizeLeft();
        m_rect.w-=borderSizeRight();
		m_rect.w-=paddingLeft();
		m_rect.w-=paddingRight();
        m_rect.w=width.toPixels(m_rect.w, style());
		m_rect.w+=borderSizeLeft();
        m_rect.w+=borderSizeRight();
		 m_rect.w+=paddingLeft();
		 m_rect.w+=paddingRight();
    }
    
}

bool TWRenderBox::shouldComputeSizeAsReplaced(){
    return isReplaced();
}

int TWRenderBox::computeReplacedLogicalWidth(){
    int cw; // TODO: Complete implementation
    if(style().textIndent().unit==TWHTMLStyleUnitPercent)
        cw=containingBlock()->availableLogicalWidth();
	else if(style().textIndent().unit==TWHTMLStyleUnitAuto || true)
		cw=static_cast<TWRenderReplaced *>(this)->replacedDefaultWidth();
    return style().width().toPixels(cw, style());
}

bool TWRenderBox::shrinkToAvoidFloats() const{
    if(isInline() || !avoidsFloat() || isFloating())
        return false;
    return style().width().unit==TWHTMLStyleUnitAuto;
}

bool TWRenderBox::avoidsFloat() const{
	// HAHAHA!
	if(node() && node()->type()==TWHTMLElementTypeH2)
		return true;
	
	// YEAH!
	if(node() && node()->type()==TWHTMLElementTypePre)
		return true;
	
    return isReplaced();
}

int TWRenderBox::containingBlockLogicalWidthForContent() const{
    TWRenderBlock *cb=containingBlock();
    if(shrinkToAvoidFloats())
        return cb->availableLogicalWidthForLine(m_rect.y, false);
    return cb->availableLogicalWidth();
}

int TWRenderBox::availableLogicalWidth() const{
    return contentWidth();
}

int TWRenderBox::contentWidth() const{
    return clientWidth()-paddingLeft()-paddingRight();
}

int TWRenderBox::contentHeight() const{
    return clientHeight()-paddingTop()-paddingBottom();
}

int TWRenderBox::clientWidth() const{
    return m_rect.w-borderSizeLeft()-borderSizeRight();
}

int TWRenderBox::clientHeight() const{
    return m_rect.h-borderSizeTop()-borderSizeBottom();
}

void TWRenderBox::render(TWRenderDC *dc){
	if(!dc->isRectVisible(m_boundaryRect))
		return;
    dc->setStyle(style());
    dc->fillBackground(m_screenRect);
	if(!isTableCell() && !isTableRow() && !isTable())
	dc->drawBorder(m_screenRect);
	dc->debugRenderObject(m_screenRect, this);
	
    TWRenderObject::render(dc);
}



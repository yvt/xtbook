//
//  TWRenderTableCell.cpp
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

#include "TWRenderTableCell.h"
#include "TWHTMLElement.h"
#include "TWUtils.h"
#include <assert.h>
#include "TWRenderInline.h"
#include "TWRenderFlowLine.h"

const std::wstring g_rowSpanAttrName=L"rowspan";
const std::wstring g_colSpanAttrName=L"colspan";


TWRenderTableCell::TWRenderTableCell(TWHTMLElement *element):
TWRenderBlock(element){
    m_isTableCell=true;
	if(!element){
		m_rowSpan=1;
		m_colSpan=1;
	}else{
		m_rowSpan=TWUtils::intValueFromString(element->getAttribute(g_rowSpanAttrName))?:1;
		m_colSpan=TWUtils::intValueFromString(element->getAttribute(g_colSpanAttrName))?:1;
		
		
	}
	m_colIndex=-1;
	m_rowIndex=-1;
	m_table=NULL;
	
	// set border size in case there is no table.
	if(element)
		computeBoxModelBorder();
	
}

int TWRenderTableCell::autoWidth(TWRenderManager * manager) const{
	return const_cast<TWRenderTableCell *>(this)->maximumContentWidth(manager)
	+paddingLeft()+paddingRight()+borderSizeLeft()+borderSizeRight();
}

int TWRenderTableCell::minWidth(TWRenderManager * manager) const{
	return const_cast<TWRenderTableCell *>(this)->minimumContentWidth(manager)
	+paddingLeft()+paddingRight()+borderSizeLeft()+borderSizeRight();
}

void TWRenderTableCell::computeBoxModelBorder(){
	if(m_table){
		int maxWidth;
		
		// left
		maxWidth=0;
		for(int row=0;row<m_rowSpan;row++){
			TWRenderTable::BorderInfo bi;
			bi=m_table->leftBorderAt(m_rowIndex+row, m_colIndex);
			int width=(bi.width+1)>>1;
			if(width>maxWidth)
				maxWidth=width;
		}
		m_borderLeft=maxWidth;
		
		// right
		maxWidth=0;
		for(int row=0;row<m_rowSpan;row++){
			TWRenderTable::BorderInfo bi;
			bi=m_table->leftBorderAt(m_rowIndex+row, m_colIndex+m_colSpan);
			int width=(bi.width)>>1;
			if(width>maxWidth)
				maxWidth=width;
		}
		m_borderRight=maxWidth;
		
		// top
		maxWidth=0;
		for(int col=0;col<m_colSpan;col++){
			TWRenderTable::BorderInfo bi;
			bi=m_table->topBorderAt(m_rowIndex, m_colIndex+col);
			int width=(bi.width+1)>>1;
			if(width>maxWidth)
				maxWidth=width;
		}
		m_borderTop=maxWidth;
		
		// bottom
		maxWidth=0;
		for(int col=0;col<m_colSpan;col++){
			TWRenderTable::BorderInfo bi;
			bi=m_table->topBorderAt(m_rowIndex+m_rowSpan, m_colIndex+col);
			int width=(bi.width+1)>>1;
			if(width>maxWidth)
				maxWidth=width;
		}
		m_borderBottom=maxWidth;
	}else{
		m_borderLeft=TWRenderBlock::borderSizeLeft();
		m_borderRight=TWRenderBlock::borderSizeRight();
		m_borderTop=TWRenderBlock::borderSizeTop();
		m_borderBottom=TWRenderBlock::borderSizeBottom();
	}
}

void TWRenderTableCell::alignContentsVertical(){
	TWNode *firstBlock;
	if(isChildrenInline())
		firstBlock=firstLine();
	else
		firstBlock=firstChild();
	if(!firstBlock){
		// no contents.
		return;
	}
	// TODO: support vertical-align.
}


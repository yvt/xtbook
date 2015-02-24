//
//  TWRenderTableRow.cpp
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

#include "TWRenderTableRow.h"
#include "TWRenderTableCell.h"
#include<assert.h>

TWRenderTableRow::TWRenderTableRow(TWHTMLElement *element, TWRenderTable *tbl):
TWRenderBox(element){
    m_isTableRow=true;
	m_table=tbl;
	m_rowIndex=-1;
}

void TWRenderTableRow::layout(TWRenderManager *manager){
	assert(m_table!=NULL);

	
	// don't neeed to calcWidth because
	// it's set by table.

	int maxHeight=0;
	
#if 0
	for(TWNode *child=firstChild();child;
		child=child->nextSibling()){
		TWRenderObject *obj=static_cast<TWRenderObject *>(child);
		// should be(ry
		if(obj->isTableCell()){
			TWRenderTableCell *cell=static_cast<TWRenderTableCell *>(obj);
			cell->setRect(rectForCol(cell->colIndex(), cell->colSpan()));
			cell->layout(manager);
			
			int height=cell->rect().h;
			if(cell->rowSpan()>1){
				if(cell->rowIndex()+cell->rowSpan()-1!=m_rowIndex)
					continue;
				for(int row=cell->rowIndex();row<m_rowIndex;row++)
					height-=m_table->m_rows[row]->rect().h;
			}
			if(height>maxHeight)
				maxHeight=cell->rect().h;
		}
	}
	
	// uniform height.
	for(TWNode *child=firstChild();child;
		child=child->nextSibling()){
		TWRenderObject *obj=static_cast<TWRenderObject *>(child);
		// should be(ry
		if(obj->isTableCell()){
			TWRenderTableCell *cell=static_cast<TWRenderTableCell *>(obj);
			TWRect r=cell->rect();
			r.h=maxHeight;
			cell->setRect(r);
			cell->alignContentsVertical();
		}
	}
#else
	
	for(int col=0;col<m_table->m_colCount;col++){
		TWRenderTableCell *cell=m_table->cellAt(m_rowIndex,
												col);
		if(cell){
			if(cell->colIndex()!=col)
				continue;
			if(cell->rowIndex()==m_rowIndex){
				cell->setRect(rectForCol(cell->colIndex(), cell->colSpan()));
				cell->layout(manager);
			}
			
			int height=cell->rect().h;
			if(cell->rowSpan()>1){
				if(cell->rowIndex()+cell->rowSpan()-1!=m_rowIndex)
					continue;
				for(int row=cell->rowIndex();row<m_rowIndex;row++)
					height-=m_table->m_rows[row]->rect().h;
			}
			if(height>maxHeight)
				maxHeight=cell->rect().h;
		}
	}
	
	// uniform height.
	for(int col=0;col<m_table->m_colCount;col++){
		TWRenderTableCell *cell=m_table->cellAt(m_rowIndex,
												col);
		if(cell){
			if(cell->colIndex()!=col)
				continue;
			if(cell->rowIndex()+cell->rowSpan()-1==m_rowIndex){
				TWRect r=cell->rect();
				r.h=maxHeight;
				
				if(cell->rowSpan()>1){
					for(int row=cell->rowIndex();row<m_rowIndex;row++){
						r.h+=m_table->m_rows[row]->rect().h;
					}
				}
				
				cell->setRect(r);
				cell->alignContentsVertical();
			}
		}
	}
	
#endif
	
	m_rect.h=maxHeight;
	
	m_isLayouted=true;
}

TWRect TWRenderTableRow::rectForCol(int colIndex, int colSpan) const{
	TWRect rt;
	int pos1, pos2;
	pos1=m_table->m_colsPos[colIndex];
	pos2=m_table->m_colsPos[colIndex+colSpan];
	
	rt.x=pos1;
	rt.y=0;
	rt.w=pos2-pos1;
	rt.h=0;
	
	return rt;
}

void TWRenderTableRow::calcChildrenFlowIndex(){
	unsigned int index=flowIndexStart();
	for(TWNode *node=firstChild();node;
		node=node->nextSibling()){
		if(!static_cast<TWRenderObject *>(node)->isBlock())
			continue;
		TWRenderBlock *cell=static_cast<TWRenderBlock *>(node);
		if(!cell->isTableCell())
			continue;
		cell->setFlowIndexStart(index);
		cell->calcChildrenFlowIndex();
		index=cell->flowIndexEnd();
	}
	setFlowIndexEnd(index);
}

unsigned int TWRenderTableRow::flowIndexAtPoint(const TWPoint &pt) const{
	for(TWNode *node=firstChild();node;
		node=node->nextSibling()){
		if(!static_cast<TWRenderObject *>(node)->isBlock())
			continue;
		TWRenderBlock *cell=static_cast<TWRenderBlock *>(node);
		if(!cell->isTableCell())
			continue;
		if(cell->screenRect() && pt){
			return cell->flowIndexAtPoint(pt);
		}
	}
	return (unsigned int)-1;
}

void TWRenderTableRow::captureSelectionString
(std::wstring &out, int begin, int end){
	if(end<=flowIndexStart() || begin>=flowIndexEnd())
		return;
	for(TWNode *node=firstChild();node;
		node=node->nextSibling()){
		if(!static_cast<TWRenderObject *>(node)->isBlock())
			continue;
		TWRenderBlock *cell=static_cast<TWRenderBlock *>(node);
		if(!cell->isTableCell())
			continue;
		cell->captureSelectionString(out, begin, end);
	}
}

TWRect TWRenderTableRow::drawSelection
(TWRenderDC *dc, int begin, int end){
	TWRect selRect(0,0,0,0);
	if(end<=flowIndexStart() || begin>=flowIndexEnd())
		return selRect;
	for(TWNode *node=firstChild();node;
		node=node->nextSibling()){
		if(!static_cast<TWRenderObject *>(node)->isBlock())
			continue;
		TWRenderBlock *cell=static_cast<TWRenderBlock *>(node);
		if(!cell->isTableCell())
			continue;
		TWRect rt;
		rt=cell->drawSelection(dc, begin, end);
		if(rt.w){
			if(selRect.w)
				selRect=selRect|rt;
			else
				selRect=rt;
		}
	}
	return selRect;
}

void TWRenderTableRow::addChild(TWNode *node){
	TWRenderObject *obj=static_cast<TWRenderObject *>(node);
	if(obj->isTableCell()){
		assert(dynamic_cast<TWRenderTableCell *>(node));
		TWRenderBox::addChild(node);
	}else if(obj->isTableRow()){
		assert(dynamic_cast<TWRenderTableRow *>(node));
		TWRenderBox::addChild(node);
	}else{
		// create anonymous cell;
		TWRenderObject *last=static_cast<TWRenderObject *>(lastChild());
		
		if(last==NULL || !last->isAnonymous()){
			TWRenderTableCell *newCell=new TWRenderTableCell(NULL);
			addChild(newCell);
		}
		
		TWRenderTableCell *cell=static_cast<TWRenderTableCell *>(lastChild());
		assert(cell->isTableCell());
		assert(cell->isAnonymous());
		
		cell->addChild(node);
	}
}


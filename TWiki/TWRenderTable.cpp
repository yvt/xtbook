//
//  TWRenderTable.cpp
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

#include "TWRenderTable.h"
#include "TWRenderTableRow.h"
#include "TWRenderTableCell.h"
#include <assert.h>
#include "TWRenderDC.h"

TWRenderTable::TWRenderTable(TWHTMLElement *element):
TWRenderBlock(element),
m_colCount(-1),
m_rowCount(-1),
m_tablized(false),
m_borderLeft(-1),
m_borderTop(-1),
m_borderRight(-1),
m_borderBottom(-1){
    m_isTable=true;
	m_maximumContentWidth=-1;
	m_minimumContentWidth=-1;
}

/** look for all rows. */
void TWRenderTable::scanRows(TWRenderObject *parent){
	if(!m_rows.empty())
		return;
	for(TWNode *child=parent->firstChild();child;child=child->nextSibling()){
		TWRenderObject *obj=static_cast<TWRenderObject *>(child);
		if(!obj->isAnonymous() && 
		   (obj->style().display()==TWHTMLStyleDisplayTableRowGroup ||
			obj->style().display()==TWHTMLStyleDisplayTableHeaderGroup ||
			obj->style().display()==TWHTMLStyleDisplayTableFooterGroup)){
			scanRows(obj);
		}else if(obj->isTableRow()){
			assert(dynamic_cast<TWRenderTableRow *>(obj));
			static_cast<TWRenderTableRow *>(obj)->setRowIndex((int)m_rows.size());
			static_cast<TWRenderTableRow *>(obj)->setTable(this);
			m_rows.push_back(static_cast<TWRenderTableRow *>(obj));
		}
	}
	
	m_rowCount=(int)m_rows.size();
}

/** determine where each cells are. */
void TWRenderTable::tablize(){
	if(m_tablized)
		return;
	std::vector<int> cols;
	
	for(size_t i=0;i<m_rows.size();i++){
		for(size_t j=0;j<cols.size();j++){
			if(cols[j])
				cols[j]--;
		}
		
		TWRenderTableRow *row=m_rows[i];
		int colIndex=0;
		
		for(TWNode *child=row->firstChild();child;child=child->nextSibling()){
			TWRenderObject *obj=static_cast<TWRenderObject *>(child);
			// this should be cell.
			assert(obj->isTableCell());
			if(obj->isTableCell()){ 
				assert(dynamic_cast<TWRenderTableCell *>(obj));
				TWRenderTableCell *cell=static_cast<TWRenderTableCell *>(obj);
				
				cell->setTable(this);
				
				while(colIndex>=cols.size())
					cols.push_back(0);
				
				// skip filled column.
				while(cols[colIndex]!=0){
					colIndex++;
					if(colIndex>=cols.size())
						cols.push_back(0); // new column.
				}
				
				int colSpan=cell->colSpan();
				int rowSpan=cell->rowSpan();
				
				// expand columns so that cell fits.
				while(colIndex+colSpan>(int)cols.size())
					cols.push_back(0);
				
				// set row/col index.
				cell->setColIndex(colIndex);
				cell->setRowIndex(row->rowIndex());
				
				if(i+rowSpan>m_rowCount){
					// shrink row span...
					cell->m_rowSpan=m_rowCount-i;
				}
				
				// fill cols.
				for(int k=0;k<colSpan;k++){
					cols[colIndex++]=rowSpan;
				}
				
			}
		}
		
	}
	
	m_colCount=(int)cols.size();
	
	// make map.
	
	for(size_t rowIndex=0;rowIndex<m_rows.size();rowIndex++){
		TWRenderTableRow *row=m_rows[rowIndex];
		for(int i=0;i<m_colCount;i++)
			row->m_cells.push_back(NULL);
	}
	
	for(size_t rowIndex=0;rowIndex<m_rows.size();rowIndex++){
		TWRenderTableRow *row=m_rows[rowIndex];
		for(TWNode *child=row->firstChild();child;child=child->nextSibling()){
		
			TWRenderObject *obj=static_cast<TWRenderObject *>(child);
			// this should be cell.
			if(obj->isTableCell()){ 
			assert(dynamic_cast<TWRenderTableCell *>(obj));
				TWRenderTableCell *cell=static_cast<TWRenderTableCell *>(obj);
				
				int colIndex, rowIndex;
				int colSpan, rowSpan;
				
				colIndex=cell->colIndex();
				rowIndex=cell->rowIndex();
				colSpan=cell->colSpan();
				rowSpan=cell->rowSpan();
				
				assert(colIndex!=-1);
				assert(rowIndex!=-1);
				
				colSpan+=colIndex;
				rowSpan+=rowIndex;
				
				for(int r=rowIndex;r<rowSpan;r++){
					assert(r>=0);
					assert(r<m_rowCount);
					
					TWRenderTableRow *row2=m_rows[r];
					
					for(int c=colIndex;c<colSpan;c++){
						row2->m_cells[c]=cell;
					}
				}
					
			}
			
		}
	}
	
	m_tablized=true;
}

/** check for each cell's style, determining columns' width. */
void TWRenderTable::determineColsWidth(TWRenderManager *manager){
	if(!m_colsWidth.empty())
		return;
	
	// create cols.
	for(int i=0;i<m_colCount;i++){
		m_colsWidth.push_back(TWHTMLStyleValueAuto);
		m_colsAutoWidth.push_back(8);
	}
	
	// for each rows...
	assert((int)m_rows.size()==m_rowCount);
	for(size_t rowIndex=0;rowIndex<m_rows.size();rowIndex++){
		TWRenderTableRow *row=m_rows[rowIndex];
		for(TWNode *child=row->firstChild();child;child=child->nextSibling()){
			TWRenderObject *obj=static_cast<TWRenderObject *>(child);
			// this should be cell.
			if(obj->isTableCell()){ 
				assert(dynamic_cast<TWRenderTableCell *>(obj));
				TWRenderTableCell *cell=static_cast<TWRenderTableCell *>(obj);
				
				cell->computeBoxModelBorder();
				
				int autoWidth;
				autoWidth=cell->autoWidth(manager);
				autoWidth+=cell->borderSizeLeft();
				autoWidth+=cell->borderSizeRight();
				autoWidth+=cell->paddingLeft();
				autoWidth+=cell->paddingRight();
				
				int colIndex=cell->colIndex();
				int colSpan=cell->colSpan();
				
				for(int i=colIndex;i<colIndex+colSpan;i++){
					if(colSpan==1)
						m_colsAutoWidth[i]=std::max(m_colsAutoWidth[i], autoWidth);
					else
						m_colsAutoWidth[i]=std::max(m_colsAutoWidth[i], autoWidth/colSpan);
				}
					
				
				if(colSpan>1){
					// a cell spanning multiple column is ignored.
					continue;
				}
				
				TWHTMLStyleValue value;
				value=cell->style().width();
				
				if(value.unit!=TWHTMLStyleUnitAuto){
					m_colsWidth[colIndex]=value;
				}
		
				
			
			}
		}
	}
}

void TWRenderTable::computeColsWidth(TWRenderManager *manager){
	TWHTMLStyleValue tableWidth=style().width();
	
	// create cols.
	m_computedColsWidth.clear();
	for(int i=0;i<m_colCount;i++){
		m_computedColsWidth.push_back(0);
	}
	
	if(tableWidth.unit==TWHTMLStyleUnitAuto){
		
		// calculate remaining percents.
		
		int remainingPercents=25600; // s24.8, = 100.00
		for(int i=0;i<m_colCount;i++){
			const TWHTMLStyleValue& colWidth=m_colsWidth[i];
			if(colWidth.unit==TWHTMLStyleUnitPercent)
				remainingPercents-=colWidth.fixedValue;
		}
		
		// calculate non-percent columns.
		
		int otherWidth=0;
		for(int i=0;i<m_colCount;i++){
			const TWHTMLStyleValue& colWidth=m_colsWidth[i];
			if(colWidth.unit!=TWHTMLStyleUnitPercent){
				if(colWidth.unit==TWHTMLStyleUnitAuto){
					m_computedColsWidth[i]=m_colsAutoWidth[i];
				}else{
					m_computedColsWidth[i]=colWidth.toPixels(0, style());
					// TODO: use cell's style.
				}
				otherWidth+=m_computedColsWidth[i];
			}
		}
		
		if(remainingPercents<1){
			// impossible to compute.
			// use default width(auto.)
			for(int i=0;i<m_colCount;i++){
				const TWHTMLStyleValue& colWidth=m_colsWidth[i];
				if(colWidth.unit==TWHTMLStyleUnitPercent){
					m_computedColsWidth[i]=m_colsAutoWidth[i];
				}
			}
		}else{
			for(int i=0;i<m_colCount;i++){
				const TWHTMLStyleValue& colWidth=m_colsWidth[i];
				if(colWidth.unit==TWHTMLStyleUnitPercent){
					m_computedColsWidth[i]=colWidth.toPixels(m_rect.w, style());
				}
			}
		}
		
		// and spaned cells...
		// for each rows...
		for(size_t rowIndex=0;rowIndex<m_rows.size();rowIndex++){
			TWRenderTableRow *row=m_rows[rowIndex];
			for(TWNode *child=row->firstChild();child;child=child->nextSibling()){
				TWRenderObject *obj=static_cast<TWRenderObject *>(child);
				// this should be cell.
				if(obj->isTableCell()){ 
					assert(dynamic_cast<TWRenderTableCell *>(obj));
					TWRenderTableCell *cell=static_cast<TWRenderTableCell *>(obj);
					if(cell->colSpan()==1)
						continue;
					
					int totalWidth=0;
					int colIndex=cell->colIndex();
					int colSpan=cell->colSpan();
					
					for(int col=colIndex;col<colIndex+colSpan;col++)
						totalWidth+=m_computedColsWidth[col];
					
					if(totalWidth>=cell->maximumContentWidth(manager))
						continue;
					
					// fit with resizing auto columns...
					
					int freeColumns=0;;
					
					for(int col=colIndex;col<colIndex+colSpan;col++){
						if(m_colsWidth[col].unit==TWHTMLStyleUnitAuto)
							freeColumns++;
					}
					
					if(freeColumns>0){
						int amount=cell->maximumContentWidth(manager)-totalWidth;
						amount/=freeColumns;
						for(int col=colIndex;col<colIndex+colSpan;col++){
							if(m_colsWidth[col].unit==TWHTMLStyleUnitAuto)
								m_computedColsWidth[col]+=amount;
						}
					}else{
						// forced expansion.
						int amount=cell->maximumContentWidth(manager)-totalWidth;
						amount/=colSpan;
						for(int col=colIndex;col<colIndex+colSpan;col++){
							m_computedColsWidth[col]+=amount;
						}
					}
				}
			}
		}
		
		// (here's optimization for mobile devices...)
		// does this table span too wide?
		int wid=0;
		wid=borderSizeLeft();
		wid+=borderSizeRight();
		for(int i=0;i<m_colCount;i++){
			wid+=m_computedColsWidth[i];
		}
		
		if(wid>m_rect.w){
			// too wide.
			// switch to "fixed width layout".
			for(int i=0;i<m_colCount;i++){
				m_colsWidth[i].unit=TWHTMLStyleUnitAuto;
			}
			goto NonAuto;
		}
		
	}else{
		
	NonAuto:
		
		// calculate non-auto columns.
		calcWidth();
		int remainingPixels=m_rect.w;
		
		remainingPixels-=borderSizeLeft();
		remainingPixels-=borderSizeRight();
		
		for(int i=0;i<m_colCount;i++){
			const TWHTMLStyleValue& colWidth=m_colsWidth[i];
			if(colWidth.unit!=TWHTMLStyleUnitAuto){
				m_computedColsWidth[i]=colWidth.toPixels(m_rect.w, style());
				remainingPixels-=m_computedColsWidth[i];
			}
		}
		
		if(remainingPixels<0){
			// impossible to fit!
			// ignore all column width specification.
			
			remainingPixels=m_rect.w;
			remainingPixels-=borderSizeLeft();
			remainingPixels-=borderSizeRight();
			
			for(int i=0;i<m_colCount;i++){
				m_colsWidth[i].unit=TWHTMLStyleUnitAuto;
			}
			
		}
		
		// give width to auto-columns corresponding to
		// auto width.
		int totalAutoWidth=0;
		for(int i=0;i<m_colCount;i++){
			const TWHTMLStyleValue& colWidth=m_colsWidth[i];
			if(colWidth.unit==TWHTMLStyleUnitAuto){
				totalAutoWidth+=std::min(m_colsAutoWidth[i], m_rect.w);
			}
		}
		
		for(int i=0;i<m_colCount;i++){
			const TWHTMLStyleValue& colWidth=m_colsWidth[i];
			if(colWidth.unit==TWHTMLStyleUnitAuto){
				m_computedColsWidth[i]=remainingPixels*std::min(m_colsAutoWidth[i], m_rect.w)/totalAutoWidth;
			}
		}
		
		// forcefully expand columns so that colspaned cells can exist (final resort)
		for(size_t rowIndex=0;rowIndex<m_rows.size();rowIndex++){
			TWRenderTableRow *row=m_rows[rowIndex];
			for(TWNode *child=row->firstChild();child;child=child->nextSibling()){
				TWRenderObject *obj=static_cast<TWRenderObject *>(child);
				// this should be cell.
				if(obj->isTableCell()){ 
					assert(dynamic_cast<TWRenderTableCell *>(obj));
					TWRenderTableCell *cell=static_cast<TWRenderTableCell *>(obj);
					if(cell->colSpan()==1)
						continue;
					
					int totalWidth=0;
					int colIndex=cell->colIndex();
					int colSpan=cell->colSpan();
					
					for(int col=colIndex;col<colIndex+colSpan;col++)
						totalWidth+=m_computedColsWidth[col];
					
					if(totalWidth>=cell->minWidth(manager))
						continue;
					
					// fit with resizing auto columns...
					
					int freeColumns=0;;
					
					for(int col=colIndex;col<colIndex+colSpan;col++){
						if(m_colsWidth[col].unit==TWHTMLStyleUnitAuto)
							freeColumns++;
					}
					
					if(freeColumns>0){
						int amount=cell->minWidth(manager)-totalWidth;
						amount/=freeColumns;
						for(int col=colIndex;col<colIndex+colSpan;col++){
							m_computedColsWidth[col]+=amount;
						}
					}else{
						// forced expansion.
						int amount=cell->minWidth(manager)-totalWidth;
						amount/=colSpan;
						for(int col=colIndex;col<colIndex+colSpan;col++){
							m_computedColsWidth[col]+=amount;
						}
					}
				}
			}
		}
		
	}
	
	m_rect.w=borderSizeLeft();
	m_rect.w+=borderSizeRight();
	//puts("table done");
	for(int i=0;i<m_colCount;i++){
		m_rect.w+=m_computedColsWidth[i];
		//printf("%8d, %d\n", m_colsAutoWidth[i], m_computedColsWidth[i]);
	}
}

void TWRenderTable::computeBoxModelBorder(){
	int maxWidth;
	
	// left border
	maxWidth=0;
	for(int row=0;row<m_rowCount;row++){
		BorderInfo info=leftBorderAt(row, 0);
		int width=info.width>>1;
		if(width>maxWidth)
			maxWidth=width;
	}
	m_borderLeft=maxWidth;
	
	// top border
	maxWidth=0;
	for(int col=0;col<m_colCount;col++){
		BorderInfo info=topBorderAt(0, col);
		int width=info.width>>1;
		if(width>maxWidth)
			maxWidth=width;
	}
	m_borderTop=maxWidth;
	
	// right border
	maxWidth=0;
	for(int row=0;row<m_rowCount;row++){
		BorderInfo info=leftBorderAt(row, m_colCount);
		int width=(info.width+1)>>1;
		if(width>maxWidth)
			maxWidth=width;
	}
	m_borderRight=maxWidth;

	// bottom border
	maxWidth=0;
	for(int col=0;col<m_colCount;col++){
		BorderInfo info=topBorderAt(m_rowCount, col);
		int width=(info.width+1)>>1;
		if(width>maxWidth)
			maxWidth=width;
	}
	m_borderBottom=maxWidth;
}

void TWRenderTable::computeColsPos(){
	int x=0;
	m_colsPos.clear();
	for(int col=0;col<m_colCount;col++){
		m_colsPos.push_back(x);
		x+=m_computedColsWidth[col];
	}
	m_colsPos.push_back(x);
}

/** resize columns so that they doesn't narrower than
 * they can be. */
bool TWRenderTable::regardMinimumContentWidthAndShouldRelayout(TWRenderManager *manager){
	std::vector<int> newColWidth=m_computedColsWidth;
	
	// for each rows...
	assert((int)m_rows.size()==m_rowCount);
	for(size_t rowIndex=0;rowIndex<m_rows.size();rowIndex++){
		TWRenderTableRow *row=m_rows[rowIndex];
		for(TWNode *child=row->firstChild();child;child=child->nextSibling()){
			TWRenderObject *obj=static_cast<TWRenderObject *>(child);
			// this should be cell.
			if(obj->isTableCell()){ 
				assert(dynamic_cast<TWRenderTableCell *>(obj));
				TWRenderTableCell *cell=static_cast<TWRenderTableCell *>(obj);
				
				cell->computeBoxModelBorder();
				
				if(cell->colSpan()>1){
					// a cell spanning multiple column is ignored.
					continue;
				}
				
				int minWidth=cell->minWidth(manager);
				
				int colIndex=cell->colIndex();
				if(minWidth>newColWidth[colIndex])
					newColWidth[colIndex]=minWidth;
			}
		}
	}
	
	bool changed=false;
	for(int i=0;i<m_colCount;i++){
		if(newColWidth[i]>m_computedColsWidth[i]){
			// need to expand.
			m_colsWidth[i]=TWHTMLStyleValue(TWHTMLStyleUnitPixels, newColWidth[i]);
			changed=true;
		}
	}
	
	return changed;
}

void TWRenderTable::computeCornerShifts(){
	if(!m_cornerShifts.empty())
		return;
	m_cornerShifts.reserve((m_rowCount+1)*(m_colCount+1));
	for(int i=0;i<=m_colCount;i++){
		for(int j=0;j<=m_rowCount;j++){
			m_cornerShifts.push_back(TWPoint(0,0));
		}
	}
	
	/* calculate horizonal offset. */
	
	// very left.
	cornerShiftAt(0, 0).x=-((leftBorderAt(0, 0).width)>>1);
	cornerShiftAt(m_rowCount, 0).x=-((leftBorderAt(m_rowCount-1, 0).width)>>1);
	for(int row=1;row<m_rowCount;row++){
		BorderInfo bi1=leftBorderAt(row, 0);
		BorderInfo bi2=leftBorderAt(row-1, 0);
		int width=std::max(bi1.width, bi2.width);
		cornerShiftAt(row, 0).x=-((width)>>1);
	}
	
	// very right.
	cornerShiftAt(0, m_colCount).x=(leftBorderAt(0, m_colCount).width+1)>>1;
	cornerShiftAt(m_rowCount, m_colCount).x=(leftBorderAt(m_rowCount-1, m_colCount).width+1)>>1;
	for(int row=1;row<m_rowCount;row++){
		BorderInfo bi1=leftBorderAt(row, 0);
		BorderInfo bi2=leftBorderAt(row-1, 0);
		int width=std::max(bi1.width, bi2.width);
		cornerShiftAt(row, 0).x=-((width)>>1);
	}
	
	for(int col=1;col<m_colCount;col++){
		// top corner.
		{
			BorderInfo bi1=topBorderAt(0, col-1);
			BorderInfo bi2=topBorderAt(0, col);
			BorderInfo bi=leftBorderAt(0, col);
			if(resolveBorders(bi1, bi2)){
				// left precedece.
				cornerShiftAt(0, col).x=(bi.width+1)>>1;
			}else{
				// right precedence.
				cornerShiftAt(0, col).x=-(bi.width>>1);
			}
		}
		
		// bottom corner.
		{
			BorderInfo bi1=topBorderAt(m_rowCount, col-1);
			BorderInfo bi2=topBorderAt(m_rowCount, col);
			BorderInfo bi=leftBorderAt(m_rowCount-1, col);
			if(resolveBorders(bi1, bi2)){
				// left precedece.
				cornerShiftAt(m_rowCount, col).x=(bi.width+1)>>1;
			}else{
				// right precedence.
				cornerShiftAt(m_rowCount, col).x=-(bi.width>>1);
			}
		}
		
		for(int row=1;row<m_rowCount;row++){
			BorderInfo bi1=topBorderAt(row, col-1);
			BorderInfo bi2=topBorderAt(row, col);
			BorderInfo bi3=leftBorderAt(row-1, col);
			BorderInfo bi4=leftBorderAt(row, col);
			int width=std::max(bi3.width, bi4.width);
			if(resolveBorders(bi1, bi2)){
				// left precedece.
				cornerShiftAt(row, col).x=(width+1)>>1;
			}else{
				// right precedence.
				cornerShiftAt(row, col).x=-(width>>1);
			}
		}
	}
	
	/* calculate vertical offset. */
	
	// very top.
	cornerShiftAt(0, 0).y=-(topBorderAt(0, 0).width>>1);
	cornerShiftAt(0, m_colCount).y=-(topBorderAt(0, m_colCount-1).width>>1);
	for(int col=1;col<m_colCount;col++){
		BorderInfo bi1=topBorderAt(0, col-1);
		BorderInfo bi2=topBorderAt(0, col);
		int width=std::max(bi1.width, bi2.width);
		cornerShiftAt(0, col).y=-(width>>1);
	}
	
	// very bottom.
	cornerShiftAt(m_rowCount, 0).y=(topBorderAt(m_rowCount, 0).width+1)>>1;
	cornerShiftAt(m_rowCount, m_colCount).y=(topBorderAt(m_rowCount, m_colCount-1).width+1)>>1;
	for(int col=1;col<m_colCount;col++){
		BorderInfo bi1=topBorderAt(m_rowCount, col-1);
		BorderInfo bi2=topBorderAt(m_rowCount, col);
		int width=std::max(bi1.width, bi2.width);
		cornerShiftAt(m_rowCount, col).y=(width+1)>>1;
	}
	
	for(int row=1;row<m_rowCount;row++){
		// left corner.
		{
			BorderInfo bi1=leftBorderAt(row-1, 0);
			BorderInfo bi2=leftBorderAt(row, 0);
			BorderInfo bi=topBorderAt(row, 0);
			if(resolveBorders(bi1, bi2)){
				// top precedence.
				cornerShiftAt(row, 0).y=(bi.width+1)>>1;
			}else{
				// bottom precedence.
				cornerShiftAt(row, 0).y=-(bi.width>>1);
			}
		}
		
		// right corner.
		{
			BorderInfo bi1=leftBorderAt(row-1, m_colCount);
			BorderInfo bi2=leftBorderAt(row, m_colCount);
			BorderInfo bi=topBorderAt(row, m_colCount-1);
			if(resolveBorders(bi1, bi2)){
				// top precedence.
				cornerShiftAt(row, m_colCount).y=(bi.width+1)>>1;
			}else{
				// bottom precedence.
				cornerShiftAt(row, m_colCount).y=-(bi.width>>1);
			}
		}
		
		for(int col=1;col<m_colCount;col++){
			BorderInfo bi1=leftBorderAt(row-1, col);
			BorderInfo bi2=leftBorderAt(row, col);
			BorderInfo bi3=topBorderAt(row, col-1);
			BorderInfo bi4=topBorderAt(row, col);
			int width=std::max(bi3.width, bi4.width);
			if(resolveBorders(bi1, bi2)){
				// top precedence.
				cornerShiftAt(row, col).y=(width+1)>>1;
			}else{
				// bottom precedence.
				cornerShiftAt(row, col).y=-(width>>1);
			}
		}
	}
	
}

// cover all inner elements into new anonymous cell.
void TWRenderTable::shieldTable(){
	TWRenderTableRow *m_anonymousRow;
	TWRenderTableCell *m_anonymousCell;
	m_anonymousRow=new TWRenderTableRow(NULL, this);
	m_anonymousCell=new TWRenderTableCell(NULL);
	m_anonymousCell->setTable(this);
	
	TWNode *child=firstChild();
	for(;child;){
		TWNode *next=child->nextSibling();
		m_anonymousCell->addChild(child);
		child=next;
	}
	
	m_anonymousRow->addChild(m_anonymousCell);
	addChild(m_anonymousRow);
	
	calcStyle();
}

void TWRenderTable::initialBuildTable(TWRenderManager *manager){
	// make sure we can access each rows.
	scanRows(this);
	
	if(!m_rowCount){
		// empty table. create anonymous cells to
		// correct this.
		shieldTable();
		scanRows(this);
	}
	if(!m_rowCount){
		// still empty? ignore.
		return;
	}
	
	// and position cells (in tablar meaning).
	tablize();
	
	if(!m_colCount){
		// empty table.
		return;
	}
	
	// compute border width of table.
	computeBoxModelBorder();
	
	// compute cols' style width.
	determineColsWidth(manager);
}

void TWRenderTable::layout(TWRenderManager *manager){
	
	// build table in memory.
	initialBuildTable(manager);
	
	calcWidth();
	
	// compute cols' actual width.
	int layoutCount=32;
	do{
		computeColsWidth(manager);
	}while(regardMinimumContentWidthAndShouldRelayout(manager) &&
		   (layoutCount--)>0);
	computeColsPos();
	
	// compute "corner shift."
	computeCornerShifts();
	
	// layout rows.
	m_rect.h=borderSizeTop();
	for(int i=0;i<m_rowCount;i++){
		TWRenderTableRow *row=m_rows[i];
		TWRect rt;
		rt.x=borderSizeLeft(); rt.y=m_rect.h;
		rt.w=m_rect.w-borderSizeLeft()-borderSizeRight();
		rt.h=0;
		row->setRect(rt);
		row->layout(manager);
		
		rt=row->rect();
		m_rect.h+=rt.h;
	}
	m_rect.h+=borderSizeBottom();
	
	m_isLayouted=true;
}

int TWRenderTable::levelForBorderStyle(TWHTMLStyleBorderStyle style){
	switch(style){
		case TWHTMLStyleBorderStyleInset:
			return 0;
		case TWHTMLStyleBorderStyleGroove:
			return 1;
		case TWHTMLStyleBorderStyleOutset:
			return 2;
		case TWHTMLStyleBorderStyleRidge:
			return 3;
		case TWHTMLStyleBorderStyleDotted:
			return 4;
		case TWHTMLStyleBorderStyleDashed:
			return 5;
		case TWHTMLStyleBorderStyleSolid:
			return 6;
		case TWHTMLStyleBorderStyleDouble:
			return 7;
		case TWHTMLStyleBorderStyleNone:
			return -1;
	}
}

/** determines which border takes precedence.
 * see also: http://www.w3.org/TR/CSS2/tables.html */
bool TWRenderTable::resolveBorders(const BorderInfo &a, 
								   const BorderInfo &b){
	if(a.border.style==TWHTMLStyleBorderStyleNone)
		return false;
	if(b.border.style==TWHTMLStyleBorderStyleNone)
		return true;
	if(a.width>b.width){
		return true;
	}
	if(a.width<b.width){
		return false;
	}
	if(levelForBorderStyle(a.border.style)>levelForBorderStyle(b.border.style))
	   return true;
	if(levelForBorderStyle(a.border.style)<levelForBorderStyle(b.border.style))
		return false;
	return true;
}

TWRenderTableCell *TWRenderTable::cellAt(int row, int col) const{
	assert(row>=0);
	assert(row<m_rowCount);
	assert(col>=0);
	assert(col<m_colCount);
	TWRenderTableRow *rowObj=m_rows[row];
	assert(rowObj!=NULL);
	return rowObj->m_cells[col];
}

void TWRenderTable::BorderInfo::calcWidth(){
	if(border.style==TWHTMLStyleBorderStyleNone)
		width=0;
	else{
		width=border.width.toPixels(0, owner->style());
	}
}

TWRenderTable::BorderInfo TWRenderTable::leftBorderAt(int row, int col) const{
	BorderInfo bi;
	bi.owner=NULL;
	bi.width=0;
	
	if(row<0 || row>=m_rowCount || col<0 || col>m_colCount){
		// no table there!
		return bi;
	}
	
	BorderInfo bi1, bi2;
	
	if(col==m_colCount){
		bi1.owner=const_cast<TWRenderTable *>(this);
		bi1.border=bi1.owner->style().borderRight();
		bi1.leftOrTop=false;
	}else{
		bi1.owner=cellAt(row, col);
		bi1.leftOrTop=true;
		if(bi1.owner){
			bi1.border=bi1.owner->style().borderLeft();
			assert(dynamic_cast<TWRenderTableCell *>(bi1.owner));
			if(col>static_cast<TWRenderTableCell *>(bi1.owner)->colIndex()){
				// middle of cell.
				bi1.owner=NULL;
			}
		}
	}
	
	if(col==0){
		bi2.owner=const_cast<TWRenderTable *>(this);
		bi2.border=bi2.owner->style().borderLeft();
		bi2.leftOrTop=true;
	}else{
		bi2.owner=cellAt(row, col-1);
		if(bi2.owner){
			bi2.border=bi2.owner->style().borderRight();
			assert(dynamic_cast<TWRenderTableCell *>(bi2.owner));
			if(static_cast<TWRenderTableCell *>(bi2.owner)->colIndex()+
			   static_cast<TWRenderTableCell *>(bi2.owner)->colSpan()!=col){
				// middle of cell.
				bi2.owner=NULL;
			}
		}
		bi2.leftOrTop=false;
	}

	
	if(bi1.owner==NULL && bi2.owner==NULL){
		// no cell.
		return bi;
	}
	
	if(bi1.owner==NULL){
		bi2.calcWidth();
		return bi2;
	}
	
	if(bi2.owner==NULL){
		bi1.calcWidth();
		return bi1;
	}
	
	bi1.calcWidth();
	bi2.calcWidth();
	
	if(!resolveBorders(bi2, bi1)){
		return bi1;
	}else{
		return bi2;
	}
}

TWRenderTable::BorderInfo TWRenderTable::topBorderAt(int row, int col) const{
	BorderInfo bi;
	bi.owner=NULL;
	bi.width=0;
	
	if(row<0 || row>m_rowCount || col<0 || col>=m_colCount){
		// no table there!
		return bi;
	}
	
	BorderInfo bi1, bi2;
	
	if(row==m_rowCount){
		bi1.owner=const_cast<TWRenderTable *>(this);
		bi1.border=bi1.owner->style().borderBottom();
		bi1.leftOrTop=false;
	}else{
		bi1.owner=cellAt(row, col);
		bi1.leftOrTop=true;
		if(bi1.owner){
			bi1.border=bi1.owner->style().borderTop();
			assert(dynamic_cast<TWRenderTableCell *>(bi1.owner));
			if(row>static_cast<TWRenderTableCell *>(bi1.owner)->rowIndex()){
				// middle of cell.
				bi1.owner=NULL;
			}
		}
	}
	
	if(row==0){
		bi2.owner=const_cast<TWRenderTable *>(this);
		bi2.border=bi2.owner->style().borderTop();
		bi2.leftOrTop=true;
	}else{
		bi2.owner=cellAt(row-1, col);
		if(bi2.owner){
			bi2.border=bi2.owner->style().borderBottom();
			assert(dynamic_cast<TWRenderTableCell *>(bi2.owner));
			if(static_cast<TWRenderTableCell *>(bi2.owner)->rowIndex()+
			   static_cast<TWRenderTableCell *>(bi2.owner)->rowSpan()!=row){
				// middle of cell.
				bi2.owner=NULL;
			}
		}
		bi2.leftOrTop=false;
	}
	
	if(bi1.owner==NULL && bi2.owner==NULL){
		// no cell.
		return bi;
	}
	
	if(bi1.owner==NULL){
		bi2.calcWidth();
		return bi2;
	}
	
	if(bi2.owner==NULL){
		bi1.calcWidth();
		return bi1;
	}
	
	bi1.calcWidth();
	bi2.calcWidth();
	
	if(!resolveBorders(bi2, bi1)){
		return bi1;
	}else{
		return bi2;
	}
}

void TWRenderTable::render(TWRenderDC *dc){
	TWRenderBlock::render(dc);
	if(m_colCount==0 || m_rowCount==0)
		return;
	renderTableBorder(dc);
}

void TWRenderTable::renderTableBorder(TWRenderDC *dc){
	// vertical
	for(int col=0;col<=m_colCount;col++){
		BorderInfo lastBorder=leftBorderAt(0, col);
		int lastRow=0;
		
		for(int row=1;row<m_rowCount;row++){
			BorderInfo newBorder=leftBorderAt(row, col);
			if(lastBorder!=newBorder){
				renderVerticalBorder(dc,
									 lastBorder, 
									 col, 
									 lastRow, 
									 row);
				lastBorder=newBorder;
				lastRow=row;
			}
		}
		
		renderVerticalBorder(dc, 
							 lastBorder, 
							 col, 
							 lastRow, 
							 m_rowCount);
		
	}
	
	// horizonal
	for(int row=0;row<=m_rowCount;row++){
		BorderInfo lastBorder=topBorderAt(row, 0);
		int lastCol=0;
		
		for(int col=1;col<m_colCount;col++){
			BorderInfo newBorder=topBorderAt(row, col);
			if(lastBorder!=newBorder){
				renderHorizonalBorder(dc,
									  lastBorder, 
									  row, 
									  lastCol, 
									  col);
				lastBorder=newBorder;
				lastCol=col;
			}
		}
		
		renderHorizonalBorder(dc, 
							  lastBorder, 
							  row, 
							  lastCol, 
							  m_colCount);
	}
}

int TWRenderTable::screenXForCol(int col) const{
	return m_colsPos[col]+borderSizeLeft()+m_screenRect.x;
}

int TWRenderTable::screenYForRow(int row) const{
	if(row==m_rowCount){
		return m_rect.h-borderSizeBottom()+m_screenRect.y;
	}else{
		return m_rows[row]->rect().y+m_screenRect.y;
	}
}

void TWRenderTable::renderVerticalBorder(TWRenderDC *dc,
										 const TWRenderTable::BorderInfo &border,
										 int col, 
										 int row1, int row2){
	// "none" border.
	if(border.width==0)
		return;
	
	// get coordinates for the border.
	int colX, rowY1, rowY2;
	colX=screenXForCol(col);
	rowY1=screenYForRow(row1);
	rowY2=screenYForRow(row2);
	
	// get "corner offsets".
	int offset1=cornerShiftAt(row1, col).y;
	int offset2=cornerShiftAt(row2, col).y;
	
	// make rect for border.
	TWRect rt;
	rt.x=colX-(border.width>>1);
	rt.y=rowY1+offset1;
	rt.w=border.width;
	rt.h=rowY2-rowY1+offset2-offset1;
	
	// create style to draw only border.
	TWHTMLStyle style;
	TWHTMLStyleBorder cssBorder=border.border;
	cssBorder.width=TWHTMLStyleValue(TWHTMLStyleUnitPixels, border.width);
	if(border.leftOrTop)
		style.setBorderLeft(cssBorder);
	else
		style.setBorderRight(cssBorder);
	
	// draw.
	dc->setStyle(style);
	dc->drawBorder(rt);
	
}

void TWRenderTable::renderHorizonalBorder(TWRenderDC *dc,
										  const TWRenderTable::BorderInfo &border,
										  int row, 
										  int col1, int col2){
	// "none" border.
	if(border.width==0)
		return;
	
	// get coordinates for the border.
	int rowY, colX1, colX2;
	rowY=screenYForRow(row);
	colX1=screenXForCol(col1);
	colX2=screenXForCol(col2);
	
	// get "corner offsets".
	int offset1=cornerShiftAt(row, col1).x;
	int offset2=cornerShiftAt(row, col2).x;
	
	// make rect for border.
	TWRect rt;
	rt.x=colX1+offset1;
	rt.y=rowY-(border.width>>1);
	rt.w=colX2-colX1+offset2-offset1;
	rt.h=border.width;
	
	// create style to draw only border.
	TWHTMLStyle style;
	TWHTMLStyleBorder cssBorder=border.border;
	cssBorder.width=TWHTMLStyleValue(TWHTMLStyleUnitPixels, border.width);
	if(border.leftOrTop)
		style.setBorderTop(cssBorder);
	else
		style.setBorderBottom(cssBorder);
	
	// draw.
	dc->setStyle(style);
	dc->drawBorder(rt);
}

int TWRenderTable::maximumContentWidth(TWRenderManager *manager){
	if(m_maximumContentWidth!=-1)
		return m_maximumContentWidth;
	
	initialBuildTable(manager);
	
	int totalWidth=0;
	for(int col=0;col<m_colCount;col++){
		int maxWidth=0;
		for(int row=0;row<m_rowCount;row++){
			TWRenderTableCell *cell=cellAt(row, col);
			if(!cell)
				continue;
			if(cell->rowSpan()>1)
				continue;
			
			int w=cell->maximumContentWidth(manager);
			if(cell->colSpan()>1)
				w/=cell->colSpan();
			if(w>maxWidth)
				maxWidth=w;
		}
		totalWidth+=maxWidth;
	}
	
	m_maximumContentWidth=totalWidth;
	
	return totalWidth;
}

int TWRenderTable::minimumContentWidth(TWRenderManager *manager){
	if(m_minimumContentWidth!=-1)
		return m_minimumContentWidth;
	
	initialBuildTable(manager);
	
	int totalWidth=0;
	for(int col=0;col<m_colCount;col++){
		int maxWidth=0;
		for(int row=0;row<m_rowCount;row++){
			TWRenderTableCell *cell=cellAt(row, col);
			if(!cell)
				continue;
			if( cell->rowSpan()>1)
				continue;
			
			int w=cell->minimumContentWidth(manager);
			if(cell->colSpan()>1)
				w/=cell->colSpan();
			if(w>maxWidth)
				maxWidth=w;
		}
		totalWidth+=maxWidth;
	}
	
	m_minimumContentWidth=totalWidth;
	
	return totalWidth;
}

void TWRenderTable::calcChildrenFlowIndex(){
	unsigned int index=flowIndexStart();
	for(int row=0;row<m_rowCount;row++){
		TWRenderTableRow *r=m_rows[row];
		r->setFlowIndexStart(index);
		r->calcChildrenFlowIndex();
		index=r->flowIndexEnd();
	}
	setFlowIndexEnd(index);
}

unsigned int TWRenderTable::flowIndexAtPoint(const TWPoint &pt)const{
	for(int row=0;row<m_rowCount;row++){
		TWRenderTableRow *r=m_rows[row];
		unsigned int i;
		i=r->flowIndexAtPoint(pt);
		if(i!=-1)
			return i;
	}
	return flowIndexEnd();
}

void TWRenderTable::captureSelectionString
(std::wstring &out, int begin, int end){
	if(end<=flowIndexStart() || begin>=flowIndexEnd())
		return;
	for(int row=0;row<m_rowCount;row++){
		TWRenderTableRow *r=m_rows[row];
		r->captureSelectionString(out, begin, end);
	}
}

TWRect TWRenderTable::drawSelection
(TWRenderDC *dc, int begin, int end){
	TWRect selRect(0,0,0,0);
	if(end<=flowIndexStart() || begin>=flowIndexEnd())
		return selRect;
	for(int row=0;row<m_rowCount;row++){
		TWRenderTableRow *r=m_rows[row];
		TWRect rt=r->drawSelection(dc, begin, end);
		if(rt.w){
			if(selRect.w)
				selRect=selRect|rt;
			else
				selRect=rt;
		}
	}
	return selRect;
}

void TWRenderTable::addChild(TWNode *node){
	assert(node!=NULL);
	TWRenderObject *obj=static_cast<TWRenderObject *>(node);
	
	if(obj->isTableRow() || obj->isTableCol()){
		TWRenderBox::addChild(node);
	}else if(obj->isTableCell()){
		// add anonymous row, if needed.
		TWRenderObject *last=static_cast<TWRenderObject *>(lastChild());
		if(last==NULL || !last->isAnonymous()){
			TWRenderTableRow *row=new TWRenderTableRow(NULL, this);
			addChild(row);
		}
		
		TWRenderTableRow *row=static_cast<TWRenderTableRow *>(lastChild());
		assert(row->isAnonymous());
		assert(row->isTableRow());
		
		row->addChild(node);
		
	}else{
		// unknown.
		// create anomymous cell.
		// first create anonymous row.
		TWRenderObject *last=static_cast<TWRenderObject *>(lastChild());
		if(last==NULL || !last->isAnonymous()){
			TWRenderTableRow *row=new TWRenderTableRow(NULL, this);
			addChild(row);
		}
		
		TWRenderTableRow *row=static_cast<TWRenderTableRow *>(lastChild());
		assert(row->isAnonymous());
		assert(row->isTableRow());
		
		// create anomymous cell.
		TWRenderTableCell *cell=new TWRenderTableCell(NULL);
		row->addChild(cell);
		
		cell->addChild(node);
	}
}


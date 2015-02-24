//
//  TWRenderTableRow.h
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

#pragma once

#include "TWRenderBox.h"

class TWRenderTable;
class TWRenderTableCell;

class TWRenderTableRow : public TWRenderBox {
	friend class TWRenderTable;
	TWRenderTable *m_table;
	int m_rowIndex;
	std::vector<TWRenderTableCell *> m_cells;
	
	unsigned int m_flowIndexStart;
	unsigned int m_flowIndexEnd;
	
	TWRect rectForCol(int colIndex, int colSpan) const;
public:
    TWRenderTableRow(TWHTMLElement *, TWRenderTable *);
	virtual ~TWRenderTableRow(){}
	
	TWRenderTable *table()const{return m_table;}
	int rowIndex() const{return m_rowIndex;}
	
	void setTable(TWRenderTable *t){m_table=t;}
	void setRowIndex(int i){m_rowIndex=i;}
	
	virtual void layout(TWRenderManager *);
	
	unsigned int flowIndexStart() const{return m_flowIndexStart;}
	unsigned int flowIndexEnd() const{return m_flowIndexEnd;}
	void setFlowIndexStart(unsigned int i){m_flowIndexStart=i;}
	void setFlowIndexEnd(unsigned int i){m_flowIndexEnd=i;}
	
	void calcChildrenFlowIndex();
	unsigned int flowIndexAtPoint(const TWPoint&) const;
	TWRect drawSelection(TWRenderDC *, int begin, int end); 
	void captureSelectionString(std::wstring&, int begin, int end);
	
	virtual void addChild(TWNode *node);
};


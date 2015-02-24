//
//  TWRenderTableCell.h
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

#include "TWRenderBlock.h"
#include "TWRenderTable.h"

class TWRenderTableCell: public TWRenderBlock {
	friend class TWRenderTable;
	friend class TWRenderTableRow;
	int m_colSpan;
	int m_rowSpan;
	int m_colIndex, m_rowIndex;
	TWRenderTable *m_table;
	
	int m_borderLeft;
	int m_borderTop;
	int m_borderRight;
	int m_borderBottom;
	
	void computeBoxModelBorder();
	
public:
    TWRenderTableCell(TWHTMLElement *);
	virtual ~TWRenderTableCell(){}
	
	int colSpan() const{return m_colSpan;}
	int rowSpan() const{return m_rowSpan;}
	
	int colIndex() const{return m_colIndex;}
	int rowIndex() const{return m_rowIndex;}
	
	TWRenderTable *table() const{return m_table;}
	
	void setColIndex(int i){m_colIndex=i;}
	void setRowIndex(int i){m_rowIndex=i;}
	
	void setTable(TWRenderTable *t){m_table=t;}
	
	int autoWidth(TWRenderManager *) const;
	int minWidth(TWRenderManager *) const;
	
	virtual int borderSizeLeft() const{
		return m_borderLeft;
	}
	virtual int borderSizeTop() const{
		return m_borderTop;
	}
	virtual int borderSizeRight() const{
		return m_borderRight;
	}
	virtual int borderSizeBottom() const{
		return m_borderBottom;
	}
	
	void alignContentsVertical();
};

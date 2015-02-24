//
//  TWRenderTable.h
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

#pragma once

#include "TWRenderBlock.h"

class TWRenderTableRow;

/* About Collapsed Table Border Rounding...
 *
 *  Left * | ** Right
 * 
 * Left = floor(width/2)
 * Right = ceil(width/2) = floor((width+1)/2)
 *
 */

class TWRenderTableCell;

class TWRenderTable: public TWRenderBlock {
	friend class TWRenderTableRow;
	friend class TWRenderTableCell;
public:
	struct BorderInfo;
protected:
	std::vector<TWRenderTableRow *> m_rows;
	std::vector<TWHTMLStyleValue> m_colsWidth;
	std::vector<int> m_colsAutoWidth;
	std::vector<int> m_computedColsWidth;
	std::vector<int> m_colsPos;
	std::vector<TWPoint> m_cornerShifts;
	int m_rowCount;
	int m_colCount;
	bool m_tablized;
	
	int m_borderLeft;
	int m_borderTop;
	int m_borderRight;
	int m_borderBottom;
	
	void scanRows(TWRenderObject *);
	void tablize();
	void determineColsWidth(TWRenderManager *);
	void computeColsWidth(TWRenderManager *);
	void computeColsPos();
	void computeCornerShifts();
	bool regardMinimumContentWidthAndShouldRelayout(TWRenderManager *);
	
	void computeBoxModelBorder();
	
	TWRect rectForCell(int row, int col);
	TWPoint& cornerShiftAt(int row, int col){
		return m_cornerShifts[row*(m_colCount+1)+col];
	}
	const TWPoint& cornerShiftAt(int row, int col) const{
		return m_cornerShifts[row*(m_colCount+1)+col];
	}
	
	void renderTableBorder(TWRenderDC *);
	void renderVerticalBorder(TWRenderDC *,
							  const BorderInfo&,
							  int col,
							  int row1,
							  int row2);
	void renderHorizonalBorder(TWRenderDC *,
							   const BorderInfo&,
							   int row,
							   int col1,
							   int col2);
	int screenXForCol(int col) const;
	int screenYForRow(int row) const;
	
	void shieldTable();
	
	void initialBuildTable(TWRenderManager *);
	
	int m_maximumContentWidth;
	int m_minimumContentWidth;
	
public:
    
	struct BorderInfo{
		TWHTMLStyleBorder border;
		TWRenderBlock *owner;
		int width;
		bool leftOrTop;
		
		void calcWidth();
	
		bool operator ==(const BorderInfo& bi) const{
			// ?: maybe ok...
			return owner==bi.owner ||
			(width==bi.width && border.style==bi.border.style &&
			 border.color==bi.border.color);
		}
		
		bool operator !=(const BorderInfo& bi) const{
			return !(*this==bi);
		}
	};
	
    TWRenderTable(TWHTMLElement *);
    
	virtual void layout(TWRenderManager *);
	
	static int levelForBorderStyle(TWHTMLStyleBorderStyle);
	
	/** a is left or top. 
	 * @return true means a is chosen. */
	static bool resolveBorders(const BorderInfo& a,
							   const BorderInfo& b);
  
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
	
	TWRenderTableCell *cellAt(int row, int col) const;
	
	BorderInfo leftBorderAt(int row, int col) const;
	BorderInfo topBorderAt(int row, int col) const;
	
	virtual void addChild(TWNode *node);
	
	virtual void render(TWRenderDC *);
	
	virtual int maximumContentWidth(TWRenderManager *manager);
	
	virtual int minimumContentWidth(TWRenderManager *manager);
	
	virtual void calcChildrenFlowIndex();
	
	virtual unsigned int flowIndexAtPoint(const TWPoint&) const;
	
	virtual TWRect drawSelection(TWRenderDC *, int begin, int end); 
	
	virtual void captureSelectionString(std::wstring&, int begin, int end);
	
};

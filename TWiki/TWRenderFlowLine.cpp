//
//  TWRenderFlowLine.cpp
//  XTBook
//
//  Created by Nexhawks on 3/20/11.
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

#include "TWRenderFlowLine.h"
#include "TWRenderDC.h"
#include "TWRenderBlock.h"

TWRenderFlowLine::TWRenderFlowLine():
TWRenderFlowObject(NULL),
m_curMinY(0),
m_curMaxY(0){
	m_isRenderFlowLine=true;
	setRect(TWRect(0, 0, 0, 0));
	
}

void TWRenderFlowLine::addedNodeWithRect(TWRect childRect){
	TWRect rt=rect();
	int minY, maxY;
	minY=childRect.y;
	maxY=childRect.y+childRect.h;
	
	if(minY<m_curMinY)
		m_curMinY=minY;
	if(maxY>m_curMaxY)
		m_curMaxY=maxY;
	
	rt.h=-(m_curMaxY-m_curMinY);
	
	int maxX;
	maxX=childRect.x+childRect.w;
	if(maxX>rt.w)
		rt.w=maxX;
	
	setRect(rt);
}

TWRect TWRenderFlowLine::drawSelection(TWRenderDC *dc,
									  int begin, int end){
	// outta range?
	if(end<=flowIndexStart())
		return TWRect(0,0,0,0);
	if(begin>=flowIndexEnd())
		return TWRect(0,0,0,0);
	
	int flowStart=flowIndexStart();
	
	// check for floating objects.
	for(std::list<TWRenderObject *>::iterator it=m_floats.begin();
		it!=m_floats.end();it++){
		TWRenderBlock *floatBlock=static_cast<TWRenderBlock *>(*it);
		floatBlock->drawSelection(dc, begin, end);
		flowStart=floatBlock->flowIndexEnd();
	}
	
	if(flowIndexStart()>=begin && flowIndexEnd()<=end){
		// completely filled.
		dc->fillSelection(screenRect());
		return screenRect();
	}
	
	// clip range.
	if(begin<flowStart)
		begin=flowStart;
	if(end>flowIndexEnd())
	   end=flowIndexEnd();
	
	if(begin>=end)
		return TWRect(0,0,0,0);
	
	// get horizonal position.
	int beginPos=horizonalPositionForFlowIndex(begin);
	int endPos=horizonalPositionForFlowIndex(end);
	
	TWRect scrRect=screenRect();
	
	TWRect selRect=TWRect(beginPos, scrRect.y, endPos-beginPos, scrRect.h).sanitize();
	
	dc->fillSelection(selRect);
	return selRect;
	
}



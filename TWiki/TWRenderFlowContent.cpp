//
//  TWRenderFlowContent.cpp
//  XTBook
//
//  Created by Nexhawks on 3/24/11.
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

#include "TWRenderFlowContent.h"
#include "TWRenderReplaced.h"
#include <assert.h>

TWRenderFlowContent::TWRenderFlowContent(TWRenderReplaced *renderer, int segment):
TWRenderFlowObject(NULL),
m_renderer(renderer),
m_minSegment(segment),
m_maxSegment(segment+1){
	m_isRenderFlowContent=true;
}
TWRenderFlowContent::TWRenderFlowContent(TWRenderReplaced *renderer, int minSegment, int maxSegment):
TWRenderFlowObject(NULL),
m_renderer(renderer),
m_minSegment(minSegment),
m_maxSegment(maxSegment){
	m_isRenderFlowContent=true;
}

void TWRenderFlowContent::render(TWRenderDC *dc){
	m_renderer->render(this, dc);
}

bool TWRenderFlowContent::isAppendable(TWRenderReplaced *renderer, int segment) const{
	if(renderer!=m_renderer)
		return false;
	if(segment!=m_maxSegment)
		return false;
	return true;
}

void TWRenderFlowContent::append(TWRenderReplaced *renderer, int segment){
	
	m_maxSegment++;
}

int TWRenderFlowContent::segment() const{
	assert(m_maxSegment-m_minSegment==1);
	return m_minSegment;
}

void TWRenderFlowContent::calcStyle(){
	m_style=m_renderer->style();
	m_styleValid=true;
	
}


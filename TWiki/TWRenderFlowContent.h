//
//  TWRenderFlowContent.h
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

#pragma once

#include "TWRenderFlowObject.h"

class TWRenderReplaced;

class TWRenderFlowContent: public TWRenderFlowObject {
protected:
	TWRenderReplaced *m_renderer;
	int m_minSegment TW_BITFIELD(16);
	int m_maxSegment TW_BITFIELD(16);
	unsigned int m_flowIndex;
public:
	TWRenderFlowContent(TWRenderReplaced *, int);
	TWRenderFlowContent(TWRenderReplaced *, int, int);
	
	int minSegment() const{return m_minSegment;}
	int maxSegment() const{return m_maxSegment;}
	int segment() const;
	
	bool isAppendable(TWRenderReplaced *, int) const;
	void append(TWRenderReplaced *, int);
	
	virtual void render(TWRenderDC *);
    
	
	virtual void calcStyle();
	
	int flowIndexStart() const{return m_flowIndex;}
	int flowIndexEnd() const{return m_flowIndex+m_maxSegment-m_minSegment;}
	void setFlowIndexStart(unsigned int i){m_flowIndex=i;}

	TWRenderReplaced *renderer() const{return m_renderer;}
	
};

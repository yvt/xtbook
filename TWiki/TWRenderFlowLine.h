//
//  TWRenderFlowLine.h
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

#pragma once

#include "TWRenderFlowObject.h"

#include <list>

class TWRenderFlowLine: public TWRenderFlowObject{
	friend class TWRenderFlowObject;
protected:
	int m_curMinY, m_curMaxY;
	unsigned int m_flowIndexStart;
	unsigned int m_flowIndexEnd;
	std::list<TWRenderObject *> m_floats;
public:
	TWRenderFlowLine();
	
	void addedNodeWithRect(TWRect);
	
	int flowIndexStart() const{return m_flowIndexStart;}
	int flowIndexEnd() const{return m_flowIndexEnd;}
	void setFlowIndexStart(unsigned int i){m_flowIndexStart=i;}
	void setFlowIndexEnd(unsigned int i){m_flowIndexEnd=i;}

	void addFloatingObject(TWRenderObject *obj){m_floats.push_back(obj);}
	
	const std::list<TWRenderObject *>& floats() const{return m_floats;}
	
	TWRect drawSelection(TWRenderDC *, int begin, int end); 
	
};

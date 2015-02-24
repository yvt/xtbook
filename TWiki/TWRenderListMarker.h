//
//  TWRenderListMarker.h
//  XTBook
//
//  Created by Kawada Tomoaki on 8/4/11.
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

class TWRenderListMarker: public TWRenderBlock{
	bool m_isNumbered;
	int m_number;
	
	TWSize markerSize(TWRenderManager *) const;
	std::wstring textForNumberedMarker() const;
	TWPoint markerPosition() const;
public:
	TWRenderListMarker(TWHTMLElement *, bool numbered);
	virtual ~TWRenderListMarker();
	
	bool isNumbered() const{return m_isNumbered;}
	
	int number() const{return m_number;}
	void setNumber(int i){m_number=i;}
	
	virtual void render(TWRenderDC *);
	virtual void layout(TWRenderManager *);
	
	
	virtual void calcChildrenFlowIndex();
	
	virtual unsigned int flowIndexAtPoint(const TWPoint&) const;
	
	virtual TWRect drawSelection(TWRenderDC *, int begin, int end); 

	virtual void captureSelectionString(std::wstring&, int begin, int end){}
	
};



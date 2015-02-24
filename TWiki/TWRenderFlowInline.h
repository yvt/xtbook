//
//  TWRenderFlowInline.h
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

#include "TWRenderFlowObject.h"

class TWRenderInline;

class TWRenderFlowInline: public TWRenderFlowObject{
protected:	
	TWRenderInline *m_renderer;
	bool m_hasLeftBorder  TW_BITFIELD(1);
	bool m_hasRightBorder  TW_BITFIELD(1);
	unsigned int m_flowIndexStart TW_BITFIELD(30);
	unsigned int m_flowIndexEnd;
public:
	TWRenderFlowInline(TWRenderInline *);

	virtual void render(TWRenderDC *);
	
	void removeLeftborder();
	void removeRightBorder();
	
	TWRenderInline *renderer()const{
		return m_renderer;
	}
	
	virtual void calcStyle();
	
	int flowIndexStart() const{return m_flowIndexStart;}
	int flowIndexEnd() const{return m_flowIndexEnd;}
	void setFlowIndexStart(unsigned int i){m_flowIndexStart=i;}
	void setFlowIndexEnd(unsigned int i){m_flowIndexEnd=i;}

	
};




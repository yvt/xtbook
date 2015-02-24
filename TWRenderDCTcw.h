//
//  TWRenderDCTcw.h
//  XTBook
//
//  Created by Nexhawks on 3/13/11.
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
#include "TWiki/TWRenderDC.h"
#include "TWiki/TWHTMLStyleProxy.h"

class twDC;
class twFont;
class TWRenderManager;

class TWRenderDCTcw: public TWRenderDC {
protected:
    TWHTMLStyleProxy m_style;
    twDC *m_dc;
    TWRenderManager *m_fontManager;
    twPoint m_selectionShift;
public:
    TWRenderDCTcw(twDC *, TWRenderManager *);
    virtual ~TWRenderDCTcw();
    
    virtual void setStyle(const TWHTMLStyleProxy& style){
        m_style=style;
    }
	virtual void fillBackground(const TWRect&);
	virtual void drawBorder(const TWRect&);
	virtual void drawPlaceHolder(const TWRect&);
	virtual bool isRectVisible(const TWRect&);
	virtual TWRenderManager *fontManager(){
		return m_fontManager;
	}
	virtual void fillSelection(const TWRect&);
	
	virtual void debugRenderObject(const TWRect&, TWRenderObject *);
	
	twDC *internalDC() const{
		return m_dc;
	}
	
	void setSelectionShift(const twPoint& p){
		m_selectionShift=p;
	}
};

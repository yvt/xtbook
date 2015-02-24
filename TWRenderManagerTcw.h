//
//  TWRenderManagerTcw.h
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
#include "TWiki/TWRenderManager.h"
#include "TWiki/TWHTMLStyleProxy.h"
#include <tcw/twTypes.h>

class twFont;

class TWViewerImageManager;

class TWRenderManagerTcw: public TWRenderManager{
	static twDC *getImageNotLoadedImage();
	
protected:
	TWViewerImageManager *m_imageManager;
    const twFont *m_font;
	bool m_isXTBFont;
    twColor m_color;
	int m_alpha;
	int m_weight;
	int m_size;
	TWHTMLStyleTextDecoration m_decoration;
public:
    TWRenderManagerTcw(const twFont *,
					   TWViewerImageManager *);
    virtual ~TWRenderManagerTcw();
    
    virtual void setStyle(const TWHTMLStyleProxy& style);
	virtual TWSize sizeForString(const std::wstring&);
	virtual void drawString(const std::wstring&, const TWPoint&,
							TWRenderDC *dc);
	
	virtual TWSize sizeForImage(const std::wstring&);
	virtual void drawImage(const std::wstring&, const TWRect&, TWRenderDC *);
	
	virtual TWSize sizeForListMarker();
	virtual void drawListMarker(const TWRect&, TWRenderDC *);
};

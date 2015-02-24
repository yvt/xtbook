/*
 *  TWRenderInline.h
 *  XTBook
 *
 *  Created by Nexhawks on 3/1/11.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */
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

#include "TWRenderBoxModelObject.h"

class TWRenderInline: public TWRenderBoxModelObject {
private:
	TWRenderManager *m_manager;
	
public:
    TWRenderInline(TWHTMLElement *);
	virtual void layout(TWRenderManager *);
	virtual int contentHeight() const;
	virtual int contentWidth() const;
	
	virtual void render(TWRenderDC *){}
	
	void handleMaximumContentWidth(TWRenderManager *manager);
	void handleMinimumContentWidth(TWRenderManager *manager);
	
};

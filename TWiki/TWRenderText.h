//
//  TWRenderText.h
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

#include "TWRenderReplaced.h"
#include <vector>

class TWRenderText: public TWRenderReplaced{
private:
	TWRenderManager *m_manager;
	mutable std::wstring m_text;
	mutable std::vector<int> m_segments;
	std::vector<short> m_segmentWidths;
	int m_defaultHeight;
	void generateSegments() const;
	void calcSegmentWidths();
	
public:
	TWRenderText(TWHTMLElement *);
	
	virtual void layout(TWRenderManager *);
	
	virtual wchar_t firstChar(int segment) const;
	virtual wchar_t lastChar(int segment) const;
	
	virtual int replacedDefaultWidth() const;
	virtual int replacedDefaultHeight() const;
	
	virtual int contentHeight() const;
	virtual int contentWidth() const;
	virtual int contentWidth(int segment) const;
	virtual int segments() const;
	
	std::wstring substringForSegmentRange(int minSegment, int maxSegment) const;
	
	
	virtual void render(TWRenderFlowContent *, TWRenderDC *);
    
	virtual bool isEmpty() const{return m_text.empty();}
	
	const std::wstring& text(){return m_text;}
	
	virtual void handleMaximumContentWidth(TWRenderManager *manager);
};


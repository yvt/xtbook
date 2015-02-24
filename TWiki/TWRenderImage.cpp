//
//  TWRenderImage.cpp
//  XTBook
//
//  Created by Nexhawks on 3/12/11.
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

#include "TWRenderImage.h"
#include "TWRenderFlowContent.h"
#include "TWRenderDC.h"
#include "TWRenderManager.h"
#include <assert.h>
#include "TWHTMLElement.h"
TWRenderImage::TWRenderImage(TWHTMLElement *element):
TWRenderReplaced(element){
    m_isImage=true;
	m_defaultSize=TWSize(-1, -1);
}

void TWRenderImage::render(TWRenderFlowContent *content, TWRenderDC *dc){
	TWRect screenRect=content->screenRect();
	int segment=content->segment();
	
	assert(segment==0);
	TWRenderReplaced::render(content, dc);
	
	dc->setStyle(style());
	
	TWRect innerRect=screenRect;
	innerRect.x+=paddingLeft()+borderSizeLeft();
	innerRect.y+=paddingTop()+borderSizeTop();
	innerRect.w-=paddingLeft()+paddingRight();
	innerRect.w-=borderSizeLeft()+borderSizeRight();
	innerRect.h-=paddingTop()+paddingBottom();
	innerRect.h-=borderSizeTop()+borderSizeBottom();
	
	
	
	//dc->drawPlaceHolder(innerRect);
	assert(m_manager);
	m_manager->drawImage(url(), innerRect, dc);
}

const std::wstring g_srcAttrName(L"src");
const std::wstring g_emptyUrl;

std::wstring TWRenderImage::url() const{
	if(node()->hasAttribute(g_srcAttrName))
		return node()->getAttribute(g_srcAttrName);
	else
		return g_emptyUrl;
}

void TWRenderImage::layout(TWRenderManager *manager){
	m_manager=manager;
	m_defaultSize=m_manager->sizeForImage(url());
	TWRenderReplaced::layout(manager);
}
int TWRenderImage::replacedDefaultWidth() const{
	assert(m_defaultSize.w!=-1);
	return m_defaultSize.w;
}

int TWRenderImage::replacedDefaultHeight() const{
	assert(m_defaultSize.h!=-1);
	return m_defaultSize.h;
}

void TWRenderImage::handleMaximumContentWidth(TWRenderManager *manager){/*
	m_manager=manager;
	m_defaultSize=m_manager->sizeForImage(url());*/
	TWRenderReplaced::handleMaximumContentWidth(manager);
}


//
//  TWRenderBR.cpp
//  XTBook
//
//  Created by Nexhawks on 4/10/11.
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

#include "TWRenderBR.h"
#include "TWRenderBlock.h"

TWRenderBR::TWRenderBR(TWHTMLElement *element):
TWRenderReplaced(element){
	m_isBR=true;
}

void TWRenderBR::layout(TWRenderManager *manager){
	TWRenderBlock *cb=containingBlock();
	if(cb->isBuildingFlow()){
		cb->flushFlowTags();
		m_rect.w=0; m_rect.h=0;
		cb->newLine(style().clear());
		if(style().clear()!=TWHTMLStyleFloatNone)
			cb->clearFloats();
	}
	
	m_isLayouted=true;
}
void TWRenderBR::handleMaximumContentWidth(TWRenderManager *manager){
	TWRenderBlock *cb=containingBlock();
	cb->maximumContentWidthNewLine();
}

/*
 *  TWRenderObject.h
 *  XTBook
 *
 *  Created by Nexhawks on 2/26/11.
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

#pragma once

#include "TWNode.h"
#include "TWHTMLStyle.h"
#include "TWHTMLStyleProxy.h"

class TWHTMLElement;
class TWRenderDC;
class TWRenderBlock;
class TWRenderManager;


class TWRenderObject : public TWNode {
protected:
	TWHTMLElement *m_node;
    TWHTMLStyleProxy m_style;
	
	bool m_isLayouted TW_BITFIELD(1);
	
    bool m_childrenInline TW_BITFIELD(1);
    bool m_styleValid TW_BITFIELD(1);
	
	bool m_isBR TW_BITFIELD(1);
	bool m_isBlockFlow TW_BITFIELD(1);
	bool m_isBoxModelObject TW_BITFIELD(1);
	bool m_isImage TW_BITFIELD(1);
	bool m_isBlock TW_BITFIELD(1);
	bool m_isInline TW_BITFIELD(1);
	bool m_isView TW_BITFIELD(1);
	bool m_isTable TW_BITFIELD(1);
	bool m_isTableCell TW_BITFIELD(1);
	bool m_isTableRow TW_BITFIELD(1);
	bool m_isTableCol TW_BITFIELD(1);
	bool m_isTableSection TW_BITFIELD(1);
	bool m_isReplaced TW_BITFIELD(1);
	bool m_isRenderFlowObject TW_BITFIELD(1);
	bool m_isRenderFlowContent TW_BITFIELD(1);
	bool m_isRenderFlowInline TW_BITFIELD(1);
	bool m_isRenderFlowLine TW_BITFIELD(1);
	bool m_isText TW_BITFIELD(1);
	bool m_isListMarker TW_BITFIELD(1);
    bool m_isListItem TW_BITFIELD(1);
	
public:
	TWRenderObject(TWHTMLElement *node, const std::wstring&);
	virtual ~TWRenderObject();
	static TWRenderObject *objectWithNode(TWHTMLElement *node,
										  TWRenderObject *parent=NULL);
	
	void calcLogicalWidth(int containerWidth);
	
    TWRenderBlock *containingBlock() const;
    const TWHTMLStyleProxy& style() const;
    virtual void calcStyle();
	
	virtual void layout(TWRenderManager *);
	virtual void render(TWRenderDC *);
    
	bool isBR() const{return m_isBR;}
	bool isBlockFlow() const{return m_isBlockFlow;}
	bool isBoxModelObject() const{return m_isBoxModelObject;}
	bool isImage() const{return m_isImage;}
	bool isBlock() const{return m_isBlock;}
    bool isInline() const{return m_isInline;}
	bool isView() const{return m_isView;}
	bool isTable() const{return m_isTable;}
	bool isTableCell() const{return m_isTableCell;}
	bool isTableRow() const{return m_isTableRow;}
	bool isTableCol() const{return m_isTableCol;}
	bool isTableSection() const{return m_isTableSection;}
	bool isReplaced() const{return m_isReplaced;}	
	bool isListMarker() const{return m_isListMarker;}
	bool isListItem() const{return m_isListItem;}
	bool isRenderFlowObject() const{return m_isRenderFlowObject;}
	bool isRenderFlowContent() const{return m_isRenderFlowContent;}
	bool isRenderFlowInline() const{return m_isRenderFlowInline;}
	bool isRenderFlowLine() const{return m_isRenderFlowLine;}
	bool isText() const{return m_isText;}
	
    bool isFloating() const;
    bool isAnonymous() const{
        return m_node==NULL;
    }
    bool isChildrenInline() const{
        return m_childrenInline;
    }
    void setChildrenInline(bool v){
        m_childrenInline=v;
    }
    
    TWHTMLElement *node() const{return m_node;}
	
	void validateLayout();
};

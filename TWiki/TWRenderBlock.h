/*
 *  TWRenderBlock.h
 *  XTBook
 *
 *  Created by Nexhawks on 2/28/11.
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

#include "TWRenderBox.h"
#include <set>
#include <map>
#include <list>
#include <stack>

class TWRenderInline;
class TWRenderFlowLine;
class TWRenderFlowInline;

class TWRenderBlock : public TWRenderBox {
    enum FloatingObjectType{
        FloatingObjectTypeLeft=0,
        FloatingObjectTypeRight,
        FloatingObjectTypeBoth
    };
    struct FloatingObject{
        TWRect rect;
        TWRenderBox *renderer;
        FloatingObjectType type;
        bool placed;
    };
public:
	enum FlowTagType{
		FlowTagTypeStart=0,
		FlowTagTypeEnd,
		FlowTagTypeContent
	};
	struct FlowTag{
		FlowTagType type;
		TWRenderObject *renderer;
		int segment; // for Content
	};
	struct FlowInfo{
		std::vector<FlowTag> flowTags;  // TODO: make sure this is released.
		std::list<TWRenderInline *> inlineStack;
		std::list<TWRenderFlowInline *> flowStack;
		int flowTagsWidth;
		int currentAvailableWidth;
		bool currentAvailableWidthValid;
		int currentAvailableWidthLineHeight;
		int specifiedLineHeight;
		int maxAvailableWidth;
		void initialize(){
			flowTagsWidth=0;
			flowTags.clear();
			inlineStack.clear();
			flowStack.clear();
			currentAvailableWidthValid=false;
			currentAvailableWidthLineHeight=0;
			specifiedLineHeight=0;
			maxAvailableWidth=0;
		}
	};
private:
	
	TWRenderFlowLine *m_firstLine;
	std::list<TWRenderFlowLine *> m_lines;
	FlowInfo *m_flowInfo;
	
	wchar_t m_lastChar;
	int m_minY, m_maxY;
	int m_lineMinY, m_lineMaxY;
	int m_lastMaxY;
	int m_maximumContentWidth;
	int m_currentMaximumContentWidth;
	int m_minimumContentWidth;
	
	unsigned int m_flowIndexStart;
	unsigned int m_flowIndexEnd;

protected:

    int logicalRightOffsetForLine(int position, int fixedOffset, bool applyTextIndent = true, int* logicalHeightRemaining = 0, int lineHeight=1) const;
    int logicalLeftOffsetForLine(int position, int fixedOffset, bool applyTextIndent = true, int* logicalHeightRemaining = 0, int lineHeight=1) const;
    
public:
	TWRenderBlock(TWHTMLElement *node);
	virtual ~TWRenderBlock(){}
	virtual void layout(TWRenderManager *);
    
    
    virtual int availableLogicalWidth() const;
    int availableLogicalWidthForLine(int position, bool firstLine,
									 int lineHeight=1) const;
    
    int logicalRightOffsetForContent() const;
    int logicalLeftOffsetForContent() const;

    
    int logicalRightOffsetForLine(int position, bool firstLine, int lineHeight=1) const {
		return logicalRightOffsetForLine(position, logicalRightOffsetForContent(), firstLine, NULL, lineHeight); 
	}
    int logicalLeftOffsetForLine(int position, bool firstLine, int lineHeight=1) const { 
		return logicalLeftOffsetForLine(position, logicalLeftOffsetForContent(), firstLine, NULL, lineHeight); 
	}
    
	
	void newLine(TWHTMLStyleFloat clear);
	void addFlowTag(FlowTag);
	void flushFlowTags();
	
	void addFloatingObject(TWRenderBox *);
	
	virtual int maximumContentWidth(TWRenderManager *manager);
	void maximumContentWidthNewLine();
	void addMaximumContentWidth(int);
	
	virtual int minimumContentWidth(TWRenderManager *manager);
	void addMinimumContentWidth(int);
	
	bool isBuildingFlow() const{return m_flowInfo;}
	
	TWRenderFlowLine *firstLine() const{
		return m_firstLine;
	}
	
	 virtual void addChild(TWNode *);
	void clearFloats();
	
	unsigned int flowIndexStart() const{return m_flowIndexStart;}
	unsigned int flowIndexEnd() const{return m_flowIndexEnd;}
	void setFlowIndexStart(unsigned int i){m_flowIndexStart=i;}
	void setFlowIndexEnd(unsigned int i){m_flowIndexEnd=i;}
	
	virtual void calcChildrenFlowIndex();
	
	virtual unsigned int flowIndexAtPoint(const TWPoint&) const;
	
	virtual TWRect drawSelection(TWRenderDC *, int begin, int end); 
	
	virtual void captureSelectionString(std::wstring&, int begin, int end);
	
private:
	
	void clearLines();
	
	int widthOfFlowTag(const FlowTag&) const;
	
    void layoutBlockChildren(TWRenderManager *);
	void layoutInlineChildren(TWRenderManager *);
	void layoutEmpty(TWRenderManager *);
    
	void alignLines();
	
    std::list<FloatingObject> m_floatingObjects;
    
    
    void removeFloatingObject(TWRenderBox *);
    void positionNewFloats();
    
    int lowestFloatLogicalBottom(FloatingObjectType) const;
    
    bool handleFloatingChild(TWRenderBox *child, TWRenderManager *fontManager);
	
    int clearDelta(TWRenderBox *child, int yPos) const;
	
	int fittingLineDelta(int position, int lineHeight, 
						 int lineWidth, bool firstLine) const;
	
	int currentLineHeight() const;
	int currentLineHeightAfterFlushingFlowTags() const;
	
	int maxAvailableWidthForContents() const;
    
    TWRenderBlock *createAnonymousBlock() const;
    void makeChildrenNonInline();
    
	void swapFloatingObjects(TWRenderBlock *, TWPoint offset);
   
	TWPoint offsetToContainingBlock() const;
	
	int currentAvailableWidthForFlow();
	
};

//
//  TWRenderText.cpp
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

#include "TWRenderText.h"
#include "TWRenderBlock.h"
#include "TWRenderManager.h"
#include "TWHTMLText.h"
#include <assert.h>
#include "TWRenderFlowContent.h"
#include "TWRenderDC.h"


static bool canCharCollapsed(wchar_t ch){
	if(ch==L' ' || ch==L'\n' || ch==L'\r')
		return true;
	return false;
}


static std::wstring collapseString(const std::wstring& str){
	std::wstring result;
	std::wstring::size_type pos=0;
	const wchar_t *collapsedChars=L" \n\r\t";
	while(pos<str.size()){
		std::wstring::size_type nextPos;
		nextPos=str.find_first_of(collapsedChars, pos);
		if(nextPos==std::wstring::npos)
			break;
		result+=str.substr(pos, nextPos-pos);
		result+=L" ";
		pos=nextPos;
		nextPos=str.find_first_not_of(collapsedChars, pos);
		if(nextPos==std::wstring::npos)
			nextPos=str.size();
		pos=nextPos;
	}
	result+=str.substr(pos, str.size()-pos);
	return result;
}

enum CharGroup{
	CharGroupAlphabet=0,
	CharGroupNumeric=1,
	CharGroupSymbol=2,
	CharGroupOther
};

static inline CharGroup charGroupForChar(wchar_t c){
	if((c>=L'a' && c<=L'z') || (c>=L'A' && c<=L'Z'))
		return CharGroupAlphabet;
	if(c>=L'0' && c<=L'9')
		return CharGroupNumeric;
	if(c>=1 && c<=255)
		return CharGroupSymbol;
	return CharGroupOther;
}

static bool isCharsBreakable(wchar_t c1, wchar_t c2){
	CharGroup g1=charGroupForChar(c1);
	CharGroup g2=charGroupForChar(c2);
	if(g1!=g2)
		return true;
	if(g1==CharGroupOther ||
	   g2==CharGroupOther)
		return true;
	if(g1==CharGroupSymbol ||
	   g2==CharGroupSymbol)
		return true;
	return false;
}

TWRenderText::TWRenderText(TWHTMLElement *element):
TWRenderReplaced(element),
m_manager(NULL),
m_defaultHeight(-1){
	m_isText=true;
}

void TWRenderText::generateSegments() const{
	if(!m_segments.empty())
		return;
	
	TWHTMLText *htmlText=static_cast<TWHTMLText *>(node());
	
	TWHTMLStyleWhiteSpace ws=style().whitespace();
	
	if(ws==TWHTMLStyleWhiteSpacePre){
		m_text=(htmlText->innerText());
	}else{
		m_text=collapseString(htmlText->innerText());
	}
	
	m_segments.clear();
	
	wchar_t lastChar=0;
	std::wstring::size_type length=m_text.size();
	for(std::wstring::size_type i=0;i<length;i++){
		wchar_t c=m_text[i];
		if(isCharsBreakable(lastChar, c)){
			m_segments.push_back(i);
		}
		
		lastChar=c;
	}
	m_segments.push_back(length);
}

void TWRenderText::calcSegmentWidths(){
	
	assert(m_manager);
	
	m_manager->setStyle(style());
	
	m_segmentWidths.clear();
	
	int segmentCount=segments();
	for(int n=0;n<segmentCount;n++){
		std::wstring subStr=substringForSegmentRange(n, n+1);
		m_segmentWidths.push_back(m_manager->sizeForString(subStr).w);
	}
}

void TWRenderText::layout(TWRenderManager * manager){
	m_manager=manager;
	if(!m_segmentWidths.size())
		calcSegmentWidths();
	m_defaultHeight=m_manager->sizeForString(L"A").h;
	
	TWRenderBox::layout(manager);
	TWRenderBlock *cb=containingBlock();
	calcWidth();
	m_rect.h=style().height().toPixels(replacedDefaultHeight(), style());
	
	if(cb->isBuildingFlow()){
		int segmentCount=segments();
		for(int i=0;i<segmentCount;i++){
			TWRenderBlock::FlowTag tag;
			
			// is line break?
			if(m_segments[i]==m_segments[i+1]-1){
				if(m_text[m_segments[i]]==L'\n'){
					cb->flushFlowTags();
					cb->newLine(style().clear());
					continue;
				}
			}
			
			tag.type=TWRenderBlock::FlowTagTypeContent;
			tag.renderer=this;
			tag.segment=i;
			cb->addFlowTag(tag);
		}
	}
	
	
}

int TWRenderText::replacedDefaultWidth() const{
	m_manager->setStyle(style());
	return m_manager->sizeForString(m_text).w;
}

int TWRenderText::replacedDefaultHeight() const{
	assert(m_defaultHeight!=-1);
	return m_defaultHeight;
}

int TWRenderText::contentWidth() const{
	return replacedDefaultWidth();
}
int TWRenderText::contentHeight() const{
	return replacedDefaultHeight();
}
int TWRenderText::segments() const{
	generateSegments();
	return m_segments.size()-1;
}
int TWRenderText::contentWidth(int segment) const{
	assert(segment>=0);
	assert(segment<segments());
	if(m_segmentWidths.size())
		return m_segmentWidths[segment];
	std::wstring subStr=substringForSegmentRange(segment, segment+1);
	m_manager->setStyle(style());
	return m_manager->sizeForString(subStr).w;
}
wchar_t TWRenderText::firstChar(int segment) const{
	assert(segment>=0);
	assert(segment<segments());
	if(m_text.size())
		return m_text[m_segments[segment]];
	else
		return 0;
}
wchar_t TWRenderText::lastChar(int segment) const{
	assert(segment>=0);
	assert(segment<segments());
	if(m_text.size())
		return m_text[m_segments[segment+1]-1];
	else
		return 0;
}
std::wstring TWRenderText::substringForSegmentRange(int minSegment, int maxSegment) const{
	assert(minSegment>=0);
	assert(maxSegment<=segments());
	assert(minSegment<=maxSegment);
	return m_text.substr(m_segments[minSegment],
						 m_segments[maxSegment]-m_segments[minSegment]);
}

void TWRenderText::render(TWRenderFlowContent *content, TWRenderDC *dc){
	int minSegment=content->minSegment();
	int maxSegment=content->maxSegment();
	
	TWRect screenRect=content->screenRect();
	std::wstring str;
	str=substringForSegmentRange(minSegment, maxSegment);
	
	TWRenderManager *manager=dc->fontManager();
	manager->setStyle(style());
	
	TWPoint pt;
	pt.x=screenRect.x;
	pt.y=screenRect.y+screenRect.h-manager->sizeForString(L"A").h;
	
	manager->drawString(str, pt, dc);
}

void TWRenderText::handleMaximumContentWidth(TWRenderManager *manager){/*
	m_manager=manager;
	if(!m_segmentWidths.size())
		calcSegmentWidths();*/
	TWRenderReplaced::handleMaximumContentWidth(manager);
}


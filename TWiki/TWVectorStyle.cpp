/*
 *  TWVectorStyle.cpp
 *  XTBook
 *
 *  Created by Nexhawks on 2/27/11.
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

#include "TWHTMLElement.h"
#include "TWVectorStyle.h"
#include "TWUtils.h"

static const std::wstring g_idAttrName=L"id";
static const std::wstring g_classAttrName=L"class";
static const std::wstring g_borderAttrName=L"border";
static const std::wstring g_cellPaddingAttrName=L"cellpadding";
static const TWHTMLStyleBorder g_tableBorder(L"1px #aaa solid");
static const TWHTMLStyleBorder g_thumbInnerBorder(L"1px #ccc solid");
static const TWHTMLStyleBorder g_thumbImageBorder(L"1px #ccc solid");
static const TWHTMLStyleBorder g_tocColoursBorder(L"1px #aaa solid");
static const TWHTMLStyleValue g_tablePadding(L"0.2em");
static const TWHTMLStyleValue g_thumbInnerFontSize(L"94%");
static const TWHTMLStyleValue g_thumbCaptionLineHeight(L"1.4em");
static const TWHTMLStyleValue g_thumbMargin(L"0.5em");
static const TWHTMLStyleBorder g_amboxBorder1(L"1px #aaa solid");
static const TWHTMLStyleBorder g_amboxBorder2(L"10px #1e90ff solid");
static const TWHTMLStyleBorder g_amboxBorder3(L"10px #f28500 solid");
static const TWHTMLStyleBorder g_preBorder(L"1px dashed #2f6fab");

void TWVectorStyle::apply(TWHTMLElement *elm, TWHTMLStyle *style){
	
	
	
	switch (elm->type()) {
		case TWHTMLElementTypeBody:
			style->setLineHeight(TWHTMLStyleValue(TWHTMLStyleUnitEm, 1.8f));
			break;
		case TWHTMLElementTypeP:
			style->setMarginTop(TWHTMLStyleValue(TWHTMLStyleUnitEm, .4f));
			style->setMarginBottom(TWHTMLStyleValue(TWHTMLStyleUnitEm, .5f));
			break;
		case TWHTMLElementTypeH2:
			style->setBorderBottom(TWHTMLStyleBorder(L"1px solid #aaa"));
			style->setMarginTop(TWHTMLStyleValueZero);
			style->setMarginBottom(TWHTMLStyleValueZero);
			style->setPaddingTop(TWHTMLStyleValue(TWHTMLStyleUnitEm, .5f));
			style->setPaddingBottom(TWHTMLStyleValue(TWHTMLStyleUnitEm, .17f));
			style->setMarginRight(TWHTMLStyleValue(TWHTMLStyleUnitPixels, 2));
			style->setMarginLeft(TWHTMLStyleValue(TWHTMLStyleUnitPixels, 2));
			style->setFontSize(TWHTMLStyleValue(TWHTMLStyleUnitPercent, 116));
			style->setLineHeight(TWHTMLStyleValue(TWHTMLStyleUnitEm, 1.2f));
			break;
		case TWHTMLElementTypeH3:
			style->setMarginTop(TWHTMLStyleValueZero);
			style->setMarginBottom(TWHTMLStyleValueZero);
			style->setPaddingTop(TWHTMLStyleValue(TWHTMLStyleUnitEm, .5f));
			style->setPaddingBottom(TWHTMLStyleValue(TWHTMLStyleUnitEm, .17f));
			style->setFontSize(TWHTMLStyleValue(TWHTMLStyleUnitPercent, 108));
			break;
		case TWHTMLElementTypeUl:
		case TWHTMLElementTypeOl:
			style->setMarginTop(TWHTMLStyleValue(TWHTMLStyleUnitEm, .3f));
			style->setMarginBottom(TWHTMLStyleValue(TWHTMLStyleUnitEm, 1.5f));
			break;
		case TWHTMLElementTypeDl:
			style->setMarginTop(TWHTMLStyleValue(TWHTMLStyleUnitEm, .2f));
			style->setMarginBottom(TWHTMLStyleValue(TWHTMLStyleUnitEm, 0.5f));
			break;
		case TWHTMLElementTypeA:
			style->setColor(TWHTMLStyleColor(0x06, 0x45, 0xad, 0xff));
			
			break;
		case TWHTMLElementTypePre:
			style->setBorderLeft(g_preBorder);
			style->setBorderRight(g_preBorder);
			style->setBorderTop(g_preBorder);
			style->setBorderBottom(g_preBorder);
			style->setPaddingLeft(TWHTMLStyleValue(TWHTMLStyleUnitPixels, 8));
			style->setPaddingRight(TWHTMLStyleValue(TWHTMLStyleUnitPixels, 8));
			style->setPaddingTop(TWHTMLStyleValue(TWHTMLStyleUnitPixels, 8));
			style->setPaddingBottom(TWHTMLStyleValue(TWHTMLStyleUnitPixels, 8));
			style->setLineHeight(TWHTMLStyleValue(TWHTMLStyleUnitEm, 1.1f));
			style->setBackgroundColor(TWHTMLStyleColor(0xf9, 0xf9, 0xf9, 0xff));
			break;
		default:
			break;
	}
	
	if(0){
		std::wstring id=elm->getAttribute(g_idAttrName);
		if(id==L"infoboxCountry"){
			style->setFloat(TWHTMLStyleFloatRight);
			style->setWidth(TWHTMLStyleValue(TWHTMLStyleUnitPixels, 300));
			style->setClear(TWHTMLStyleFloatRight);
			style->setMarginLeft(TWHTMLStyleValue(TWHTMLStyleUnitPixels, 5));
		}
	}
	
	{
		std::wstring className=elm->getAttribute(g_classAttrName);
		className+=L' ';
		
		if(className.find(L"infobox ")!=std::wstring::npos){
			style->setFloat(TWHTMLStyleFloatRight);
			style->setClear(TWHTMLStyleFloatRight);
			style->setMarginLeft(TWHTMLStyleValue(TWHTMLStyleUnitPixels, 2));
			style->setMarginRight(TWHTMLStyleValue(TWHTMLStyleUnitPixels, 2));
			style->setMarginTop(TWHTMLStyleValue(TWHTMLStyleUnitPixels, 2));
			style->setMarginBottom(TWHTMLStyleValue(TWHTMLStyleUnitPixels, 2));
			
		}
		if(className==L"thumbinner " || className.find(L"infobox ")!=std::wstring::npos){
			style->setBorderLeft(g_thumbInnerBorder);
			style->setBorderRight(g_thumbInnerBorder);
			style->setBorderTop(g_thumbInnerBorder);
			style->setBorderBottom(g_thumbInnerBorder);
			style->setBackgroundColor(TWHTMLStyleColor(0xf9, 0xf9, 0xf9, 0xff));
			style->setFontSize(g_thumbInnerFontSize);
			style->setPaddingLeft(TWHTMLStyleValue(TWHTMLStyleUnitPixels, 4));
			style->setPaddingRight(TWHTMLStyleValue(TWHTMLStyleUnitPixels, 4));
			style->setPaddingTop(TWHTMLStyleValue(TWHTMLStyleUnitPixels, 4));
			style->setPaddingBottom(TWHTMLStyleValue(TWHTMLStyleUnitPixels, 4));
		}else if(className==L"thumbimage " || className==L"thumbborder "){
			style->setBorderLeft(g_thumbImageBorder);
			style->setBorderRight(g_thumbImageBorder);
			style->setBorderTop(g_thumbImageBorder);
			style->setBorderBottom(g_thumbImageBorder);
			
		}else if(className==L"thumbcaption "){
			style->setLineHeight(g_thumbCaptionLineHeight);
		}else if(className==L"toccolours "){
			style->setBorderLeft(g_tocColoursBorder);
			style->setBorderRight(g_tocColoursBorder);
			style->setBorderTop(g_tocColoursBorder);
			style->setBorderBottom(g_tocColoursBorder);
			style->setBackgroundColor(TWHTMLStyleColor(0xf9, 0xf9, 0xf9, 0xff));
			style->setPaddingLeft(TWHTMLStyleValue(TWHTMLStyleUnitPixels, 5));
			style->setPaddingRight(TWHTMLStyleValue(TWHTMLStyleUnitPixels, 5));
			style->setPaddingTop(TWHTMLStyleValue(TWHTMLStyleUnitPixels, 5));
			style->setPaddingBottom(TWHTMLStyleValue(TWHTMLStyleUnitPixels, 5));
		}else if(className.find(L"thumb ")!=std::wstring::npos){
			style->setMarginLeft(g_thumbMargin);
			style->setMarginRight(g_thumbMargin);
			style->setMarginTop(g_thumbMargin);
			style->setMarginBottom(g_thumbMargin);
			style->setFloat(TWHTMLStyleFloatRight);
			style->setClear(TWHTMLStyleFloatRight);
		}else if(className.find(L"ambox-content ")!=std::wstring::npos){
			style->setBorderLeft(g_amboxBorder3);
			style->setBorderRight(g_amboxBorder1);
			style->setBorderTop(g_amboxBorder1);
			style->setBorderBottom(g_amboxBorder1);
			style->setBackgroundColor(TWHTMLStyleColor(0xfb, 0xfb, 0xfb, 0xff));
		}else if(className.find(L"ambox ")!=std::wstring::npos){
			style->setBorderLeft(g_amboxBorder2);
			style->setBorderRight(g_amboxBorder1);
			style->setBorderTop(g_amboxBorder1);
			style->setBorderBottom(g_amboxBorder1);
			style->setBackgroundColor(TWHTMLStyleColor(0xfb, 0xfb, 0xfb, 0xff));
			style->setMarginLeft(TWHTMLStyleValue(TWHTMLStyleUnitPixels, 2));
			style->setMarginRight(TWHTMLStyleValue(TWHTMLStyleUnitPixels, 2));
			style->setMarginTop(TWHTMLStyleValue(TWHTMLStyleUnitPixels, 2));
			style->setMarginBottom(TWHTMLStyleValue(TWHTMLStyleUnitPixels, 2));
		}
		if(className.find(L"tright ")!=std::wstring::npos){
			style->setFloat(TWHTMLStyleFloatRight);
			style->setClear(TWHTMLStyleFloatRight);
		}
		if(className.find(L"tleft ")!=std::wstring::npos){
			style->setFloat(TWHTMLStyleFloatLeft);
			style->setClear(TWHTMLStyleFloatLeft);
		}
		if(className.find(L"sister-wiki ")!=std::wstring::npos ||
		   className.find(L"sisterproject")!=std::wstring::npos){
			style->setDisplay(TWHTMLStyleDisplayNone);
		}
		/*if(className.find(L"infoboxCountry")!=std::wstring::npos){
			style->setBorderLeft(g_thumbInnerBorder);
			style->setBorderRight(g_thumbInnerBorder);
			style->setBorderTop(g_thumbInnerBorder);
			style->setBorderBottom(g_thumbInnerBorder);
			style->setBackgroundColor(TWHTMLStyleColor(0xf9, 0xf9, 0xf9, 0xff));
			style->setPaddingLeft(TWHTMLStyleValueZero);
			style->setPaddingRight(TWHTMLStyleValueZero);
			style->setPaddingTop(TWHTMLStyleValueZero);
			style->setPaddingBottom(TWHTMLStyleValueZero);
			style->setMarginLeft(TWHTMLStyleValueZero);
			style->setMarginRight(TWHTMLStyleValueZero);
			style->setMarginTop(TWHTMLStyleValueZero);
			style->setMarginBottom(TWHTMLStyleValueZero);
		}*/
	}
	
	TWHTMLElement *parent=static_cast<TWHTMLElement *>(elm->parent());
	if(parent){
		std::wstring className=parent->getAttribute(g_classAttrName);
		std::wstring border;
		std::wstring cellpadding;
		TWHTMLElement *parent2=static_cast<TWHTMLElement *>(parent->parent());
		if(parent2){
			className+=parent2->getAttribute(g_classAttrName);
			TWHTMLElement *parent3=static_cast<TWHTMLElement *>(parent2->parent());
			if(parent3){
				className+=parent3->getAttribute(g_classAttrName);
				if(parent3->type()==TWHTMLElementTypeTable){
					if(parent3->hasAttribute(g_borderAttrName)){
						border=parent3->getAttribute(g_borderAttrName);
					}
					if(parent3->hasAttribute(g_cellPaddingAttrName)){
						cellpadding=parent3->getAttribute(g_cellPaddingAttrName);
					}
				}
			}
			if(parent2->type()==TWHTMLElementTypeTable){
				if(parent2->hasAttribute(g_borderAttrName)){
					border=parent2->getAttribute(g_borderAttrName);
				}
				if(parent2->hasAttribute(g_cellPaddingAttrName)){
					cellpadding=parent2->getAttribute(g_cellPaddingAttrName);
				}
			}
		}
		if(elm->type()==TWHTMLElementTypeTd || elm->type()==TWHTMLElementTypeTh){
			if((!border.empty())){
				int borderWidth=TWUtils::intValueFromString(border);
				if(borderWidth){
					TWHTMLStyleBorder hborder;
					hborder.width=TWHTMLStyleValue(TWHTMLStyleUnitPixels, borderWidth);
					hborder.style=TWHTMLStyleBorderStyleSolid;
					hborder.color=TWHTMLStyleColor(0x7f, 0x7f, 0x7f, 0xff);
					style->setBorderLeft(hborder);
					style->setBorderRight(hborder);
					style->setBorderTop(hborder);
					style->setBorderBottom(hborder);
				}
			}
			if(!cellpadding.empty()){
				int paddingWidth=TWUtils::intValueFromString(cellpadding);
				TWHTMLStyleValue value(TWHTMLStyleUnitPixels, paddingWidth);
				style->setPaddingLeft(value);
				style->setPaddingRight(value);
				style->setPaddingTop(value);
				style->setPaddingBottom(value);
			}
			if(className.find(L"wikitable")!=std::wstring::npos){
				switch(elm->type()){
					case TWHTMLElementTypeTh:
						style->setBackgroundColor(TWHTMLStyleColor(0xf2, 0xf2, 0xf2, 0xff));
						style->setTextAlign(TWHTMLStyleTextAlignCenter);
						style->setBorderLeft(g_tableBorder);
						style->setBorderRight(g_tableBorder);
						style->setBorderTop(g_tableBorder);
						style->setBorderBottom(g_tableBorder);
						style->setPaddingLeft(g_tablePadding);
						style->setPaddingRight(g_tablePadding);
						style->setPaddingTop(g_tablePadding);
						style->setPaddingBottom(g_tablePadding);
						break;
					case TWHTMLElementTypeTd:
						style->setBorderLeft(g_tableBorder);
						style->setBorderRight(g_tableBorder);
						style->setBorderTop(g_tableBorder);
						style->setBorderBottom(g_tableBorder);
						style->setBackgroundColor(TWHTMLStyleColor(0xf9, 0xf9, 0xf9, 0xff));
						style->setPaddingLeft(g_tablePadding);
						style->setPaddingRight(g_tablePadding);
						style->setPaddingTop(g_tablePadding);
						style->setPaddingBottom(g_tablePadding);
						
						break;
					default:
						break;
				}
			}
		}
	}
	if(elm->type()==TWHTMLElementTypeTable){
		if(elm->parent()){
			if(static_cast<TWHTMLElement *>(elm->parent())->type()==TWHTMLElementTypeBody){
				//style->setWidth(TWHTMLStyleValue(TWHTMLStyleUnitPercent, 40));
			}
		}
	}
}

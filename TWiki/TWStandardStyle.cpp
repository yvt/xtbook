//
//  TWStandardStyle.cpp
//  XTBook
//
//  Created by Nexhawks on 4/17/11.
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

#include "TWStandardStyle.h"
#include "TWHTMLElement.h"
#include "TWHTMLStyle.h"

void TWStandardStyle::apply(TWHTMLElement *element, TWHTMLStyle *style){
	std::wstring str;
	
	TWHTMLElement *parent=static_cast<TWHTMLElement *>(element->parent());
	
	// ref: http://www.w3.org/TR/CSS2/sample.html
	// ref: http://gabrieleromanato.altervista.org/the-css-switch-project/december-2006/default-style-sheet-for-html-elements/style.html
	
	if(element->type()==TWHTMLElementTypeHtml || 
	   element->type()==TWHTMLElementTypeDiv || 
	   element->type()==TWHTMLElementTypeMap || 
	   element->type()==TWHTMLElementTypeDt || 
	   element->type()==TWHTMLElementTypeIsindex || 
	   element->type()==TWHTMLElementTypeForm || 
	   element->type()==TWHTMLElementTypeBody || 
	   element->type()==TWHTMLElementTypeP || 
	   element->type()==TWHTMLElementTypeDl || 
	   element->type()==TWHTMLElementTypeDd || 
	   element->type()==TWHTMLElementTypeBlockquote || 
	   element->type()==TWHTMLElementTypeAddress || 
	   element->type()==TWHTMLElementTypeCenter || 
	   element->type()==TWHTMLElementTypeH1 || 
	   element->type()==TWHTMLElementTypeH2 || 
	   element->type()==TWHTMLElementTypeH3 ||
	   element->type()==TWHTMLElementTypeH4 || 
	   element->type()==TWHTMLElementTypeH5 || 
	   element->type()==TWHTMLElementTypeH6 || 
	   element->type()==TWHTMLElementTypePre){
		style->setDisplay(TWHTMLStyleDisplayBlock);
	}
	
	
	
	if(element->type()==TWHTMLElementTypeHead){
		style->setDisplay(TWHTMLStyleDisplayNone);
	}else if(element->type()==TWHTMLElementTypeLi){
		style->setDisplay(TWHTMLStyleDisplayListItem);
	}else if(element->type()==TWHTMLElementTypeTable){
		style->setDisplay(TWHTMLStyleDisplayTable);
		// TODO: border-spacing: 2px;
	}else if(element->type()==TWHTMLElementTypeTr){
		style->setDisplay(TWHTMLStyleDisplayTableRow);
	}else if(element->type()==TWHTMLElementTypeThead){
		style->setDisplay(TWHTMLStyleDisplayTableHeaderGroup);
	}else if(element->type()==TWHTMLElementTypeTbody){
		style->setDisplay(TWHTMLStyleDisplayTableRowGroup);
	}else if(element->type()==TWHTMLElementTypeTfoot){
		style->setDisplay(TWHTMLStyleDisplayTableFooterGroup);
		// TODO: vertical-align: middle;
	}else if(element->type()==TWHTMLElementTypeCol){
		style->setDisplay(TWHTMLStyleDisplayTableCol);
	}else if(element->type()==TWHTMLElementTypeColgroup){
		style->setDisplay(TWHTMLStyleDisplayTableColGroup);
	}else if(element->type()==TWHTMLElementTypeTd){
		style->setDisplay(TWHTMLStyleDisplayTableCell);
	}else if(element->type()==TWHTMLElementTypeTh){
		style->setDisplay(TWHTMLStyleDisplayTableCell);
		style->setTextAlign(TWHTMLStyleTextAlignCenter);
			style->setFontWeight(300);
	}else if(element->type()==TWHTMLElementTypeCaption){
		style->setDisplay(TWHTMLStyleDisplayTableCaption);
		style->setTextAlign(TWHTMLStyleTextAlignCenter);
	}else if(element->type()==TWHTMLElementTypeBody){
		style->setPaddingLeft(TWHTMLStyleValue(TWHTMLStyleUnitPixels, 8));
		style->setPaddingRight(TWHTMLStyleValue(TWHTMLStyleUnitPixels, 8));
		style->setPaddingTop(TWHTMLStyleValue(TWHTMLStyleUnitPixels, 8));
		style->setPaddingBottom(TWHTMLStyleValue(TWHTMLStyleUnitPixels, 8));
		style->setFontSize(TWHTMLStyleValue(TWHTMLStyleUnitPixels, 11));
		style->setBackgroundColor(TWHTMLStyleColor(L"#ffffff"));
	}else if(element->type()==TWHTMLElementTypeH1){
		style->setFontSize(TWHTMLStyleValue(TWHTMLStyleUnitEm, 2.f));
		style->setMarginTop(TWHTMLStyleValue(TWHTMLStyleUnitEm, .67f));
		style->setMarginBottom(TWHTMLStyleValue(TWHTMLStyleUnitEm, .67f));
		style->setMarginLeft(TWHTMLStyleValueZero);
		style->setMarginRight(TWHTMLStyleValueZero);
	}else if(element->type()==TWHTMLElementTypeH2){
		style->setFontSize(TWHTMLStyleValue(TWHTMLStyleUnitEm, 1.5f));
		style->setMarginTop(TWHTMLStyleValue(TWHTMLStyleUnitEm, .75f));
		style->setMarginBottom(TWHTMLStyleValue(TWHTMLStyleUnitEm, .75f));
		style->setMarginLeft(TWHTMLStyleValueZero);
		style->setMarginRight(TWHTMLStyleValueZero);
	}else if(element->type()==TWHTMLElementTypeH3){
		style->setFontSize(TWHTMLStyleValue(TWHTMLStyleUnitEm, 1.17f));
		style->setMarginTop(TWHTMLStyleValue(TWHTMLStyleUnitEm, .83f));
		style->setMarginBottom(TWHTMLStyleValue(TWHTMLStyleUnitEm, .83f));
		style->setMarginLeft(TWHTMLStyleValueZero);
		style->setMarginRight(TWHTMLStyleValueZero);
	}else if(element->type()==TWHTMLElementTypeH4){
		style->setFontSize(TWHTMLStyleValue(TWHTMLStyleUnitEm, 1.f));
		style->setMarginTop(TWHTMLStyleValue(TWHTMLStyleUnitEm, 1.12f));
		style->setMarginBottom(TWHTMLStyleValue(TWHTMLStyleUnitEm, 1.12f));
		style->setMarginLeft(TWHTMLStyleValueZero);
		style->setMarginRight(TWHTMLStyleValueZero);
	}else if(element->type()==TWHTMLElementTypeH5){
		style->setFontSize(TWHTMLStyleValue(TWHTMLStyleUnitEm, .83f));
		style->setMarginTop(TWHTMLStyleValue(TWHTMLStyleUnitEm, 1.5f));
		style->setMarginBottom(TWHTMLStyleValue(TWHTMLStyleUnitEm, 1.5f));
		style->setMarginLeft(TWHTMLStyleValueZero);
		style->setMarginRight(TWHTMLStyleValueZero);
	}else if(element->type()==TWHTMLElementTypeH6){
		style->setFontSize(TWHTMLStyleValue(TWHTMLStyleUnitEm, .75f));
		style->setMarginTop(TWHTMLStyleValue(TWHTMLStyleUnitEm, 1.67f));
		style->setMarginBottom(TWHTMLStyleValue(TWHTMLStyleUnitEm, 1.67f));
		style->setMarginLeft(TWHTMLStyleValueZero);
		style->setMarginRight(TWHTMLStyleValueZero);
	}else if(element->type()==TWHTMLElementTypeBlockquote){
		style->setMarginLeft(TWHTMLStyleValue(TWHTMLStyleUnitPixels, 40));
		style->setMarginRight(TWHTMLStyleValue(TWHTMLStyleUnitPixels, 40));
	}else if(element->type()==TWHTMLElementTypePre){
		style->setWhitespace(TWHTMLStyleWhiteSpacePre);
		
	}else if(element->type()==TWHTMLElementTypeBig){
		style->setFontSize(TWHTMLStyleValue(TWHTMLStyleUnitEm, 1.17f));
	}else if(element->type()==TWHTMLElementTypeSmall){
		style->setFontSize(TWHTMLStyleValue(TWHTMLStyleUnitEm, 0.83f));
	}else if(element->type()==TWHTMLElementTypeSub){
		style->setFontSize(TWHTMLStyleValue(TWHTMLStyleUnitEm, 0.83f));
		// TODO: vertical-align: sub;
	}else if(element->type()==TWHTMLElementTypeSup){
		style->setFontSize(TWHTMLStyleValue(TWHTMLStyleUnitEm, 0.83f));
		// TODO: vertical-align: super;
	}else if(element->type()==TWHTMLElementTypeS){
		style->setTextDecoration(TWHTMLStyleTextDecorationLineThrough);
	}else if(element->type()==TWHTMLElementTypeStrike){
		style->setTextDecoration(TWHTMLStyleTextDecorationLineThrough);
	}else if(element->type()==TWHTMLElementTypeDel){
		style->setTextDecoration(TWHTMLStyleTextDecorationLineThrough);
	}else if(element->type()==TWHTMLElementTypeHr){
		TWHTMLStyleBorder border(L"1px solid gray");
		style->setBorderBottom(border);
		style->setBorderTop(border);
		style->setBorderLeft(border);
		style->setBorderRight(border);
		style->setDisplay(TWHTMLStyleDisplayBlock);
		style->setHeight(TWHTMLStyleValue(TWHTMLStyleUnitPixels, 2));
		style->setMarginTop(TWHTMLStyleValue(TWHTMLStyleUnitEm, .5f));
		style->setMarginBottom(TWHTMLStyleValue(TWHTMLStyleUnitEm, .5f));
		style->setMarginLeft(TWHTMLStyleValueAuto);
		style->setMarginRight(TWHTMLStyleValueAuto);
	}else if(element->type()==TWHTMLElementTypeOl){
		// TODO: list-style-type: decimal;
		style->setDisplay(TWHTMLStyleDisplayBlock);
	}else if(element->type()==TWHTMLElementTypeUl){
		style->setDisplay(TWHTMLStyleDisplayBlock); // TODO: list
	}else if(element->type()==TWHTMLElementTypeU){
		style->setTextDecoration(TWHTMLStyleTextDecorationUnderline);
	}else if(element->type()==TWHTMLElementTypeIns){
		style->setTextDecoration(TWHTMLStyleTextDecorationUnderline);
	}else if(element->type()==TWHTMLElementTypeCenter){
		style->setTextAlign(TWHTMLStyleTextAlignCenter);
	}else if(element->type()==TWHTMLElementTypeA){
		style->setTextDecoration(TWHTMLStyleTextDecorationUnderline);
		style->setColor(TWHTMLStyleColor(L"#0000ff"));
	}
	
	if(element->type()==TWHTMLElementTypeH4 ||
	   element->type()==TWHTMLElementTypeP ||
	   element->type()==TWHTMLElementTypeBlockquote ||
	   element->type()==TWHTMLElementTypeUl ||
	   element->type()==TWHTMLElementTypeFieldset ||
	   element->type()==TWHTMLElementTypeForm ||
	   element->type()==TWHTMLElementTypeOl ||
	   element->type()==TWHTMLElementTypeDl ||
	   element->type()==TWHTMLElementTypeDir ||
	   element->type()==TWHTMLElementTypeMenu ||
	   element->type()==TWHTMLElementTypeH4){
		style->setMarginTop(TWHTMLStyleValue(TWHTMLStyleUnitEm, 1.12f));
		style->setMarginBottom(TWHTMLStyleValue(TWHTMLStyleUnitEm, 1.12f));
		style->setMarginLeft(TWHTMLStyleValueZero);
		style->setMarginRight(TWHTMLStyleValueZero);
	}
	
	if(element->type()==TWHTMLElementTypeH1 ||
	   element->type()==TWHTMLElementTypeH2 ||
	   element->type()==TWHTMLElementTypeH3 ||
	   element->type()==TWHTMLElementTypeH4 ||
	   element->type()==TWHTMLElementTypeH5 ||
	   element->type()==TWHTMLElementTypeH6 ||
	   element->type()==TWHTMLElementTypeB ||
	   element->type()==TWHTMLElementTypeStrong){
		style->setFontWeight(300);
	}
	
	if(element->type()==TWHTMLElementTypeI ||
	   element->type()==TWHTMLElementTypeCite ||
	   element->type()==TWHTMLElementTypeEm ||
	   element->type()==TWHTMLElementTypeVar ||
	   element->type()==TWHTMLElementTypeAddress){
		// TODO: font-style: italic;
	}
	
	if(element->type()==TWHTMLElementTypeOl ||
	   element->type()==TWHTMLElementTypeUl ||
	   element->type()==TWHTMLElementTypeDir ||
	   element->type()==TWHTMLElementTypeMenu ||
	   element->type()==TWHTMLElementTypeDd){
		style->setMarginLeft(TWHTMLStyleValue(TWHTMLStyleUnitPixels, 40));
	}
	
	
	if((element->type()==TWHTMLElementTypeOl ||
		element->type()==TWHTMLElementTypeUl) && 
	   parent){
		if(parent->type()==TWHTMLElementTypeOl || 
		   parent->type()==TWHTMLElementTypeUl){
			style->setMarginTop(TWHTMLStyleValueZero);
			style->setMarginBottom(TWHTMLStyleValueZero);
		}
	}
	
	
#if 0
	style->setMarginTop(TWHTMLStyleValue(TWHTMLStyleUnitPixels, 15));
	style->setMarginBottom(TWHTMLStyleValue(TWHTMLStyleUnitPixels, 15));
	style->setMarginLeft(TWHTMLStyleValue(TWHTMLStyleUnitPixels, 15));
	style->setMarginRight(TWHTMLStyleValue(TWHTMLStyleUnitPixels, 15));
	style->setPaddingTop(TWHTMLStyleValue(TWHTMLStyleUnitPixels, 15));
	style->setPaddingBottom(TWHTMLStyleValue(TWHTMLStyleUnitPixels, 15));
	style->setPaddingLeft(TWHTMLStyleValue(TWHTMLStyleUnitPixels, 15));
	style->setPaddingRight(TWHTMLStyleValue(TWHTMLStyleUnitPixels, 15));
#endif
}

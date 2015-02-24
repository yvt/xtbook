/*
 *  TWHTMLStyle.cpp
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

#include "TWHTMLStyle.h"
#include "TWUtils.h"
#include <assert.h>
#include "CSSColors.h"
#include "Styles.h"
#include "TWStringSplitter.h"
#include "TWHTMLStyleProxy.h"
#include "TWNumber.h"

static TWHTMLStyleValue g_fontSizes[]={
	TWHTMLStyleValue(TWHTMLStyleUnitPixels, 8),
	TWHTMLStyleValue(TWHTMLStyleUnitPixels, 9),
	TWHTMLStyleValue(TWHTMLStyleUnitPixels, 10),
	TWHTMLStyleValue(TWHTMLStyleUnitPixels, 11),
	TWHTMLStyleValue(TWHTMLStyleUnitPixels, 12),
	TWHTMLStyleValue(TWHTMLStyleUnitPixels, 13),
	TWHTMLStyleValue(TWHTMLStyleUnitPixels, 14),
	TWHTMLStyleValue(TWHTMLStyleUnitPercent, 120),
	TWHTMLStyleValue(TWHTMLStyleUnitPercent, 80),
};

static int intFromHexChar(wchar_t ch){
	if(ch>=L'0' && ch<=L'9')
		return (int)(ch-L'0');
	else if(ch>=L'a' && ch<='f')
		return 10+(int)(ch-L'a');
	else if(ch>=L'A' && ch<='F')
		return 10+(int)(ch-L'A');
	return 0;
}

struct UnitFactor{
#if TWHTMLSTYLE_FIXED
	int fixedRate;
#else
	float floatRate;
#endif
	UnitFactor(){
#if TWHTMLSTYLE_FIXED
		fixedRate=256;
#else
		floatRate=1.f;
#endif
	}
	UnitFactor(float rate){
#if TWHTMLSTYLE_FIXED
		fixedRate=(int)(rate*256.f);
#else
		floatRate=rate;
#endif
	}
};



TWHTMLStyleValue::TWHTMLStyleValue(const std::wstring&str){
	std::wstring inStr=TWUtils::trimString(str);
	TWUtils::toLowerInplace(inStr);
	if(inStr==L"auto"){
		unit=TWHTMLStyleUnitAuto;
		return;
	}
	float value=TWUtils::floatValueFromString(inStr);
	UnitFactor factor;
	if(value==0.f){
		unit=TWHTMLStyleUnitPixels;
#if TWHTMLSTYLE_FIXED
		fixedValue=0;
#else
		intValue=0;
		floatValue=0;
#endif
		return;
	}
	if(inStr.size()>=1){
		if(inStr.substr(inStr.size()-1, 1)==L"%"){
			unit=TWHTMLStyleUnitPercent;
		}
	}
	if(inStr.size()>=2){
	
		if(inStr.substr(inStr.size()-2, 2)==L"px"){
			unit=TWHTMLStyleUnitPixels;
		}
		if(inStr.substr(inStr.size()-2, 2)==L"pt"){
			unit=TWHTMLStyleUnitPixels;
		}
		if(inStr.substr(inStr.size()-2, 2)==L"in"){
			unit=TWHTMLStyleUnitPixels;
			value*=72.f;
		}
		if(inStr.substr(inStr.size()-2, 2)==L"cm"){
			unit=TWHTMLStyleUnitPixels;
			value*=0.393700787f*72.f;
		}
		if(inStr.substr(inStr.size()-2, 2)==L"mm"){
			unit=TWHTMLStyleUnitPixels;
			value*=0.0393700787f*72.f;
		}
		if(inStr.substr(inStr.size()-2, 2)==L"pc"){
			unit=TWHTMLStyleUnitPixels;
			value*=12.f;
		}
		if(inStr.substr(inStr.size()-2, 2)==L"em"){
			unit=TWHTMLStyleUnitEm;
		}
		if(inStr.substr(inStr.size()-2, 2)==L"ex"){
			unit=TWHTMLStyleUnitEm;
			value*=.5f;
		}
	}
#if TWHTMLSTYLE_FIXED
	fixedValue=(int)(value*256.f);
#else
	intValue=(int)value;
	floatValue=value;
#endif
}

int TWHTMLStyleValue::toPixels(int referenceSize, const TWHTMLStyleProxy& style) const{
	if(unit==TWHTMLStyleUnitAuto)
		return referenceSize; // FIXME: this is not good.
	else if(unit==TWHTMLStyleUnitPixels)
#if TWHTMLSTYLE_FIXED
		return (fixedValue+128)>>8;
#else	
		return intValue;
#endif
	else if(unit==TWHTMLStyleUnitPercent)
#if TWHTMLSTYLE_FIXED
		return (fixedValue*referenceSize+12800)/25600;
#else
		return (intValue*referenceSize+50)/100;
#endif
	else if(unit==TWHTMLStyleUnitEm){
#if TWHTMLSTYLE_FIXED
		TWHTMLStyleValue fontSize=style.fontSize();
		if(fontSize.unit==TWHTMLStyleUnitEm)
			return (fixedValue*style.inheritedFontSize()+128)>>8;
		return (fixedValue*(style.fontSize().toPixels(style.inheritedFontSize(), style))+128)>>8;
#else
		TWHTMLStyleValue fontSize=style.fontSize();
		if(fontSize.unit==TWHTMLStyleUnitEm)
			return (int)(floatValue*(float)style.inheritedFontSize()+.5f);
		return (int)(floatValue*(float)(style.fontSize().toPixels(style.inheritedFontSize(), style))+.5f);
#endif
	}
	assert(false);
	return 0;
}

int TWHTMLStyleValue::toPixels(int referenceSize, const TWHTMLStyle& style) const{
	return toPixels(referenceSize, TWHTMLStyleProxy(style));
}

TWHTMLStyleColor::TWHTMLStyleColor(const std::wstring& str){
	std::wstring inStr=TWUtils::trimString(str);
	TWUtils::toLowerInplace(inStr);
	red=green=blue=alpha=0;
	if(inStr.size()==0){
		// empty.
		return;
	}
	if(inStr[0]=='#'){
		if(inStr.size()==4){
			// #rgb format
			red=intFromHexChar(inStr[1])*0x11;
			green=intFromHexChar(inStr[2])*0x11;
			blue=intFromHexChar(inStr[3])*0x11;
			alpha=255;
		}else if(inStr.size()==7){
			// #rrggbb format
			red=intFromHexChar(inStr[1])*0x10+intFromHexChar(inStr[2]);
			green=intFromHexChar(inStr[3])*0x10+intFromHexChar(inStr[4]);
			blue=intFromHexChar(inStr[5])*0x10+intFromHexChar(inStr[6]);
			alpha=255;
		}
	}else if(inStr.substr(0, 4)==L"rgb(" && inStr[inStr.size()-1]==L')'){
		TWStringSplitter<4> strs(inStr, L",",
								 4, inStr.size()-5);
		
		if(strs.elementCount()==3){
			// rgb(red, green, blue) format
			red=TWUtils::intValueFromString(strs[0]);
			green=TWUtils::intValueFromString(strs[1]);
			blue=TWUtils::intValueFromString(strs[2]);
			alpha=255;
		}
	}else if(inStr.substr(0, 5)==L"rgba(" && inStr[inStr.size()-1]==L')'){
		TWStringSplitter<5> strs(inStr, L",",
								 5, inStr.size()-6);
		if(strs.elementCount()==4){
			// rgba(red, green, blue, alpha) format
			red=TWUtils::intValueFromString(strs[0]);
			green=TWUtils::intValueFromString(strs[1]);
			blue=TWUtils::intValueFromString(strs[2]);
			alpha=(uint8_t)(TWUtils::floatValueFromString(strs[3])*255.f);
			// TODO: support for alpha
		}
	}else{
		// maybe color name.
		uint32_t rgbValue=CSSColorWithName(inStr.c_str());
		if(rgbValue!=CSSColorNotFound){
			red=(rgbValue&  0x00ff0000)>>16;
			green=(rgbValue&0x0000ff00)>>8;
			blue=(rgbValue& 0x000000ff)>>0;
			alpha=255;
		}
		
	}
}

TWHTMLStyleBorder::TWHTMLStyleBorder(const std::wstring& str):
width(TWHTMLStyleUnitPixels, 1),
style(TWHTMLStyleBorderStyleNone),
color(){
	std::wstring trimmedString=TWUtils::trimString(str);
	TWStringSplitter<4> strs(trimmedString, L" ");
	for(unsigned int i=0;i<strs.elementCount();i++){
		switch(strs[i][0]){
			case L'0':
			case L'1':
			case L'2':
			case L'3':
			case L'4':
			case L'5':
			case L'6':
			case L'7':
			case L'8':
			case L'9':
			case L'.':
				width=TWHTMLStyleValue(strs[i]);
				break;
			case L't':
				if(!wcscmp(strs[i], L"thin")){
					width=TWHTMLStyleValue(TWHTMLStyleUnitPixels, 1);
					break;
				}else if(!wcscmp(strs[i], L"thick")){
					width=TWHTMLStyleValue(TWHTMLStyleUnitPixels, 3);
					break;
				}
			case L'm':
				if(!wcscmp(strs[i], L"medium")){
					width=TWHTMLStyleValue(TWHTMLStyleUnitPixels, 2);
					break;
				}
			case L'd':
				if(!wcscmp(strs[i], L"double")){
					setStyle(strs[i]);
					break;
				}else if(!wcscmp(strs[i], L"dotted")){
					setStyle(strs[i]);
					break;
				}else if(!wcscmp(strs[i], L"dashed")){
					setStyle(strs[i]);
					break;
				}
			case L's':
				if(!wcscmp(strs[i], L"solid")){
					setStyle(strs[i]);
					break;
				}
			case L'i':
				if(!wcscmp(strs[i], L"inset")){
					setStyle(strs[i]);
					break;
				}
			case L'o':
				if(!wcscmp(strs[i], L"outset")){
					setStyle(strs[i]);
					break;
				}
			case L'r':
				if(!wcscmp(strs[i], L"ridge")){
					setStyle(strs[i]);
					break;
				}
			case L'g':
				if(!wcscmp(strs[i], L"groove")){
					setStyle(strs[i]);
					break;
				}
			case L'n':
				if(!wcscmp(strs[i], L"none")){
					setStyle(strs[i]);
					break;
				}
			case L'a':
				if(!wcscmp(strs[i], L"auto")){
					width=TWHTMLStyleValue(strs[i]);
					break;
				}
			default:
				color=TWHTMLStyleColor(strs[i]);
		}
	}
   /* if(strs.elementCount()==3){
		switch(strs[0][0]){
			case L't':
				if(!wcscmp(strs[0], L"thin")){
					width=TWHTMLStyleValue(TWHTMLStyleUnitPixels, 1);
					break;
				}else if(!wcscmp(strs[0], L"thick")){
					width=TWHTMLStyleValue(TWHTMLStyleUnitPixels, 3);
					break;
				}
			case L'm':
				if(!wcscmp(strs[0], L"medium")){
					width=TWHTMLStyleValue(TWHTMLStyleUnitPixels, 2);
					break;
				}
			default:
				width=TWHTMLStyleValue(strs[0]);
		}
		
        setStyle(strs[1]);
        color=TWHTMLStyleColor(strs[2]);
    }*/
}

void TWHTMLStyleBorder::setStyle(std::wstring const &inStr){
    std::wstring str=TWUtils::trimString(inStr);
	TWUtils::toLowerInplace(str);
    style=TWHTMLStyleBorderStyleNone;
    if(str==L"dotted")
        style=TWHTMLStyleBorderStyleDotted;
    else if(str==L"dashed")
        style=TWHTMLStyleBorderStyleDashed;
    else if(str==L"solid")
        style=TWHTMLStyleBorderStyleSolid;
    else if(str==L"double")
        style=TWHTMLStyleBorderStyleDouble;
    else if(str==L"groove")
        style=TWHTMLStyleBorderStyleGroove;
    else if(str==L"ridge")
        style=TWHTMLStyleBorderStyleRidge;
    else if(str==L"inset")
        style=TWHTMLStyleBorderStyleInset;
    else if(str==L"outset")
        style=TWHTMLStyleBorderStyleOutset;
}

TWHTMLStyle::TWHTMLStyle():
m_display(TWHTMLStyleDisplayInline),
m_visibility(true),
m_width(TWHTMLStyleValueAuto),
m_height(TWHTMLStyleValueAuto),
m_marginLeft(TWHTMLStyleValueZero),
m_marginRight(TWHTMLStyleValueZero),
m_marginTop(TWHTMLStyleValueZero),
m_marginBottom(TWHTMLStyleValueZero),
m_paddingLeft(TWHTMLStyleValueZero),
m_paddingRight(TWHTMLStyleValueZero),
m_paddingTop(TWHTMLStyleValueZero),
m_paddingBottom(TWHTMLStyleValueZero),
m_float(TWHTMLStyleFloatNone),
m_clear(TWHTMLStyleFloatNone),
m_fontWeight(100),
m_fontSize(TWHTMLStyleUnitPixels, 12),
m_lineHeight(TWHTMLStyleValueAuto),
m_textAlign(TWHTMLStyleTextAlignLeft),
m_textIndent(TWHTMLStyleValueZero),
m_inheritedFontSize(12),
m_position(TWHTMLStylePositionStatic),
m_whitespace(TWHTMLStyleWhiteSpaceNormal),
m_textDecoration(TWHTMLStyleTextDecorationNone)
{
	//setValue(L"border-style",L"solid");
	m_color.alpha=255;
}

void TWHTMLStyle::inheritFrom(const TWHTMLStyle& parentStyle){
	m_color=parentStyle.color();
	m_fontWeight=parentStyle.fontWeight();
	m_fontSize=parentStyle.fontSize();
	m_lineHeight=parentStyle.lineHeight();
	m_textAlign=parentStyle.textAlign();
	m_textDecoration=parentStyle.textDecoration();
	m_inheritedFontSize=parentStyle.fontSize().toPixels(parentStyle.inheritedFontSize(), parentStyle);
	m_whitespace=parentStyle.whitespace();
}

void TWHTMLStyle::apply(const std::wstring& styleStr){
	std::wstring str=styleStr;str+=L";";
	TWStringSplitter<64> styles(str, L";");

    for(size_t i=0;i<styles.elementCount();i++){
        std::wstring styleString=styles[i];
        std::wstring::size_type separatorPos=styleString.find(L':');
        
        if(separatorPos==std::wstring::npos)
            continue; // invalid format, or there is no content
        
        std::wstring nameString=TWUtils::trimString(styleString.substr(0, separatorPos));
        
        std::wstring valueString=TWUtils::trimString(styleString.substr(separatorPos+1));
        
        setValue(nameString, valueString);
    }
}

void TWHTMLStyle::setValue(const std::wstring& name, const std::wstring& value){
	if(name.empty())
		return;
	if(value.empty())
		return;
	
    std::wstring lowerName=TWUtils::toLower(name);
    std::wstring lowerValue=TWUtils::toLower(value);
	
	StyleType styleType=styleTypeWithName(lowerName.c_str());
	
	switch(styleType){
		case StyleTypeBackgroundColor:
			m_backgroundColor=TWHTMLStyleColor(lowerValue);
			break;
		case StyleTypeBackground:
			m_backgroundColor=TWHTMLStyleColor(lowerValue);
			break;
		case StyleTypeColor:
			m_color=TWHTMLStyleColor(lowerValue);
			break;
		case StyleTypeDisplay:
			switch(lowerValue[0]){
				case L'n':
					if(lowerValue==L"none")
						m_display=TWHTMLStyleDisplayNone;
					break;
				case L'i':
					if(lowerValue==L"inline")
						m_display=TWHTMLStyleDisplayInline;
					break;
				case L'b':
					if(lowerValue==L"block")
						m_display=TWHTMLStyleDisplayBlock;
					break;
				case L't':
					if(lowerValue==L"table")
						m_display=TWHTMLStyleDisplayTable;
					else if(lowerValue==L"table-row")
						m_display=TWHTMLStyleDisplayTableRow;
					else if(lowerValue==L"table-row-group")
						m_display=TWHTMLStyleDisplayTableRowGroup;
					else if(lowerValue==L"table-header-group")
						m_display=TWHTMLStyleDisplayTableHeaderGroup;
					else if(lowerValue==L"table-footer-group")
						m_display=TWHTMLStyleDisplayTableFooterGroup;
					else if(lowerValue==L"table-col")
						m_display=TWHTMLStyleDisplayTableCol;
					else if(lowerValue==L"table-col-group")
						m_display=TWHTMLStyleDisplayTableColGroup;
					else if(lowerValue==L"table-cell")
						m_display=TWHTMLStyleDisplayTableCell;
					else if(lowerValue==L"table-caption")
						m_display=TWHTMLStyleDisplayTableCaption;
					break;
			}
			break;
		case StyleTypeWidth:
			m_width=TWHTMLStyleValue(lowerValue);
			break;
		case StyleTypeHeight:
			m_height=TWHTMLStyleValue(lowerValue);
			break;
		case StyleTypeMargin:
			{
				TWStringSplitter<5> values
				=TWStringSplitter<5>::splitterWithTrimmedString(lowerValue, L" ");
				
				if(values.elementCount()==1){
					m_marginLeft=m_marginTop=
					m_marginRight=m_marginBottom=TWHTMLStyleValue(values[0]);
				}else if(values.elementCount()==2){
					m_marginTop=m_marginBottom=TWHTMLStyleValue(values[0]);
					m_marginLeft=m_marginRight=TWHTMLStyleValue(values[1]);
				}else if(values.elementCount()==3){
					m_marginTop=TWHTMLStyleValue(values[0]);
					m_marginLeft=m_marginRight=TWHTMLStyleValue(values[1]);
					m_marginBottom=TWHTMLStyleValue(values[2]);
				}else if(values.elementCount()==4){
					m_marginTop=TWHTMLStyleValue(values[0]);
					m_marginRight=TWHTMLStyleValue(values[1]);
					m_marginBottom=TWHTMLStyleValue(values[2]);
					m_marginLeft=TWHTMLStyleValue(values[3]);
				}
			}
			break;
		case StyleTypeMarginLeft:
			m_marginLeft=TWHTMLStyleValue(lowerValue);
			break;
		case StyleTypeMarginRight:
			m_marginRight=TWHTMLStyleValue(lowerValue);
			break;
		case StyleTypeMarginTop:
			m_marginTop=TWHTMLStyleValue(lowerValue);
			break;
		case StyleTypeMarginBottom:
			m_marginBottom=TWHTMLStyleValue(lowerValue);
			break;
		case StyleTypePadding:
			{
				TWStringSplitter<5> values
				=TWStringSplitter<5>::splitterWithTrimmedString(lowerValue, L" ");
				if(values.elementCount()==1){
					m_paddingLeft=m_paddingTop=
					m_paddingRight=m_paddingBottom=TWHTMLStyleValue(values[0]);
				}else if(values.elementCount()==2){
					m_paddingTop=m_paddingBottom=TWHTMLStyleValue(values[0]);
					m_paddingLeft=m_paddingRight=TWHTMLStyleValue(values[1]);
				}else if(values.elementCount()==3){
					m_paddingTop=TWHTMLStyleValue(values[0]);
					m_paddingLeft=m_paddingRight=TWHTMLStyleValue(values[1]);
					m_paddingBottom=TWHTMLStyleValue(values[2]);
				}else if(values.elementCount()==4){
					m_paddingTop=TWHTMLStyleValue(values[0]);
					m_paddingRight=TWHTMLStyleValue(values[1]);
					m_paddingBottom=TWHTMLStyleValue(values[2]);
					m_paddingLeft=TWHTMLStyleValue(values[3]);
				}
			}
			break;
		case StyleTypePaddingLeft:
			m_paddingLeft=TWHTMLStyleValue(lowerValue);
			break;
		case StyleTypePaddingRight:
			m_paddingRight=TWHTMLStyleValue(lowerValue);
			break;
		case StyleTypePaddingTop:
			m_paddingTop=TWHTMLStyleValue(lowerValue);
			break;
		case StyleTypePaddingBottom:
			m_paddingBottom=TWHTMLStyleValue(lowerValue);
			break;
		case StyleTypeBorder:
			m_borderLeft=m_borderRight=
			m_borderTop=m_borderBottom=TWHTMLStyleBorder(lowerValue);
			break;
		case StyleTypeBorderLeft:
			m_borderLeft=TWHTMLStyleBorder(lowerValue);
			break;
		case StyleTypeBorderRight:
			m_borderRight=TWHTMLStyleBorder(lowerValue);
			break;
		case StyleTypeBorderTop:
			m_borderTop=TWHTMLStyleBorder(lowerValue);
			break;
		case StyleTypeBorderBottom:
			m_borderBottom=TWHTMLStyleBorder(lowerValue);
			break;
		case StyleTypeBorderWidth:
			{
				TWStringSplitter<5> values
				=TWStringSplitter<5>::splitterWithTrimmedString(lowerValue, L" ");
				if(values.elementCount()==1){
					m_borderLeft.width=m_borderTop.width=
					m_borderRight.width=m_borderBottom.width=TWHTMLStyleValue(values[0]);
				}else if(values.elementCount()==2){
					m_borderTop.width=m_borderBottom.width=TWHTMLStyleValue(values[0]);
					m_borderLeft.width=m_borderRight.width=TWHTMLStyleValue(values[1]);
				}else if(values.elementCount()==3){
					m_borderTop.width=TWHTMLStyleValue(values[0]);
					m_borderLeft.width=m_borderRight.width=TWHTMLStyleValue(values[1]);
					m_borderBottom.width=TWHTMLStyleValue(values[2]);
				}else if(values.elementCount()==4){
					m_borderTop.width=TWHTMLStyleValue(values[0]);
					m_borderRight.width=TWHTMLStyleValue(values[1]);
					m_borderBottom.width=TWHTMLStyleValue(values[2]);
					m_borderLeft.width=TWHTMLStyleValue(values[3]);
				}
			}
			break;
		case StyleTypeFontSize:
			switch(lowerValue[0]){
				case L'x':
					if(lowerValue==L"xx-small"){
						m_fontSize=g_fontSizes[0];
						break;
					}else if(lowerValue==L"x-small"){
						m_fontSize=g_fontSizes[1];
						break;
					}else if(lowerValue==L"x-large"){
						m_fontSize=g_fontSizes[5];
						break;
					}else if(lowerValue==L"xx-large"){
						m_fontSize=g_fontSizes[6];
						break;
					}
				case L's':
					if(lowerValue==L"small"){
						m_fontSize=g_fontSizes[2];
						break;
					}else if(lowerValue==L"smaller"){
						m_fontSize=g_fontSizes[8];
						break;
					}
				case L'm':
					if(lowerValue==L"medium"){
						m_fontSize=g_fontSizes[3];
						break;
					}
				case L'l':
					if(lowerValue==L"large"){
						m_fontSize=g_fontSizes[4];
						break;
					}else if(lowerValue==L"larger"){
						m_fontSize=g_fontSizes[7];
						break;
					}
				default:
					m_fontSize=TWHTMLStyleValue(lowerValue);
			}
			break;
		case StyleTypeBorderLeftWidth:
			m_borderLeft.width=TWHTMLStyleValue(lowerValue);
			break;
		case StyleTypeBorderRightWidth:
			m_borderRight.width=TWHTMLStyleValue(lowerValue);
			break;
		case StyleTypeBorderTopWidth:
			m_borderTop.width=TWHTMLStyleValue(lowerValue);
			break;
		case StyleTypeBorderBottomWidth:
			m_borderBottom.width=TWHTMLStyleValue(lowerValue);
			break;
		case StyleTypeBorderStyle:
			{
				TWStringSplitter<5> values
				=TWStringSplitter<5>::splitterWithTrimmedString(lowerValue, L" ");
				if(values.elementCount()==1){
					m_borderTop.setStyle(values[0]);
					m_borderRight.setStyle(values[0]);
					m_borderBottom.setStyle(values[0]);
					m_borderLeft.setStyle(values[0]);
				}else if(values.elementCount()==2){
					m_borderTop.setStyle(values[0]);
					m_borderRight.setStyle(values[1]);
					m_borderBottom.setStyle(values[0]);
					m_borderLeft.setStyle(values[1]);
				}else if(values.elementCount()==3){
					m_borderTop.setStyle(values[0]);
					m_borderRight.setStyle(values[1]);
					m_borderBottom.setStyle(values[2]);
					m_borderLeft.setStyle(values[1]);
				}else if(values.elementCount()==4){
					m_borderTop.setStyle(values[0]);
					m_borderRight.setStyle(values[1]);
					m_borderBottom.setStyle(values[2]);
					m_borderLeft.setStyle(values[3]);
				}
			}
			break;
		case StyleTypeBorderLeftStyle:
			m_borderLeft.setStyle(lowerValue);
			break;
		case StyleTypeBorderRightStyle:
			m_borderRight.setStyle(lowerValue);
			break;
		case StyleTypeBorderTopStyle:
			m_borderTop.setStyle(lowerValue);
			break;
		case StyleTypeBorderBottomStyle:
			m_borderBottom.setStyle(lowerValue);
			break;
		case StyleTypeBorderColor:
			{
				TWStringSplitter<5> values
				=TWStringSplitter<5>::splitterWithTrimmedString(lowerValue, L" ");
				if(values.elementCount()==1){
					m_borderLeft.color=m_borderTop.color=
					m_borderRight.color=m_borderBottom.color=TWHTMLStyleColor(values[0]);
				}else if(values.elementCount()==2){
					m_borderTop.color=m_borderBottom.color=TWHTMLStyleColor(values[0]);
					m_borderLeft.color=m_borderRight.color=TWHTMLStyleColor(values[1]);
				}else if(values.elementCount()==3){
					m_borderTop.color=TWHTMLStyleColor(values[0]);
					m_borderLeft.color=m_borderRight.color=TWHTMLStyleColor(values[1]);
					m_borderBottom.color=TWHTMLStyleColor(values[2]);
				}else if(values.elementCount()==4){
					m_borderTop.color=TWHTMLStyleColor(values[0]);
					m_borderRight.color=TWHTMLStyleColor(values[1]);
					m_borderBottom.color=TWHTMLStyleColor(values[2]);
					m_borderLeft.color=TWHTMLStyleColor(values[3]);
				}
			}
			break;
		case StyleTypeTextDecoration:
			{
				TWStringSplitter<8> values
				=TWStringSplitter<8>::splitterWithTrimmedString(lowerValue, L" ");
				m_textDecoration=TWHTMLStyleTextDecorationNone;
				for(int i=0;i<values.elementCount();i++){
					const wchar_t *s=values[i];
					switch(s[0]){
						case L'n':
							m_textDecoration=TWHTMLStyleTextDecorationNone;
							break;
						case L'u':
							m_textDecoration|=TWHTMLStyleTextDecorationUnderline;
							break;
						case L'o':
							m_textDecoration|=TWHTMLStyleTextDecorationOverline;
							break;
						case L'l':
							m_textDecoration|=TWHTMLStyleTextDecorationLineThrough;
							break;
					}
				}
			}
			break;
		case StyleTypeBorderLeftColor:
			m_borderLeft.color=TWHTMLStyleColor(lowerValue);
			break;
		case StyleTypeBorderRightColor:
			m_borderRight.color=TWHTMLStyleColor(lowerValue);
			break;
		case StyleTypeBorderTopColor:
			m_borderTop.color=TWHTMLStyleColor(lowerValue);
			break;
		case StyleTypeBorderBottomColor:
			m_borderBottom.color=TWHTMLStyleColor(lowerValue);
			break;
		case StyleTypeFloat:
			switch(lowerValue[0]){
				case L'n':
					if(lowerValue==L"none")
						m_float=TWHTMLStyleFloatNone;
					break;
				case L'l':
					if(lowerValue==L"left")
						m_float=TWHTMLStyleFloatLeft;
					break;
				case L'r':
					if(lowerValue==L"right")
						m_float=TWHTMLStyleFloatRight;
					break;
			}
			break;
		case StyleTypeClear:
			switch(lowerValue[0]){
				case L'n':
					if(lowerValue==L"none")
						m_clear=TWHTMLStyleFloatNone;
					break;
				case L'l':
					if(lowerValue==L"left")
						m_clear=TWHTMLStyleFloatLeft;
					break;
				case L'r':
					if(lowerValue==L"right")
						m_clear=TWHTMLStyleFloatRight;
					break;
				case L'b':
					if(lowerValue==L"both")
						m_clear=TWHTMLStyleFloatBoth;
					break;
			}
			break;
		case StyleTypeWhitespace:
			switch(lowerValue[0]){
				case L'n':
					if(lowerValue==L"normal"){
						m_whitespace=TWHTMLStyleWhiteSpaceNormal;
					}else if(lowerValue==L"nowrap"){
						m_whitespace=TWHTMLStyleWhiteSpaceNoWrap;
					}
					break;
				case L'p':
					if(lowerValue==L"pre"){
						m_whitespace=TWHTMLStyleWhiteSpacePre;
					}
					break;
			}
			break;
		case StyleTypeFontWeight:
			switch(lowerValue[0]){
				case L'n':
					if(lowerValue==L"normal"){
						m_fontWeight=100;
					}
					break;
				case L'b':
					if(lowerValue==L"bold"){
						m_fontWeight=500;
					}else if(lowerValue==L"bolder"){
						m_fontWeight=300;
					}
					break;
				case L'l':
					if(lowerValue==L"lighter"){
						m_fontWeight=10;
					}
					break;
				default:
					m_fontWeight=TWUtils::intValueFromString(lowerValue);
			}
			break;
		case StyleTypeTextAlign:
			switch(lowerValue[0]){
				case L'l':
					if(lowerValue==L"left")
						m_textAlign=TWHTMLStyleTextAlignLeft;
					break;
				case L'c':
					if(lowerValue==L"center")
						m_textAlign=TWHTMLStyleTextAlignCenter;
					break;
				case L'r':
					if(lowerValue==L"right")
						m_textAlign=TWHTMLStyleTextAlignRight;
					break;
					
			}
			break;
		case StyleTypePosition:
			switch(lowerValue[0]){
				case L's':
					if(lowerValue==L"static")
						m_position=TWHTMLStylePositionStatic;
					break;
				case L'r':
					if(lowerValue==L"relative")
						m_position=TWHTMLStylePositionRelative;
					break;
				case L'a':
					if(lowerValue==L"absolute")
						m_position=TWHTMLStylePositionAbsolute;
					break;
				case L'f':
					if(lowerValue==L"fixed")
						m_position=TWHTMLStylePositionFixed;
					break;
			}
			break;
		case StyleTypeUnknown:
			break;
	}
}


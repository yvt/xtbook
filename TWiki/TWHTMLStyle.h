/*
 *  TWHTMLStyle.h
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

#include <string>
#include <vector>

#include "TWTypes.h"

#define TWHTMLSTYLE_FIXED			1
#define TWHTMLSTYLE_USE_BITFIELD	TW_USE_BITFIELD

#define TWHTMLSTYLE_BITFIELD(s) TW_BITFIELD(s)

class TWHTMLStyle;
class TWHTMLStyleProxy;

enum TWHTMLStyleDisplay{
	TWHTMLStyleDisplayNone=0,
	TWHTMLStyleDisplayInline,
	TWHTMLStyleDisplayBlock,
    TWHTMLStyleDisplayTable,
    TWHTMLStyleDisplayTableRow,
    TWHTMLStyleDisplayTableRowGroup,
    TWHTMLStyleDisplayTableHeaderGroup,
    TWHTMLStyleDisplayTableFooterGroup,
    TWHTMLStyleDisplayTableCol,
    TWHTMLStyleDisplayTableColGroup,
    TWHTMLStyleDisplayTableCell,
    TWHTMLStyleDisplayTableCaption,
	TWHTMLStyleDisplayListItem
};

enum TWHTMLStyleUnit{
	TWHTMLStyleUnitPixels=0,
	TWHTMLStyleUnitEm,
	TWHTMLStyleUnitPercent,
	TWHTMLStyleUnitAuto
};
struct TWHTMLStyleValue{
	
#if TWHTMLSTYLE_FIXED
	TWHTMLStyleUnit unit TWHTMLSTYLE_BITFIELD(2);
	int fixedValue TWHTMLSTYLE_BITFIELD(30);
#else
	TWHTMLStyleUnit unit;
	int intValue;
	float floatValue;
#endif
	TWHTMLStyleValue(){}
	TWHTMLStyleValue(TWHTMLStyleUnit un, int value){
		unit=un;
#if TWHTMLSTYLE_FIXED
		fixedValue=value<<8;
#else
		intValue=(int)value;
		floatValue=(float)value;
#endif
	}
	TWHTMLStyleValue(TWHTMLStyleUnit un, float value){
		unit=un;
#if TWHTMLSTYLE_FIXED
		fixedValue=(int)(value*256.f);
#else
		intValue=(int)value;
		floatValue=(float)value;
#endif
	}
	TWHTMLStyleValue(const std::wstring&);
	int toPixels(int referenceSize, const TWHTMLStyle&) const;
    int toPixels(int referenceSize, const TWHTMLStyleProxy&) const;
    
};
static const TWHTMLStyleValue TWHTMLStyleValueZero(TWHTMLStyleUnitPixels, 0);
static const TWHTMLStyleValue TWHTMLStyleValueAuto(TWHTMLStyleUnitAuto, 0);
struct TWHTMLStyleColor{
	uint8_t red, green, blue, alpha;
	TWHTMLStyleColor(){
		red=green=blue=alpha=0;
	}	
	TWHTMLStyleColor(const std::wstring&);
	TWHTMLStyleColor(uint8_t r, uint8_t g,
					 uint8_t b, uint8_t a):
	red(r), green(g), blue(b), alpha(a){}
	bool operator ==(const TWHTMLStyleColor& c) const{
		return red==c.red && green==c.green && blue==c.blue && alpha==c.alpha;
	}
};
enum TWHTMLStyleTextAlign{
	TWHTMLStyleTextAlignLeft=0,
	TWHTMLStyleTextAlignRight,
	TWHTMLStyleTextAlignCenter
};
typedef unsigned int TWHTMLStyleTextDecoration;
enum{
	TWHTMLStyleTextDecorationNone=0,
	TWHTMLStyleTextDecorationUnderline=0x1,
	TWHTMLStyleTextDecorationOverline=0x2,
	TWHTMLStyleTextDecorationLineThrough=0x4
};
enum TWHTMLStyleWhiteSpace{
	TWHTMLStyleWhiteSpaceNormal=0,
	TWHTMLStyleWhiteSpacePre,
	TWHTMLStyleWhiteSpaceNoWrap
};
enum TWHTMLStyleLineBreak{
	TWHTMLStyleLineBreakNormal=0,
	TWHTMLStyleLineBreakStrict
};
struct TWHTMLStyleTextShadow{
	TWHTMLStyleColor color;
	TWHTMLStyleValue x, y;
	TWHTMLStyleValue shading;
};
enum TWHTMLStyleBorderStyle{
	TWHTMLStyleBorderStyleNone=0,
	TWHTMLStyleBorderStyleDotted,
	TWHTMLStyleBorderStyleDashed,
	TWHTMLStyleBorderStyleSolid,
	TWHTMLStyleBorderStyleDouble,
	TWHTMLStyleBorderStyleGroove,
	TWHTMLStyleBorderStyleRidge,
	TWHTMLStyleBorderStyleInset,
	TWHTMLStyleBorderStyleOutset
};
struct TWHTMLStyleBorder{
	TWHTMLStyleValue width;
	TWHTMLStyleBorderStyle style;
	TWHTMLStyleColor color;
    TWHTMLStyleBorder():
    width(TWHTMLStyleUnitPixels, 1),
    style(TWHTMLStyleBorderStyleNone),
    color(){
    }
    TWHTMLStyleBorder(const std::wstring&);
    void setStyle(const std::wstring&);
};
enum TWHTMLStyleFloat{
	TWHTMLStyleFloatNone=0,
	TWHTMLStyleFloatLeft,
	TWHTMLStyleFloatRight,
    TWHTMLStyleFloatBoth // for clear
};
enum TWHTMLStylePosition{
	TWHTMLStylePositionStatic=0,
	TWHTMLStylePositionRelative,
	TWHTMLStylePositionAbsolute,
	TWHTMLStylePositionFixed
};
class TWHTMLStyle {
protected:
	TWHTMLStyleColor m_backgroundColor;
	TWHTMLStyleColor m_color;
	
	TWHTMLStyleDisplay m_display;
	
	TWHTMLStyleValue m_width;
	TWHTMLStyleValue m_height;
	
	TWHTMLStyleValue m_marginLeft;
	TWHTMLStyleValue m_marginRight;
	TWHTMLStyleValue m_marginTop;
	TWHTMLStyleValue m_marginBottom;
	
	TWHTMLStyleValue m_paddingLeft;
	TWHTMLStyleValue m_paddingRight;
	TWHTMLStyleValue m_paddingTop;
	TWHTMLStyleValue m_paddingBottom;
	
	TWHTMLStyleBorder m_borderLeft;
	TWHTMLStyleBorder m_borderRight;
	TWHTMLStyleBorder m_borderTop;
	TWHTMLStyleBorder m_borderBottom;
	
	TWHTMLStyleFloat m_float TWHTMLSTYLE_BITFIELD(2);
	TWHTMLStyleFloat m_clear TWHTMLSTYLE_BITFIELD(2);
	
	int m_inheritedFontSize TWHTMLSTYLE_BITFIELD(8);
	TWHTMLStylePosition m_position TWHTMLSTYLE_BITFIELD(2);
	int m_fontWeight TWHTMLSTYLE_BITFIELD(11);
	TWHTMLStyleTextAlign m_textAlign TWHTMLSTYLE_BITFIELD(2);
	bool m_visibility TWHTMLSTYLE_BITFIELD(1);
	TWHTMLStyleWhiteSpace m_whitespace TWHTMLSTYLE_BITFIELD(2);
	TWHTMLStyleTextDecoration m_textDecoration TWHTMLSTYLE_BITFIELD(3);
	TWHTMLStyleValue m_fontSize;
	TWHTMLStyleValue m_lineHeight;
	
	
	
	TWHTMLStyleValue m_textIndent;
	
	
	
public:
	TWHTMLStyle();
	void inheritFrom(const TWHTMLStyle&);
	void apply(const std::wstring&);
	void setValue(const std::wstring&, const std::wstring&);
	
	int inheritedFontSize() const{
		return m_inheritedFontSize;
	}
	
	const TWHTMLStyleColor& backgroundColor() const{return m_backgroundColor;}
	void setBackgroundColor(const TWHTMLStyleColor& col){m_backgroundColor=col;}
	const TWHTMLStyleColor& color() const{return m_color;}
	void setColor(const TWHTMLStyleColor& col){m_color=col;}
	
	TWHTMLStyleDisplay display() const{return m_display;}
	void setDisplay(TWHTMLStyleDisplay v){m_display=v;}
	bool visibility() const{return m_visibility;}
	void setVisibility(bool v){m_visibility=v;}
	
	const TWHTMLStyleValue& width() const{return m_width;}
	void setWidth(const TWHTMLStyleValue& v){m_width=v;}
	const TWHTMLStyleValue& height() const{return m_height;}
	void setHeight(const TWHTMLStyleValue& v){m_height=v;}
	
	const TWHTMLStyleValue& marginLeft() const{return m_marginLeft;}
	void setMarginLeft(const TWHTMLStyleValue& v){m_marginLeft=v;}
	const TWHTMLStyleValue& marginRight() const{return m_marginRight;}
	void setMarginRight(const TWHTMLStyleValue& v){m_marginRight=v;}
	const TWHTMLStyleValue& marginTop() const{return m_marginTop;}
	void setMarginTop(const TWHTMLStyleValue& v){m_marginTop=v;}
	const TWHTMLStyleValue& marginBottom() const{return m_marginBottom;}
	void setMarginBottom(const TWHTMLStyleValue& v){m_marginBottom=v;}
	
	const TWHTMLStyleValue& paddingLeft() const{return m_paddingLeft;}
	void setPaddingLeft(const TWHTMLStyleValue& v){m_paddingLeft=v;}
	const TWHTMLStyleValue& paddingRight() const{return m_paddingRight;}
	void setPaddingRight(const TWHTMLStyleValue& v){m_paddingRight=v;}
	const TWHTMLStyleValue& paddingTop() const{return m_paddingTop;}
	void setPaddingTop(const TWHTMLStyleValue& v){m_paddingTop=v;}
	const TWHTMLStyleValue& paddingBottom() const{return m_paddingBottom;}
	void setPaddingBottom(const TWHTMLStyleValue& v){m_paddingBottom=v;}
	
	const TWHTMLStyleBorder& borderLeft() const{return m_borderLeft;}
	void setBorderLeft(const TWHTMLStyleBorder& v){m_borderLeft=v;}
	const TWHTMLStyleBorder& borderRight() const{return m_borderRight;}
	void setBorderRight(const TWHTMLStyleBorder& v){m_borderRight=v;}
	const TWHTMLStyleBorder& borderTop() const{return m_borderTop;}
	void setBorderTop(const TWHTMLStyleBorder& v){m_borderTop=v;}
	const TWHTMLStyleBorder& borderBottom() const{return m_borderBottom;}
	void setBorderBottom(const TWHTMLStyleBorder& v){m_borderBottom=v;}
	
	TWHTMLStyleFloat _float() const{return m_float;}
	void setFloat(TWHTMLStyleFloat v){m_float=v;}
	TWHTMLStyleFloat clear() const{return m_clear;}
	void setClear(TWHTMLStyleFloat v){m_clear=v;}
	
	int fontWeight() const{return m_fontWeight;}
	void setFontWeight(int v){m_fontWeight=v;}
	const TWHTMLStyleValue& fontSize() const{return m_fontSize;}
	void setFontSize(const TWHTMLStyleValue&v){m_fontSize=v;}
	const TWHTMLStyleValue& lineHeight() const{return m_lineHeight;}
	void setLineHeight(const TWHTMLStyleValue& v){m_lineHeight=v;}
	
	TWHTMLStyleTextAlign textAlign() const{return m_textAlign;}
	void setTextAlign(TWHTMLStyleTextAlign v){m_textAlign=v;}
	TWHTMLStyleTextDecoration textDecoration() const{return m_textDecoration;}
	void setTextDecoration(TWHTMLStyleTextDecoration v){m_textDecoration=v;}
	const TWHTMLStyleValue& textIndent() const{return m_textIndent;}
	void setTextIndent(const TWHTMLStyleValue& v){m_textIndent=v;}
	
	TWHTMLStylePosition position() const{return m_position;}
	void setPosition(TWHTMLStylePosition p){m_position=p;}
	
	TWHTMLStyleWhiteSpace whitespace() const{return m_whitespace;}
	void setWhitespace(TWHTMLStyleWhiteSpace v){m_whitespace=v;}
};

//
//  TWHTMLStyleProxy.cpp
//  XTBook
//
//  Created b Kawada Tomoaki on 7/27/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "TWHTMLStyleProxy.h"

static const TWHTMLStyleColor g_transparentColor(0,0,0,0);
static const TWHTMLStyleColor g_blackColor(0,0,0,255);
static const TWHTMLStyleBorder g_noBorder; // cup noodle.
static const TWHTMLStyleValue g_defaultFontSize(TWHTMLStyleUnitPixels, 12);
static const std::vector<TWHTMLStyleTextDecoration> g_noTextDecoration;

TWHTMLStyleProxy::TWHTMLStyleProxy():
m_mode(TWHTMLStyleProxyModeNullStyle){
}

TWHTMLStyleProxy::TWHTMLStyleProxy(const TWHTMLStyle& style):
m_mode(TWHTMLStyleProxyModeSpecified){
	m_style=&style;
}

TWHTMLStyleProxy::TWHTMLStyleProxy(const TWHTMLStyleProxy& style):
m_mode(style.m_mode){
	if(m_mode==TWHTMLStyleProxyModeUnique){
		// don't just copy pointer of unique style.
		m_uniqueStyle=new TWHTMLStyle(*(style.m_uniqueStyle));
	}else{
		m_style=style.m_style;
	}
}

TWHTMLStyleProxy::~TWHTMLStyleProxy(){
	if(m_mode==TWHTMLStyleProxyModeUnique)
		delete m_uniqueStyle;
}

void TWHTMLStyleProxy::makeUnique(){
	
	switch(m_mode){
		case TWHTMLStyleProxyModeNullStyle:
			m_uniqueStyle=new TWHTMLStyle();
			break;
		case TWHTMLStyleProxyModeSpecified:
			m_uniqueStyle=new TWHTMLStyle(*m_style);
			break;
		case TWHTMLStyleProxyModeInherited:
			m_uniqueStyle=new TWHTMLStyle();
			m_uniqueStyle->inheritFrom(*m_style);
			break;
		case TWHTMLStyleProxyModeUnique:
			return;
	}
	
	
	m_mode=TWHTMLStyleProxyModeUnique;
	
}

void TWHTMLStyleProxy::inheritFrom(const TWHTMLStyle &s){
	if(m_mode==TWHTMLStyleProxyModeUnique)
		delete m_uniqueStyle;
	
	m_style=&s;
	m_mode=TWHTMLStyleProxyModeInherited;
}

void TWHTMLStyleProxy::operator=(const TWHTMLStyle &s){
	if(m_mode==TWHTMLStyleProxyModeUnique)
		delete m_uniqueStyle;
	
	m_style=&s;
	m_mode=TWHTMLStyleProxyModeSpecified;
}

void TWHTMLStyleProxy::operator=(const TWHTMLStyleProxy &style){
	if(&style==this)
		return;
	if(m_mode==TWHTMLStyleProxyModeUnique)
		delete m_uniqueStyle;
	m_mode=style.m_mode;
	if(m_mode==TWHTMLStyleProxyModeUnique){
		// don't just copy pointer of unique style.
		m_uniqueStyle=new TWHTMLStyle(*(style.m_uniqueStyle));
	}else{
		m_style=style.m_style;
	}
}

TWHTMLStyleProxy::operator TWHTMLStyle() const{
	switch(m_mode){
		case TWHTMLStyleProxyModeNullStyle:
			return TWHTMLStyle();
		case TWHTMLStyleProxyModeSpecified:
		case TWHTMLStyleProxyModeUnique:
			return *m_style;
		case TWHTMLStyleProxyModeInherited:
			{
				TWHTMLStyle s;
				s.inheritFrom(*m_style);
				return s;
			}
			
	}
}

int TWHTMLStyleProxy::inheritedFontSize() const{
	switch(m_mode){
		case TWHTMLStyleProxyModeNullStyle:
			return 12;
		case TWHTMLStyleProxyModeSpecified:
		case TWHTMLStyleProxyModeUnique:
			return m_style->inheritedFontSize();
		case TWHTMLStyleProxyModeInherited:
			return m_style->fontSize().toPixels(m_style->inheritedFontSize(), *m_style);
	}
}

#pragma mark - Getting Inherited Styles

const TWHTMLStyleColor& TWHTMLStyleProxy::color() const{
	switch(m_mode){
		case TWHTMLStyleProxyModeNullStyle:
			return g_blackColor;
		case TWHTMLStyleProxyModeSpecified:
		case TWHTMLStyleProxyModeUnique:
		case TWHTMLStyleProxyModeInherited:
			return m_style->color();
	}
}

int TWHTMLStyleProxy::fontWeight() const{
	switch(m_mode){
		case TWHTMLStyleProxyModeNullStyle:
			return 100;
		case TWHTMLStyleProxyModeSpecified:
		case TWHTMLStyleProxyModeUnique:
		case TWHTMLStyleProxyModeInherited:
			return m_style->fontWeight();
	}
}

const TWHTMLStyleValue& TWHTMLStyleProxy::fontSize() const{
	switch(m_mode){
		case TWHTMLStyleProxyModeNullStyle:
			return g_defaultFontSize;
		case TWHTMLStyleProxyModeSpecified:
		case TWHTMLStyleProxyModeUnique:
		case TWHTMLStyleProxyModeInherited:
			return m_style->fontSize();
	}
}

const TWHTMLStyleValue& TWHTMLStyleProxy::lineHeight() const{
	switch(m_mode){
		case TWHTMLStyleProxyModeNullStyle:
			return TWHTMLStyleValueAuto;
		case TWHTMLStyleProxyModeSpecified:
		case TWHTMLStyleProxyModeUnique:
		case TWHTMLStyleProxyModeInherited:
			return m_style->lineHeight();
	}
}

TWHTMLStyleTextAlign TWHTMLStyleProxy::textAlign() const{
	switch(m_mode){
		case TWHTMLStyleProxyModeNullStyle:
			return TWHTMLStyleTextAlignLeft;
		case TWHTMLStyleProxyModeSpecified:
		case TWHTMLStyleProxyModeUnique:
		case TWHTMLStyleProxyModeInherited:
			return m_style->textAlign();
	}
}

TWHTMLStyleWhiteSpace TWHTMLStyleProxy::whitespace() const{
	switch(m_mode){
		case TWHTMLStyleProxyModeNullStyle:
			return TWHTMLStyleWhiteSpaceNormal;
		case TWHTMLStyleProxyModeSpecified:
		case TWHTMLStyleProxyModeUnique:
		case TWHTMLStyleProxyModeInherited:
			return m_style->whitespace();
	}
}

TWHTMLStyleTextDecoration TWHTMLStyleProxy::textDecoration() const{
	switch(m_mode){
		case TWHTMLStyleProxyModeNullStyle:
			return TWHTMLStyleTextDecorationNone;
		case TWHTMLStyleProxyModeSpecified:
		case TWHTMLStyleProxyModeUnique:
		case TWHTMLStyleProxyModeInherited:
			return m_style->textDecoration();
	}
}

#pragma mark - Getting Non-inherited Styles

const TWHTMLStyleColor& TWHTMLStyleProxy::backgroundColor() const{
	switch(m_mode){
		case TWHTMLStyleProxyModeNullStyle:
		case TWHTMLStyleProxyModeInherited:
			return g_transparentColor;
		case TWHTMLStyleProxyModeSpecified:
		case TWHTMLStyleProxyModeUnique:
			return m_style->backgroundColor();
	}
}

TWHTMLStyleDisplay TWHTMLStyleProxy::display() const{
	switch(m_mode){
		case TWHTMLStyleProxyModeNullStyle:
		case TWHTMLStyleProxyModeInherited:
			return TWHTMLStyleDisplayInline;
		case TWHTMLStyleProxyModeSpecified:
		case TWHTMLStyleProxyModeUnique:
			return m_style->display();
	}
}

bool TWHTMLStyleProxy::visibility() const{
	switch(m_mode){
		case TWHTMLStyleProxyModeNullStyle:
		case TWHTMLStyleProxyModeInherited:
			return true;
		case TWHTMLStyleProxyModeSpecified:
		case TWHTMLStyleProxyModeUnique:
			return m_style->visibility();
	}
}

const TWHTMLStyleValue& TWHTMLStyleProxy::width() const{
	switch(m_mode){
		case TWHTMLStyleProxyModeNullStyle:
		case TWHTMLStyleProxyModeInherited:
			return TWHTMLStyleValueAuto;
		case TWHTMLStyleProxyModeSpecified:
		case TWHTMLStyleProxyModeUnique:
			return m_style->width();
	}
}

const TWHTMLStyleValue& TWHTMLStyleProxy::height() const{
	switch(m_mode){
		case TWHTMLStyleProxyModeNullStyle:
		case TWHTMLStyleProxyModeInherited:
			return TWHTMLStyleValueAuto;
		case TWHTMLStyleProxyModeSpecified:
		case TWHTMLStyleProxyModeUnique:
			return m_style->height();
	}
}

const TWHTMLStyleValue& TWHTMLStyleProxy::marginLeft() const{
	switch(m_mode){
		case TWHTMLStyleProxyModeNullStyle:
		case TWHTMLStyleProxyModeInherited:
			return TWHTMLStyleValueZero;
		case TWHTMLStyleProxyModeSpecified:
		case TWHTMLStyleProxyModeUnique:
			return m_style->marginLeft();
	}
}

const TWHTMLStyleValue& TWHTMLStyleProxy::marginRight() const{
	switch(m_mode){
		case TWHTMLStyleProxyModeNullStyle:
		case TWHTMLStyleProxyModeInherited:
			return TWHTMLStyleValueZero;
		case TWHTMLStyleProxyModeSpecified:
		case TWHTMLStyleProxyModeUnique:
			return m_style->marginRight();
	}
}

const TWHTMLStyleValue& TWHTMLStyleProxy::marginTop() const{
	switch(m_mode){
		case TWHTMLStyleProxyModeNullStyle:
		case TWHTMLStyleProxyModeInherited:
			return TWHTMLStyleValueZero;
		case TWHTMLStyleProxyModeSpecified:
		case TWHTMLStyleProxyModeUnique:
			return m_style->marginTop();
	}
}

const TWHTMLStyleValue& TWHTMLStyleProxy::marginBottom() const{
	switch(m_mode){
		case TWHTMLStyleProxyModeNullStyle:
		case TWHTMLStyleProxyModeInherited:
			return TWHTMLStyleValueZero;
		case TWHTMLStyleProxyModeSpecified:
		case TWHTMLStyleProxyModeUnique:
			return m_style->marginBottom();
	}
}

const TWHTMLStyleValue& TWHTMLStyleProxy::paddingLeft() const{
	switch(m_mode){
		case TWHTMLStyleProxyModeNullStyle:
		case TWHTMLStyleProxyModeInherited:
			return TWHTMLStyleValueZero;
		case TWHTMLStyleProxyModeSpecified:
		case TWHTMLStyleProxyModeUnique:
			return m_style->paddingLeft();
	}
}

const TWHTMLStyleValue& TWHTMLStyleProxy::paddingRight() const{
	switch(m_mode){
		case TWHTMLStyleProxyModeNullStyle:
		case TWHTMLStyleProxyModeInherited:
			return TWHTMLStyleValueZero;
		case TWHTMLStyleProxyModeSpecified:
		case TWHTMLStyleProxyModeUnique:
			return m_style->paddingRight();
	}
}

const TWHTMLStyleValue& TWHTMLStyleProxy::paddingTop() const{
	switch(m_mode){
		case TWHTMLStyleProxyModeNullStyle:
		case TWHTMLStyleProxyModeInherited:
			return TWHTMLStyleValueZero;
		case TWHTMLStyleProxyModeSpecified:
		case TWHTMLStyleProxyModeUnique:
			return m_style->paddingTop();
	}
}

const TWHTMLStyleValue& TWHTMLStyleProxy::paddingBottom() const{
	switch(m_mode){
		case TWHTMLStyleProxyModeNullStyle:
		case TWHTMLStyleProxyModeInherited:
			return TWHTMLStyleValueZero;
		case TWHTMLStyleProxyModeSpecified:
		case TWHTMLStyleProxyModeUnique:
			return m_style->paddingBottom();
	}
}

const TWHTMLStyleBorder& TWHTMLStyleProxy::borderLeft() const{
	switch(m_mode){
		case TWHTMLStyleProxyModeNullStyle:
		case TWHTMLStyleProxyModeInherited:
			return g_noBorder;
		case TWHTMLStyleProxyModeSpecified:
		case TWHTMLStyleProxyModeUnique:
			return m_style->borderLeft();
	}
}

const TWHTMLStyleBorder& TWHTMLStyleProxy::borderRight() const{
	switch(m_mode){
		case TWHTMLStyleProxyModeNullStyle:
		case TWHTMLStyleProxyModeInherited:
			return g_noBorder;
		case TWHTMLStyleProxyModeSpecified:
		case TWHTMLStyleProxyModeUnique:
			return m_style->borderRight();
	}
}

const TWHTMLStyleBorder& TWHTMLStyleProxy::borderTop() const{
	switch(m_mode){
		case TWHTMLStyleProxyModeNullStyle:
		case TWHTMLStyleProxyModeInherited:
			return g_noBorder;
		case TWHTMLStyleProxyModeSpecified:
		case TWHTMLStyleProxyModeUnique:
			return m_style->borderTop();
	}
}

const TWHTMLStyleBorder& TWHTMLStyleProxy::borderBottom() const{
	switch(m_mode){
		case TWHTMLStyleProxyModeNullStyle:
		case TWHTMLStyleProxyModeInherited:
			return g_noBorder;
		case TWHTMLStyleProxyModeSpecified:
		case TWHTMLStyleProxyModeUnique:
			return m_style->borderBottom();
	}
}

TWHTMLStyleFloat TWHTMLStyleProxy::_float() const{
	switch(m_mode){
		case TWHTMLStyleProxyModeNullStyle:
		case TWHTMLStyleProxyModeInherited:
			return TWHTMLStyleFloatNone;
		case TWHTMLStyleProxyModeSpecified:
		case TWHTMLStyleProxyModeUnique:
			return m_style->_float();
	}
}

TWHTMLStyleFloat TWHTMLStyleProxy::clear() const{
	switch(m_mode){
		case TWHTMLStyleProxyModeNullStyle:
		case TWHTMLStyleProxyModeInherited:
			return TWHTMLStyleFloatNone;
		case TWHTMLStyleProxyModeSpecified:
		case TWHTMLStyleProxyModeUnique:
			return m_style->clear();
	}
}

const TWHTMLStyleValue& TWHTMLStyleProxy::textIndent() const{
	switch(m_mode){
		case TWHTMLStyleProxyModeNullStyle:
		case TWHTMLStyleProxyModeInherited:
			return TWHTMLStyleValueZero;
		case TWHTMLStyleProxyModeSpecified:
		case TWHTMLStyleProxyModeUnique:
			return m_style->textIndent();
	}
}

TWHTMLStylePosition TWHTMLStyleProxy::position() const{
	switch(m_mode){
		case TWHTMLStyleProxyModeNullStyle:
		case TWHTMLStyleProxyModeInherited:
			return TWHTMLStylePositionStatic;
		case TWHTMLStyleProxyModeSpecified:
		case TWHTMLStyleProxyModeUnique:
			return m_style->position();
	}
}

#pragma mark - Letting Styles

void TWHTMLStyleProxy::apply(const std::wstring &s){
	makeUnique();
	m_uniqueStyle->apply(s);
}

void TWHTMLStyleProxy::setValue(const std::wstring &a, const std::wstring &b){
	makeUnique();
	m_uniqueStyle->setValue(a, b);
}

void TWHTMLStyleProxy::setBackgroundColor(const TWHTMLStyleColor& col){
	makeUnique();
	m_uniqueStyle->setBackgroundColor(col);
}
void TWHTMLStyleProxy::setColor(const TWHTMLStyleColor& col){
	makeUnique();
	m_uniqueStyle->setColor(col);
}
void TWHTMLStyleProxy::setDisplay(TWHTMLStyleDisplay v){
	makeUnique();
	m_uniqueStyle->setDisplay(v);
}
void TWHTMLStyleProxy::setVisibility(bool v){
	makeUnique();
	m_uniqueStyle->setVisibility(v);
}
void TWHTMLStyleProxy::setWidth(const TWHTMLStyleValue& v){
	makeUnique();
	m_uniqueStyle->setWidth(v);
}
void TWHTMLStyleProxy::setHeight(const TWHTMLStyleValue& v){
	makeUnique();
	m_uniqueStyle->setHeight(v);
}
void TWHTMLStyleProxy::setMarginLeft(const TWHTMLStyleValue& v){
	makeUnique();
	m_uniqueStyle->setMarginLeft(v);
}
void TWHTMLStyleProxy::setMarginRight(const TWHTMLStyleValue& v){
	makeUnique();
	m_uniqueStyle->setMarginRight(v);
}
void TWHTMLStyleProxy::setMarginTop(const TWHTMLStyleValue& v){
	makeUnique();
	m_uniqueStyle->setMarginTop(v);
}
void TWHTMLStyleProxy::setMarginBottom(const TWHTMLStyleValue& v){
	makeUnique();
	m_uniqueStyle->setMarginBottom(v);
}
void TWHTMLStyleProxy::setPaddingLeft(const TWHTMLStyleValue& v){
	makeUnique();
	m_uniqueStyle->setPaddingLeft(v);
}
void TWHTMLStyleProxy::setPaddingRight(const TWHTMLStyleValue& v){
	makeUnique();
	m_uniqueStyle->setPaddingRight(v);
}
void TWHTMLStyleProxy::setPaddingTop(const TWHTMLStyleValue& v){
	makeUnique();
	m_uniqueStyle->setPaddingTop(v);
}
void TWHTMLStyleProxy::setPaddingBottom(const TWHTMLStyleValue& v){
	makeUnique();
	m_uniqueStyle->setPaddingBottom(v);
}
void TWHTMLStyleProxy::setBorderLeft(const TWHTMLStyleBorder& v){
	makeUnique();
	m_uniqueStyle->setBorderLeft(v);
}
void TWHTMLStyleProxy::setBorderRight(const TWHTMLStyleBorder& v){
	makeUnique();
	m_uniqueStyle->setBorderRight(v);
}
void TWHTMLStyleProxy::setBorderTop(const TWHTMLStyleBorder& v){
	makeUnique();
	m_uniqueStyle->setBorderTop(v);
}
void TWHTMLStyleProxy::setBorderBottom(const TWHTMLStyleBorder& v){
	makeUnique();
	m_uniqueStyle->setBorderBottom(v);
}
void TWHTMLStyleProxy::setFloat(TWHTMLStyleFloat v){
	makeUnique();
	m_uniqueStyle->setFloat(v);
}
void TWHTMLStyleProxy::setClear(TWHTMLStyleFloat v){
	makeUnique();
	m_uniqueStyle->setClear(v);
}
void TWHTMLStyleProxy::setFontWeight(int v){
	makeUnique();
	m_uniqueStyle->setFontWeight(v);
}
void TWHTMLStyleProxy::setFontSize(const TWHTMLStyleValue&v){
	makeUnique();
	m_uniqueStyle->setFontSize(v);
}
void TWHTMLStyleProxy::setLineHeight(const TWHTMLStyleValue& v){
	makeUnique();
	m_uniqueStyle->setLineHeight(v);
}
void TWHTMLStyleProxy::setTextAlign(TWHTMLStyleTextAlign v){
	makeUnique();
	m_uniqueStyle->setTextAlign(v);
}
void TWHTMLStyleProxy::setTextDecoration(TWHTMLStyleTextDecoration v){
	makeUnique();
	m_uniqueStyle->setTextDecoration(v);
}
void TWHTMLStyleProxy::setTextIndent(const TWHTMLStyleValue& v){
	makeUnique();
	m_uniqueStyle->setTextIndent(v);
}
void TWHTMLStyleProxy::setPosition(TWHTMLStylePosition pos){
	makeUnique();
	m_uniqueStyle->setPosition(pos);
}
void TWHTMLStyleProxy::setWhitespace(TWHTMLStyleWhiteSpace w){
	makeUnique();
	m_uniqueStyle->setWhitespace(w);
}
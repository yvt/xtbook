//
//  TWHTMLStyleProxy.h
//  XTBook
//
//  Created by Kawada Tomoaki on 7/27/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "TWHTMLStyle.h"

#pragma once

enum TWHTMLStyleProxyMode{
	TWHTMLStyleProxyModeNullStyle,
	TWHTMLStyleProxyModeSpecified,
	TWHTMLStyleProxyModeInherited,
	TWHTMLStyleProxyModeUnique
};

class TWHTMLStyleProxy{
	union{
		const TWHTMLStyle *m_style;
		TWHTMLStyle *m_uniqueStyle;
	};
	TWHTMLStyleProxyMode m_mode;
	
	void makeUnique();
	
public:
	TWHTMLStyleProxy();
	TWHTMLStyleProxy(const TWHTMLStyle&);
	TWHTMLStyleProxy(const TWHTMLStyleProxy&);
	~TWHTMLStyleProxy();
	
	void operator =(const TWHTMLStyle&);
	void operator =(const TWHTMLStyleProxy&);
	operator TWHTMLStyle() const;
	
	void inheritFrom(const TWHTMLStyle&);
	void apply(const std::wstring&);
	void setValue(const std::wstring&, const std::wstring&);
	
	int inheritedFontSize() const;
	
	const TWHTMLStyleColor& backgroundColor() const;
	void setBackgroundColor(const TWHTMLStyleColor& col);
	const TWHTMLStyleColor& color() const;
	void setColor(const TWHTMLStyleColor& col);
	
	TWHTMLStyleDisplay display() const;
	void setDisplay(TWHTMLStyleDisplay v);
	bool visibility() const;
	void setVisibility(bool v);
	
	const TWHTMLStyleValue& width() const;
	void setWidth(const TWHTMLStyleValue& v);
	const TWHTMLStyleValue& height() const;
	void setHeight(const TWHTMLStyleValue& v);
	
	const TWHTMLStyleValue& marginLeft() const;
	void setMarginLeft(const TWHTMLStyleValue& v);
	const TWHTMLStyleValue& marginRight() const;
	void setMarginRight(const TWHTMLStyleValue& v);
	const TWHTMLStyleValue& marginTop() const;
	void setMarginTop(const TWHTMLStyleValue& v);
	const TWHTMLStyleValue& marginBottom() const;
	void setMarginBottom(const TWHTMLStyleValue& v);
	
	const TWHTMLStyleValue& paddingLeft() const;
	void setPaddingLeft(const TWHTMLStyleValue& v);
	const TWHTMLStyleValue& paddingRight() const;
	void setPaddingRight(const TWHTMLStyleValue& v);
	const TWHTMLStyleValue& paddingTop() const;
	void setPaddingTop(const TWHTMLStyleValue& v);
	const TWHTMLStyleValue& paddingBottom() const;
	void setPaddingBottom(const TWHTMLStyleValue& v);
	
	const TWHTMLStyleBorder& borderLeft() const;
	void setBorderLeft(const TWHTMLStyleBorder& v);
	const TWHTMLStyleBorder& borderRight() const;
	void setBorderRight(const TWHTMLStyleBorder& v);
	const TWHTMLStyleBorder& borderTop() const;
	void setBorderTop(const TWHTMLStyleBorder& v);
	const TWHTMLStyleBorder& borderBottom() const;
	void setBorderBottom(const TWHTMLStyleBorder& v);
	
	TWHTMLStyleFloat _float() const;
	void setFloat(TWHTMLStyleFloat v);
	TWHTMLStyleFloat clear() const;
	void setClear(TWHTMLStyleFloat v);
	
	int fontWeight() const;
	void setFontWeight(int v);
	const TWHTMLStyleValue& fontSize() const;
	void setFontSize(const TWHTMLStyleValue&v);
	const TWHTMLStyleValue& lineHeight() const;
	void setLineHeight(const TWHTMLStyleValue& v);
	
	TWHTMLStyleTextAlign textAlign() const;
	void setTextAlign(TWHTMLStyleTextAlign v);
	TWHTMLStyleTextDecoration textDecoration() const;
	void setTextDecoration(TWHTMLStyleTextDecoration v);
	const TWHTMLStyleValue& textIndent() const;
	void setTextIndent(const TWHTMLStyleValue& v);
	
	TWHTMLStylePosition position() const;
	void setPosition(TWHTMLStylePosition);
	
	TWHTMLStyleWhiteSpace whitespace() const;
	void setWhitespace(TWHTMLStyleWhiteSpace);
};

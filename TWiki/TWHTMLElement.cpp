/*
 *  TWHTMLElement.cpp
 *  XTBook
 *
 *  Created by Nexhawks on 2/24/11.
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

#include <stdio.h>
#include <string>
#include "TWHTMLElement.h"
#include "TWUtils.h"
#include "TWHTMLBodyElement.h"

#include "HTMLElements.h"

TWHTMLElement::TWHTMLElement(const std::wstring& name):
TWElement(name),
m_type(HTMLElementTypeForName(name.c_str())){
}

TWHTMLElement *TWHTMLElement::nodeWithName(const std::wstring& name){
	TWHTMLElementType type=HTMLElementTypeForName(name.c_str());
    if(type==TWHTMLElementTypeBody)
        return new TWHTMLBodyElement();
	
	return new TWHTMLElement(name);
}

static const std::wstring g_bgcolorAttrName=L"bgcolor";

void TWHTMLElement::calcStyle(){
	m_style=TWHTMLStyle();
	if(TWHTMLElement *parentElement=dynamic_cast<TWHTMLElement *> (parent())){
		m_style.inheritFrom(parentElement->style());
	}
	
	if(m_type!=TWHTMLElementTypeText){
		
		applyCustomStyle();
		body()->applyStylesheet(this, &m_style);
		
		if(hasAttribute(g_bgcolorAttrName)){
			std::wstring col=getAttribute(g_bgcolorAttrName);
			if(!col.empty())
				m_style.setBackgroundColor(col);
		}
		
		m_style.apply(styleAttr());
		
		if(m_style._float()!=TWHTMLStyleFloatNone){
			if(m_style.display()==TWHTMLStyleDisplayInline){
				// inline cannot be float.
				// fix to block.
				m_style.setDisplay(TWHTMLStyleDisplayBlock);
			}
		}
		
		if(m_style.position()!=TWHTMLStylePositionStatic){
			if(styleAttr().find(L"left:")==std::wstring::npos
			   && styleAttr().find(L"right:")==std::wstring::npos){
				m_style.setPosition(TWHTMLStylePositionStatic);
			}
		}
		
		for(TWNode *node=firstChild();node;node=node->nextSibling()){
			if(TWHTMLElement *element=static_cast<TWHTMLElement *> (node))
				element->calcStyle();
		}
		
		
			
	}
}

std::vector<std::wstring> TWHTMLElement::classNames() const{
	std::wstring className=getAttribute(L"class");
	if(className.size()==0)
		return std::vector<std::wstring>();
	else
		return TWUtils::split(className, L" ");
}

TWHTMLBodyElement *TWHTMLElement::body() const{
	TWHTMLElement *element=const_cast<TWHTMLElement *>(this);
	while(element->parent())
		element=static_cast<TWHTMLElement *>(element->parent());
	return dynamic_cast<TWHTMLBodyElement *> (element);
}

std::wstring TWHTMLElement::innerText() const{
	std::wstring text=L"";
	for(TWNode *node=firstChild();node;node=node->nextSibling()){
		if(TWHTMLElement *element=static_cast<TWHTMLElement *> (node))
			text+=element->innerText();
	}
	return text;
}

std::wstring TWHTMLElement::innerHTML() const{
	std::wstring html=L"";
	for(TWNode *node=firstChild();node;node=node->nextSibling()){
		if(TWHTMLElement *element=static_cast<TWHTMLElement *> (node))
			html+=element->outerHTML();
	}
	return html;
}

std::wstring TWHTMLElement::outerHTML() const{
	std::wstring html;
	html=L"<"+name();
	
	for(std::map<std::wstring, std::wstring>::const_iterator it=m_attributes.begin();it!=m_attributes.end();it++){
		html+=L" "+it->first+L"=";
		html+=L"\"";
		html+=TWUtils::escapeHTMLSpecialChars(it->second);
		html+=L"\"";
	}
	
	std::wstring innerHTML=this->innerHTML();
	if(innerHTML.empty()){
		html+=L" />";
	}else{
		html+=L">";
		html+=innerHTML;
		html+=L"</"+name()+L">";
	}
	return html;
}

void TWHTMLElement::setAttribute(const std::wstring& name, 
							 const std::wstring& value){
	if(name==L"style"){
		setStyleAttr(value);
		return;
	}
	if(name.empty()) // fasten!
		return;
	if(value.empty()) // fasten!
		return;
	TWElement::setAttribute(name, value);
}
std::wstring TWHTMLElement::getAttribute(const std::wstring& name) const{
	if(name==L"style")
		return styleAttr();
	return TWElement::getAttribute(name);
}
bool TWHTMLElement::hasAttribute(const std::wstring& name) const{
	if(name==L"style")
		return true;
	return TWElement::hasAttribute(name);
}
void TWHTMLElement::removeAttribute(const std::wstring& name){
	if(name==L"style")
		return;
	TWElement::removeAttribute(name);
}



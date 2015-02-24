/*
 *  TWHTMLBuilder.cpp
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

#include "TWHTMLBuilder.h"
#include "TWHTMLElement.h"
#include "TWHTMLText.h"
#include "assert.h"
#include "TWUtils.h"
#include "TWReader.h"

TWHTMLBuilder::TWHTMLBuilder(TWHTMLElement *root){
	m_elementStack.push_back(root);
	m_rootElement=root;
}
TWHTMLBuilder::~TWHTMLBuilder(){
}

void TWHTMLBuilder::beginElement(const std::wstring& name){
	
	// <li> <dd> <dt> doesn't need close tag.
	// so automatically close it.
	
	std::wstring ee=topElementName();
	if(ee==L"li" || ee==L"dd" || ee==L"dt"){
		if(name==L"li" || name==L"dd" || name==L"dt"){
			endElement();
		}
	}
	
	// <td>, <th> too.
	if(ee==L"td" || ee==L"th"){
		if(name==L"td" || name==L"th"){
			endElement();
		}
	}
	
	// in traditional html, <tr> doesn't need close tag.
	if(name==L"tr"){
		if(ee==L"td" || ee==L"th" || ee==L"tr"){
			endElement();
		}
	}
	
	TWHTMLElement *enclosingElement=topElement();
	TWHTMLElement *childElement=TWHTMLElement::nodeWithName(name);
	enclosingElement->addChild(childElement);
	m_elementStack.push_back(childElement);
}

bool TWHTMLBuilder::hasElementInStack(const std::wstring &name){
	if(topElementName()==name)
		return true;
	for(std::list<TWHTMLElement *>::iterator it=m_elementStack.begin();
		it!=m_elementStack.end();it++){
		TWHTMLElement *element=*it;
		if(element->name()==name)
			return true;
	}
	return false;
}

void TWHTMLBuilder::endElement(const std::wstring& name){
	//assert(topElementName()==name);
	if(!hasElementInStack(name)){
		// cannot close element with the name.
		// such a element doesn't exist in stack.
		return;
	}
	while(!m_elementStack.empty()){
		if(topElementName()==name)
			break;
		if(m_elementStack.size()==1){
			// tidy; don't close <body> without correct endElement call.
			if(topElementName()!=name)
				return;
		}
		endElement();
	}
	try{
		endElement();
	}catch(...){
		throw std::wstring(L"TWHTMLBuilder: couldn't close <"+name+L">.");
	}
}
void TWHTMLBuilder::endElement(){
	if(m_elementStack.empty()){
#ifndef NDEBUG
		abort();
#endif
		return;
		throw std::wstring(L"TWHTMLBuilder: stack is empty.");
	}
	m_elementStack.pop_back();
}

void TWHTMLBuilder::addText(std::wstring const & str){
	TWHTMLElement *top;
	top=topElement();
	
	if(str.empty())
		return;
	
	TWHTMLText *lastText=NULL;
	if(!top->isEmpty()){
		TWHTMLElement *lastElement=static_cast<TWHTMLElement *>(top->lastChild());
		if(lastElement->type()==TWHTMLElementTypeText){
			lastText=static_cast<TWHTMLText *>(lastElement);
		}
	}
	
	if(lastText){
		lastText->appendText(str);
	}else{
		TWHTMLText *newText;
		newText=new TWHTMLText(str);
		top->addChild(newText);
	}
	
}

void TWHTMLBuilder::addRawText(std::wstring const & str){
	if(str.find_first_not_of(L" \r\n")==std::wstring::npos)
		return;
	addText(TWUtils::unescapeHTMLSpecialChars(str));
}

void TWHTMLBuilder::push(TWHTMLElement *elm){
	m_elementStack.push_back(elm);
}
TWHTMLElement *TWHTMLBuilder::pop(){
	TWHTMLElement *oldElement=topElement();
	m_elementStack.pop_back();
	return oldElement;
}
TWHTMLElement *TWHTMLBuilder::topElement(){
	return m_elementStack.back();
}
std::wstring TWHTMLBuilder::topElementName(){
	return topElement()->name();
}

void TWHTMLBuilder::addAttribute(const std::wstring& name, const std::wstring& value){
	topElement()->setAttribute(name, value);
}

int TWHTMLBuilder::depth(){
	return m_elementStack.size();
}

void TWHTMLBuilder::expand(TWReader *reader){
	while(!reader->eof()){
		if(reader->peek()==L'<'){
			//TWReader::PositionType pos=reader->position();
			reader->read();
			
			bool isComment=(reader->peek()==L'!');
			if(isComment){
				reader->read();
				reader->readUntil(L"-->");
				reader->read();reader->read();reader->read();
				continue;
			}
			
			bool isEndElement=(reader->peek()==L'/');
			if(isEndElement)
				reader->read();
			
			std::wstring elementName;
			elementName=reader->readUntilFirstOf(L" />");
			
			if(isEndElement){
				// closing element
				endElement(elementName);
				reader->readUntilFirstOf(L">");
				reader->read();
			}else{
				// opening element. read attributes.
				bool instantClose=false;
				beginElement(elementName);
				while(1){
					while(reader->peek()==L' ' || reader->peek()==13 || reader->peek()==10)
						reader->read();
					
					wchar_t c=reader->peek();
					if(c==L'>'){
						reader->read();
						break;
					}else if(c==L'/'){
						instantClose=true;
						reader->read();
					}else{
						std::wstring attrName;
						std::wstring attrValue;
						attrName=reader->readUntilFirstOf(L"= />");
						
						while(reader->peek()==L' ' || reader->peek()==13 || reader->peek()==10)
							reader->read();
						
						if(reader->peek()==L'='){
							// has value.
							reader->read();
							
							wchar_t closeChar[]=L" />";
							while(reader->peek()==L' ')
								reader->read();
							if(reader->peek()==L'"' || reader->peek()==L'\''){
								closeChar[0]=reader->read();
								closeChar[1]=0;
							}else{
								
							}
							
							
							// read value.
							attrValue=reader->readUntilFirstOf(closeChar);
							
							
							if(closeChar[0]==L'\"' || closeChar[0]==L'\'')
								reader->read();
							
							attrValue=TWUtils::unescapeHTMLSpecialChars(attrValue);
						}else{
							// no value.
							attrValue=attrName;
						}
						
						addAttribute(attrName, attrValue);
						
					}
				}
				
				if(elementName==L"br"){
					instantClose=true;
				}
				
				if(instantClose)
					endElement();
			}
			
		}else{
			std::wstring literal;
			literal=reader->readUntilFirstOf(L"<");
			addRawText(literal);
		}
	}
}





/*
 *  TWPreprocessorFrame.cpp
 *  XTBook
 *
 *  Created by Nexhawks on 2/23/11.
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

#include "TWPreprocessorFrame.h"
#include "TWPreprocessor.h"
#include "TWParser.h"
#include "TWPreprocessorNode.h"
#include "TWPreprocessorText.h"
#include "TWUtils.h"
#include "TWPreprocessorAttribute.h"
#include "TWPreprocessorTemplateFrame.h"
#include <stdlib.h>
#include <wctype.h>

#pragma mark Lifecycle

TWPreprocessorFrame::TWPreprocessorFrame(TWPreprocessor *preprocessor):
m_preprocessor(preprocessor),
m_parser(preprocessor->parser()),
m_title(preprocessor->parser()->title()){
	m_depth=0;
	m_templateDepth=0;
}
TWPreprocessorFrame::~TWPreprocessorFrame(){
}

TWPreprocessorTemplateFrame *TWPreprocessorFrame::newChild(const std::vector<TWPreprocessorNode *>& args, const std::wstring& ttl){
	return new TWPreprocessorTemplateFrame(args, preprocessor(), this, ttl);
}
TWPreprocessorTemplateFrame *TWPreprocessorFrame::newChild(const std::vector<TWPreprocessorNode *>& args){
	return newChild(args, m_title);
}
std::wstring TWPreprocessorFrame::expand(TWPreprocessorNode *root, int flags){
	if(root->isText()){
		TWPreprocessorText *txt=static_cast<TWPreprocessorText *>(root);
		return txt->value();
	}
	if(root->isAttribute())
		return std::wstring();
	m_depth++;
	if(m_depth>128){
		return L"<strong class=\"error\">ERROR: infinite recursion detected!</strong>";
	}
	if(m_templateDepth>16){
		return L"<strong class=\"error\">ERROR: infinite template recursion detected!</strong>";
	}
	const std::wstring& name=root->name();
	std::wstring out;
	
	switch(name[0]){
		case L't':
			if(name==L"template"){
				TWPreprocessorNodeTemplate bits=root->splitTemplate();
				if(flags&TWPreprocessorFlagsNoTemplates){
				expandTemplateWithoutParsing:
					out+=L"{{";
					out+=expand(bits.titleNode, flags);
					for(std::vector<TWPreprocessorNode *>::iterator it=bits.partNodes.begin();
						it!=bits.partNodes.end();it++){
						out+=L'|';
						out+=expand(*it, flags);
					}
					out+=L"}}";
				}else{
					try{
						std::wstring tempText=parser()->braceSubstitution(root, this);
						if(tempText.size()>=2 &&
						   tempText[0]==L'{' &&
						   tempText[1]==L'|'){
							// this template starts with a table.
							// it needs a new line before.
							out+=L'\n';
						}
						out+=tempText;
					}catch(...){
						goto expandTemplateWithoutParsing;
					}
					
				}
			}else if(name==L"tplarg"){
				TWPreprocessorNodeTemplate bits=root->splitTemplate();
				if(flags&TWPreprocessorFlagsNoArgs){
					out+=L"{{{";
					out+=expand(bits.titleNode, flags);
					for(std::vector<TWPreprocessorNode *>::iterator it=bits.partNodes.begin();
						it!=bits.partNodes.end();it++){
						out+=L'|';
						out+=expand(*it, flags);
					}
					out+=L"}}}";
				}else{
					std::wstring tempText=parser()->argSubstitution(root, this);
					if(tempText.size()>=2 &&
					   tempText[0]==L'{' &&
					   tempText[1]==L'|'){
						// this template starts with a table.
						// it needs a new line before.
						out+=L'\n';
					}
					out+=tempText;;
				}
			}else{
				goto genericRecursion;
			}
			break;
		case L'c':
			if(name==L"comment"){
				// comment is ignored.
			}else{
				goto genericRecursion;
			}
			break;
		case L'i':
			if(name==L"ignore"){
				
			}else{
				goto genericRecursion;
			}
			break;
		case L'e':
			if(name==L"ext"){
				out+=parser()->extensionSubstitution(root, this);
			}else{
				goto genericRecursion;
			}
			break;
		case L'h':
			if(name==L"h"){
				TWPreprocessorNodeHeading bits=root->splitHeading();
				
				std::wstring s;
				for(TWNode *node=root->firstChild();node;node=node->nextSibling()){
					s+=expand(static_cast<TWPreprocessorNode *>(node), flags);
				}
				int level=TWUtils::intValueFromString(bits.level);
				s=s.substr(level, s.size()-level*2);
				s=TWUtils::trimString(s);
				out+=L"<h"+bits.level+L" id=\"";
				out+=idForHeading(s);
				out+=L"\">";
				out+=s;
				out+=L"</h"+bits.level+L">";
			}else{
				goto genericRecursion;
			}
			break;
		default:
		genericRecursion:
			for(TWNode *node=root->firstChild();node;node=node->nextSibling()){
				out+=expand(static_cast<TWPreprocessorNode *>(node), flags);
			}
	}
	m_depth--;
	return out;
}
std::wstring TWPreprocessorFrame::implode(const wchar_t * separator, const std::vector<TWPreprocessorNode *>& root, int flags){
	std::wstring str;
	bool first=true;
	for(std::vector<TWPreprocessorNode *>::const_iterator it=root.begin();it!=root.end();it++){
		if(first){
			first=false;
		}else{
			first+=separator;
		}
		
		TWPreprocessorNode *node=*it;
		str+=expand(node, flags);
	}
	return str;
}

std::wstring TWPreprocessorFrame::idForHeading(const std::wstring& heading){
	std::wstring outString;
	outString.reserve(heading.size()*2);
	wchar_t buf[16];
	for(std::wstring::size_type i=0;i<heading.size();i++){
		wchar_t c=heading[i];
		if(iswalpha(c)){
			// alphabet.
			outString+=c;
		}else if(c==L' '){
			// whitespace.
			outString+=L'_';
		}else if(iswalnum(c)){
			// numeric.
			if(i==0)
				outString+=L'.';
			outString+=c;
		}else{
			// anything else.
			int code=(int)(unsigned short)c;
			//wsprintf(buf, ".%x", code);
#ifdef EV_UNSAFE_SWPRINTF
			swprintf(buf, L".%x", code);
#else
			swprintf(buf, 15, L".%x", code);
#endif
			outString+=buf;
		}
	}
	return outString;
}

std::wstring TWPreprocessorFrame::argumentForName(const std::wstring & name){
	return std::wstring();
}


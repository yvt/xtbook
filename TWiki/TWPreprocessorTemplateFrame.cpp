/*
 *  TWPreprocessorTemplateFrame.cpp
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

#include "TWPreprocessorTemplateFrame.h"
#include "TWPreprocessorNode.h"
#include "TWUtils.h"

TWPreprocessorTemplateFrame::TWPreprocessorTemplateFrame(const std::vector<TWPreprocessorNode *>& args, TWPreprocessor *preprocessor, TWPreprocessorFrame *parent,
														 const std::wstring& title):
TWPreprocessorFrame(preprocessor),
m_parent(parent){
	m_title=title;
	m_depth=parent->depth()+1;
	m_templateDepth=parent->templateDepth()+1;
	
	// deal with args...
	for(std::vector<TWPreprocessorNode *>::const_iterator it=args.begin();
		it!=args.end();it++){
		TWPreprocessorNode *arg=*it;
		TWPreprocessorNodePart bits;
		bits=arg->splitArg();
		
		if(bits.index.empty()){
			// named parameter.
			std::wstring name;
			name=expand(bits.nameNode,0);
			name=TWUtils::trimString(name);
			m_namedArgs[name]=bits.valueNode;
		}else{
			// numbered parameter.
			m_numberedArgs[bits.index]=bits.valueNode;
		}
	}
	
}

std::wstring TWPreprocessorTemplateFrame::numberedArgumentForName(const std::wstring & name) {
	std::map<std::wstring, TWPreprocessorNode *>::const_iterator it;
	it=m_numberedArgs.find(name);
	if(it!=m_numberedArgs.end()){
		TWPreprocessorNode *node=it->second;
		std::wstring text;
		text=m_parent->expand(node, 0);
		return text;
	}
	return std::wstring();
}

std::wstring TWPreprocessorTemplateFrame::namedArgumentForName(const std::wstring & name) {
	std::map<std::wstring, TWPreprocessorNode *>::const_iterator it;
	it=m_namedArgs.find(name);
	if(it!=m_namedArgs.end()){
		TWPreprocessorNode *node=it->second;
		std::wstring text;
		text=m_parent->expand(node, 0);
		text=TWUtils::trimString(text);
		return text;
	}
	return std::wstring();
}


std::wstring TWPreprocessorTemplateFrame::argumentForName(const std::wstring & name) {
	std::wstring text=numberedArgumentForName(name);
	if(text.empty()){
		text=namedArgumentForName(name);
	}
	return text;
}

void TWPreprocessorTemplateFrame::dumpArguments(){
	std::map<std::wstring, TWPreprocessorNode *>::const_iterator it;
	puts(" NAMED ARGS: ");
	for(it=m_namedArgs.begin();it!=m_namedArgs.end();it++){
		printf("\n [%s]\n", TWUtils::w2m(it->first).c_str());
		printf("raw=\"%s\"\n", TWUtils::w2m(m_parent->expand(it->second, 12)).c_str());
		printf("parsed=\"%s\"\n", TWUtils::w2m(m_parent->expand(it->second, 0)).c_str());
		
	}
	puts(" NUMBERED ARGS: ");
	for(it=m_numberedArgs.begin();it!=m_numberedArgs.end();it++){
		printf("\n [%s]\n", TWUtils::w2m(it->first).c_str());
		printf("raw=\"%s\"\n", TWUtils::w2m(m_parent->expand(it->second, 12)).c_str());
		printf("parsed=\"%s\"\n", TWUtils::w2m(m_parent->expand(it->second, 0)).c_str());
		
	}
	puts(" DUMP END");
}


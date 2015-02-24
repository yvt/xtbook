/*
 *  TWPreprocessorNode.cpp
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

#include "TWPreprocessorNode.h"
#include "TWPreprocessorAttribute.h"
#include "TWPreprocessorText.h"
#include "TWUtils.h"
#include <assert.h>

#pragma mark Lifecycle

TWPreprocessorNode::TWPreprocessorNode(const std::wstring& nm):
TWNode(nm){
	
}

TWPreprocessorNode *TWPreprocessorNode::nodeWithText(const std::wstring& name,
													 const std::wstring& text){
	TWPreprocessorNode *node=new TWPreprocessorNode(name);
	node->addChild(new TWPreprocessorText(text));
	return node;
}

TWPreprocessorNode::~TWPreprocessorNode(){
	
}


#pragma mark -
#pragma mark Serialization

std::wstring TWPreprocessorNode::toString() const{
	std::wstring inner, attribs;
	for(TWNode *outNode=m_firstChild;outNode;outNode=outNode->nextSibling()){
		TWPreprocessorNode *node=dynamic_cast<TWPreprocessorNode *>(outNode);
		if(!node)
			continue;
		if(TWPreprocessorAttribute *attr=dynamic_cast<TWPreprocessorAttribute *>(node)){
			attribs+=L" ";
			attribs+=attr->name();
			attribs+=L"=\"";
			attribs+=TWUtils::escapeHTMLSpecialChars(attr->value());
			attribs+=L"\"";
		}else{
			inner+=node->toString();
		}
	}
	if(inner.size())
		return L"<"+m_name+attribs+L">"+inner+L"</"+m_name+L">";
	else
		return L"<"+m_name+attribs+L"/>";
}

#pragma mark -
#pragma mark Split

TWPreprocessorNodePart TWPreprocessorNode::splitArg() const{
	TWPreprocessorNodePart bits={NULL, NULL, L""};
	for(TWNode *outNode=m_firstChild;outNode;outNode=outNode->nextSibling()){
		TWPreprocessorNode *node=dynamic_cast<TWPreprocessorNode *>(outNode);
		if(!node)
			continue;
		if(node->name()==L"name"){
			bits.nameNode=node;
			if(TWPreprocessorAttribute *indexNode=
			   dynamic_cast<TWPreprocessorAttribute *>(node->firstChild())){
				if(indexNode->name()==L"index"){
					bits.index=indexNode->value();
				}
			}
		}else if(node->name()==L"value"){
			bits.valueNode=node;
		}
	}
	assert(bits.nameNode != NULL);
	return bits;
}
TWPreprocessorNodeExt TWPreprocessorNode::splitExt() const{
	TWPreprocessorNodeExt bits={NULL, NULL, NULL, NULL};
	for(TWNode *outNode=m_firstChild;outNode;outNode=outNode->nextSibling()){
		TWPreprocessorNode *node=dynamic_cast<TWPreprocessorNode *>(outNode);
		if(!node)
			continue;
		if(node->name()==L"name"){
			bits.nameNode=node;
		}else if(node->name()==L"attr"){
			bits.attrNode=node;
		}else if(node->name()==L"inner"){
			bits.innerNode=node;
		}else if(node->name()==L"close"){
			bits.closeNode=node;
		}
	}
	assert(bits.nameNode != NULL);
	return bits;
}
TWPreprocessorNodeHeading TWPreprocessorNode::splitHeading() const{
	assert(name() == L"h");
	TWPreprocessorNodeHeading bits;
	for(TWNode *outNode=m_firstChild;outNode;outNode=outNode->nextSibling()){
		TWPreprocessorNode *node=dynamic_cast<TWPreprocessorNode *>(outNode);
		if(!node)
			continue;
		if(TWPreprocessorAttribute *attr=dynamic_cast<TWPreprocessorAttribute *>(node)){
			if(node->name()==L"i"){
				bits.i=attr->value();
			}else if(node->name()==L"level"){
				bits.level=attr->value();
			}
		}
	}
	assert(bits.i.size() > 0);
	return bits;
}
TWPreprocessorNodeTemplate TWPreprocessorNode::splitTemplate() const{
	TWPreprocessorNodeTemplate bits;
	for(TWNode *outNode=m_firstChild;outNode;outNode=outNode->nextSibling()){
		TWPreprocessorNode *node=dynamic_cast<TWPreprocessorNode *>(outNode);
		if(!node)
			continue;
		if(node->name()==L"title"){
			bits.titleNode=node;
		}else if(node->name()==L"lineStart"){
			bits.lineStart=L"1";
		}else if(node->name()==L"part"){
			bits.partNodes.push_back(node);
		}
	}
	assert(bits.titleNode != NULL);
	return bits;
}
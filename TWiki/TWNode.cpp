/*
 *  TWNode.cpp
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

#include "TWNode.h"
#include <assert.h>

#pragma mark Lifecycle

TWNode::TWNode(const std::wstring& name):
m_name(name),
m_firstChild(NULL),
m_lastChild(NULL),
m_prevSibling(NULL),
m_nextSibling(NULL),
m_parent(NULL),
m_count(0){
	
}
TWNode::~TWNode(){
	TWNode *node=m_firstChild;
	while(node){
		TWNode *nextNode=node->nextSibling();
		delete node;
		node=nextNode;
	}
	if(m_parent){
		m_parent->removeChild(this);
	}
}


#pragma mark -
#pragma mark Tree
void TWNode::addChild(TWNode *newChild){
	if(!newChild)
		return;
	if(newChild->m_parent){
		newChild->m_parent->removeChild(newChild);
	}
	assert(newChild->m_parent==NULL);
	if(m_lastChild==NULL){
		// empty.
		m_firstChild=newChild;
		m_lastChild=newChild;
		newChild->m_parent=this;
		newChild->m_prevSibling=NULL;
		newChild->m_nextSibling=NULL;
	}else{
		assert(m_lastChild->m_nextSibling==NULL);
		m_lastChild->m_nextSibling=newChild;
		newChild->m_parent=this;
		newChild->m_prevSibling=m_lastChild;
		newChild->m_nextSibling=NULL;
		m_lastChild=newChild;
	}
	m_count++;
}

void TWNode::insertChild(TWNode *newChild, TWNode *nodeBefore){
	if(newChild->m_parent){
		newChild->m_parent->removeChild(newChild);
	}
	assert(newChild->m_parent==NULL);
	if(nodeBefore==m_firstChild){
		newChild->m_nextSibling=m_firstChild;
		newChild->m_prevSibling=NULL;
		newChild->m_parent=this;
		m_firstChild=newChild;
	}else{
		newChild->m_nextSibling=nodeBefore;
		newChild->m_prevSibling=nodeBefore->m_prevSibling;
		nodeBefore->m_prevSibling->m_nextSibling=newChild;
		nodeBefore->m_prevSibling=newChild;
		newChild->m_parent=this;
	}
	m_count++;
}

void TWNode::removeChild(TWNode *oldChild){
	assert(oldChild->m_parent==this);
	if(oldChild==m_firstChild && oldChild==m_lastChild){
		// it is the last child.
		assert(oldChild->m_prevSibling==NULL);
		assert(oldChild->m_nextSibling==NULL);
		m_firstChild=NULL;
		m_lastChild=NULL;
		oldChild->m_parent=NULL;
		oldChild->m_prevSibling=NULL;
		oldChild->m_nextSibling=NULL;
	}else if(oldChild==m_firstChild){
		assert(oldChild->m_prevSibling==NULL);
		m_firstChild=oldChild->m_nextSibling;
		m_firstChild->m_prevSibling=NULL;
		oldChild->m_parent=NULL;
		oldChild->m_prevSibling=NULL;
		oldChild->m_nextSibling=NULL;
	}else if(oldChild==m_lastChild){
		assert(oldChild->m_nextSibling==NULL);
		m_lastChild=oldChild->m_prevSibling;
		m_lastChild->m_nextSibling=NULL;
		oldChild->m_parent=NULL;
		oldChild->m_prevSibling=NULL;
		oldChild->m_nextSibling=NULL;
	}else{
		oldChild->m_prevSibling->m_nextSibling=oldChild->m_nextSibling;
		oldChild->m_nextSibling->m_prevSibling=oldChild->m_prevSibling;
		oldChild->m_parent=NULL;
		oldChild->m_prevSibling=NULL;
		oldChild->m_nextSibling=NULL;
	}
	m_count--;
}

void TWNode::removeAllChildren(){
	TWNode *node=m_firstChild;
	while(node){
		TWNode *nextNode=node->nextSibling();
		assert(node->m_parent==this);
		node->m_parent=NULL;
		node->m_prevSibling=NULL;
		node->m_nextSibling=NULL;
		node=nextNode;
	}
	m_firstChild=NULL;
	m_lastChild=NULL;
}
void TWNode::removeAndDestroyAllChildren(){
	TWNode *node=m_firstChild;
	while(node){
		TWNode *nextNode=node->nextSibling();
		delete node;
		node=nextNode;
	}
	m_firstChild=NULL;
	m_lastChild=NULL;
}



std::vector<TWNode *> 
TWNode::children() const{
	std::vector<TWNode *> children;
	for(TWNode *node=m_firstChild;node;node=node->m_nextSibling){
		children.push_back(node);
	}
	return children;
}

std::vector<TWNode *> 
TWNode::childrenOfType(const std::wstring& name) const{
	std::vector<TWNode *> children;
	for(TWNode *node=m_firstChild;node;node=node->m_nextSibling){
		if(node->m_name==name)
			children.push_back(node);
	}
	return children;
}

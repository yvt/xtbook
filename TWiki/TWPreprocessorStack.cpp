/*
 *  TWPreprocessorStack.cpp
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

#include "TWPreprocessorStack.h"
#include <assert.h>

static const std::wstring g_rootTagName=L"root";
static const std::wstring g_breakTagName=L"break";
static const std::wstring g_partTagName=L"part";
static const std::wstring g_pipe=L"|";

#pragma mark Part

TWPreprocessorPart::TWPreprocessorPart(const std::wstring& outString){
	accum=new TWPreprocessorAccum(g_partTagName);
	if(outString.size())
		accum->addLiteral(outString);
	commentEnd=std::wstring::npos;
	visualEnd=std::wstring::npos;
	eqpos=NULL;
}
TWPreprocessorPart::~TWPreprocessorPart(){
	if(accum->parent()==NULL)
		delete accum;
}

void TWPreprocessorPart::markLastChildAsEqpos(){
	eqpos=static_cast<TWPreprocessorNode *>(accum->lastChild());
}

#pragma mark -
#pragma mark Stack Element

TWPreprocessorStackElement::~TWPreprocessorStackElement(){
	removeAllParts();
}

void TWPreprocessorStackElement::removeAllParts(){
	for(std::vector<TWPreprocessorPart *>::iterator it=parts.begin();it!=parts.end();it++)
		delete *it;
	parts.clear();
}

void TWPreprocessorStackElement::addPart(const std::wstring& outString){
	parts.reserve(16);
	parts.push_back(new TWPreprocessorPart(outString));
}

TWPreprocessorStackFlags TWPreprocessorStackElement::getFlags() const{
	TWPreprocessorStackFlags bits;
	
	/* optimizing:
	bits.findPipe=open!=L"\n" && open!=L"[";*/
	bits.findPipe=true;
	if(open.size()==1){
		if(open[0]==L'\n')
			bits.findPipe=false;
		else if(open[0]==L'[')
			bits.findPipe=false;
	}
	
	bits.findEquals=bits.findPipe && parts.size()>1;
	if(parts.size())
		bits.findEquals=bits.findEquals && (parts[parts.size()-1]->eqpos==NULL);
	bits.inHeading=open==L"\n";
	return bits;
}

TWPreprocessorAccum *TWPreprocessorStackElement::accum() const{
	return parts[parts.size()-1]->accum;
}

TWPreprocessorPart *TWPreprocessorStackElement::currentPart() const{
	return parts[parts.size()-1];
}

TWPreprocessorAccum *TWPreprocessorStackElement::breakSyntax(int openingCount){
	if(openingCount==-1)
		openingCount=count;
	if(open==L"\n"){
		return this->parts[0]->accum;
	}else{
		TWPreprocessorAccum *accum=new TWPreprocessorAccum(g_breakTagName);
		std::wstring opening;
		for(int n=0;n<openingCount;n++)
			opening+=open;
		accum->addLiteral(opening);
		for(int n=0;n<parts.size();n++){
			if(n>0)
				accum->addLiteral(g_pipe);
			accum->addAccum(parts[n]->accum);
		}
		return accum;
	}
}

#pragma mark -
#pragma mark Stack

TWPreprocessorStack::TWPreprocessorStack(TWPreprocessorStackArena*arena){
	rootAccum=new TWPreprocessorAccum(g_rootTagName);
	m_arena=arena;
	m_baseIndex=arena->stackCount;
}
TWPreprocessorStack::~TWPreprocessorStack(){
	// pop any elements from arena stack.
	for(size_t i=m_baseIndex;i<m_arena->stackCount;i++)
		delete m_arena->stack[i];
	m_arena->stackCount=m_baseIndex;
}
TWPreprocessorAccum *TWPreprocessorStack::accum() const{
	if(empty())
		return rootAccum;
	else
		return top()->accum();
}
TWPreprocessorPart *TWPreprocessorStack::currentPart() const{
	if(empty()){
		return NULL;
	}else{
		return top()->currentPart();
	}
}
void TWPreprocessorStack::push(TWPreprocessorStackElement *element){
	//elementStack.push_back(element);
	m_arena->push(element);
}
TWPreprocessorStackElement *TWPreprocessorStack::pop(){
	if(empty())
		return NULL;
	TWPreprocessorStackElement *oldElement=m_arena->top();
	m_arena->pop();
	return oldElement;
}
void TWPreprocessorStack::addPart(const std::wstring& outString){
	assert(!empty());
	top()->addPart(outString);
}
TWPreprocessorStackFlags TWPreprocessorStack::getFlags() const{
	if(empty()){
		return TWPreprocessorStackFlags();
	}else{
		return top()->getFlags();
	}
}
TWPreprocessorStackElement *TWPreprocessorStack::top() const{
	return m_arena->top();
}

/*
 *  TWPreprocessorStack.h
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

#pragma once
#include <string>
#include <vector>
#include <stack>
#include "TWPreprocessorAccum.h"
#include <assert.h>

struct TWPreprocessorStackFlags{
	bool findEquals;
	bool findPipe;
	bool inHeading;
	TWPreprocessorStackFlags():
	findEquals(false),
	findPipe(false),
	inHeading(false){}
};
struct TWPreprocessorPart{
	
	TWPreprocessorNode *eqpos;
	std::wstring::size_type commentEnd;
	std::wstring::size_type visualEnd;
	
	// accum will be automatically freed ONLY when
	// it doesn't have a parent.
	TWPreprocessorAccum *accum;
	TWPreprocessorPart(const std::wstring& outString);
	~TWPreprocessorPart();
	
	void markLastChildAsEqpos();
};
struct TWPreprocessorStackElement{
	std::wstring open;
	std::wstring close;
	int count;
	std::vector<TWPreprocessorPart *> parts; // will be freed
	bool lineStart;
	std::wstring::size_type startPos;
	
	TWPreprocessorStackElement(){
		startPos=std::wstring::npos;
	}
	~TWPreprocessorStackElement();
	void addPart(const std::wstring& outString);
	TWPreprocessorStackFlags getFlags() const;
	TWPreprocessorAccum *accum() const;
	TWPreprocessorPart *currentPart() const;
	TWPreprocessorAccum *breakSyntax(int openingCount=-1);
	void removeAllParts();
	
};	

struct TWPreprocessorStackArena{
	TWPreprocessorStackElement *stack[64];
	size_t stackCount;
	TWPreprocessorStackArena(){
		stackCount=0;
	}
	void push(TWPreprocessorStackElement *e){
		assert(stackCount<512);
		stack[stackCount]=e;
		stackCount++;
	}
	TWPreprocessorStackElement *top() const{
		if(stackCount==0)
			return NULL;
		return stack[stackCount-1];
	}
	void pop(){
		stackCount--;
	}
	
};

class TWPreprocessorStack{
	
protected:
	/*
	// elements in the stack will be automacically freed by ~TWPreprocessorStack().
	// using vector for performance.
	std::vector<TWPreprocessorStackElement *> elementStack;*/
	
	TWPreprocessorStackArena *m_arena;
	size_t m_baseIndex;
	
public:
	// rootAccum will NOT be automatcally freed, but
	// created in the constructor.
	TWPreprocessorAccum *rootAccum;
	
	TWPreprocessorStack(TWPreprocessorStackArena *arena);
	~TWPreprocessorStack();
	//int count() const{return elementStack.size();}
	int count() const{return (int)(m_arena->stackCount-m_baseIndex);}
	TWPreprocessorAccum *accum() const;
	TWPreprocessorPart *currentPart() const;
	TWPreprocessorStackElement *top() const;
	void push(TWPreprocessorStackElement *);
	TWPreprocessorStackElement *pop();
	void addPart(const std::wstring&);
	TWPreprocessorStackFlags getFlags() const;
	bool empty() const{return m_arena->stackCount==m_baseIndex;}
};




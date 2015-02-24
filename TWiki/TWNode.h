/*
 *  TWNode.h
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
#include "TWTypes.h"


class TWNode{
protected:
	TWNode *m_parent;
	TWNode *m_firstChild;
	TWNode *m_lastChild;
	TWNode *m_nextSibling;
	TWNode *m_prevSibling;
	std::wstring m_name;
	int m_count;
public:
	TWNode(const std::wstring&);
	virtual ~TWNode();
	
	TWNode *parent() const{return m_parent;}
	TWNode *firstChild() const{return m_firstChild;}
	TWNode *lastChild() const{return m_lastChild;}
	TWNode *nextSibling() const{return m_nextSibling;}
	TWNode *prevSibling() const{return m_prevSibling;}
	const std::wstring& name() const{return m_name;}
	void setName(const std::wstring& name){m_name=name;}
	
	virtual void addChild(TWNode *newChild);
	void insertChild(TWNode *newChild, TWNode *nodeBefore);
	void removeChild(TWNode *oldChild);
	void removeAllChildren();
	void removeAndDestroyAllChildren();
	
	
	std::vector<TWNode *> children() const;
	std::vector<TWNode *> childrenOfType(const std::wstring&) const;
	int countOfChildren() const{return m_count;}
	bool isEmpty() const{return countOfChildren()==0;}
	
};


typedef void (*TWNodeIterateCallback)(void *, TWNode *);



//
//  TPLObject.cpp
//  XTBook
//
//  Created by Kawada Tomoaki on 5/21/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//
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

#include "TPLObject.h"
#include <assert.h>

TPLObject::TPLObject():
m_referenceCount(1){
	 
}

TPLObject::TPLObject(const TPLObject&):
m_referenceCount(1){
}

TPLObject::~TPLObject(){
	
}

TPLObject *TPLObject::retain(){
	assert(this);
	m_referenceCount++;
	return this;
}

void TPLObject::release(){
	assert(this);
	m_referenceCount--;
	if(!m_referenceCount){
		delete this;
	}
}

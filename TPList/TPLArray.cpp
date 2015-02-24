//
//  TPLArray.cpp
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

#include "TPLArray.h"
#include <stdarg.h>
#include <algorithm>

TPLArray::TPLArray(){
}

TPLArray::TPLArray(TPLObject *firstObject){
	addObject(firstObject);
}

TPLArray::TPLArray(TPLObject *firstObject, ...){
	if(!firstObject)
		return;
	addObject(firstObject);
	
	va_list va;
	TPLObject *obj;
	
	va_start(va, firstObject);
	while ((obj=va_arg(va, TPLObject *))!=NULL) {
		addObject(obj);
	}
	va_end(va);
}

TPLArray::TPLArray(const TPLArray *array){
	addObjectsFromArray(array);
}

TPLArray::~TPLArray(){
	removeAllObjects();
}

#pragma mark - Adding Object

void TPLArray::addObject(TPLObject *obj){
	if(!obj)
		return;
	m_objects.push_back(obj->retain());
}

void TPLArray::setObject(TPLObject *obj, TPLUInteger atIndex){
	if(atIndex>=count())
		atIndex=count()-1;
	if(!obj)
		return;
	if(m_objects[atIndex]==obj)
		return;
	m_objects[atIndex]->release();
	m_objects[atIndex]=obj->retain();
}

void TPLArray::addObjects(TPLObject *firstObject, ...){
	if(!firstObject)
		return;
	addObject(firstObject->retain());
	
	va_list va;
	TPLObject *obj;
	
	va_start(va, firstObject);
	while ((obj=va_arg(va, TPLObject *))!=NULL) {
		addObject(obj->retain());
	}
	va_end(va);
}

void TPLArray::addObjectsFromArray(const TPLArray * array){
	for(TPLUInteger i=0;i<array->count();i++)
		addObject(array->objectAtIndex(i));
}

void TPLArray::insertObject(TPLObject *obj, TPLUInteger atIndex){
	if(!obj)
		return;
	if(atIndex>count())
		atIndex=count();
	m_objects.insert(m_objects.begin()+atIndex, obj->retain());
}

void TPLArray::insertObject(TPLObject *obj, TPLObject *at){
	insertObject(obj, indexForObject(at));
}

#pragma mark - Removing Object

void TPLArray::removeAllObjects(){
	for(std::vector<TPLObject *>::iterator it=m_objects.begin();
		it!=m_objects.end();it++){
		(*it)->release();
	}
	m_objects.clear();
}

void TPLArray::removeObjectAtIndex(TPLUInteger index){
	if(index>=count())
		return;
	std::vector<TPLObject *>::iterator it=m_objects.begin()+index;
	(*it)->release();
	m_objects.erase(it);
}

void TPLArray::removeObject(TPLObject *obj){
	removeObjectAtIndex(indexForObject(obj));
}

#pragma mark - Query

TPLObject *TPLArray::objectAtIndex(TPLUInteger index) const{
	if(index>=count())
		return NULL;
	return m_objects[index];
}

TPLUInteger TPLArray::indexForObject(TPLObject *obj) const{
	std::vector<TPLObject *>::const_iterator it;
	it=std::find(m_objects.begin(), m_objects.end(),
				 obj);
	if(it==m_objects.end())
		return TPLNotFound;
	return it-m_objects.begin();
}

TPLUInteger TPLArray::count() const{
	return m_objects.size();
}



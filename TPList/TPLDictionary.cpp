//
//  TPLDictionary.cpp
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

#include "TPLDictionary.h"
#include <stdarg.h>
#include "TPLArray.h"
#include "TPLString.h"

TPLDictionary::TPLDictionary(){
}

TPLDictionary::TPLDictionary(TPLObject *firstObject,
							 const char *firstKey){
	setObject(firstObject, firstKey);
}

TPLDictionary::TPLDictionary(TPLObject *firstObject,
							 const char *firstKey,
							 ...){
	if(!firstObject)
		return;
	setObject(firstObject, firstKey);
	
	va_list va;
	TPLObject *obj;
	
	va_start(va, firstKey);
	while((obj=va_arg(va, TPLObject *))!=NULL){
		const char *key=va_arg(va, const char *);
		setObject(obj, key);
	}
	va_end(va);
}
TPLDictionary::TPLDictionary(const TPLDictionary *dic){
	addEntriesFromDictionary(dic);
}

TPLDictionary::~TPLDictionary(){
	removeAllObjects();
}

#pragma mark - Modification of Object

void TPLDictionary::addEntriesFromDictionary(const TPLDictionary *dic){
	if(!dic)
		return;
	const std::map<std::string, TPLObject *>& d=dic->STLDictionary();
	for(std::map<std::string, TPLObject *>::const_iterator it=d.begin();
		it!=d.end();it++){
		setObject(it->second, it->first.c_str());
	}
}

void TPLDictionary::setObject(TPLObject *obj, const char *forKey){
	if(!obj)
		return;
	if(!forKey)
		return;
	std::map<std::string, TPLObject *>::iterator it;
	it=m_objects.find(forKey);
	if(it==m_objects.end()){
		m_objects[forKey]=obj->retain();
	}else{
		if(it->second==obj)
			return;
		it->second->release();
		it->second=obj->retain();
	}
}

#pragma mark - Removing Object

void TPLDictionary::removeAllObjects(){
	for(std::map<std::string, TPLObject *>::iterator it=m_objects.begin();
		it!=m_objects.end();it++){
		it->second->release();
	}
	m_objects.clear();
}

void TPLDictionary::removeObjectForKey(const char *key){
	if(!key)
		return;
	std::map<std::string, TPLObject *>::iterator it;
	it=m_objects.find(key);
	if(it!=m_objects.end()){
		it->second->release();
		m_objects.erase(it);
	}
}

#pragma mark - Query

TPLObject *TPLDictionary::objectForKey(const char *key) const{
	if(!key)
		return NULL;
	std::map<std::string, TPLObject *>::const_iterator it;
	it=m_objects.find(key);
	if(it==m_objects.end()){
		return NULL;
	}else{
		return it->second;
	}
}

TPLArray *TPLDictionary::allKeys() const{
	TPLArray *array=new TPLArray();
	for(std::map<std::string, TPLObject *>::const_iterator it=m_objects.begin();
		it!=m_objects.end();it++){
		TPLString *str=new TPLString(it->first.c_str());
		array->addObject(str);
		str->release();
	}
	return array;
}

TPLArray *TPLDictionary::allObjects() const{
	TPLArray *array=new TPLArray();
	for(std::map<std::string, TPLObject *>::const_iterator it=m_objects.begin();
		it!=m_objects.end();it++){
		array->addObject(it->second);
	}
	return array;
}

TPLUInteger TPLDictionary::count() const{
	return m_objects.size();
}


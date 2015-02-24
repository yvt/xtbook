//
//  TPLNumber.cpp
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

#include <stdio.h>
#include <stdlib.h>
#include "TPLNumber.h"
#include "TPLString.h"
#include <string.h>

TPLNumber::TPLNumber(){
	setZero();
}

TPLNumber::TPLNumber(int value){
	setIntValue(value);
}

TPLNumber::TPLNumber(unsigned int value){
	setUIntValue(value);
}

TPLNumber::TPLNumber(double value){
	setDoubleValue(value);
}

TPLNumber::TPLNumber(bool value){
	setBoolValue(value);
}

TPLNumber::TPLNumber(const char *value){
	setStringValue(value);
}

TPLNumber::~TPLNumber(){
}

#pragma mark - Set

void TPLNumber::setZero(){
	m_intValue=0;
	m_unsignedIntValue=0;
	m_doubleValue=0.;
	m_isBool=false;
}

void TPLNumber::setIntValue(int i){
	m_intValue=i;
	m_unsignedIntValue=(unsigned int)i;
	m_doubleValue=i;
	m_isBool=false;
}

void TPLNumber::setUIntValue(unsigned int i){
	m_intValue=(int)i;
	m_unsignedIntValue=i;
	m_doubleValue=i;
	m_isBool=false;
}

void TPLNumber::setDoubleValue(double value){
	m_intValue=(int)value;
	m_unsignedIntValue=(unsigned int)value;
	m_doubleValue=value;
	m_isBool=false;
}

void TPLNumber::setBoolValue(bool value){
	m_intValue=(value?1:0);
	m_unsignedIntValue=(value?1:0);
	m_doubleValue=(value?1.:0.);
	m_isBool=true;
}

void TPLNumber::setStringValue(const char *str){
	if(!str){
		setZero();
		return;
	}
	sscanf(str, "%d", &m_intValue);
	sscanf(str, "%u", &m_unsignedIntValue);
	sscanf(str, "%lf", &m_doubleValue);
	m_isBool=false;
	if(!strcmp(str, "yes")){
		setBoolValue(true);
	}
	if(!strcmp(str, "no")){
		setBoolValue(false);
	}
	if(!strcmp(str, "true")){
		setBoolValue(true);
	}
	if(!strcmp(str, "false")){
		setBoolValue(false);
	}
}

#pragma mark - Query

TPLString *TPLNumber::stringValue() const{
	char buf[64];
	if(m_isBool)
		return new TPLString(m_intValue?"true":"false");
	if((double)m_intValue==m_doubleValue){
		sprintf(buf, "%d", m_intValue);
	}else{
		sprintf(buf, "%lf", m_doubleValue);
	}
	return new TPLString(buf);
}






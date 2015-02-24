//
//  TPLNumber.h
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

#pragma once

#include <stdint.h>
#include "TPLObject.h"

class TPLString;

class TPLNumber: public TPLObject{
private:
	bool m_isBool;
	int m_intValue;
	unsigned int m_unsignedIntValue;
	double m_doubleValue;
public:
	TPLNumber();
	TPLNumber(int);
	TPLNumber(unsigned int);
	TPLNumber(double);
	TPLNumber(bool);
	TPLNumber(const char *);
	virtual ~TPLNumber();
	
	void setZero();
	void setIntValue(int);
	void setUIntValue(unsigned int);
	void setDoubleValue(double);
	void setBoolValue(bool);
	void setStringValue(const char *);
	
	int intValue() const{
		return m_intValue;
	}
	unsigned int uintValue() const{
		return m_unsignedIntValue;
	}
	double doubleValue() const{
		return m_doubleValue;
	}
	bool boolValue() const{
		return m_intValue!=0;
	}
	TPLString *stringValue() const;
};

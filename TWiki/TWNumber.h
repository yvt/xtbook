//
//  TWNumber.h
//  XTBook
//
//  Created by Kawada Tomoaki on 7/25/11.
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
#include <string>

typedef double TWNumberFloat;
typedef int64_t TWNumberInteger;

enum TWNumberRepresentationType{
	TWNumberRepresentationTypeInteger=0,
	TWNumberRepresentationTypeFloat
};

union TWNumberRepresentation{
	TWNumberFloat floatValue;
	TWNumberInteger integerValue;
};

TWNumberRepresentation TWMakeNumberRepresentation(TWNumberInteger);
TWNumberRepresentation TWMakeNumberRepresentation(TWNumberFloat);

class TWNumber{
protected:
	TWNumberRepresentation m_representations;
	TWNumberRepresentationType m_representationType;
	
public:
	TWNumber();
	TWNumber(TWNumberInteger);
	TWNumber(TWNumberFloat);
	TWNumber(bool);
	TWNumber(const std::wstring&);
	~TWNumber(){}
	
	TWNumber floatNumber() const;
	TWNumberFloat floatValue() const;
	TWNumberInteger integerValue() const;
	bool isZero() const;
	bool isFloat() const{
		return m_representationType==TWNumberRepresentationTypeFloat;
	}
	
	std::wstring toString() const;
	
	TWNumber operator +(const TWNumber&) const;
	TWNumber operator -(const TWNumber&) const;
	TWNumber operator *(const TWNumber&) const;
	TWNumber operator /(const TWNumber&) const;
	TWNumber operator %(const TWNumber&) const;
	TWNumber operator &&(const TWNumber&) const;
	TWNumber operator ||(const TWNumber&) const;
	TWNumber operator !() const;
	TWNumber operator -() const;
	
	TWNumber operator ==(const TWNumber&) const;
	TWNumber operator !=(const TWNumber&) const;
	TWNumber operator <(const TWNumber&) const;
	TWNumber operator >(const TWNumber&) const;
	TWNumber operator <=(const TWNumber&) const;
	TWNumber operator >=(const TWNumber&) const;
	
	static TWNumber makeExponent(const TWNumber&,
								 const TWNumber&);

	static TWNumber pow(const TWNumber&,
								 const TWNumber&);
	
	static TWNumber exp(const TWNumber&);
	static TWNumber ln(const TWNumber&);
	
	static TWNumber sin(const TWNumber&);
	static TWNumber cos(const TWNumber&);
	static TWNumber tan(const TWNumber&);
	
	static TWNumber asin(const TWNumber&);
	static TWNumber acos(const TWNumber&);
	static TWNumber atan(const TWNumber&);
	
	static TWNumber ceil(const TWNumber&);
	static TWNumber floor(const TWNumber&);
	static TWNumber trunc(const TWNumber&);
	static TWNumber abs(const TWNumber&);
	static TWNumber round(const TWNumber&,
						  const TWNumber&);
	
};

extern const TWNumber TWNumberPi;
extern const TWNumber TWNumberE;

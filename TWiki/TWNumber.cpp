//
//  TWNumber.cpp
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

#include <stdlib.h>
#include "TWNumber.h"
#include "TWUtils.h"
#include <string>
#include <math.h>

const TWNumber TWNumberPi(M_PI);
const TWNumber TWNumberE(M_E);

TWNumberRepresentation TWMakeNumberRepresentation(TWNumberInteger value){
	TWNumberRepresentation rep;
	rep.integerValue=value;
	return rep;
}
TWNumberRepresentation TWMakeNumberRepresentation(TWNumberFloat value){
	TWNumberRepresentation rep;
	rep.floatValue=value;
	return rep;
}

TWNumber::TWNumber():
m_representations(TWMakeNumberRepresentation((TWNumberInteger)0)){}

TWNumber::TWNumber(bool value):
m_representations(TWMakeNumberRepresentation(value?1LL:0LL)),
m_representationType(TWNumberRepresentationTypeInteger){}

TWNumber::TWNumber(TWNumberInteger value):
m_representations(TWMakeNumberRepresentation(value)),
m_representationType(TWNumberRepresentationTypeInteger){}

TWNumber::TWNumber(TWNumberFloat value):
m_representations(TWMakeNumberRepresentation(value)),
m_representationType(TWNumberRepresentationTypeFloat){}

TWNumber::TWNumber(const std::wstring& str){
	std::string mbsstr=TWUtils::w2m(str);
	if(mbsstr.find('.')!=std::wstring::npos){
		// dot found; float number.
		m_representationType=TWNumberRepresentationTypeFloat;
		m_representations.floatValue=atof(mbsstr.c_str());
	}else{
		// maybe integer.
		if(str.size()>18){
			// its' possible it doesn't fit in 64-bit.
			// convert it to float.
			m_representationType=TWNumberRepresentationTypeFloat;
			m_representations.floatValue=atof(mbsstr.c_str());
		}else{
			// it's safe to assume it fit in 64-bit.
			m_representationType=TWNumberRepresentationTypeInteger;
			sscanf(mbsstr.c_str(), "%lld", &(m_representations.integerValue));
		}
	}
}

TWNumber TWNumber::floatNumber() const{
	if(m_representationType==TWNumberRepresentationTypeFloat)
		return *this;
	else//if(m_representationType==TWNumberRepresentationTypeInteger)
		return TWNumber(floatValue());
}

TWNumberFloat TWNumber::floatValue() const{
	if(m_representationType==TWNumberRepresentationTypeFloat)
		return m_representations.floatValue;
	else
		return (double)(m_representations.integerValue);
}

TWNumberInteger TWNumber::integerValue() const{
	if(m_representationType==TWNumberRepresentationTypeInteger)
		return m_representations.integerValue;
	else
		return (TWNumberInteger)(m_representations.floatValue);
}

bool TWNumber::isZero() const{
	if(m_representationType==TWNumberRepresentationTypeInteger)
		return m_representations.integerValue==0;
	else
		return m_representations.floatValue==0.;
}

#pragma mark - Operators

TWNumber TWNumber::operator +(const TWNumber& v) const{
	if(isFloat() || v.isFloat()){
		return TWNumber(floatValue()+v.floatValue());
	}else{
		return TWNumber(integerValue()+v.integerValue());
	}
}
TWNumber TWNumber::operator -(const TWNumber& v) const{
	if(isFloat() || v.isFloat()){
		return TWNumber(floatValue()-v.floatValue());
	}else{
		return TWNumber(integerValue()-v.integerValue());
	}
}
TWNumber TWNumber::operator *(const TWNumber& v) const{
	if(isFloat() || v.isFloat()){
		return TWNumber(floatValue()*v.floatValue());
	}else{
		return TWNumber(integerValue()*v.integerValue());
	}
}
TWNumber TWNumber::operator /(const TWNumber& v) const{
	if(v.isZero()){
		throw std::wstring(L"division by zero");
	}
	return TWNumber(floatValue()/v.floatValue());
}
TWNumber TWNumber::operator %(const TWNumber& v) const{
	TWNumberInteger a=integerValue();
	TWNumberInteger b=v.integerValue();
	if(b==0)
		throw std::wstring(L"modular division by zero");
	return a%b;
}
TWNumber TWNumber::operator &&(const TWNumber& v) const{
	return (!isZero()) && (!(v.isZero()));
}
TWNumber TWNumber::operator ||(const TWNumber& v) const{
	return (!isZero()) || (!(v.isZero()));
}
TWNumber TWNumber::operator -() const{
	if(isFloat())
		return TWNumber(-floatValue());
	else
		return TWNumber(-integerValue());
}
TWNumber TWNumber::operator !() const{
	return isZero();
}

#pragma mark - Comparsions

TWNumber TWNumber::operator ==(const TWNumber& v) const{
	if(isFloat() || v.isFloat()){
		return floatValue()==v.floatValue();
	}else{
		return integerValue()==v.integerValue();
	}
}

TWNumber TWNumber::operator !=(const TWNumber& v) const{
	if(isFloat() || v.isFloat()){
		return floatValue()!=v.floatValue();
	}else{
		return integerValue()!=v.integerValue();
	}
}

TWNumber TWNumber::operator <(const TWNumber& v) const{
	if(isFloat() || v.isFloat()){
		return floatValue()<v.floatValue();
	}else{
		return integerValue()<v.integerValue();
	}
}

TWNumber TWNumber::operator >(const TWNumber& v) const{
	if(isFloat() || v.isFloat()){
		return floatValue()>v.floatValue();
	}else{
		return integerValue()>v.integerValue();
	}
}

TWNumber TWNumber::operator <=(const TWNumber& v) const{
	if(isFloat() || v.isFloat()){
		return floatValue()<=v.floatValue();
	}else{
		return integerValue()<=v.integerValue();
	}
}

TWNumber TWNumber::operator >=(const TWNumber& v) const{
	if(isFloat() || v.isFloat()){
		return floatValue()>=v.floatValue();
	}else{
		return integerValue()>=v.integerValue();
	}
}

#pragma mark - Functions

TWNumber TWNumber::makeExponent(const TWNumber &a, const TWNumber &b){
	TWNumberFloat v=a.floatValue()*::pow(10., b.floatValue());
	return v;
}

TWNumber TWNumber::pow(const TWNumber &a, const TWNumber &b){
	TWNumberFloat v=::pow(a.floatValue(), b.floatValue());
	return v;
}

TWNumber TWNumber::exp(const TWNumber &v){
	TWNumberFloat r=::exp(v.floatValue());
	return r;
}

TWNumber TWNumber::ln(const TWNumber &v){
	TWNumberFloat r=::log(v.floatValue());
	return r;
}

TWNumber TWNumber::sin(const TWNumber &v){
	TWNumberFloat r=::sin(v.floatValue());
	return r;
}

TWNumber TWNumber::cos(const TWNumber &v){
	TWNumberFloat r=::cos(v.floatValue());
	return r;
}

TWNumber TWNumber::tan(const TWNumber &v){
	TWNumberFloat r=::tan(v.floatValue());
	return r;
}

TWNumber TWNumber::asin(const TWNumber &v){
	TWNumberFloat r=::asin(v.floatValue());
	return r;
}

TWNumber TWNumber::acos(const TWNumber &v){
	TWNumberFloat r=::acos(v.floatValue());
	return r;
}

TWNumber TWNumber::atan(const TWNumber &v){
	TWNumberFloat r=::atan(v.floatValue());
	return r;
}

TWNumber TWNumber::ceil(const TWNumber &v){
	TWNumberFloat r=::ceil(v.floatValue());
	return r;
}

TWNumber TWNumber::floor(const TWNumber &v){
	TWNumberFloat r=::floor(v.floatValue());
	return r;
}

TWNumber TWNumber::trunc(const TWNumber &v){
	TWNumberInteger i=v.integerValue();
	return i;
}

TWNumber TWNumber::abs(const TWNumber &v){
	TWNumberFloat r=::fabs(v.floatValue());
	return r;
}

TWNumber TWNumber::round(const TWNumber &a,
						 const TWNumber &b){
	TWNumberFloat r=a.floatValue();
	TWNumberInteger i=b.integerValue();
	TWNumberFloat scale=::pow(10., i);
	r*=scale;
	r=::floor(r+.5);
	r/=scale;
	return r;
}

#pragma mark - To String

std::wstring TWNumber::toString() const{
	if(isFloat()){
		int digits=0;
		double v=floatValue();
		switch(fpclassify(v)){
			case FP_INFINITE:
				return L"INF";
			case FP_NAN:
				return L"NAN";
		}
		v-=::floor(v+.5);
		while(fabs(v)>.00001 && digits<12){
			v*=10.;
			v-=::floor(v+.5);
			digits++;
		}
		if(fabs(floatValue())<1e-12){
			char output[256];
			
			int digits2=0;
			double v2=floatValue();
			while(fabs(v2)<=1.){
				v2*=10.;
				digits2++;
				if(v2==0.){
					return L"0";
				}
				if(digits2>1000){
					return L"0";
				}
			}
			
			v=v2;
			digits=0;
			
			v-=::floor(v+.5);
			while(fabs(v)>.00001 && digits<12){
				v*=10.;
				v-=::floor(v+.5);
				digits++;
			}
			
			char formatString[64];
			sprintf(formatString, "%c.%dlfE-%d", '%', digits, digits2);
			
			sprintf(output, formatString, v2);
			
			return TWUtils::m2w(output);
		}else if(digits==0 && fabs(floatValue())<1e+12){
			char output[256];
			sprintf(output, "%lld", (long long)::floor(floatValue()+.5));
			
			return TWUtils::m2w(output);
		}else if(digits==0){
			char output[256];
			
			int digits2=0;
			double v2=floatValue();
			while(fabs(v2)>=10.){
				v2/=10.;
				digits2++;
			}
			
			v=v2;
			digits=0;
			
			v-=::floor(v+.5);
			while(fabs(v)>.00001 && digits<12){
				v*=10.;
				v-=::floor(v+.5);
				digits++;
			}
			
			char formatString[64];
			sprintf(formatString, "%c.%dlfE+%d", '%', digits, digits2);
			
			sprintf(output, formatString, v2);
			
			return TWUtils::m2w(output);
		}
		
		char formatString[64];
		sprintf(formatString, "%c.%dlf", '%', digits);
		
		char output[256];
		sprintf(output, formatString, floatValue());
		
		return TWUtils::m2w(output);
	}else{
		char output[256];
		sprintf(output, "%lld", integerValue());
		
		return TWUtils::m2w(output);
	}
}


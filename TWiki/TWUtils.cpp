/*
 *  TWUtils.cpp
 *  XTBook
 *
 *  Created by Nexhawks on 2/23/11.
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

#include <algorithm>
#include <stdlib.h>
#include "TWUtils.h"
#include <map>
#include "Entities.h"
#include <assert.h>
#include "TWExpressionEvaluator.h"
#include "TWExpressionReader.h"
#include "TWStringSplitter.h"

const std::wstring TWEmptyString=L"";


std::wstring TWUtils::escapeHTMLSpecialChars(const std::wstring& oldString){
	const wchar_t *escapeFrom=L"&<>\"";
	const wchar_t *escapeTo[]={
		L"&amp;", L"&lt;", L"&gt;", L"&quot;"
	};
	std::wstring newString;
	std::wstring::size_type i=0;
	newString.reserve(oldString.size());
	while(i<oldString.size()){
		std::wstring::size_type j=oldString.find_first_of(escapeFrom, i);
		if(j==std::wstring::npos){
			newString+=oldString.substr(i);
			i=oldString.size();
		}else{
			newString+=oldString.substr(i, j-i);
			
			wchar_t chr=oldString[j];
			newString+=escapeTo[wcschr(escapeFrom, chr)-escapeFrom];
			
			i=j+1;
		}
	}
	assert(newString.find(L'<')==std::wstring::npos);
	assert(newString.find(L'>')==std::wstring::npos);
	
	return newString;
}

std::wstring TWUtils::escapeSpecialChars(const std::wstring& oldString){
	const std::wstring escapedChars=L" &<>\"[]{}|!?&:";
	std::wstring::size_type pos=0, nextPos;
	std::wstring newString;
	newString.reserve(oldString.size());
	while(pos<oldString.size()){
		nextPos=oldString.find_first_of(escapedChars, pos);
		if(nextPos==std::wstring::npos){
			newString.append(oldString, pos, oldString.size()-pos);
			break;
		}
		
		newString.append(oldString, pos, nextPos-pos);
		
		unsigned int code=(unsigned int)oldString[nextPos];
		newString+=L"&#";
		newString+=TWUtils::stringWithIntValue(code&0xff);
		newString+=L';';
		pos=nextPos+1;
	}
	return newString;
}

std::wstring TWUtils::unescapeHTMLSpecialChars(const std::wstring& oldString){
	std::wstring newString;
	std::wstring::size_type pos=0;
	newString.reserve(oldString.size());
	while(pos<oldString.size()){
		std::wstring::size_type newPos;
		newPos=oldString.find(L'&', pos);
		if(newPos==std::wstring::npos){
			newString+=oldString.substr(pos);
			pos=oldString.size();
		}else{
			newString+=oldString.substr(pos, newPos-pos);
			pos=newPos+1;
			
			newPos=oldString.find(L';', pos);
			if(newPos==std::wstring::npos){
				// not closed; invalid syntax.
				newString+=L'&';
				newString+=oldString.substr(pos);
				pos=oldString.size();
			}else{
				std::wstring entityName;
				entityName=oldString.substr(pos, newPos-pos);
				pos=newPos+1;
				
				if(entityName[0]==L'#'){
					int entityValue=intValueFromString(entityName.substr(1));
					newString+=(wchar_t)entityValue;
				}else{
					wchar_t chr=charForEntity(entityName.c_str());
					
					if(!chr){
						// not found.
						newString+=L'&';
						newString+=entityName;
						newString+=L';';
					}else{
						// found
						newString+=chr;
					}
				}
			}
		}
	}
	return newString;
}

int TWUtils::intValueFromString(const std::wstring& str){
	char buf[256];
	wcstombs(buf, str.c_str(), 256);
	return atoi(buf);
}
float TWUtils::floatValueFromString(const std::wstring& str){
	char buf[256];
	wcstombs(buf, str.c_str(), 256);
	return atof(buf);
}
const std::wstring g_zeroString=L"0";
const std::wstring g_minIntString=L"-2147483648";
const std::wstring g_positives[]=
{L"0", L"1", L"2", L"3", L"4", L"5", L"6", L"7", L"8", L"9"};
const std::wstring g_negatives[]=
{L"-1", L"-2", L"-3", L"-4", L"-5", L"-6", L"-7", L"-8", L"-9"};

std::wstring TWUtils::stringWithIntValue(int iv){
	/* optimizing:
	wchar_t buf[256];
#ifdef EV_UNSAFE_SWPRINTF
	swprintf(buf, L"%d", iv);
#else
	swprintf(buf, 256, L"%d", iv);
#endif
	return buf;*/
	if(iv>=0 && iv<10){
		return g_positives[iv];
	}else if(iv>=-9 && iv<=-1){
		return g_positives[-1-iv];
	}else if(iv==-2147483648)
		// we have to filter this case because
		// if this value was negated, result is unknown.
		return g_minIntString;
	else if(iv>0){
		// positive.
		std::wstring out;
		size_t digits;
		if(iv<10)
			digits=1;
		else if(iv<100)
			digits=2;
		else if(iv<1000)
			digits=3;
		else if(iv<10000)
			digits=4;
		else if(iv<100000)
			digits=5;
		else if(iv<1000000)
			digits=6;
		else if(iv<10000000)
			digits=7;
		else if(iv<100000000)
			digits=8;
		else if(iv<1000000000)
			digits=9;
		else
			digits=10;
		
		out.resize(digits);
		// tricky.
		for(size_t i=digits-1;i<digits;i--){
			int reminder, dv;
			if(i==0){
				dv=0;
				reminder=iv;
			}else{
				dv=iv/10;
				reminder=iv%10;
			}
			
			assert(reminder>=0);
			assert(reminder<=9);
			
			out[i]=L'0'+reminder;
			
			iv=dv;
		}
		return out;
	}else{
		// negative.
		std::wstring out;
		size_t digits;
		iv=-iv;
		if(iv<10)
			digits=1;
		else if(iv<100)
			digits=2;
		else if(iv<1000)
			digits=3;
		else if(iv<10000)
			digits=4;
		else if(iv<100000)
			digits=5;
		else if(iv<1000000)
			digits=6;
		else if(iv<10000000)
			digits=7;
		else if(iv<100000000)
			digits=8;
		else if(iv<1000000000)
			digits=9;
		else
			digits=10;
			
		out.resize(digits+1);
		out[0]=L'-';
		for(size_t i=digits;i>=1;i--){
			int reminder, dv;
			if(i==1){
				dv=0;
				reminder=iv;
			}else{
				dv=iv/10;
				reminder=iv%10;
			}
			
			assert(reminder>=0);
			assert(reminder<=9);
			
			out[i]=L'0'+reminder;
			
			iv=dv;
		}
		return out;
	}
}
std::wstring TWUtils::trimString(const std::wstring& str){
	std::wstring::size_type index1, index2;
	index1=str.find_first_not_of(L" \n\t\r");
	if(index1==std::wstring::npos)
		return std::wstring();
	index2=str.find_last_not_of(L" \n\t\r");
	return str.substr(index1, index2-index1+1);
}

static inline wchar_t easytolower(wchar_t in){
	if(in<=L'Z' && in>=L'A')
		return in-(L'Z'-L'z');
	return in;
} 
static inline wchar_t easytoupper(wchar_t in){
	if(in<=L'z' && in>=L'a')
		return in-(L'z'-L'Z');
	return in;
} 


std::wstring TWUtils::toLower(const std::wstring& str){
	std::wstring result=str;
	std::transform(result.begin(), result.end(), result.begin(), easytolower);
	return result;
}

void TWUtils::toLowerInplace(std::wstring& str){
	std::transform(str.begin(), str.end(), str.begin(), easytolower);
}

std::vector<std::wstring> TWUtils::split(const std::wstring& str, const std::wstring& delimiter){
	std::vector<std::wstring> resultStrings;
	std::wstring::size_type i=0;
	std::wstring::size_type nextPos;
	while(nextPos=str.find(delimiter, i), nextPos!=std::wstring::npos){
		resultStrings.push_back(str.substr(i, nextPos-i));
		i=nextPos+delimiter.size();
	}
	resultStrings.push_back(str.substr(i));
	return resultStrings;
}

std::vector<wchar_t *> TWUtils::splitInplace(wchar_t * str, const wchar_t * delimiter){
	std::vector<wchar_t *> resultStrings;
	wchar_t *i=str;
	wchar_t *nextPos;
	size_t delimiterLength=wcslen(delimiter);
	while(nextPos=wcsstr(i, delimiter), nextPos){
		*nextPos=0;
		resultStrings.push_back(i);
		i=nextPos+delimiterLength;
	}
	resultStrings.push_back(i);
	return resultStrings;
}

inline wchar_t TWUtils::complementaryBracketCharFor(wchar_t c){
	switch(c){
		case L'{':
			return L'}';
		case L'[':
			return L']';
		case L'(':
			return L')';
		case L'<':
			return L'>';
		case L'}':
			return L'{';
		case L']':
			return L'[';
		case L')':
			return L'(';
		case L'>':
			return L'<';
		default:
			return 0;
	}
}

std::wstring::size_type TWUtils::findMatchingBracket(const std::wstring& str, std::wstring::size_type pos){
	if(pos>=str.size())
		return std::wstring::npos;
	wchar_t openChar=str[pos];
	wchar_t closeChar=complementaryBracketCharFor(openChar);
	assert(closeChar!=0);
	unsigned int level=0;
	while(pos<str.size()){
		const wchar_t c=str[pos];
		if(c==openChar){
			level++;
		}else if(c==closeChar){
			level--;
			if(level==0)
				return pos;
		}
		pos++;
	}
	return std::wstring::npos;
}

static inline wchar_t toDbKey(wchar_t in){
	if(in==L' ')
		return L'_';
	return in;
} 

static inline wchar_t fromDbKey(wchar_t in){
	if(in==L'_')
		return L' ';
	return in;
} 

std::wstring TWUtils::dbKeyFor(const std::wstring&str){
	std::wstring out=trimString(str);
	std::transform(out.begin(), out.end(), out.begin(), toDbKey);
	
	std::wstring::size_type pos;
	pos=str.find(L':');
	if(pos==std::wstring::npos)
		pos=0;
	else
		pos++;
	if(pos<out.size())
		out[pos]=easytoupper(out[pos]);
	return out;
}

std::wstring TWUtils::displayTitleFor(const std::wstring&str){
	std::wstring out=trimString(str);
	std::transform(out.begin(), out.end(), out.begin(), fromDbKey);
	
	std::wstring::size_type pos;
	pos=str.find(L':');
	if(pos==std::wstring::npos)
		pos=0;
	else
		pos++;
	if(pos<out.size())
		out[pos]=easytoupper(out[pos]);
	
	return out;
}

std::wstring TWUtils::resolveRelativeUrl(const std::wstring& url,
										  const std::wstring& baseUrl){
	if(url[0]==L'/'){
		return resolveRelativeUrl(L'.'+url, baseUrl);
	}
	
	std::wstring u=url;
	TWStringSplitter<32> splitter(u, L"/");
	
	std::wstring outUrl;
	for(int i=0;i<splitter.elementCount();i++){
		const wchar_t *component=splitter[i];
		if(!wcscmp(component, L".")){
			if(i==0)
				outUrl=baseUrl;
		}else if(!wcscmp(component, L"..")){
			if(i==0)
				outUrl=baseUrl;
			outUrl=parentUrlFor(outUrl);
		}else if(*component!=L'\0'){ // not empty
			if(!outUrl.empty())
				outUrl+=L'/';
			outUrl+=component;
		}
	}
	
	return outUrl;
	
}

std::wstring TWUtils::parentUrlFor(const std::wstring& url){
	if(url.empty())
		return url;
	
	std::wstring ns, bn;
	ns=namespaceForUrl(url);
	bn=baseNameForUrl(url);
	
	std::wstring::size_type pos;
	
	pos=bn.rfind(L'/');
	if(pos==std::wstring::npos){
		// remove namespace prefix
		return std::wstring();
	}
	
	ns+=L':';
	ns.append(bn, 0, pos);
	
	return ns;
}

std::wstring TWUtils::namespaceForUrl(const std::wstring&str){
	std::wstring::size_type pos;
	pos=str.find(L':');
	if(pos==std::wstring::npos)
		return std::wstring();
	else
		return str.substr(0, pos);
}

std::wstring TWUtils::baseNameForUrl(const std::wstring&str){
	std::wstring::size_type pos;
	pos=str.find(L':');
	if(pos==std::wstring::npos)
		return str;
	else
		return str.substr(pos+1);
}

bool TWUtils::matchStart(const std::wstring& str, const std::wstring& match){
	if(str.size()<match.size())
		return false;
	for(int n=0;n<match.size();n++)
		if(str[n]!=match[n])
			return false;
	return true;
}


bool TWUtils::matchStartAndRemove(std::wstring& str, const std::wstring& match){
	if(matchStart(str, match)){
		str=str.substr(match.size());
		return true;
	}else{
		return false;
	}
}

std::wstring::size_type TWUtils::findColonNoLinks(const std::wstring& str, std::wstring::size_type pos,
										 std::wstring::size_type n){
	if(n==std::string::npos){
		n=str.size()-pos;
	}
	if(pos+n>str.size()){
		n=str.size()-pos;
	}
	if(pos>=str.size()){
		return std::wstring::npos;
	}
	
	n+=pos;
	
	while(pos<n){
		std::wstring::size_type nextPos;
		nextPos=findInRangeFirstOf(str, L":<", pos, n-pos);
		if(nextPos==std::wstring::npos)
			return nextPos;
		pos=nextPos;
		if(str[pos]==L':'){
			return pos;
		}
		
		// '<' was found. find the '>'.
		assert(str[pos]==L'<');
		
		nextPos=findInRange(str, L">",pos, n-pos);
		if(nextPos==std::wstring::npos){
			// this shouldn't happen --- no '>' found.
			return nextPos;
		}
		
		pos=nextPos+1;
	}
	return std::wstring::npos;
}

std::wstring::size_type TWUtils::commonCharCount(const std::wstring& s1, const std::wstring& s2){
	std::wstring::size_type pos=0;
	while(pos<s1.size() && pos<s2.size()){
		if(s1[pos]!=s2[pos])
			break;
		pos++;
	}
	return pos;
}

std::wstring::size_type TWUtils::findInRange(const std::wstring& text, const wchar_t *key,
									std::wstring::size_type pos,
									std::wstring::size_type n){
	n+=pos;
	if(n>=text.size())
		return text.find(key, pos);
	wchar_t *ptr=const_cast<wchar_t *>(text.c_str());
	wchar_t oldPos=0;
	std::swap(oldPos, ptr[n]);
	wchar_t *result=wcsstr(ptr+pos, key);
	std::swap(oldPos, ptr[n]);
	if(!result){
		return std::wstring::npos;
	}
	return result-ptr;
}


std::wstring::size_type TWUtils::findInRangeFirstOf(const std::wstring& text, const wchar_t *key,
											 std::wstring::size_type pos,
											 std::wstring::size_type n){
	n+=pos;
	if(n>=text.size())
		return text.find_first_of(key, pos);
	wchar_t *ptr=const_cast<wchar_t *>(text.c_str());
	wchar_t oldPos=0;
	std::swap(oldPos, ptr[n]);
	size_t result=wcscspn(ptr+pos, key);
	std::swap(oldPos, ptr[n]);
	if(!ptr[pos+result]){
		return std::wstring::npos;
	}else if(pos+result==n){
		return std::wstring::npos;
	}
	assert(wcschr(key, ptr[pos+result]));
	return result+pos;
}



std::wstring::size_type TWUtils::findInRangeFirstNotOf(const std::wstring& text, const wchar_t *key,
													std::wstring::size_type pos,
													std::wstring::size_type n){
	n+=pos;
	if(n>=text.size())
		return text.find_first_not_of(key, pos);
	if(pos>=text.size())
		return std::wstring::npos;
	
	wchar_t *ptr=const_cast<wchar_t *>(text.c_str());
	wchar_t oldPos=0;
	std::swap(oldPos, ptr[n]);
	size_t result=wcsspn(ptr+pos, key);
	std::swap(oldPos, ptr[n]);
	if(!ptr[pos+result]){
		return std::wstring::npos;
	}else if(pos+result==n){
		return std::wstring::npos;
	}
	assert(!wcschr(key, ptr[pos+result]));
	if(result>0){
		assert(wcschr(key, ptr[pos]));
	}
	return result+pos;
}

std::wstring TWUtils::m2w(const std::string& str){
	if(str.size()>128){
		std::wstring out;
		for(size_t i=0;i<str.size();i+=128){
			out+=m2w(str.substr(i, 128));
		}
		return out;
	}
	wchar_t buf[256];
	buf[255]=0;
	mbstowcs(buf, str.c_str(), 255);
	return buf;
}

std::string TWUtils::w2m(const std::wstring& str){
	if(str.size()>64){
		std::string out;
		for(size_t i=0;i<str.size();i+=64){
			out+=w2m(str.substr(i, 64));
		}
		return out;
	}
	char buf[256];
	buf[255]=0;
	wcstombs(buf, str.c_str(), 255);
	return buf;
}

std::wstring TWUtils::evaluateExpression(const std::wstring &expr){
	TWNumber num;
	try{
	TWExpressionReader reader(expr);
	TWExpressionEvaluator evaluator(reader);
	
	
		num=evaluator.evaluate();
	}catch(...){
#if 0
		XTBDumpWString(XTBFormatStd(L"CANNOT evaluate \"%ls\"",
									expr.c_str()));
#endif
		throw;
	}
#if 0
	XTBDumpWString(XTBFormatStd(L"evaluate \"%ls\"=\"%ls\"",
								expr.c_str(),
								num.toString().c_str()));
#endif
	return num.toString();	
}

bool TWUtils::isNumeric(const std::wstring& str){
	if(str.find_first_not_of(L"0123456789.e-+ ")==std::wstring::npos)
		return true;
	else
		return false;
}

bool TWUtils::isEqual(const std::wstring& a,
					const std::wstring& b){
	if(isNumeric(a) && isNumeric(b)){
		TWNumber aNum(a);
		TWNumber bNum(b);
		return !((aNum==bNum).isZero());
	}else{
		return trimString(a)==trimString(b);
	}
}

std::wstring TWUtils::replace(const std::wstring& str, const std::wstring& from, const std::wstring& to){
	std::wstring out;
	std::wstring::size_type pos=0, nextPos;
	while(pos<str.size()){
		nextPos=str.find(from, pos);
		if(nextPos==std::wstring::npos){
			out.append(str, pos, str.size()-pos);
			break;
		}
		out.append(str, pos, nextPos-pos);
		out+=to;
		pos=nextPos+from.size();
	}
	return out;
}

std::wstring TWUtils::sanitizeAttribute(const std::wstring& attrs){
	std::wstring s=replace(attrs, L"\n", std::wstring());
	return s;
}


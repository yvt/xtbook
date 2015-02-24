//
//  TWEval.cpp
//  XTBook
//
//  Created by 河田 智明 on 7/26/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

#include <string>

#include "../TWiki/TWUtils.h"
#include "../TWiki/TWExpressionReader.h"
#include "../TWiki/TWExpressionEvaluator.h"


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
	TWExpressionReader reader(expr);
	TWExpressionEvaluator evaluator(reader);
	TWNumber num=evaluator.evaluate();
	return num.toString();	
}

int main(){
	char buf[32767];
	while(gets(buf)){
		std::wstring s;
		s=TWUtils::m2w(buf);
		
		std::string  out;
		out=TWUtils::w2m(s);
		printf("%s=", out.c_str());
		
		try{
			s=TWUtils::evaluateExpression(s);
		}catch(const std::wstring& e){
			s=L"ERROR: "+e;
		}
		
		out=TWUtils::w2m(s);
		puts(out.c_str());
	}
}



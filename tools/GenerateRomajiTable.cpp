/*
 *  GenerateRomajiTable.cpp
 *  XTBook
 *
 *  Created by Nexhawks on 2/16/11.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

using namespace std;

wstring wstringFromString(const string& str){
	wchar_t buf[1024];
	mbstowcs(buf, str.c_str(), 1024);
	return buf;
}

void writeWCharArray(const wstring& str){
	printf("{");
	for(int i=0;i<str.size();i++){
		unsigned int chr;
		chr=(unsigned int)(str[i]);
		printf("0x%x,", chr);
	}
	printf("0}");
}

int main(int argc, char **argv){
	char str[1024];
	setlocale(LC_ALL, "ja_JP.UTF-8");
	while(fgets(str, 1024, stdin)){
		string s=str;
		string inputString;
		string convertString;
		string newInputString;
		while(*(s.rbegin())=='\n' || *(s.rbegin())=='\r')
			s=s.substr(0, s.size()-1);
		inputString=s.substr(0, s.find('\t'));
		s=s.substr(s.find('\t')+1);
		if(s.find('\t')==string::npos){
			convertString=s;
			newInputString="";
		}else{
			convertString=s.substr(0, s.find('\t'));
			newInputString=s.substr(s.find('\t')+1);
		}
		convertString+=newInputString;
		printf("{L\"%s\", ", inputString.c_str());
		writeWCharArray(wstringFromString(convertString));
		printf(", false},\n");
	}
	return 0;
}

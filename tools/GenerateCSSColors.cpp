/*
 *  GenerateCSSColors.cpp
 *  XTBook
 *
 *  Created by Nexhawks on 2/27/11.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */


#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char **argv){
	char buf[256];
	string str;
	while(gets(buf)){
		if(buf[0]==0)
			continue;
		if(buf[1]=='!')
			continue;
		str=buf;
		if(str.find('\t')==string::npos)
			continue;
		
		unsigned int col;
		col=atoi(str.substr(0, 4).c_str())<<16;
		col|=atoi(str.substr(4, 4).c_str())<<8;
		col|=atoi(str.substr(8, 4).c_str());
		
		str=str.substr(12);
		str=str.substr(str.find('\t')+1);
		while(str[0]=='\t')
			str=str.substr(1);
		printf("{L\"%s\", 0x%08x},\n", str.c_str(), col);
	}
	
	return 0;
}

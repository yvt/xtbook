/*
 *  GenerateRGBTable.cpp
 *  XTBook
 *
 *  Created by Nexhawks on 2/18/11.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

using namespace std;


static char easytolower(char in){
	if(in<='Z' && in>='A')
		return in-('Z'-'z');
	return in;
} 

static std::string toLower(const std::string& str){
	std::string result=str;
	std::transform(result.begin(), result.end(), result.begin(), easytolower);
	return result;
}

int main(int argc, char **argv){
	char buf[256];
	string str;
	
	if(argc==1){
	
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
			printf("{\"%s\", 0x%08x},\n", str.c_str(), col);
		}
	
	}else if(argc==2){
		if(!strcmp(argv[1], "-h")){
			puts("USAGE: GenerateRGBTable [MODE]");
			puts("MDOES: obs colors");
		}else if(!strcmp(argv[1], "obs")){
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
				str=toLower(str);
				printf("%s\n", str.c_str());
			}
		}else if(!strcmp(argv[1], "colors")){
			int i=0;
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
				printf("0x%08x,", col);
				i++;
				if(i%4==0)
					printf("\n");
			}
		}
	}
		
	return 0;
}

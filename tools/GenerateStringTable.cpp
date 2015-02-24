//
//  GenerateStringTable.cpp
//  XTBook
//
//  Created by Nexhawks on 4/16/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//


#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

using namespace std;

static void writeLE32(uint32_t value){
	uint8_t buf[4];
	buf[0]=(value&0x000000ff)>>0;
	buf[1]=(value&0x0000ff00)>>8;
	buf[2]=(value&0x00ff0000)>>16;
	buf[3]=(value&0xff000000)>>24;
	fwrite(buf, 1, 4, stdout);
}

static void parseLine(char *line){
	
	{
		char *ptr=strchr(line, '#');
		if(ptr)
			*ptr=0;
	}
	
	while(*line==' ')
		line++;
	
	if(*line==13 || *line==10 || *line==0)
		return;
	
	uint32_t length;
	length=strlen(line);
	
	writeLE32(length);
	
	fwrite(line, 1, length, stdout);
}

int main(int argc, char **argv){
	char buf[4096];
	
	if(argc==3){
		freopen(argv[1], "r", stdin);
		freopen(argv[2], "wb", stdout);
		
	}
	
	while(gets(buf)){
		parseLine(buf);
	}
	
	writeLE32(0);
	
	return 0;
}

//
//  GenerateEntityTable.cpp
//  XTBook
//
//  Created by Nexhawks on 4/17/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <string.h>
#include <stdlib.h>

using namespace std;


/* remove any spaces from @param str. */
string removeSpace(const string& str){
	string s;
	s.reserve(str.size());
	
	for(int n=0;n<str.size();n++){
		if(str[n]==' ')
			continue;
		if(str[n]=='\t')
			continue;
		if(str[n]=='"')
			continue;
		if(str[n]=='\n'){
			s+=' ';
			continue;
		}
		if(str[n]=='\r'){
			s+=' ';
			continue;
		}
		s+=str[n];
	}
	
	return s;
}

string replaceString(const string& str, const string& from, const string& to){
	string::size_type pos=0;
	string newStr;
	while(pos<str.size()){
		string::size_type nextPos;
		nextPos=str.find(from, pos);
		if(nextPos==string::npos)
			break;
		newStr+=str.substr(pos, nextPos-pos);
		newStr+=to;
		pos=nextPos+from.size();
	}
	newStr+=str.substr(pos);
	return newStr;
}


std::vector<string> splitString(const std::string& str, const std::string& delimiter){
	std::vector<std::string> resultStrings;
	std::string::size_type i=0;
	std::string::size_type nextPos;
	while(nextPos=str.find(delimiter, i), nextPos!=std::string::npos){
		resultStrings.push_back(str.substr(i, nextPos-i));
		i=nextPos+delimiter.size();
	}
	resultStrings.push_back(str.substr(i));
	return resultStrings;
}

int main(int argc, char **argv){
	char buf[4096];
	if(argc==1){
		while(gets(buf)){
			std::string str=buf;
			std::vector<string> strs=splitString(str, ",");
			printf("registerChar(L\"%s\", (wchar_t)%s);\n", strs[0].c_str(), strs[2].c_str());
		}
	}else if(argc==2){
		if(!strcmp(argv[1], "obs")){
			while(gets(buf)){
				std::string str=buf;
				std::vector<string> strs=splitString(str, ",");
				printf("%s\n", strs[0].c_str());
			}
		}else if(!strcmp(argv[1], "chars")){
			int i=0;
			while(gets(buf)){
				std::string str=buf;
				std::vector<string> strs=splitString(str, ",");
				printf("(wchar_t)0x%04x,", atoi(strs[2].c_str()));
				i++;
				if(i%4==0)
					printf("\n");
			}
		}
	}
	return 0;
}

/*
 *  TWPreprocessorRunner.cpp
 *  XTBook
 *
 *  Created by Nexhawks on 2/25/11.
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

#include <stdio.h>
#include "../TWPreprocessor.h"
#include <stdlib.h>
#include <string>
#include "../TWParser.h"
#include <exception>
#include "../TWPreprocessorNode.h"

static volatile inline unsigned long long rdtsc() {
    unsigned long long ret;
    __asm__ volatile ("rdtsc" : "=A" (ret));
    return ret;
}

int main(int argc, char **argv){
	std::wstring buffer;
	char inputBuffer[4096];
	wchar_t wcharBuffer[4096];

	inputBuffer[4095]=0;
	while(fgets(inputBuffer, 4095, stdin)){
		mbstowcs(wcharBuffer, inputBuffer, 4096);
		buffer+=wcharBuffer;
	}
	
	TWParser parser(NULL);
	TWPreprocessor *preprocessor=parser.preprocessor();
	
	//sleep(10);
	
	try{
	
		unsigned long long ot=rdtsc();
		
		//while(1){
		TWPreprocessorNode *node=preprocessor->nodeFromText(buffer, 0);
		
		buffer=node->toString();
		delete node;
		//}
			
		unsigned long long tm=rdtsc()-ot;
		
		wprintf(L"%ls\n", buffer.c_str());
		
		
		
		fflush(stdout);
		fprintf(stderr, "time: %llu clock(s)\n", tm);
		
		buffer=L"";
		
	}catch(const std::exception& ex){
		fprintf(stderr, "exception: %s\n", ex.what());
		return 1;
	}catch(const char  *str){
		fprintf(stderr, "exception: %s\n", str);
		return 1;
	}catch(const wchar_t *str){
		fwprintf(stderr, L"exception: %ls\n", str);
		return 1;
	}
	
	//sleep(100);
	
	
	return 0;
	
}

//
//  TWRenderingProcessor.cpp
//  XTBook
//
//  Created by Nexhawks on 3/12/11.
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

#include <stdio.h>
#include "../TWPreprocessor.h"
#include <stdlib.h>
#include <string>
#include "../TWParser.h"
#include <exception>
#include "../TWPreprocessorNode.h"
#include "../TWRenderObject.h"
#include "../TWHTMLBuilder.h"
#include "../TWHTMLElement.h"

static volatile inline unsigned long long rdtsc() {
    unsigned long long ret;
    __asm__ volatile ("rdtsc" : "=A" (ret));
    return ret;
}

int main(int argc, char **argv){

	try{
        
		unsigned long long ot=rdtsc();
		
		TWHTMLElement *rootElement;
        rootElement=TWHTMLElement::nodeWithName(L"body");
        
        TWHTMLBuilder builder(rootElement);
        builder.beginElement(L"div");
        
        builder.endElement();
        
        
        rootElement->setAttribute(L"style", L"width: 200px");
        
        rootElement->calcStyle();
        
        TWRenderObject *obj=TWRenderObject::objectWithNode(rootElement);
		
        obj->layout(NULL);
        
        delete obj;
        delete rootElement;
        
		unsigned long long tm=rdtsc()-ot;
		
		
		
		fflush(stdout);
		fprintf(stderr, "time: %llu clock(s)\n", tm);
		
		
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
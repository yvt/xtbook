//
//  TWDateParserFunction.cpp
//  XTBook
//
//  Created by Kawada Tomoaki on 5/14/11.
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
#include <time.h>
#include "TWDateParserVariable.h"
#include <string.h>

TWDateParserVariable::TWDateParserVariable(const char *fmt, bool local):
m_local(local){
	strcpy(m_format, fmt);
}

std::wstring TWDateParserVariable::execute(TWPreprocessorFrame *){
	char buf[64];
	wchar_t buf2[64];
	struct tm t2;
	time_t t1=time(NULL);
	if(m_local)
		t2=*(localtime(&t1));
	else
		t2=*(gmtime(&t1));
	strftime(buf, 64, m_format, &t2);
	mbstowcs(buf2, buf, 64);
	return buf2;
}

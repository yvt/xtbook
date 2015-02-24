/*
 *  twException.cpp
 *  
 *
 *  Created by Nexhawks on 10/01/13.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#include <tcw/twException.h>
#include <tcw/twStrConv.h>

using namespace std;

twException::twException(const wstring& ms) throw(){
	wcscpy(msg, ms.c_str());
	twW2M(amsg, msg);
}

const char *twException::what() const throw(){
	return amsg;
}

const wchar_t *twException::description() const throw(){
	return msg;
}




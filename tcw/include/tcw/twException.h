/*
 *  twException.h
 *  
 *
 *  Created by Nexhawks on 10/01/13.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#pragma once

#include <wchar.h>
#include <exception>
#include <string>

class twException : public std::exception{
protected:
	wchar_t msg[256]; char amsg[256];
public:
	twException(const std::wstring& ms) throw();
	virtual const char *what() const throw() ;
	virtual const wchar_t *description() const throw();
};


//
//  XTBException.h
//  XTBook
//
//  Created by Kawada Tomoaki on 5/28/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include <exception>
#include <string>
#include <tcw/twException.h>

#pragma once

class XTBException: public twException{
	std::wstring m_name;
	std::wstring m_reason;
	std::wstring m_description;
	std::string m_what;
public:
	XTBException(const char *functionName, const char *file, int lineNumber, 
				 const std::wstring& name,  ...) throw();
	virtual ~XTBException() throw();
	
	virtual const char* what() const throw(){return m_what.c_str();}

	const std::wstring& name() const throw(){return m_name;}
	const std::wstring& reason() const throw(){return m_reason;}
	virtual const wchar_t *description() const throw(){return m_description.c_str();}

};

#define XTBThrowException(name, ...) throw XTBException(__PRETTY_FUNCTION__, __FILE__, __LINE__, name, __VA_ARGS__ )

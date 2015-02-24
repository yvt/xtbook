//
//  XTBException.cpp
//  XTBook
//
//  Created by Kawada Tomoaki on 5/28/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "XTBException.h"
#include "global.h"
#include "i18n.h"
#include <tcw/twStrConv.h>

XTBException::XTBException(const char *functionName, const char *file, int lineNumber, 
						   const std::wstring& name, ...) throw():
twException(L""),
m_name(name){
	va_list list;
	va_start(list, name);
	
	m_reason=XTBVarArgFormat(XTBLocalizedString(name), list);
	
	va_end(list);
	
	m_description=XTBFormat(XTBLocalizedString(L"XTBExceptionDescription"),
							name.c_str(), twM2W(functionName).c_str(),
							twM2W(file).c_str(), XTBStringWithInt(lineNumber).c_str(),
							m_reason.c_str());
	
	m_what=twW2M(m_description);
}
XTBException::~XTBException() throw(){
	
}


//
//  XTBStringTableFileHandle.h
//  XTBook
//
//  Created by Nexhawks on 4/16/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#pragma once

#include "platform.h"

class XTBStringTableFileHandle{
protected:
	FILE *m_stdHandle;
	bool m_eof;
	
public:
	XTBStringTableFileHandle(const XTBSysString&);
	virtual ~XTBStringTableFileHandle();
	
	std::wstring readLine();
	bool isEOF() const{
		return m_eof;
	}
};


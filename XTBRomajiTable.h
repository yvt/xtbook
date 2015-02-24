/*
 *  XTBRomajiTable.h
 *  XTBook
 *
 *  Created by Nexhawks on 2/17/11.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#pragma once

#include <string>

class XTBRomajiTable{
public:
	XTBRomajiTable();
	virtual ~XTBRomajiTable();
	static XTBRomajiTable *defaultRomajiTable();
	
	virtual std::wstring convert(const std::wstring&, bool strong=false)=0;
};

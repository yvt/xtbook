//
//  XTBStringTable.h
//  XTBook
//
//  Created by Nexhawks on 4/16/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#pragma once

#include "platform.h"

#include <string>
#include <map>
#include <vector>

struct XTBStringTableInfo{
	XTBSysString path;
	std::wstring identifier;
	std::wstring description;
};

class XTBStringTable {
private:
	std::map<std::wstring, std::wstring> m_table;
	
	
	void parseLine(const std::wstring&);
	
public:
	
	static XTBStringTableInfo infoForStringTableAtPath(const XTBSysString& path);
	
	XTBStringTable(const XTBSysString& path);
	virtual ~XTBStringTable();
	
	const std::wstring& stringForKey(const std::wstring&) const;
};



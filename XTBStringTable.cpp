//
//  XTBStringTable.cpp
//  XTBook
//
//  Created by Nexhawks on 4/16/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "XTBStringTable.h"
#include "XTBStringTableFileHandle.h"
#include "platform.h"
#include <tcw/twStrConv.h>

XTBStringTableInfo XTBStringTable::infoForStringTableAtPath(const XTBSysString& path){
	XTBStringTableInfo info;
	XTBStringTableFileHandle handle(path);
	info.identifier=handle.readLine();
	info.description=handle.readLine();
	info.path=path;
	return info;
}



XTBStringTable::XTBStringTable(const XTBSysString& path){
	XTBStringTableFileHandle handle(path);
	std::wstring str;
	while(str=handle.readLine(), !handle.isEOF()){
		parseLine(str);
	}
}

void XTBStringTable::parseLine(const std::wstring& line){
	std::wstring::size_type pos;
	pos=line.find(L'=');
	if(pos==std::wstring::npos)
		return;
	
	std::wstring name=line.substr(0, pos);
	std::wstring value=line.substr(pos+1);
	
	if(!value.empty()){
		if(value[0]==L'"'){
			value=value.substr(1, value.size()-2);
		}
	}
	
	m_table[name]=value;
}

XTBStringTable::~XTBStringTable(){
	
}

const std::wstring& XTBStringTable::stringForKey(const std::wstring& key) const{
	std::map<std::wstring, std::wstring>::const_iterator it;
	it=m_table.find(key);
	if(it==m_table.end()){
		XTBLog("string table key \"%s\" not found.", twW2M(key).c_str());
		return key;
	}else
		return it->second;
}

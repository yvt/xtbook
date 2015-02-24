//
//  i18n.cpp
//  XTBook
//
//  Created by Nexhawks on 4/17/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "i18n.h"
#include "platform.h"
#include <tcw/twException.h>
#include "XTBException.h"
#include <tcw/twStrConv.h>
#include <memory>
#include <time.h>

static XTBStringTable *g_stringTable=NULL;
static std::wstring g_currentLocale;

const std::wstring& XTBLocalizedString(const std::wstring& key){
	if(!g_stringTable){
		return key;
	}
	return g_stringTable->stringForKey(key);
}

std::vector<XTBStringTableInfo> XTBScanStringTables(){
	std::vector<XTBStringTableInfo> infos;
	XTBStringTableInfo info;
	std::vector<XTBSysString> paths;
	
	paths=XTBScanResourceDirectory(XTBResourceDirectoryLanguages);
	
	
	for(std::vector<XTBSysString>::iterator it=paths.begin();it!=paths.end();it++){
		info=XTBStringTable::infoForStringTableAtPath(*it);
		infos.push_back(info);
	}
	
	return infos;
}

const std::wstring& XTBGetLocale(){
	return g_currentLocale;
}

void XTBSetLocale(const std::wstring& identifier){
	std::vector<XTBStringTableInfo> infos=XTBScanStringTables();
	for(int i=0;i<infos.size();i++){
		const XTBStringTableInfo& info=infos[i];
		if(info.identifier==identifier){
			
			std::auto_ptr<XTBStringTable> newStringTable(new XTBStringTable(info.path));
			if(newStringTable->stringForKey(L"Type")==L"com.nexhawks.XTBook.langs"){
				if(g_stringTable)
					delete g_stringTable;
				g_stringTable=NULL;
				g_stringTable=newStringTable.release();
				g_currentLocale=identifier;
				return;
			}
			
			
			
		}
	}
	XTBLog("locale \"%s\" not found in path \"%s\".", twW2M(identifier).c_str(),
		   twW2M(XTBWStringFromSysString(XTBGetResourceDirectory(XTBResourceDirectoryLanguages))).c_str());
	
	// NOTE: DO NOT USE XTBThrowException HERE
	//       because locale is not yet initialized.
	throw twException(L"locale not found.");
}


std::wstring XTBFormatStd(const std::wstring& format, ...){
	va_list list;
	wchar_t buf[4096];
	va_start(list, format);
#ifdef EV_UNSAFE_SWPRINTF
	vswprintf(buf, format.c_str(), list);
#else
	vswprintf(buf, 1024, format.c_str(), list);
#endif
	va_end(list);
	return buf;
}

std::wstring XTBVarArgFormat(const std::wstring& format, va_list list){
	std::vector<const wchar_t *> args;
	const wchar_t *arg;
	
	while((arg=va_arg(list, const wchar_t *))!=NULL){
		args.push_back(arg);
	}

	if(args.empty())
		return format;
	
	std::wstring outStr;
	std::wstring::size_type pos=0;
	outStr.reserve(format.size());
	while(pos<format.size()){
		std::wstring::size_type newPos=format.find(L'{', pos);
		if(newPos==std::wstring::npos){
			outStr.append(format, pos, format.size()-pos);
			break;
		}
		outStr.append(format, pos, newPos-pos);
		pos=newPos+1;
		newPos=format.find(L'}', pos);
		if(newPos==std::wstring::npos)
			newPos=format.size();
		
		int argId=XTBIntWithString(format.substr(pos, newPos-pos));
		pos=newPos+1;
		if(argId<0 || argId>=args.size()){
			// over range.
			XTBLog("Argument Id %d is out of range. (argument count=%d)", args.size());
			outStr+=L'{';
			outStr+=XTBStringWithInt(argId);
			outStr+=L'}';
			continue;
		}
		
		arg=args[argId];
		if(arg)
			outStr+=arg;
		else
			outStr+=L"(null)";
		
	}
	return outStr;
}

std::wstring XTBFormat(const std::wstring& format, ...){
	va_list list;
	va_start(list, format);
	std::wstring str=XTBVarArgFormat(format, list);
	va_end(list);
	return str;
}

int XTBIntWithString(const std::wstring& str){
	char buf[256];
	wcstombs(buf, str.c_str(), 256);
	return atoi(buf);
}
std::wstring XTBStringWithInt(int iv){
	wchar_t buf[256];
#ifdef EV_UNSAFE_SWPRINTF
	swprintf(buf, L"%d", iv);
#else
	swprintf(buf, 256, L"%d", iv);
#endif
	return buf;
}

std::wstring XTBShortStringForDate(time_t combinedTime){
	struct tm expandedTime=*localtime(&combinedTime);
	time_t currentCombinedTime=time(0);
	struct tm expandedCurrentTime=*localtime(&currentCombinedTime);

	if(expandedTime.tm_year==expandedCurrentTime.tm_year){
		if(expandedTime.tm_yday==expandedCurrentTime.tm_yday){
			// same day!
			
			char buf[64];
			strftime(buf, 64, "%H:%M", &expandedTime);
			
			return XTBLocalizedString(L"XTBDateToday")+L' '+twM2W(buf);
			
		}
	}
	
	if(combinedTime>currentCombinedTime){
		time_t tomorrow=currentCombinedTime+3600*24;
		struct tm expandedTomorrow=*localtime(&tomorrow);
		
		if(expandedTomorrow.tm_year==expandedTime.tm_year){
			if(expandedTomorrow.tm_yday==expandedTime.tm_yday){
				// tomorrow.
				
				char buf[64];
				strftime(buf, 64, "%H:%M", &expandedTime);
				
				return XTBLocalizedString(L"XTBDateTomorrow")+L' '+twM2W(buf);
			}
		}
		
	}else{
		time_t yesterday=currentCombinedTime-3600*24;
		struct tm expandedYesterday=*localtime(&yesterday);
		
		if(expandedYesterday.tm_year==expandedTime.tm_year){
			if(expandedYesterday.tm_yday==expandedTime.tm_yday){
				// yesterday.
				
				char buf[64];
				strftime(buf, 64, "%H:%M", &expandedTime);
				
				return XTBLocalizedString(L"XTBDateYesterday")+L' '+twM2W(buf);
			}
		}
		
	}
	
	// same year?
	if(expandedTime.tm_year==expandedCurrentTime.tm_year){
		char buf[64];
		strftime(buf, 64, "%m/%d %H:%M", &expandedTime);
		
		return twM2W(buf);
	}
	
	// full date.
	{
		char buf[64];
		strftime(buf, 64, "%G/%m/%d %H:%M", &expandedTime);
		
		return twM2W(buf);
	}
	
}


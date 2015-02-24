//
//  XTBDictionaryType.cpp
//  XTBook
//
//  Created by Kawada Tomoaki on 5/22/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "XTBDictionaryType.h"
#include <map>
#include "XTBException.h"

static std::map<std::wstring, XTBDictionaryType *> g_dictionaryTypes;

void XTBDictionaryType::registerDictionaryType(XTBDictionaryType *factory){
	std::map<std::wstring, XTBDictionaryType *>::iterator it=g_dictionaryTypes.find(factory->identifier());
	if(it==g_dictionaryTypes.end()){
		// no duplication. we can add it.
		g_dictionaryTypes[factory->identifier()]=factory;
	}else{
		XTBThrowException(L"XTBDictionaryTypeAlreadyExistException", factory->identifier().c_str(), NULL);
	}
}

const XTBDictionaryType *XTBDictionaryType::dictionaryTypeWithIdentifier(std::wstring const & identifier){
	std::map<std::wstring, XTBDictionaryType *>::iterator it=g_dictionaryTypes.find(identifier);
	if(it==g_dictionaryTypes.end()){
		XTBThrowException(L"XTBDictionaryTypeNotFoundException", identifier.c_str(), NULL);
	}else{
		return it->second;
	}
}

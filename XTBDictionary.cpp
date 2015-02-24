//
//  XTBDictionary.cpp
//  XTBook
//
//  Created by Kawada Tomoaki on 5/22/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "XTBDictionary.h"
#include <tcw/twStrConv.h>
#include "TPList/TPLString.h"
#include "TPList/TPLDictionary.h"
#include "TPList/TPLPropertyListSerialization.h"
#include "TPList/TPLAutoReleasePtr.h"
#include <map>
#include "platform.h"
#include "commondlg.h"
#include "XTBDictionaryType.h"
#include "i18n.h"
#include <memory>
#include "XTBException.h"
#include "XTBUserDefaults.h"
#include "TPList/TPLArray.h"
#include "TPList/TPLString.h"

static std::map<std::wstring, XTBDictionary *> g_dictionaries;

XTBDictionary::XTBDictionary(const TPLDictionary *options){
	TPLString *schemeValue=NULL;
	if(options)
		schemeValue=dynamic_cast<TPLString *>(options->objectForKey(XTBDictionarySchemeKey));
	if(!schemeValue){
		XTBThrowException(L"XTBDictionarySchemeKeyNotFoundException", twM2W(XTBDictionarySchemeKey).c_str(), NULL);
	}
	m_scheme=twM2W(schemeValue->UTF8String());
		
	TPLString *identifierValue=NULL;
	if(options)
		identifierValue=dynamic_cast<TPLString *>(options->objectForKey(XTBDictionaryIdentifierKey));
	if(!identifierValue){
		XTBThrowException(L"XTBDictionarySchemeKeyNotFoundException", twM2W(XTBDictionaryIdentifierKey).c_str(), NULL);
	}
	m_identifier=twM2W(identifierValue->UTF8String());
	m_displayName=m_identifier;
	
	TPLString *displayNameValue=NULL;
	if(options)
		displayNameValue=dynamic_cast<TPLString *>(options->objectForKey(XTBDictionaryDisplayNameKey));
	if(displayNameValue){
		m_displayName=twM2W(displayNameValue->UTF8String());
	}
	
}	

XTBDictionary::~XTBDictionary(){
}

XTBDictionary *XTBDictionary::dictionaryWithName(const std::wstring& name){
	std::map<std::wstring, XTBDictionary *>::iterator it=g_dictionaries.find(name);
	if(it==g_dictionaries.end()){
		XTBThrowException(L"XTBDictionarySchemeNotFoundException", name.c_str(), NULL);
	}
	return it->second;
}
std::map<std::wstring, XTBDictionary *>& XTBDictionary::allDictionaries(){
	return g_dictionaries;
}
void XTBDictionary::addDictionaryAtPath(const XTBSysString& path){
	do{
		XTBSysString plistPath=path+XTBPathSeparator+XTBInfoPListName;
		FILE *f=XTBOpen(plistPath, "rb");
		if(!f){
			// couldn't open; Info.plist doesn't exist.
			XTBLog("\"%s\" not found, ignoring this dictionary bundle.", 
				   twW2M(XTBWStringFromSysString(plistPath)).c_str());
			continue;
		}
		TPLAutoReleasePtr<TPLObject> obj=TPLPropertyListSerialization::propertyListWithStream(f);
		fclose(f);
		TPLDictionary *info=dynamic_cast<TPLDictionary *>((TPLObject *)obj);
		if(!info){
			// invalid Info.plist format.
			XTBLog("\"%s\" was invald Info.plist file, ignoring this dictionary bundle.", 
				   twW2M(XTBWStringFromSysString(plistPath)).c_str());
			continue;
		}
		
		TPLString *identifier=dynamic_cast<TPLString *>(info->objectForKey(XTBDictionaryIdentifierKey));
		if(!identifier){
			XTBLog("\"%s\" have no identifier, ignoring this dictionary bundle.", 
				   twW2M(XTBWStringFromSysString(plistPath)).c_str());
			continue;
		}
		std::wstring ident=twM2W(identifier->UTF8String());
		XTBSetProgressText(XTBFormat(XTBLocalizedString(L"XTBLoadingSomething"), twM2W(identifier->UTF8String()).c_str(), NULL));
		
		TPLString *dictionaryTypeIdentifier=dynamic_cast<TPLString *>(info->objectForKey(XTBDictionaryTypeIdentifierKey));
		if(!dictionaryTypeIdentifier){
			XTBLog("\"%s\" have no dictionary type identifier, ignoring this dictionary.", 
				   twW2M(ident).c_str());
			continue;
		}
		try{
			std::wstring typeName=twM2W(dictionaryTypeIdentifier->UTF8String());
			const XTBDictionaryType *type=XTBDictionaryType::dictionaryTypeWithIdentifier(typeName);
			std::auto_ptr<XTBDictionary> dic(type->dictionaryForPath(path, info));
			if(!dic.get()){
				XTBLog("dictionary \"%s\" wasn't correctly opened.", 
					   twW2M(ident).c_str());
				continue;
			}
			
			if(g_dictionaries.find(dic->scheme())!=g_dictionaries.end()){
				// duplicating scheme name.
				XTBLog("\"%s\" has the scheme \"%s\", but another dictionary has too. Ignoring this dictionary.", 
					   twW2M(ident).c_str(), twW2M(dic->scheme()).c_str());
				continue;
			}
			
			XTBDictionary *dicPtr=dic.release();
			g_dictionaries[dicPtr->scheme()]=dicPtr;
			
		}catch(const std::wstring& msg){
			XTBLog("error while opening the dictionary \"%s\": %s", 
				   twW2M(ident).c_str(), twW2M(msg).c_str());
		}catch(const std::exception& ex){
			XTBLog("error while opening the dictionary \"%s\": %s", 
				   twW2M(ident).c_str(),ex.what());
		}catch(...){
			XTBLog("exception while opening the dictionary \"%s\".", 
				   twW2M(ident).c_str());
		}
	}while(0);
}
void XTBDictionary::scanDictionaries(){
	
	// core dictionaries.
	
	XTBSetProgressText(XTBLocalizedString(L"XTBDictionaryLoadingCoreDictionary"));
	
	// find user directories.
	TPLArray *userDirs=XTBUserDefaults::standardUserDefaults()->arrayForKey(XTBUserDefaultsKeyDictionarySearchPathes);
	for(TPLUInteger i=0;i<userDirs->count();i++){
		TPLString *str=dynamic_cast<TPLString *>(userDirs->objectAtIndex(i));
		if(!str)
			continue;
		
		XTBSysString dirPath=XTBSysStringFromWString(twM2W(str->UTF8String()));
		
		std::vector<XTBSysString> pathes=XTBScanDirectory(dirPath);
		for(size_t i=0;i<pathes.size();i++){
			
			XTBSysString path=pathes[i];
			
			addDictionaryAtPath(path);
			
		}
		
	}
	
	// dictionaries in the standard path.
	{
		std::vector<XTBSysString> pathes=XTBScanResourceDirectory(XTBResourceDirectoryInternalDictionaries);
		for(size_t i=0;i<pathes.size();i++){
			
			XTBSysString path=pathes[i];
			
			addDictionaryAtPath(path);
			
		}
	}

	{
		std::vector<XTBSysString> pathes=XTBScanResourceDirectory(XTBResourceDirectoryDictionaries);
		for(size_t i=0;i<pathes.size();i++){
			
			XTBSysString path=pathes[i];
			
			addDictionaryAtPath(path);
			
		}
	}
	

	
}

//
//  XTBUserDefaults.cpp
//  XTBook
//
//  Created by Kawada Tomoaki on 5/21/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "XTBUserDefaults.h"
#include "platform.h"
#include "TPList/TPLPropertyListSerialization.h"
#include "TPList/TPLString.h"
#include "TPList/TPLAutoReleasePtr.h"
#include <assert.h>
#include "TPList/TPLArray.h"
#include "TPList/TPLNumber.h"

// NOTE: Do not use XTBFileStream here; TPList doesn't support it.

XTBUserDefaults::XTBUserDefaults(TPLDictionary *dic, const XTBSysString& path):
m_path(path), TPLDictionary(dic){
	
}

void XTBUserDefaults::synchronize(){
	FILE *f=XTBOpen(m_path, "wb");
	TPLPropertyListSerialization::writePropertyList(this, f);
	fclose(f);
}

XTBUserDefaults *XTBUserDefaults::m_standardUserDefaults=NULL;
XTBUserDefaults *XTBUserDefaults::standardUserDefaults(){
	if(!m_standardUserDefaults){
		TPLDictionary *dic=NULL;
		FILE *f=XTBOpen(XTBGetStandardUserDefaultsPath(), "rb");
		if(f){
			dic=dynamic_cast<TPLDictionary *>(TPLPropertyListSerialization::propertyListWithStream(f));
			fclose(f);
		}
		if(!dic){
			dic=new TPLDictionary();
		}
		m_standardUserDefaults=new XTBUserDefaults(dic, XTBGetStandardUserDefaultsPath());
		dic->release();
	}
	return m_standardUserDefaults;
}

TPLString *XTBUserDefaults::stringForKey(const char *key, const char *defaultValue){
	if(dynamic_cast<TPLString *>(objectForKey(key))==NULL){
		if(defaultValue!=NULL){
			TPLAutoReleasePtr<TPLString> str=new TPLString(defaultValue);
			setObject(&(*str), key);
		}else{
			TPLAutoReleasePtr<TPLString> str=new TPLString();
			setObject(&(*str), key);
		}
	}
	{
		TPLString *str;
		str=dynamic_cast<TPLString *>(objectForKey(key));
		assert(str!=NULL);
		return str;
	}
}
TPLArray *XTBUserDefaults::arrayForKey(const char *key){
		if(dynamic_cast<TPLArray *>(objectForKey(key))==NULL){
				TPLAutoReleasePtr<TPLArray> str=new TPLArray();
				setObject(&(*str), key);
		}
		{
			TPLArray *str;
			str=dynamic_cast<TPLArray *>(objectForKey(key));
			assert(str!=NULL);
			return str;
		}
}
TPLDictionary *XTBUserDefaults::dictionaryForKey(const char *key){
	if(dynamic_cast<TPLDictionary *>(objectForKey(key))==NULL){
		TPLAutoReleasePtr<TPLDictionary> str=new TPLDictionary();
		setObject(&(*str), key);
	}
	{
		TPLDictionary *str;
		str=dynamic_cast<TPLDictionary *>(objectForKey(key));
		assert(str!=NULL);
		return str;
	}
}
TPLNumber *XTBUserDefaults::numberForKey(const char *key, const char *defaultValue){
	if(defaultValue==NULL)
		defaultValue="0";
	if(dynamic_cast<TPLNumber *>(objectForKey(key))==NULL){
		TPLAutoReleasePtr<TPLNumber> num=new TPLNumber(defaultValue);
		setObject(&(*num), key);
	}
	{
		TPLNumber *num;
		num=dynamic_cast<TPLNumber *>(objectForKey(key));
		assert(num!=NULL);
		return num;
	}
}



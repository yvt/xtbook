//
//  XTBUserDefaults.h
//  XTBook
//
//  Created by Kawada Tomoaki on 5/21/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#pragma once

#include "platform.h"
#include "TPList/TPLDictionary.h"

class TPLString;
class TPLNumber;

#define XTBUserDefaultsKeyLocale "XTBLocale"
#define XTBUserDefaultsKeyBookmarks "XTBBookmarks"
#define XTBUserDefaultsKeySingleTouchLink "XTBSingleTouchLink"
#define XTBUserDefaultsKeyDictionarySearchPathes "XTBDictionarySearchPathes"
#define XTBUserDefaultsKeyFullScreen "XTBFullScreen"
#define XTBUserDefaultsKeySinglePage "XTBSinglePageMode"
#define XTBUserDefaultsKeyIgnoreTemplates "XTBIgnoreTemplates"


class XTBUserDefaults: public TPLDictionary{
private:
	static XTBUserDefaults *m_standardUserDefaults;
	XTBSysString m_path;
public:
	XTBUserDefaults(TPLDictionary *dic, const XTBSysString& path);
	void synchronize();
	
	static XTBUserDefaults *standardUserDefaults();
	
	TPLString *stringForKey(const char *, const char *defaultValue=NULL);
	TPLArray *arrayForKey(const char *);
	TPLDictionary *dictionaryForKey(const char *);
	TPLNumber *numberForKey(const char *, const char *defaultValue=NULL);
};


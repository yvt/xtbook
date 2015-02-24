//
//  XTBWikiplexusSiteInfo.cpp
//  XTBook
//
//  Created by Kawada Tomoaki on 7/10/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "XTBWikiplexusSiteInfo.h"
#include "XTBException.h"
#include "TPList/TPLNumber.h"
#include "TPList/TPLArray.h"
#include "TPList/TPLDictionary.h"
#include "TPList/TPLString.h"
#include "TPList/TPLAutoReleasePtr.h"
#include "TPList/TPLPropertyListSerialization.h"

#include <tcw/twStrConv.h>

#define XTBWikiplexusSiteInfoBaseUrlKey "XTBWikiplexusBaseUrl"
#define XTBWikiplexusSiteInfoGeneratorKey "XTBWikiplexusGenerator"
#define XTBWikiplexusSiteInfoNamespacesKey "XTBWikiplexusNamespaces"
#define XTBWikiplexusSiteInfoSiteNameKey "XTBWikiplexusSiteName"
#define XTBWikiplexusSiteInfoNamespaceKeyKey "XTBWikiplexusNamespaceKey"
#define XTBWikiplexusSiteInfoNamespaceNameKey "XTBWikiplexusNamespaceName"


XTBWikiplexusNamespaceInfo::XTBWikiplexusNamespaceInfo(const TPLDictionary *dic):
key(-100),
name(L"#invalid"){
	if(TPLNumber *value=dynamic_cast<TPLNumber *>(dic->objectForKey(XTBWikiplexusSiteInfoNamespaceKeyKey))){
		key=value->intValue();
	}
	if(TPLString *value=dynamic_cast<TPLString *>(dic->objectForKey(XTBWikiplexusSiteInfoNamespaceNameKey))){
		name=twM2W(value->UTF8String());
	}
}

XTBWikiplexusSiteInfo::XTBWikiplexusSiteInfo(const XTBSysString& path):
baseUrl(L"http://www.nexhawks.net/"),
generator(L"multimediaTCPPSH"),
siteName(L"Nexhawks.net"){
	FILE *f;
	f=XTBOpen(path, "rb");
	if(f==NULL){
		XTBThrowException(L"XTBFileNotFoundException",
						  XTBWStringFromSysString(path).c_str(),
						  NULL);
	}
	
	try{
		
		TPLAutoReleasePtr<TPLDictionary> dic
		(dynamic_cast<TPLDictionary *>
		 (TPLPropertyListSerialization::propertyListWithStream(f)));
		
		if(TPLString *value=dynamic_cast<TPLString *>(dic->objectForKey(XTBWikiplexusSiteInfoBaseUrlKey))){
			baseUrl=twM2W(value->UTF8String());
		}
		
		if(TPLString *value=dynamic_cast<TPLString *>(dic->objectForKey(XTBWikiplexusSiteInfoGeneratorKey))){
			generator=twM2W(value->UTF8String());
		}
		
		if(TPLString *value=dynamic_cast<TPLString *>(dic->objectForKey(XTBWikiplexusSiteInfoSiteNameKey))){
			siteName=twM2W(value->UTF8String());
		}
		
		if(TPLArray *value=dynamic_cast<TPLArray *>(dic->objectForKey(XTBWikiplexusSiteInfoNamespacesKey))){
			for(TPLUInteger i=0;i<value->count();i++){
				TPLDictionary *dic;
				dic=dynamic_cast<TPLDictionary *>(value->objectAtIndex(i));
				if(dic){
					namespaces.push_back(XTBWikiplexusNamespaceInfo(dic));
				}
			}
		}
		
	}catch(...){
		fclose(f);
		throw;
	}
	
	fclose(f);
}

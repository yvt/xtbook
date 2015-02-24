//
//  XTBWikiplexusSiteInfo.h
//  XTBook
//
//  Created by Kawada Tomoaki on 7/10/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#pragma once

#include <string>
#include "platform.h"

class TPLDictionary;

struct XTBWikiplexusNamespaceInfo{
	int key;
	std::wstring name;
	XTBWikiplexusNamespaceInfo(const TPLDictionary *);
};

struct XTBWikiplexusSiteInfo{
	std::wstring baseUrl;
	std::wstring generator;
	std::wstring siteName;
	std::vector<XTBWikiplexusNamespaceInfo> namespaces;
	
	XTBWikiplexusSiteInfo(const XTBSysString&);
};




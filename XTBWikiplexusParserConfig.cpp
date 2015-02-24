//
//  XTBWikiplexusParserConfig.cpp
//  XTBook
//
//  Created by Kawada Tomoaki on 7/7/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "XTBWikiplexusParserConfig.h"
#include "XTBWikiplexusSiteInfo.h"
#include "XTBWikiplexusDictionary.h"

XTBWikiplexusParserConfig::XTBWikiplexusParserConfig(XTBWikiplexusDictionary *dic){
	XTBWikiplexusSiteInfo *siteInfo=dic->siteInfo();
	
	std::vector<XTBWikiplexusNamespaceInfo>::iterator it;
	
	for(it=siteInfo->namespaces.begin();
		it!=siteInfo->namespaces.end();
		it++){
		
		const XTBWikiplexusNamespaceInfo&info=*it;
		
		m_namespaceNames[info.key]=info.name;
		
		
	}
	
	m_namespaceAliases[L"image"]=m_namespaceNames[TWStandardNamespaceFile];
	m_namespaceAliases[L"Image"]=m_namespaceNames[TWStandardNamespaceFile];
	m_namespaceAliases[L"file"]=m_namespaceNames[TWStandardNamespaceFile];
	m_namespaceAliases[L"File"]=m_namespaceNames[TWStandardNamespaceFile];
	
}

XTBWikiplexusParserConfig::~XTBWikiplexusParserConfig(){
	
}

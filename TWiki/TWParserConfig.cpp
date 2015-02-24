//
//  TWParserConfig.cpp
//  XTBook
//
//  Created by Kawada Tomoaki on 5/14/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//
/*
 * This file is part of XTBook.
 *
 * XTBook is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * XTBook is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with XTBook.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "TWParserConfig.h"
#include <assert.h>

TWParserConfig::TWParserConfig(){
	m_namespaceNames[TWStandardNamespaceMedia]=L"Media";
	m_namespaceNames[TWStandardNamespaceSpecial]=L"Special";
	m_namespaceNames[TWStandardNamespaceMain]=L"";
	m_namespaceNames[TWStandardNamespaceTalk]=L"Talk";
	m_namespaceNames[TWStandardNamespaceUser]=L"User";
	m_namespaceNames[TWStandardNamespaceUserTalk]=L"User_talk";
	m_namespaceNames[TWStandardNamespaceProject]=L"$1";
	m_namespaceNames[TWStandardNamespaceProjectTalk]=L"$1_talk";
	m_namespaceNames[TWStandardNamespaceFile]=L"Image";
	m_namespaceNames[TWStandardNamespaceFileTalk]=L"Image_talk";
	m_namespaceNames[TWStandardNamespaceMediaWiki]=L"MediaWiki";
	m_namespaceNames[TWStandardNamespaceMediaWikiTalk]=L"MediaWiki_talk";
	m_namespaceNames[TWStandardNamespaceTemplate]=L"Template";
	m_namespaceNames[TWStandardNamespaceTemplateTalk]=L"Template_talk";
	m_namespaceNames[TWStandardNamespaceHelp]=L"Help";
	m_namespaceNames[TWStandardNamespaceHelpTalk]=L"Help_talk";
	m_namespaceNames[TWStandardNamespaceCategory]=L"Category";
	m_namespaceNames[TWStandardNamespaceCategoryTalk]=L"Category_talk";
	
}

const std::wstring& TWParserConfig::nameForStandardNamespace(int n) const{
	std::map<int, std::wstring>::const_iterator it;
	it=m_namespaceNames.find(n);
	assert(it!=m_namespaceNames.end());
	return it->second;
}

const std::wstring& TWParserConfig::resolveNamespaceAlias(std::wstring const &s) const{
	std::map<std::wstring, std::wstring>::const_iterator it;
	if((it=m_namespaceAliases.find(s))!=m_namespaceAliases.end()){
		return it->second;
	}
	return s;
}

const std::wstring& TWParserConfig::resolveMagicWordAlias(std::wstring const &s) const{
	std::map<std::wstring, std::wstring>::const_iterator it;
	if((it=m_magicWordAliases.find(s))!=m_magicWordAliases.end()){
		return it->second;
	}
	return s;
}

//void XTBDumpWString(const std::wstring&);

int TWParserConfig::namespaceWithName(const std::wstring& str) const{
	std::map<int, std::wstring>::const_iterator it;
	for(it=m_namespaceNames.begin();it!=m_namespaceNames.end();it++){
		if(it->second==str)
			return it->first;
	}
	return TWStandardNamespaceInvalid;
}

std::wstring TWParserConfig::siteName() const{
	return L"TWiki";
}

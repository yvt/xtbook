//
//  TWParserConfig.h
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

#pragma once
#include <string>
#include <map>

enum{
	TWStandardNamespaceInvalid=-100,
	TWStandardNamespaceMedia=-2,
	TWStandardNamespaceSpecial=-1,
	TWStandardNamespaceMain=0,
	TWStandardNamespaceTalk,
	TWStandardNamespaceUser,
	TWStandardNamespaceUserTalk,
	TWStandardNamespaceProject,
	TWStandardNamespaceProjectTalk,
	TWStandardNamespaceFile,
	TWStandardNamespaceFileTalk,
	TWStandardNamespaceMediaWiki,
	TWStandardNamespaceMediaWikiTalk,
	TWStandardNamespaceTemplate,
	TWStandardNamespaceTemplateTalk,
	TWStandardNamespaceHelp,
	TWStandardNamespaceHelpTalk,
	TWStandardNamespaceCategory,
	TWStandardNamespaceCategoryTalk
};

class TWParserConfig{
protected:
	std::map<int, std::wstring> m_namespaceNames;
	std::map<std::wstring, std::wstring> m_namespaceAliases;
	std::map<std::wstring, std::wstring> m_magicWordAliases;
public:
	
	TWParserConfig();
	virtual ~TWParserConfig(){}
	
	const std::wstring& nameForStandardNamespace(int n) const;
	int namespaceWithName(const std::wstring&) const;
	const std::wstring& resolveNamespaceAlias(const std::wstring& s) const;
	const std::wstring& resolveMagicWordAlias(const std::wstring& s) const;
	
	virtual std::wstring siteName() const;
};

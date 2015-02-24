//
//  TWNamespaceAliasParserFunction.cpp
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

#include "TWNamespaceAliasParserFunction.h"
#include "TWParser.h"
#include "TWParserConfig.h"
#include "TWUtils.h"

TWNamespaceAliasParserFunction::TWNamespaceAliasParserFunction(TWParser *parser):
m_parser(parser){
}

std::wstring TWNamespaceAliasParserFunction::execute(const std::wstring& arg0, const std::vector<std::wstring>& args, TWPreprocessorFrame *){
	
	TWParserConfig *cfg=m_parser->config();
	const std::wstring& arg=arg0;
	if(arg==L"Media")
		return cfg->nameForStandardNamespace(TWStandardNamespaceMedia);
	if(arg==L"Special")
		return cfg->nameForStandardNamespace(TWStandardNamespaceSpecial);
	if(arg==L"Talk")
		return cfg->nameForStandardNamespace(TWStandardNamespaceTalk);
	if(arg==L"User_talk")
		return cfg->nameForStandardNamespace(TWStandardNamespaceUserTalk);
	if(arg==L"Project")
		return cfg->nameForStandardNamespace(TWStandardNamespaceProject);
	if(arg==L"Project_talk")
		return cfg->nameForStandardNamespace(TWStandardNamespaceProjectTalk);
	if(arg==L"Image")
		return cfg->nameForStandardNamespace(TWStandardNamespaceFile);
	if(arg==L"Image_talk")
		return cfg->nameForStandardNamespace(TWStandardNamespaceFileTalk);
	if(arg==L"MediaWiki")
		return cfg->nameForStandardNamespace(TWStandardNamespaceMediaWiki);
	if(arg==L"MediaWiki_talk")
		return cfg->nameForStandardNamespace(TWStandardNamespaceMediaWikiTalk);
	if(arg==L"Template")
		return cfg->nameForStandardNamespace(TWStandardNamespaceTemplate);
	if(arg==L"Template_talk")
		return cfg->nameForStandardNamespace(TWStandardNamespaceTemplateTalk);
	if(arg==L"Help")
		return cfg->nameForStandardNamespace(TWStandardNamespaceHelp);
	if(arg==L"Help_talk")
		return cfg->nameForStandardNamespace(TWStandardNamespaceHelpTalk);
	if(arg==L"Category")
		return cfg->nameForStandardNamespace(TWStandardNamespaceCategory);
	if(arg==L"Category_talk")
		return cfg->nameForStandardNamespace(TWStandardNamespaceCategoryTalk);
	
	int id=TWUtils::intValueFromString(arg);
	if(id>=-2 && id<=15)
		return cfg->nameForStandardNamespace(id);
	
	throw L"invalid namespace specifier:"+arg;
}


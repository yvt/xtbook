/*
 *  TWParser.h
 *  XTBook
 *
 *  Created by Nexhawks on 2/23/11.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */
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
#include <set>
#include <string>
#include <map>

class TWPreprocessor;
class TWPreprocessorNode;
class TWPreprocessorFrame;
class TWParserFunction;
class TWParserConfig;
class TWParserExtension;

struct TWParserBlockState;

struct TWTitle{
	int ns;
	std::wstring interWiki;
	std::wstring dbKey;
	
	bool isInterWiki() const{
		return !interWiki.empty();
	}
	bool operator ==(const TWTitle& ttl) const{
		if(interWiki!=ttl.interWiki)
			return false;
		if(ns!=ttl.ns)
			return false;
		if(dbKey!=ttl.dbKey)
			return false;
		return true;
	}
	bool operator !=(const TWTitle& ttl) const{
		return !(*this==ttl);
	}
	
};

class TWParser{
protected:
	std::multimap<std::wstring, TWParserFunction *> m_parserFunctions;
	std::multimap<std::wstring, TWParserFunction *> m_underscoreFunctions;
	std::multimap<std::wstring, TWParserExtension *> m_parserExtensions;
	
private:
	std::set<std::wstring> m_stripList;
	TWPreprocessor *m_preprocessor;
	std::wstring m_title;
	TWTitle m_parsedTitle;
	int m_externalLinkId;
	
	TWParserConfig *m_config;

	std::wstring m_displayTitle;
	std::wstring m_defaultSort;
	
	std::map<std::wstring, std::wstring> m_imageParams;
	std::map<std::wstring, TWPreprocessorNode *> m_cachedTemplates;
	
	std::wstring internalParse(const std::wstring& text, TWPreprocessorFrame *);
	
	std::wstring doTableStuff(const std::wstring& text);
	std::wstring doDoubleUnderscore(const std::wstring& text, TWPreprocessorFrame *frame);
	std::wstring doHeadings(const std::wstring& text);
	std::wstring doAllQuotes(const std::wstring& text);
	std::wstring replaceInternalLinks(std::wstring& text);
	std::wstring replaceExternalLinks(const std::wstring& text, const std::wstring& scheme);
	std::wstring doMagicLinks(const std::wstring& text);
	std::wstring doBlockLevels(const std::wstring& text);
	std::wstring doNoWikis(const std::wstring& text);
	
	std::wstring removeEmptyTags(const std::wstring& text);
	std::wstring removeEmptyTags2(const std::wstring& text);
	
	int flagsForPreprocessing(TWPreprocessorFrame *frame) const;
	
	
	TWPreprocessorNode *getTemplateDom(const TWTitle&);
	
	std::wstring closeParagraph(TWParserBlockState&) const;
	const wchar_t *openList(wchar_t, TWParserBlockState&) const;
	const wchar_t *nextItem(wchar_t, TWParserBlockState&) const;
	const wchar_t *closeList(wchar_t, TWParserBlockState&) const;
	
	std::wstring makeImage(const TWTitle& referenceTo, const std::wstring& param);
	
protected:
	
	
	
	virtual std::wstring fetchPage(const TWTitle&) const;
	virtual bool doesLanguageExists(const std::wstring&) const;
	virtual std::wstring makeSelfLink(const std::wstring& displayText);
	virtual std::wstring makeInternalLink(const TWTitle& referenceTo, const std::wstring& displayText);
	
	virtual std::wstring makeImage2(const TWTitle& referenceTo, std::map<std::wstring,std::wstring>& frameParam, std::map<std::wstring,std::wstring>& handlerParam);
	virtual std::wstring makeError(const std::wstring&) const;
	
public:
	TWParser(TWParserConfig *);
	virtual ~TWParser();
	
	TWTitle parseTitle(const std::wstring&) const;
	std::wstring expandTitle(const TWTitle&) const;
	
	const std::wstring& title() const{return m_title;}
	const TWTitle& parsedTitle() const{return m_parsedTitle;}
	const std::set<std::wstring>& stripList() const{return m_stripList;}
	TWParserConfig *config() const{return m_config;}
	TWPreprocessor *preprocessor() const{return m_preprocessor;};
	
	std::wstring braceSubstitution(TWPreprocessorNode *, TWPreprocessorFrame *);
	std::wstring argSubstitution(TWPreprocessorNode *, TWPreprocessorFrame *);
	std::wstring extensionSubstitution(TWPreprocessorNode *, TWPreprocessorFrame *);
	
	void clearState();
	
	std::wstring parse(const std::wstring& text, const std::wstring& title, 
					   bool lineStart=true, bool clearState=true);
	
	// these are set by {{DISPLAYTITLE}} and so on.
	const std::wstring& displayTitle() const{return m_displayTitle;}
	const std::wstring& defaultSort() const{return m_defaultSort;}
	
	void setDisplayTitle(const std::wstring& s){m_displayTitle=s;}
	void setDefaultSort(const std::wstring& s){m_defaultSort=s;}
	
	std::wstring recursiveTagParse(const std::wstring&,
								   TWPreprocessorFrame *);
	
	bool doesParserExtensionExists(const std::wstring&) const;
	
	void clearTemplateCache();
};

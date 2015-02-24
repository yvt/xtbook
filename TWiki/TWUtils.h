/*
 *  TWUtils.h
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

#include <string>
#include <vector>

extern const std::wstring TWEmptyString;

class TWUtils{
protected:
	TWUtils(){}
	~TWUtils(){}
public:
	static std::wstring escapeHTMLSpecialChars(const std::wstring&);
	static std::wstring unescapeHTMLSpecialChars(const std::wstring&);
	static int intValueFromString(const std::wstring&);
	static float floatValueFromString(const std::wstring&);
	static std::wstring stringWithIntValue(int);
	static std::wstring trimString(const std::wstring&);
	static std::wstring toLower(const std::wstring&);
	static void toLowerInplace(std::wstring&);
	static std::vector<std::wstring> split(const std::wstring&, const std::wstring&);
	static std::vector<wchar_t *> splitInplace(wchar_t *, const wchar_t *);
	static wchar_t complementaryBracketCharFor(wchar_t c);
	static std::wstring::size_type findMatchingBracket(const std::wstring&, std::wstring::size_type pos=0);
	
	static std::wstring dbKeyFor(const std::wstring&);
	static std::wstring displayTitleFor(const std::wstring&);
	static std::wstring resolveRelativeUrl(const std::wstring&, 
											const std::wstring& baseUrl);
	static std::wstring parentUrlFor(const std::wstring&);
	static std::wstring namespaceForUrl(const std::wstring&);
	static std::wstring baseNameForUrl(const std::wstring&);
	
	static bool matchStart(const std::wstring&, const std::wstring&);
	static bool matchStartAndRemove(std::wstring&, const std::wstring&);
	static std::wstring::size_type findColonNoLinks(const std::wstring&, std::wstring::size_type pos=0,
													std::wstring::size_type n=std::wstring::npos);
	static std::wstring::size_type commonCharCount(const std::wstring& s1, const std::wstring& s2);
	static std::wstring::size_type findInRange(const std::wstring& text, const wchar_t *,
											   std::wstring::size_type pos,
											   std::wstring::size_type n); // WARNING: may be thread-unsafe
	static std::wstring::size_type findInRangeFirstOf(const std::wstring& text, const wchar_t *,
											   std::wstring::size_type pos,
											   std::wstring::size_type n); // WARNING: may be thread-unsafe
	static std::wstring::size_type findInRangeFirstNotOf(const std::wstring& text, const wchar_t *,
											   std::wstring::size_type pos,
											   std::wstring::size_type n); // WARNING: may be thread-unsafe
	static std::wstring m2w(const std::string&); // mbs2wcs
	static std::string w2m(const std::wstring&); // wcs2mbs
	static std::wstring evaluateExpression(const std::wstring&);
	static bool isNumeric(const std::wstring&);
	static bool isEqual(const std::wstring&,
						const std::wstring&);
	
	static std::wstring replace(const std::wstring&, const std::wstring&, const std::wstring&);
	
	static std::wstring sanitizeAttribute(const std::wstring&);
	
	static std::wstring escapeSpecialChars(const std::wstring&);
};

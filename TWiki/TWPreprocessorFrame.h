/*
 *  TWPreprocessorFrame.h
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

class TWPreprocessor;
class TWParser;
class TWPreprocessorTemplateFrame;

class TWPreprocessorNode;

class TWPreprocessorFrame {
protected:
	TWPreprocessor *m_preprocessor;
	TWParser *m_parser;
	std::wstring m_title;
	int m_depth;
	int m_templateDepth;
	
public:
	TWPreprocessorFrame(TWPreprocessor *);
	virtual ~TWPreprocessorFrame();
	
	const std::wstring& title() const{return m_title;}
	TWParser *parser() const{return m_parser;}
	TWPreprocessor *preprocessor() const{return m_preprocessor;}
	int depth() const{return m_depth;}
	int templateDepth() const{return m_templateDepth;}
	
	TWPreprocessorTemplateFrame *newChild(const std::vector<TWPreprocessorNode *>& args, const std::wstring& ttl);
	TWPreprocessorTemplateFrame *newChild(const std::vector<TWPreprocessorNode *>& args);
	std::wstring expand(TWPreprocessorNode *root, int flags);
	std::wstring implode(const wchar_t * separator, const std::vector<TWPreprocessorNode *>& root, int flags=0);
	
	static std::wstring idForHeading(const std::wstring&);
	
	virtual std::wstring argumentForName(const std::wstring&);
};

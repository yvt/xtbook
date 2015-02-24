/*
 *  TWPreprocessorTemplateFrame.h
 *  XTBook
 *
 *  Created by Nexhawks on 2/26/11.
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

#include "TWPreprocessorFrame.h"

class TWPreprocessorTemplateFrame : public TWPreprocessorFrame{
	std::map<std::wstring, TWPreprocessorNode *> m_numberedArgs;
	std::map<std::wstring, TWPreprocessorNode *> m_namedArgs;
	
	TWPreprocessorFrame *m_parent;
	
	std::wstring numberedArgumentForName(const std::wstring&);
	std::wstring namedArgumentForName(const std::wstring&);
public:
	TWPreprocessorTemplateFrame(const std::vector<TWPreprocessorNode *>& args, TWPreprocessor *preprocessor, TWPreprocessorFrame *parent,
								const std::wstring& title);
	
	virtual std::wstring argumentForName(const std::wstring&);
	
	void dumpArguments();
};

/*
 *  TWPreprocessorNode.h
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

#include "TWNode.h"
#include <string>
#include <vector>

class TWPreprocessorNode;

struct TWPreprocessorNodePart{
	TWPreprocessorNode *nameNode;
	TWPreprocessorNode *valueNode;
	std::wstring index;
};

struct TWPreprocessorNodeExt{
	TWPreprocessorNode *nameNode;
	TWPreprocessorNode *attrNode;
	TWPreprocessorNode *innerNode;
	TWPreprocessorNode *closeNode;
};

struct TWPreprocessorNodeHeading{
	std::wstring i;
	std::wstring level;
};

struct TWPreprocessorNodeTemplate{
	TWPreprocessorNode *titleNode;
	std::wstring lineStart;
	std::vector<TWPreprocessorNode *> partNodes;
};

class TWPreprocessorNode : public TWNode{
public:
	TWPreprocessorNode(const std::wstring&);
	virtual ~TWPreprocessorNode();
	static TWPreprocessorNode *nodeWithText(const std::wstring&,
											const std::wstring&);
	
	
	virtual std::wstring toString() const;
	
	TWPreprocessorNodePart splitArg() const;
	TWPreprocessorNodeExt splitExt() const;
	TWPreprocessorNodeHeading splitHeading() const;
	TWPreprocessorNodeTemplate splitTemplate() const;
	
	virtual bool isAttribute(){return false;}
	virtual bool isText(){return false;}
	
};

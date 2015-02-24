/*
 *  TWElement.h
 *  XTBook
 *
 *  Created by Nexhawks on 2/24/11.
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

#include "TWNode.h"
#include <map>

class TWElement : public TWNode {
protected:
	std::map<std::wstring, std::wstring> m_attributes;
public:
	TWElement(const std::wstring&);
	virtual void setAttribute(const std::wstring&, const std::wstring&);
	virtual std::wstring getAttribute(const std::wstring&) const;
	virtual bool hasAttribute(const std::wstring&) const;
	virtual void removeAttribute(const std::wstring&);

};

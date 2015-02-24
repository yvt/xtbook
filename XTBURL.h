//
//  XTBURL.h
//  XTBook
//
//  Created by Kawada Tomoaki on 5/27/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#pragma once

#include <string>

class XTBDocument;

enum XTBURLType{
	XTBURLTypeDocument=0,
	XTBURLTypeSearch,
	XTBURLTypeHome,
	XTBURLTypeSystemCall
};

#define XTBURLSeparatorChar		L':'


struct XTBURL{
	std::wstring scheme;
	std::wstring articleName;
	
	XTBURL();
	XTBURL(const std::wstring&);
	
	bool isReachable() const;
	XTBDocument *document() const;
	
	XTBURLType type() const;
	
	bool operator ==(const XTBURL&) const;
	bool operator !=(const XTBURL&) const;
	
	std::wstring urlString() const;
};

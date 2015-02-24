//
//  XTBWikiplexusInfobox.h
//  XTBook
//
//  Created by 河田 智明 on 8/4/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#pragma once

#include "TWiki/TWPreparsedParserFunction.h"

class XTBWikiplexusInfobox: public TWPreparsedParserFunction{
	
	std::wstring htmlForHeaderRow
	(const std::wstring& className,		// class
	 const std::wstring& style,			// headerstyle
	 const std::wstring& header) const;	// header
	
	std::wstring htmlForDataRow
	(const std::wstring& rowClassName,	// rowclass
	 const std::wstring& className,		// class
	 const std::wstring& style,			// datastyle
	 const std::wstring& data) const;	// data
	
	std::wstring htmlForLabeledDataRow
	(const std::wstring& rowClassName,	// rowclass
	 const std::wstring& labelStyle,	// labelstyle
	 const std::wstring& label,			// label
	 const std::wstring& className,		// class
	 const std::wstring& style,			// datastyle
	 const std::wstring& data) const;	// data
	
public:
	virtual ~XTBWikiplexusInfobox(){}
	virtual std::wstring execute(const std::wstring& arg0, const std::vector<std::wstring>& args, TWPreprocessorFrame *);
};


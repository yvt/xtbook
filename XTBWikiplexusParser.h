//
//  XTBWikiplexusParser.h
//  XTBook
//
//  Created by Kawada Tomoaki on 7/7/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#pragma once

#include "TWiki/TWParser.h"

class XTBWikiplexusDictionary;

class XTBWikiplexusParser: public TWParser{
	XTBWikiplexusDictionary *m_dic;
	std::wstring urlForImage(const TWTitle&) const;
	std::wstring urlForInternalLink(const TWTitle&) const;
	
	std::wstring makeThumbLink2(const TWTitle& referenceTo, std::map<std::wstring,std::wstring>& frameParam, std::map<std::wstring,std::wstring>& handlerParam,
								int width,
								std::wstring align);
	
	std::wstring doFragmentId(const std::wstring&) const;
	
	std::wstring linkImage(const std::wstring&, std::map<std::wstring,std::wstring>& frameParam,
						   const TWTitle& referenceTo);
	
protected:
	virtual std::wstring fetchPage(const TWTitle&) const;
	virtual std::wstring makeSelfLink(const std::wstring& displayText);
	virtual std::wstring makeInternalLink(const TWTitle& referenceTo, const std::wstring& displayText);
	virtual std::wstring makeImage2(const TWTitle& referenceTo, std::map<std::wstring,std::wstring>& frameParam, std::map<std::wstring,std::wstring>& handlerParam);
	virtual std::wstring makeError(const std::wstring&) const;
public:
	XTBWikiplexusParser(TWParserConfig *, XTBWikiplexusDictionary *);
	virtual ~XTBWikiplexusParser();
};

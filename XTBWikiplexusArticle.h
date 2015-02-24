//
//  XTBWikiplexusArticle.h
//  XTBook
//
//  Created by Kawada Tomoaki on 7/7/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#pragma once

#include "XTBArticle.h"

class XTBWikiplexusDictionary;

class XTBWikiplexusArticle: public XTBArticle{
	mutable bool m_loaded;
	mutable std::string m_bytes;
	
	std::wstring m_title;
	std::string m_dbKey;
	const XTBWikiplexusDictionary *m_dic;
protected:
	uint32_t BEInt32AtAddress(size_t addr) const;
	uint16_t BEInt16AtAddress(size_t addr) const;
	
	const std::string& bytes() const; // calling this loades article.
	
	virtual std::wstring wikiText(int page) const=0;
	
public:
	XTBWikiplexusArticle(const XTBWikiplexusDictionary *,
						 const std::string& dbKey);
	virtual ~XTBWikiplexusArticle();
	
	
	virtual std::wstring htmlForPage(int) const;
	virtual std::wstring displayTitle() const;
	
	virtual int pageForFragmentId(const std::wstring&) const;
	
	const XTBWikiplexusDictionary *dictionary() const{return m_dic;}
	
};

//
//  XTBDicDBMemoryCache.h
//  XTBook
//
//  Created by Kawada Tomoaki on 7/9/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#pragma once

#include "global.h"

class XTBDicDBReader;


struct XTBDicDBMemoryCacheEntry{
	
	std::string key;
	std::string contents;
};

class XTBDicDBMemoryCache{
	typedef std::list<XTBDicDBMemoryCacheEntry> EntryList;
	
	const XTBDicDBReader *m_db;
	EntryList m_entries;
	size_t m_entryCountLimit;
	size_t m_sizeLimit;
	size_t m_size;
	size_t m_entryCount;
	size_t m_minCount;
	
	EntryList::iterator moveIteratorToFront(EntryList::iterator);
	EntryList::iterator iteratorForKey(const std::string&);
	EntryList::const_iterator iteratorForKey(const std::string&) const;
	
	EntryList::iterator oldestIterator();
	EntryList::const_iterator oldestIterator() const;
	
	void evict(EntryList::iterator);
	
	void push(const std::string& key,
				  const std::string& contents);
	
	void evictToLimit();
	
public:
	XTBDicDBMemoryCache(const XTBDicDBReader *db,
						size_t entryCountLimit,
						size_t sizeLimit,
						size_t minCount=1);
	~XTBDicDBMemoryCache();
	
	size_t entryCount() const{return m_entryCount;}
	size_t size() const{return m_size;}
	
	// WARNING: returned std::string reference is granted to be valid 
	// only until next contentsForKey call.
	const std::string& contentsForKey(const std::string& key);
	
	bool doesKeyExist(const std::string& key) const;
};

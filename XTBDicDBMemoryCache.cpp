//
//  XTBDicDBMemoryCache.cpp
//  XTBook
//
//  Created by Kawada Tomoaki on 7/9/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "XTBDicDBMemoryCache.h"
#include "XTBDicDBReader.h"
#include <assert.h>
#include "XTBException.h"
#include <tcw/twStrConv.h>

XTBDicDBMemoryCache::XTBDicDBMemoryCache
(const XTBDicDBReader *db,
 size_t entryCountLimit,
 size_t sizeLimit,
 size_t minCount):
m_db(db),
m_entryCountLimit(entryCountLimit),
m_sizeLimit(sizeLimit),
m_size(0),
m_entryCount(0),
m_minCount(minCount){}

XTBDicDBMemoryCache::~XTBDicDBMemoryCache(){
	
}

XTBDicDBMemoryCache::EntryList::iterator XTBDicDBMemoryCache::iteratorForKey(const std::string& key){
	for(EntryList::iterator it=m_entries.begin();
		it!=m_entries.end();
		it++){
		if(it->key==key)
			return it;
	}
	return m_entries.end();
}

XTBDicDBMemoryCache::EntryList::const_iterator XTBDicDBMemoryCache::iteratorForKey(const std::string& key) const{
	for(EntryList::const_iterator it=m_entries.begin();
		it!=m_entries.end();
		it++){
		if(it->key==key)
			return it;
	}
	return m_entries.end();
}

XTBDicDBMemoryCache::EntryList::iterator XTBDicDBMemoryCache::moveIteratorToFront(XTBDicDBMemoryCache::EntryList::iterator it){
	if(it==m_entries.begin())
		return it;
	
	// insert new entry
	m_entries.insert(m_entries.begin(), XTBDicDBMemoryCacheEntry());
	
	XTBDicDBMemoryCacheEntry& entry1=*(m_entries.begin());
	XTBDicDBMemoryCacheEntry& entry2=*it;
	
	// swap them.
	entry1.key.swap(entry2.key);
	entry1.contents.swap(entry2.contents);
	
	// dispose older one.
	m_entries.erase(it);
	
	return m_entries.begin();
}

void XTBDicDBMemoryCache::push(const std::string &key, const std::string &contents){
	// insert new entry
	m_entries.insert(m_entries.begin(), XTBDicDBMemoryCacheEntry());
	
	// write entry.
	XTBDicDBMemoryCacheEntry& entry=*(m_entries.begin());
	entry.key=key;
	entry.contents=contents;
	
	// compile statistics
	m_size+=contents.size();
	m_entryCount++;
}

void XTBDicDBMemoryCache::evict(EntryList::iterator it){
	m_size-=it->contents.size();
	m_entryCount--;
	
	m_entries.erase(it);
}

void XTBDicDBMemoryCache::evictToLimit(){
	while(entryCount()>m_minCount){
		if(entryCount()<m_entryCountLimit || m_entryCountLimit==0){
			if(size()<m_sizeLimit ||
			   m_sizeLimit==0){
				break;
			}
		}
		
		evict(oldestIterator());
	}
}

const std::string& XTBDicDBMemoryCache::contentsForKey(const std::string& key){
	// is key cached?
	{
		EntryList::iterator it;
		it=iteratorForKey(key);
		if(it!=m_entries.end()){
			// cached.
			it=moveIteratorToFront(it);
			assert(it->key==key);
			return it->contents;
		}
	}
	
	// read from database.
	std::string contents;
	XTBDicDBPointer ptr=m_db->pointerForKey(key);
	if(ptr==XTBDicDBInvalidPointer){
		XTBThrowException(L"XTBKeyNotFoundException",twM2W(key).c_str(), NULL);
	}
	
	contents=m_db->contentsAtPointer(ptr);
	
	// cache it.
	push(key, contents);
	
	// return the contents of the cache, not variable above.
	XTBDicDBMemoryCacheEntry& entry=*(m_entries.begin());
	assert(entry.key==key);
	
	return entry.contents;
}

bool XTBDicDBMemoryCache::doesKeyExist(const std::string& key) const{
	if(key[0]=='<') return false;
	if(iteratorForKey(key)!=m_entries.end())
		return true;
	
	XTBDicDBPointer ptr=m_db->pointerForKey(key);
	return ptr!=XTBDicDBInvalidPointer;
}

XTBDicDBMemoryCache::EntryList::iterator XTBDicDBMemoryCache::oldestIterator(){
	EntryList::iterator it=m_entries.end();
	it--; return it;
}

XTBDicDBMemoryCache::EntryList::const_iterator XTBDicDBMemoryCache::oldestIterator() const{
	EntryList::const_iterator it=m_entries.end();
	it--; return it;
}


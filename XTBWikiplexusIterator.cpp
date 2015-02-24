//
//  XTBWikiplexusIterator.cpp
//  XTBook
//
//  Created by Kawada Tomoaki on 7/29/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "XTBWikiplexusIterator.h"
#include "XTBWikiplexusDictionary.h"
#include "XTBIndexDBReader.h"
#include <tcw/twStrConv.h>

XTBWikiplexusIterator::XTBWikiplexusIterator(const XTBWikiplexusDictionary *dictionary,
											 XTBIndexDBReader *index,
											 unsigned int lowerBound,
											 unsigned int upperBound):
m_lowerBound(lowerBound),
m_upperBound(upperBound),
m_dictionary(dictionary),
m_isCurrentResultValid(false),
m_pos(m_lowerBound),
m_index(index){}

XTBWikiplexusIterator::~XTBWikiplexusIterator(){
	
}

int XTBWikiplexusIterator::resultCount() const{
	return m_upperBound-m_lowerBound;
}

XTBDictionaryResult XTBWikiplexusIterator::currentResult() const{
	if(!m_isCurrentResultValid){
		XTBIndexDBEntry entry=m_index->entryAtIndex(m_pos);
		m_currentResult.key=twM2W(entry.key);
		m_currentResult.title=twM2W(entry.title);
		m_currentResult.dictionary=m_dictionary;
		m_isCurrentResultValid=true;
	}
	return m_currentResult;
}

void XTBWikiplexusIterator::next(){
	m_pos++;
	m_isCurrentResultValid=false;
}

bool XTBWikiplexusIterator::isEOF(){
	return m_pos>=m_upperBound;
}

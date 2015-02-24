//
//  XTBWikiplexusIterator.h
//  XTBook
//
//  Created by Kawada Tomoaki on 7/29/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#pragma once

#include "XTBDictionaryIterator.h"

class XTBWikiplexusDictionary;
class XTBIndexDBReader;

class XTBWikiplexusIterator: public XTBDictionaryIterator{
	unsigned int m_lowerBound;
	unsigned int m_upperBound;
	const XTBWikiplexusDictionary *m_dictionary;
	XTBIndexDBReader *m_index;
	
	unsigned int m_pos;
	mutable XTBDictionaryResult m_currentResult;
	mutable bool m_isCurrentResultValid;
public:	
	XTBWikiplexusIterator(const XTBWikiplexusDictionary *dictionary,
						  XTBIndexDBReader *index,
						  unsigned int lowerBound,
						  unsigned int upperBound);
	virtual ~XTBWikiplexusIterator();
	
	virtual int resultCount() const;
	virtual XTBDictionaryResult currentResult() const;
	virtual void next();
	virtual bool isEOF();
};

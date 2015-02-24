//
//  XTBImageComplexIterator.h
//  XTBook
//
//  Created by Kawada Tomoaki on 8/1/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#pragma once

#include "XTBDictionaryIterator.h"

class XTBImageComplexDictionary;
class XTBIndexDBReader;

class XTBImageComplexIterator: public XTBDictionaryIterator{
	unsigned int m_lowerBound;
	unsigned int m_upperBound;
	const XTBImageComplexDictionary *m_dictionary;
	XTBIndexDBReader *m_index;
	
	unsigned int m_pos;
	mutable XTBDictionaryResult m_currentResult;
	mutable bool m_isCurrentResultValid;
public:	
	XTBImageComplexIterator(const XTBImageComplexDictionary *dictionary,
						  XTBIndexDBReader *index,
						  unsigned int lowerBound,
						  unsigned int upperBound);
	virtual ~XTBImageComplexIterator();
	
	virtual int resultCount() const;
	virtual XTBDictionaryResult currentResult() const;
	virtual void next();
	virtual bool isEOF();
};


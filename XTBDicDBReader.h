//
//  XTBDicDBReader.h
//  XTBook
//
//  Created by Kawada Tomoaki on 7/6/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#pragma once

#include "XTBStream.h"
#include "platform.h"
#include <tcw/twSemaphore.h>


typedef uint64_t XTBDicDBPointer;

static const XTBDicDBPointer XTBDicDBInvalidPointer=0xffffffffffffffffULL;

struct XTBDicDBReaderKey{
	std::string key;
	XTBDicDBPointer pointer;
	
	bool isInvalid() const{
		return pointer==XTBDicDBInvalidPointer;
	}
};

class XTBDicDBReader{
	XTBSysString m_path;
	XTBStream *m_contentsStream;
	XTBStream *m_keysStream;
	XTBStream *m_keyIndexStream;
	twSemaphore *m_contentsMutex;
	twSemaphore *m_keysMutex;
	twSemaphore *m_keyIndexMutex;
	unsigned int m_keyCount;
	
	XTBSysString pathForContents() const;
	XTBSysString pathForKeys() const;
	XTBSysString pathForKeyIndex() const;
	
	
public:
	
	XTBDicDBReader(const XTBSysString& path);
	virtual ~XTBDicDBReader();
	
	std::string contentsAtPointer(XTBDicDBPointer, 
								  uint32_t maxLength=(uint32_t)-1) const;
	XTBDicDBReaderKey keyAtIndex(unsigned int) const;
	XTBDicDBPointer pointerForKey(const std::string&) const;
	
	unsigned int count() const{return m_keyCount;}
};

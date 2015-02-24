//
//  XTBIndexDBReader.h
//  XTBook
//
//  Created by Kawada Tomoaki on 7/28/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#pragma once

#include "XTBStream.h"
#include "platform.h"
#include <tcw/twSemaphore.h>

struct XTBIndexDBEntry{
	std::string key;
	std::string title;
};

class XTBIndexDBReader{
	XTBStream *m_dbStream;
	XTBStream *m_mapStream;
	twSemaphore *m_dbSemaphore;
	twSemaphore *m_mapSemaphore;
	XTBSysString m_path;
	unsigned int m_entryCount;
	
	XTBSysString pathForDb();
	XTBSysString pathForMap();
	
	XTBOffset offsetForIndex(unsigned int);
	
public:
	XTBIndexDBReader(const XTBSysString& path);
	virtual ~XTBIndexDBReader();
	
	XTBIndexDBEntry entryAtIndex(unsigned int);
	void readKeyAtIndex(unsigned int,
						char *buffer,
						size_t maxLength); // FAST!
	
	unsigned int lowerBoundForKey(const std::string&);
	unsigned int upperBoundForKey(const std::string&);
};

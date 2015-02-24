//
//  XTBStream.h
//  XTBook
//
//  Created by Kawada Tomoaki on 5/28/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#pragma once

#include <stdio.h>
#include <tcw/twObject.h>
#include <string>
#include "platform.h"

typedef unsigned long long XTBOffset;

enum{
	XTBStreamBufferLength=4096
};

class XTBStream: public twObject{
public:
	XTBStream(){}
	virtual ~XTBStream(){}
	
	virtual size_t readToBuffer(void *, size_t maxLength);
	std::string readToString(size_t maxLength);
	std::string readToString();
	std::string readToStringUntilNullChar(size_t maxLength);
	std::string readToStringUntilNullChar();
	uint32_t readSysInt32();
	uint16_t readSysInt16();
	uint32_t readLEInt32(){uint32_t v=readSysInt32(); return XTBSwapLE32(v);}
	uint16_t readLEInt16(){uint16_t v=readSysInt16(); return XTBSwapLE16(v);}
	uint32_t readBEInt32(){uint32_t v=readSysInt32(); return XTBSwapBE32(v);}
	uint16_t readBEInt16(){uint16_t v=readSysInt16(); return XTBSwapBE16(v);}
	
	virtual size_t writeBuffer(const void *, size_t maxLength);
	void writeString(const std::string&);
	void writeSysInt32(uint32_t);
	void writeSysInt16(uint16_t);
	void writeLEInt32(uint32_t v){writeSysInt32(XTBSwapLE32(v));}
	void writeLEInt16(uint16_t v){writeSysInt16(XTBSwapLE16(v));}
	void writeBEInt32(uint32_t v){writeSysInt32(XTBSwapBE32(v));}
	void writeBEInt16(uint16_t v){writeSysInt16(XTBSwapBE16(v));}
	
	
	virtual void seekToOffset(XTBOffset);
	virtual void seekToEndOfStream();
	
	virtual XTBOffset position() const;
	
	virtual bool isReadable() const;
	virtual bool isWritable() const;
	virtual bool isSeekable() const;
	
	virtual void synchronize(){}
};

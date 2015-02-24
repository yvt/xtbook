//
//  XTBMemoryStream.h
//  XTBook
//
//  Created by Kawada Tomoaki on 7/29/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#pragma once

#include "XTBStream.h"

class XTBMemoryStream: public XTBStream{
	char *m_data;
	bool m_isWritable;
	bool m_shouldFree;
	XTBOffset m_pos;
	XTBOffset m_size;
	
	void resize(XTBOffset);
public:
	/** initializes a memory stream with no bytes, that is
	 * writable, and expandable. */
	XTBMemoryStream();
	
	/** initializes a memory stream with the given memory,
	 * but it is not expandable. */
	XTBMemoryStream(char *data, size_t size,
					bool writable);
	
	/** initializes a memory stream with the given 
	 * read-only memory. */
	XTBMemoryStream(const char *data, size_t size);
	
	virtual ~XTBMemoryStream();
	
	virtual size_t readToBuffer(void *, size_t maxLength);
	virtual size_t writeBuffer(const void *, size_t maxLength);
	
	virtual void seekToOffset(XTBOffset);
	virtual void seekToEndOfStream();
	
	virtual XTBOffset position() const;
	
	virtual bool isReadable() const{return true;}
	virtual bool isWritable() const{return m_isWritable;}
	virtual bool isSeekable() const{return true;}
	
	virtual void synchronize();
	
	// unique memory can be expanded.
	bool isExpandable() const{return m_shouldFree;}
	
	char *data() const{return m_data;}
	
	XTBOffset size() const{return m_size;}
	
};

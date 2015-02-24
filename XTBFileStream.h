//
//  XTBFileStream.h
//  XTBook
//
//  Created by Kawada Tomoaki on 5/28/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#pragma once

#include <stdio.h>
#include "XTBStream.h"
#include "platform.h"

#ifndef XTB_USE_SYSIO
#if EV_PLATFORM_WIN32
#define XTB_USE_SYSIO	1
#define XTB_USE_WIN32IO	1
#else
#define XTB_USE_SYSIO	0
#endif
#endif

class XTBFileStream: public XTBStream{	
#if XTB_USE_SYSIO
	int m_handle;
#else
	FILE *m_stdStream;
#endif
	bool m_isReadable;
	bool m_isWritable;
	bool m_isSeekable;
	bool m_shouldCloseStream;
	XTBOffset m_position;
public:
#if XTB_USE_SYSIO
	XTBFileStream(int, bool shouldCloseStream,
				  bool isReadable,
				  bool isWritable,
				  bool isSeekable);
#else
	XTBFileStream(FILE *, bool shouldCloseStream,
				  bool isReadable,
				  bool isWritable,
				  bool isSeekable);
#endif
	virtual ~XTBFileStream();
	
	static XTBStream *streamForReadingAtPath(const XTBSysString&, bool detectArchive);
	static XTBFileStream *streamForReadingAtPath(const XTBSysString&);
	static XTBFileStream *streamForWritingAtPath(const XTBSysString&);
	static XTBFileStream *streamForUpdatingAtPath(const XTBSysString&);
	static XTBFileStream *streamWithStandardError();
	static XTBFileStream *streamWithStandardInput();
	static XTBFileStream *streamWithStandardOutput();
	
	virtual bool shouldCloseStream() const{return m_shouldCloseStream;}
	
	virtual size_t readToBuffer(void *, size_t maxLength);
	
	virtual size_t writeBuffer(const void *, size_t maxLength);
	
	virtual void seekToOffset(XTBOffset);
	virtual void seekToEndOfStream();
	
	virtual XTBOffset position() const;
	
	virtual bool isReadable() const{return m_isReadable;}
	virtual bool isWritable() const{return m_isWritable;}
	virtual bool isSeekable() const{return m_isSeekable;}
	
	virtual void synchronize();
};

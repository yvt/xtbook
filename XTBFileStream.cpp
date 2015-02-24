//
//  XTBFileStream.cpp
//  XTBook
//
//  Created by Kawada Tomoaki on 5/28/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#define _LARGEFILE_SOURCE
#define _LARGEFILE64_SOURCE
#include "XTBFileStream.h"
#include "XTBException.h"
#include <stdio.h>
#include <tcw/twStrConv.h>

#include <assert.h>
#include "XTBRaxDecompressor.h"

#if EV_PLATFORM_WIN32
#include <windows.h>
static HANDLE getFileHandle(int h){
	return (HANDLE)h;
}
static XTBOffset XTBSeek (int fildes, XTBOffset offset, int whence)
{
	DWORD mode;
	switch (whence)
    {
		case SEEK_SET:
			mode = FILE_BEGIN;
			break;
		case SEEK_CUR:
			mode = FILE_CURRENT;
			break;
		case SEEK_END:
			mode = FILE_END;
			break;
		default:
			assert(false);
			
			/* Specify an invalid mode so SetFilePointer catches it.  */
			mode = (DWORD)-1;
    }
	
	DWORD lowOffset=offset&0xffffffff;
	LONG highOffset=(offset>>32)&0xffffffff;
	
	DWORD ret=SetFilePointer ((HANDLE) fildes, lowOffset, &highOffset, mode);
	DWORD err = GetLastError();
	if(err){
		XTBLog("SetFilePointer returned 0x%08x", (int)err);
	}
	
	XTBOffset returnOffset=(DWORD)ret;
	returnOffset&=0xffffffff;
	returnOffset|=(XTBOffset)(highOffset)<<32;
	return returnOffset;
}
#endif

#pragma mark - Opening Stream

#if XTB_USE_SYSIO

#include <fcntl.h>

XTBStream *XTBFileStream::streamForReadingAtPath(const XTBSysString &path, bool detectArchive){
#if EV_PLATFORM_MACOSX || EV_PLATFORM_GENERICPOSIX
	int f=open(path.c_str(), O_RDONLY);
#elif EV_PLATFORM_WIN32
#if XTB_USE_WIN32IO
	int f = (int)CreateFile(path.c_str(), GENERIC_READ, FILE_SHARE_READ,
						  NULL, OPEN_EXISTING, 0, NULL);
	if(f == (int)INVALID_HANDLE_VALUE)f = -1;
#else
	int f=_wopen(path.c_str(), O_RDONLY);
#endif
#endif
	
	if(f!=-1)
		return new XTBFileStream(f, true,
								 true, false, true);
	else if(detectArchive){
		XTBSysString archivedPath;
		archivedPath=path+XTBSysText(".rax");
		if(XTBDoesPathExist(archivedPath)){
			return new XTBRaxDecompressor(streamForReadingAtPath(archivedPath), true);
		}else{
			XTBThrowException(L"XTBFileIOErrorException", XTBWStringFromSysString(path).c_str(), NULL);
		}
	}else
		XTBThrowException(L"XTBFileIOErrorException", XTBWStringFromSysString(path).c_str(), NULL);
}

XTBFileStream *XTBFileStream::streamForReadingAtPath(const XTBSysString &path){
#if EV_PLATFORM_MACOSX || EV_PLATFORM_GENERICPOSIX
	int f=open(path.c_str(), O_RDONLY);
#elif EV_PLATFORM_WIN32
#if XTB_USE_WIN32IO
	int f = (int)CreateFile(path.c_str(), GENERIC_READ, FILE_SHARE_READ,
							NULL, OPEN_EXISTING, 0, NULL);
	if(f == (int)INVALID_HANDLE_VALUE)f = -1;
#else
	int f=_wopen(path.c_str(), O_RDONLY);
#endif
#endif
	if(f!=-1)
		return new XTBFileStream(f, true,
								 true, false, true);
	else
		XTBThrowException(L"XTBFileIOErrorException", XTBWStringFromSysString(path).c_str(), NULL);
}

XTBFileStream *XTBFileStream::streamForWritingAtPath(const XTBSysString &path){
#if EV_PLATFORM_MACOSX || EV_PLATFORM_GENERICPOSIX
	int f=open(path.c_str(), O_WRONLY|O_CREAT|O_TRUNC);
#elif EV_PLATFORM_WIN32
#if XTB_USE_WIN32IO
	int f = (int)CreateFile(path.c_str(), GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ,
							NULL, TRUNCATE_EXISTING, 0, NULL);
	if(f == (int)INVALID_HANDLE_VALUE)f = -1;
	if(f == -1){
		f = (int)CreateFile(path.c_str(), GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ,
							NULL, CREATE_ALWAYS, 0, NULL);
	}
#else
	int f=_wopen(path.c_str(), O_RDWR|O_CREAT|O_TRUNC);
#endif
#endif
	if(f)
		return new XTBFileStream(f, true,
								 false, true, true);
	else
		XTBThrowException(L"XTBFileIOErrorException", XTBWStringFromSysString(path).c_str(), NULL);
}

XTBFileStream *XTBFileStream::streamForUpdatingAtPath(const XTBSysString &path){
#if EV_PLATFORM_MACOSX || EV_PLATFORM_GENERICPOSIX
	int f=open(path.c_str(), O_RDWR|O_CREAT);
#elif EV_PLATFORM_WIN32
#if XTB_USE_WIN32IO
	int f = (int)CreateFile(path.c_str(), GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ,
							NULL, OPEN_ALWAYS, 0, NULL);
	if(f == (int)INVALID_HANDLE_VALUE)f = -1;
#else
	int f=_wopen(path.c_str(), O_RDWR|O_CREAT);
#endif
#endif
	if(f)
		return new XTBFileStream(f, true,
								 true, true, true);
	else
		XTBThrowException(L"XTBFileIOErrorException", XTBWStringFromSysString(path).c_str(), NULL);
}

#if XTB_USE_WIN32IO

XTBFileStream *XTBFileStream::streamWithStandardError(){
	XTBThrowException(L"XTBNotImplementedException", NULL);
	//return new XTBFileStream((int)GetStdHandle(STD_ERROR_HANDLE), false, false, true, false);
}

XTBFileStream *XTBFileStream::streamWithStandardInput(){
	XTBThrowException(L"XTBNotImplementedException", NULL);
	//return new XTBFileStream((int)GetStdHandle(STD_INPUT_HANDLE), false, true, false, false);
}

XTBFileStream *XTBFileStream::streamWithStandardOutput(){
	XTBThrowException(L"XTBNotImplementedException", NULL);
	//return new XTBFileStream((int)GetStdHandle(STD_OUTPUT_HANDLE), false, false, true, false);
}
#else
XTBFileStream *XTBFileStream::streamWithStandardError(){
	return new XTBFileStream(fileno(stderr), false, false, true, false);
}

XTBFileStream *XTBFileStream::streamWithStandardInput(){
	return new XTBFileStream(fileno(stdin), false, true, false, false);
}

XTBFileStream *XTBFileStream::streamWithStandardOutput(){
	return new XTBFileStream(fileno(stdout), false, false, true, false);
}
#endif

#else

XTBStream *XTBFileStream::streamForReadingAtPath(const XTBSysString &path, bool detectArchive){
	FILE *f=XTBOpen(path, "rb");
	
	if(f)
		return new XTBFileStream(f, true,
								 true, false, true);
	else if(detectArchive){
		XTBSysString archivedPath;
		archivedPath=path+XTBSysText(".rax");
		//if(XTBDoesPathExist(archivedPath)){
			return new XTBRaxDecompressor(streamForReadingAtPath(archivedPath), true);
		//}else{
		//	XTBThrowException(L"XTBFileIOErrorException", XTBWStringFromSysString(archivedPath).c_str(), NULL);
		//}
	}else
		XTBThrowException(L"XTBFileIOErrorException", XTBWStringFromSysString(path).c_str(), NULL);
}

XTBFileStream *XTBFileStream::streamForReadingAtPath(const XTBSysString &path){
	FILE *f=XTBOpen(path, "rb");
	if(f)
		return new XTBFileStream(f, true,
								 true, false, true);
	else
		XTBThrowException(L"XTBFileIOErrorException", XTBWStringFromSysString(path).c_str(), NULL);
}

XTBFileStream *XTBFileStream::streamForWritingAtPath(const XTBSysString &path){
	FILE *f=XTBOpen(path, "wb");
	if(f)
		return new XTBFileStream(f, true,
								 false, true, true);
	else
		XTBThrowException(L"XTBFileIOErrorException", XTBWStringFromSysString(path).c_str(), NULL);
}

XTBFileStream *XTBFileStream::streamForUpdatingAtPath(const XTBSysString &path){
	FILE *f=XTBOpen(path, "rb+");
	if(f)
		return new XTBFileStream(f, true,
								 true, true, true);
	else
		XTBThrowException(L"XTBFileIOErrorException", XTBWStringFromSysString(path).c_str(), NULL);
}

XTBFileStream *XTBFileStream::streamWithStandardError(){
	return new XTBFileStream(stderr, false, false, true, false);
}

XTBFileStream *XTBFileStream::streamWithStandardInput(){
	return new XTBFileStream(stdin, false, true, false, false);
}

XTBFileStream *XTBFileStream::streamWithStandardOutput(){
	return new XTBFileStream(stdout, false, false, true, false);
}

#endif



#pragma mark - Stream Lifetime

#if XTB_USE_SYSIO
XTBFileStream::XTBFileStream(int stdStream,
							 bool shouldCloseStream,
							 bool isReadable,
							 bool isWritable,
							 bool isSeekable):
m_handle(stdStream),
m_isReadable(isReadable),
m_isWritable(isWritable),
m_isSeekable(isSeekable),
m_shouldCloseStream(shouldCloseStream){
	if(!stdStream){
		XTBThrowException(L"XTBArgumentNullException", L"stdStream", NULL);
	}
#if EV_PLATFORM_MACOSX || EV_PLATFORM_GENERICPOSIX
	m_position=(XTBOffset)lseek(m_handle, 0, SEEK_CUR);
#elif EV_PLATFORM_WIN32
	m_position=XTBSeek(m_handle, 0, SEEK_CUR);
#endif
}
#else
XTBFileStream::XTBFileStream(FILE *stdStream,
							 bool shouldCloseStream,
							 bool isReadable,
							 bool isWritable,
							 bool isSeekable):
m_stdStream(stdStream),
m_isReadable(isReadable),
m_isWritable(isWritable),
m_isSeekable(isSeekable),
m_shouldCloseStream(shouldCloseStream){
	if(!stdStream){
		XTBThrowException(L"XTBArgumentNullException", L"stdStream", NULL);
	}
#if EV_PLATFORM_MACOSX || EV_PLATFORM_GENERICPOSIX
	m_position=(XTBOffset)ftello(m_stdStream);;
#elif EV_PLATFORM_WIN32
	m_position=(XTBOffset)ftell(m_stdStream);;
#endif
}
#endif

XTBFileStream::~XTBFileStream(){
	if(m_shouldCloseStream)
#if XTB_USE_SYSIO
#if XTB_USE_WIN32IO
		CloseHandle((HANDLE)m_handle);
#else
		close(m_handle);
#endif
#else
		fclose(m_stdStream);
#endif
}

size_t XTBFileStream::readToBuffer(void *buf, size_t maxLength){
	if(!isReadable())
		XTBThrowException(L"XTBStreamNotSupportedException", NULL);
#if XTB_USE_SYSIO
#if XTB_USE_WIN32IO
	DWORD size;
	if(!ReadFile((HANDLE)m_handle, buf, maxLength, &size, NULL))
		size=0;
#else
	size_t size=read(m_handle, buf, maxLength);
#endif
#else
	size_t size=fread(buf, 1, maxLength, m_stdStream);
#endif
	m_position+=size;
	return size;
}

size_t XTBFileStream::writeBuffer(const void *buf, size_t maxLength){
	if(!isWritable())
		XTBThrowException(L"XTBStreamNotSupportedException", NULL);
#if XTB_USE_SYSIO
#if XTB_USE_WIN32IO
	DWORD size;
	if(!WriteFile((HANDLE)m_handle, buf, maxLength, &size, NULL))
		size=0;
#else
	size_t size=write(m_handle, buf, maxLength);
#endif
#else
	size_t size=fwrite(buf, 1, maxLength, m_stdStream);
#endif
	m_position+=size;
	return size;
}

void XTBFileStream::seekToEndOfStream(){
	if(!isSeekable())
		XTBThrowException(L"XTBStreamNotSupportedException", NULL);
#if XTB_USE_SYSIO
#if XTB_USE_WIN32IO
	XTBSeek(m_handle, 0, SEEK_END);
#else
	lseek(m_handle, 0, SEEK_END);
#endif
#if EV_PLATFORM_MACOSX || EV_PLATFORM_GENERICPOSIX
	m_position=(XTBOffset)lseek(m_handle, 0, SEEK_CUR);
#elif EV_PLATFORM_WIN32
	m_position=XTBSeek(m_handle, 0, SEEK_CUR);
#endif
#else
	fseek(m_stdStream, 0, SEEK_END);
#if EV_PLATFORM_MACOSX
	m_position=(XTBOffset)ftello(m_stdStream);;
#elif EV_PLATFORM_GENERICPOSIX
	m_position=(XTBOffset)ftello64(m_stdStream);
#elif EV_PLATFORM_WIN32
	m_position=(XTBOffset)ftell(m_stdStream);;
#endif
#endif
}


void XTBFileStream::seekToOffset(XTBOffset off){
	if(!isSeekable())
		XTBThrowException(L"XTBStreamNotSupportedException", NULL);
#if XTB_USE_SYSIO
#if EV_PLATFORM_MACOSX || EV_PLATFORM_GENERICPOSIX
	lseek(m_handle, (off_t)off, SEEK_SET);
#elif EV_PLATFORM_WIN32
	XTBSeek(m_handle, off, SEEK_SET);
#endif
#else
#if EV_PLATFORM_MACOSX
	fseeko(m_stdStream, (off_t)off, SEEK_SET);
#elif EV_PLATFORM_GENERICPOSIX
	fseeko64(m_stdStream, (__off64_t)off, SEEK_SET);
#elif EV_PLATFORM_WIN32
	fseek(m_stdStream, (long)off, SEEK_SET);
#endif
#endif
	m_position=off;
}



XTBOffset XTBFileStream::position() const{
	return m_position;
}



void XTBFileStream::synchronize(){
#if XTB_USE_SYSIO
#else
	fflush(m_stdStream);
#endif
}



/*
 *  platform.h
 *  XTBook
 *
 *  Created by Nexhawks on 12/22/10.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#pragma once

#include <stdio.h>
#include "global.h"
#include <vector>
#include <tcw/twApp.h>

/** Possible values of XTBCurrentPlatform. */
enum{
	XTBPlatformMacOSX=0,
	XTBPlatformWin32=1,
	XTBPlatformGenericPosix=2
};


/* Mac OS X (Darwin) */
#ifdef EV_PLATFORM_MACOSX
/** Operating-system-dependent STL string type. */
typedef std::string XTBSysString;

/** Operating-system-dependent char type. */
typedef char XTBSysChar;

enum{
	XTBCurrentPlatform=XTBPlatformMacOSX
};

/** Creates operating-system-dependent string literal. */
#define XTBSysText(s)		(s)

/** The path separator for the target operating system. */
#define XTBPathSeparator	"/"

#endif


/* Windows / Windows CE */
#ifdef EV_PLATFORM_WIN32

#include <windows.h>
/** Operating-system-dependent STL string type. */
typedef std::wstring XTBSysString;

/** Operating-system-dependent char type. */
typedef WCHAR XTBSysChar;
enum{
	XTBCurrentPlatform=XTBPlatformWin32
};

/** Creates operating-system-dependent string literal. */
#define XTBSysText(s)		(L ## s)

/** The path separator for the target operating system. */
#define XTBPathSeparator	L"\\"

#endif


/* Generic Posix Operating System */
#ifdef EV_PLATFORM_GENERICPOSIX

/** Operating-system-dependent STL string type. */
typedef std::string XTBSysString;

/** Operating-system-dependent char type. */
typedef char XTBSysChar;
enum{
	XTBCurrentPlatform=XTBPlatformGenericPosix
};

/** Creates operating-system-dependent string literal. */
#define XTBSysText(s)		(s)

/** The path separator for the target operating system. */
#define XTBPathSeparator	"/"

#endif

/** Endianness */

#define XTBBigEndian		0x41424344
#define XTBLittleEndian		0x44434241

#if SDL_BYTEORDER==SDL_LIL_ENDIAN
#define XTBEndian XTBLittleEndian
#elif SDL_BYTEORDER==SDL_BIG_ENDIAN
#define XTBEndian XTBBigEndian
#else
#error Endian Unknown.
#endif

#define XTBSwapLE16(v)	SDL_SwapLE16(v)
#define XTBSwapLE32(v)	SDL_SwapLE32(v)
#define XTBSwapBE16(v)	SDL_SwapBE16(v)
#define XTBSwapBE32(v)	SDL_SwapBE32(v)



/** File name for the informational property list of the Mac OS X bundle. */
#define XTBInfoPListName	XTBSysText("Info.plist")

/** Displays a message box or a console message, terminating the application. */
__attribute__((noreturn))
void XTBErrorExit(const char *, ...);

/** run startup items. */
void XTBRunStartupItems();

/** Displays a opening file chooser of the target operating system. */
XTBSysString XTBChooseOpenFile(const std::wstring& typeName, const std::wstring& ext);

/** Opens the file whose path is specified with XTBSysString, like fopen.
 * @see XTBSysString 
 * @param path Path of the file to open.
 * @param mode Equivalent to fopen's mode.
 * @return Stdio file handle. */
FILE *XTBOpen(const XTBSysString& path, const char *mode);

/** Read the all contents of the specified file in binary mode. 
 * @return Contents of the specified file. */
std::string XTBReadAllContents(XTBSysString);

/** Checks whether the specified path exists.
 * @return true if the specified path exists. Otherwise false. */
bool XTBDoesPathExist(const XTBSysString&);

/** Used by XTBGetResourceDirectory. */
enum XTBResourceDirectory{
	XTBResourceDirectoryLanguages=0,
	XTBResourceDirectoryDictionaries,
	XTBResourceDirectoryInternalDictionaries,
	XTBResourceDirectoryStartupItems
};

/** Retrives the resource directory that is appropriate to the target operating system and
 * the application execuable's path.
 * @return Resource directory. */
XTBSysString XTBGetAppDir();

/** Retrives the path for the application preference. 
 * @return Path for the application preference. */
XTBSysString XTBGetStandardUserDefaultsPath();

/** Retrives the path for the history. 
 * @return Path for the history. */
XTBSysString XTBGetStandardHistoryPath();

/** Retrives the standard resource directory.
 * @return Path for the standard resource directory. */
XTBSysString XTBGetResourceDirectory(XTBResourceDirectory);

/** Scan the specified path, and returns the all contents. 
 * @return Full-pathes of contents of the specified path. */
std::vector<XTBSysString> XTBScanDirectory(const XTBSysString&);
static inline std::vector<XTBSysString> XTBScanResourceDirectory(XTBResourceDirectory d){
	return XTBScanDirectory(XTBGetResourceDirectory(d));
}

/** Convert std::wstring to XTBSysString. 
 * @see XTBSysString 
 * @see XTBWStringFromSysString */
XTBSysString XTBSysStringFromWString(const std::wstring&);

/** Convert XTBSysString to std::wstring.
 * @see XTBSysString
 * @see XTBSysStringFromWString */
std::wstring XTBWStringFromSysString(const XTBSysString&);

void _XTBLog(const char *functionName, const char *file, int lineNumber, const char *, ...);

/** Outputs a log to the standard output or the log file if provided.
 * The names of the calling function, line number, and file name that includes the caller are added.
 * Do not use %ls(const wchar_t *) as the argument because wcstombs sometimes doesn't convert
 * wchar_t into UTF-8 correctly. */
#define XTBLog(format, ...) _XTBLog(__FUNCTION__, __FILE__, __LINE__, format, ##__VA_ARGS__ )

/** dumps the binary data to stdout. */
void XTBDumpBytes(const std::string&);

/** dumps the wchar_t string to stdout. (for debugging-purpose) */
void XTBDumpWString(const std::wstring&);
void XTBDumpWPtr(const wchar_t *);

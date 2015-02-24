/*
 *  global.h
 *  XTBook
 *
 *  Created by Nexhawks on 12/22/10.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdarg.h>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include <list>
#include <stdint.h>
#include <exception>


using namespace std;
#include "SDL.h"

#if defined(__MACOSX__)
#define EV_PLATFORM_MACOSX			1
#elif defined(WIN32)
#define EV_PLATFORM_WIN32			1
#else
#define EV_PLATFORM_GENERICPOSIX	1
#endif

/** Formats "...{0}..." style text. NULL must be added to the end of the arguments. */
std::wstring XTBFormat(const std::wstring& str, ...);

/** Formats "...{0}..." style text using va_list. 
 * @see XTBFormat */
std::wstring XTBVarArgFormat(const std::wstring& format, va_list list);

/** Formats printf style text. It may cause the unexpected result to use non-ASCII characters. */
std::wstring XTBFormatStd(const std::wstring& str, ...);

/** Converts the string to the integer. */
int XTBIntWithString(const std::wstring&);

/** Converts the integer to the string. */
std::wstring XTBStringWithInt(int);

/** Short version. */
std::wstring XTBShortVersionInformation();

/** Full version information. */
std::wstring XTBLongVersionInformation();

#if EV_PLATFORM_WIN32
#include <excpt.h>
struct _EXCEPTION_RECORD;
struct _CONTEXT;
struct _DISPATCHER_CONTEXT;

/** Handles the Win32 exception by throwing XTBWin32Exception. */
int XTBHandleWin32Exception(struct _EXCEPTION_RECORD*, void *, struct _CONTEXT*, struct _DISPATCHER_CONTEXT*);
#endif

/** This is the virtual entry point. */
int Main(int argc, char *argv[])
#if EV_PLATFORM_WIN32
__attribute__((__exception_handler__(XTBHandleWin32Exception)));
#endif
;

#include "platform.h"
#include "XTBException.h"
#include "i18n.h"

#include <tcw/twWnd.h>
#include <tcw/twApp.h>
#include <tcw/twEvent.h>
#include <tcw/twInvocation.h>



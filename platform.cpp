/*
 *  platform.cpp
 *  XTBook
 *
 *  Created by Nexhawks on 12/22/10.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#include "global.h"
#include "platform.h"
#include <tcw/twStrConv.h>
#include <tcw/twException.h>
#include <tcw/twApp.h>
#include <time.h>
#include "XTBException.h"
#include <fcntl.h>
#include <sys/stat.h>

/** Calculates the min value. std::min is preferred. */
#define MIN(a, b) (((a)<(b))?(a):(b))

/** Calculated the max value. std::max is preferred. */
#define MAX(a, b) (((a)>(b))?(a):(b))

/** Standard name for the languages directory. */
#define XTBLanguagesDirName XTBSysText("Languages")

/** Standard name for the dictionaries directory. */
#define XTBDictionariesDirName XTBSysText("Dictionaries")

/** Standard name for the internal dictionaries directory. */
#define XTBInternalDictionariesDirName XTBSysText("InternalDictionaries")

/** Standard name for the startup items directory. */
#define XTBStartupItemsDirName XTBSysText("StartupItems")

/** Internal function to display the error message and terminate the application. */
static void _errorExit(const char * msg);

#pragma mark -
#pragma mark OS X Version
#ifdef EV_PLATFORM_MACOSX
#include "OSXSpecific.h"
#include <dirent.h>
__attribute__((noreturn))
static void _errorExit(const char * msg){
	// Show the Cocoa message box.
	showErrorAlert(msg);
	
	// exterminate.
	exit(1);
}


XTBSysString XTBChooseOpenFile(const std::wstring& typeName, const std::wstring& ext){
	char extName[256];
	char fnOut[4096];
	
	// convert the extension name to UTF-8.
	twW2M(extName, ext.c_str());
	
	// show the file chooser.
	showOpenPanel(extName, fnOut);

	// return the file name.
	return fnOut;
}

extern "C"
int SDL_main(int argc, char *argv[]){
	// create Application Support directory.
	mkdir((string(getenv("HOME"))+"/Library/Application Support/XTBook").c_str(), 0766);
	// call the entry point.
	return Main(argc, argv);
}

FILE *XTBOpen(const XTBSysString& str, const char *mode){
	return fopen(str.c_str(), mode);
}

XTBSysString XTBGetAppDir(){
	return appDirOSX();
}


std::vector<XTBSysString> XTBScanDirectory(const XTBSysString& path){
	// open the directory.
	DIR *dir=opendir(path.c_str());
	struct dirent *ent;
	
	std::vector<XTBSysString>  ret;
	string filePath;
	
	// if couldn't open the directory, return the empty vector.
	if(!dir)
		return ret;
	
	// read an entry.
	while((ent=readdir(dir))){
		if(ent->d_name[0]=='.')
			continue;
		
		// make it full-path.
		filePath=path;
		filePath+="/";
		filePath+=ent->d_name;
		
		// add to the result vector.
		ret.push_back(filePath);
	}
	
	// close the directory.
	closedir(dir);
	
	return ret;
}

XTBSysString XTBGetStandardUserDefaultsPath(){
	// Mac OS X's standard preference path.
	return string(getenv("HOME"))+"/Library/Preferences/com.nexhawks.XTBook.plist";
}

XTBSysString XTBGetStandardHistoryPath(){
	return string(getenv("HOME"))+"/Library/Application Support/XTBook/History";
}

XTBSysString XTBSysStringFromWString(const std::wstring& s){
	return twW2M(s);
}
std::wstring XTBWStringFromSysString(const XTBSysString& s){
	return twM2W(s);
}

static void XTBRunStartupItem(const XTBSysString& path){
	if(system(("\""+path+"\"").c_str())){
		XTBLog("Error in startup item \"%s\".",
			   (path).c_str());
	}
}

#endif

#pragma mark -
#pragma mark Win32 Version
#ifdef EV_PLATFORM_WIN32

#include <windows.h>
#include <shlwapi.h>
#include <commdlg.h>

__attribute__((noreturn))
static void _errorExit(const char *msg){
	std::wstring wmsg=twM2W(msg);
	
	// show the message box.
	MessageBoxW(GetActiveWindow(), wmsg.c_str(), L"XTBook", MB_ICONERROR);
	
	// EXTERMINATE!
	exit(1);
}

XTBSysString XTBChooseOpenFile(const std::wstring& typeName, const std::wstring& ext){
	OPENFILENAME dlg;
	wchar_t filter[256];
	wchar_t defExt[64];
	wchar_t fnOut[1024];
	
	// setup some properties.
	memset(&dlg, 0, sizeof(dlg));
	swprintf(filter, L"%ls (*.%ls)|*.%ls|All Files (*.*)|*.*", typeName.c_str(), ext.c_str(), ext.c_str());
	swprintf(defExt, L".%ls", ext.c_str());
	dlg.lStructSize=sizeof(OPENFILENAME);
	dlg.lpstrFilter=filter;
	dlg.nFilterIndex=1;
	dlg.lpstrFile=fnOut;
	dlg.nMaxFile=1024;
	dlg.Flags=OFN_FILEMUSTEXIST;
	dlg.lpstrDefExt=defExt;
	
	// show the file chooser.
	if(GetOpenFileName(&dlg)){
		// return the selected file.
		return fnOut;
	}else{
		// no file was selected.
		return L"";
	}
}

/** convert the win32 error code to string, and throw it as XTBWin32Exception. */
static void XTBThrowWin32Exception(DWORD code){
	wchar_t buf[256];
	// convert the win32 error code to string.
	FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM,
				   NULL, code,
				   NULL, buf, 256,
				   NULL);
	
	char buf2[256];
	// get the hexadecimal representation of the error code.
	sprintf(buf2, "0x%08x", (int)code);
	
	// throw the exception.
	XTBThrowException(L"XTBWin32Exception", twM2W(buf2).c_str(), buf);
}

int XTBHandleWin32Exception(struct _EXCEPTION_RECORD* ExceptionRecord, void *, struct _CONTEXT*, struct _DISPATCHER_CONTEXT*){
	DWORD code=ExceptionRecord->ExceptionCode;
	XTBThrowWin32Exception(code);
}

static LONG XTBUnhandledWin32ExceptionFilter(struct _EXCEPTION_POINTERS *ExceptionInfo){
	XTBThrowWin32Exception(ExceptionInfo->ExceptionRecord->ExceptionCode);
	return 0;
}

#ifdef _WIN32_WCE
typedef PVOID WINAPI (*AddVectoredExceptionHandlerFunc)(ULONG FirstHandler,
														LPTOP_LEVEL_EXCEPTION_FILTER VectoredHandler
														);

#endif

void twThreadStarted();

void twThreadStarted(){
#ifndef _WIN32_WCE
	SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)XTBUnhandledWin32ExceptionFilter);
#else
	
	AddVectoredExceptionHandlerFunc AddVectoredExceptionHandler;
	AddVectoredExceptionHandler=(AddVectoredExceptionHandlerFunc)GetProcAddress(LoadLibrary(L"COREDLL"), 
																				L"AddVectoredExceptionHandler");
	if(AddVectoredExceptionHandler){
		(*AddVectoredExceptionHandler)(0, XTBUnhandledWin32ExceptionFilter);
	}
#endif
}

extern "C"
int main(int argc, char *argv[]){
	// open the log file for XTBLog to output to.
	_wfreopen((XTBGetAppDir()+L"\\stdout.txt").c_str(), L"w", stdout);
	
	// register the exception handler.
#ifndef _WIN32_WCE
	SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)XTBUnhandledWin32ExceptionFilter);
#else
	
	// in Windows CE, SetUnhandledExceptionFilter is undefined.
	// so let's use AddVectoredExceptionHandler that exists in only Windows CE Embedded 6.
	AddVectoredExceptionHandlerFunc AddVectoredExceptionHandler;
	AddVectoredExceptionHandler=(AddVectoredExceptionHandlerFunc)GetProcAddress(LoadLibrary(L"COREDLL"), 
																				L"AddVectoredExceptionHandler");
	if(AddVectoredExceptionHandler){
		(*AddVectoredExceptionHandler)(0, XTBUnhandledWin32ExceptionFilter);
		XTBLog("vectored exception handler registered.");
	}
#endif
	return Main(argc, argv);
}

FILE *XTBOpen(const XTBSysString& str, const char *mode){
	return _wfopen(str.c_str(), twM2W(mode).c_str());
}


XTBSysString XTBGetAppDir(){
	wchar_t buf[512];
	// retrive the path of the application.
	GetModuleFileName(NULL, buf, 512);
	
	wstring str=buf;
	// find the last path separator.
	wstring::size_type pos=MIN(str.rfind(L"/"), str.rfind(L"\\"));

	if(pos==string::npos)
		// ... only to find the base name. no directory infomation available.
		return L"";
	else
		// retrive the directory name.
		return str.substr(0, pos);
}


std::vector<XTBSysString> XTBScanDirectory(const XTBSysString& path){
	WIN32_FIND_DATA fd;
	HANDLE h;
	std::vector<XTBSysString>  ret;
	wstring filePath;
	
	// open the Win32 find handle.
	h=FindFirstFileExW((path+L"\\*").c_str(),
					   FindExInfoStandard, &fd,
					   FindExSearchNameMatch,
					   NULL, 0);
	
	if(h==INVALID_HANDLE_VALUE){
		// couldn't open. return the empty vector.
		return ret;
	}
	
	do{
		// is it a directory?
		if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
			// "." and ".." mustn't be included.
			if(wcscmp(fd.cFileName, L".") && wcscmp(fd.cFileName, L"..")){
				filePath=path+L"\\";
				filePath+=fd.cFileName;
				ret.push_back(filePath);
			}
		}else{
			// usual file.
			filePath=path+L"\\";
			filePath+=fd.cFileName;
			ret.push_back(filePath);
		}
		
		// iterate!
	}while(FindNextFile(h, &fd));
	
	// close the handle.
	FindClose(h);
	
	return ret;
}

XTBSysString XTBGetStandardUserDefaultsPath(){
	return XTBGetAppDir()+L"\\Preferences.plist";
}

XTBSysString XTBGetStandardHistoryPath(){
	return XTBGetAppDir()+L"\\History";
}

XTBSysString XTBSysStringFromWString(const std::wstring& s){
	// no need convert; XTBSysString=std::wstring
	return s;
}
std::wstring XTBWStringFromSysString(const XTBSysString& s){
	// no need convert; XTBSysString=std::wstring
	return s;
}

static void XTBRunStartupItem(const XTBSysString& path){
	SHELLEXECUTEINFO info;
	if(path.size()<4 ||
	   (path.find(L".exe")!=path.size()-4
	   && path.find(L".EXE")!=path.size()-4
	   && path.find(L".Exe")!=path.size()-4))
		return;
	memset(&info, 0, sizeof(SHELLEXECUTEINFO));
	info.cbSize=sizeof(SHELLEXECUTEINFO);
	info.hwnd=GetActiveWindow();
	info.lpVerb=L"open";
	info.lpFile=path.c_str();
	info.nShow=SW_SHOW;
	info.fMask=SEE_MASK_NOCLOSEPROCESS;
	
	if(ShellExecuteEx(&info)){
		WaitForSingleObject(info.hProcess, INFINITE);
	}else{
		XTBLog("Failed to load startup item \"%s\".",
			   twW2M(path).c_str());
	}
}

#endif

#pragma mark -
#pragma mark Generic POSIX Version
#ifdef EV_PLATFORM_GENERICPOSIX

#include <dirent.h>

static std::string g_argv0;

__attribute__((noreturn))
static void _errorExit(const char *msg){
	// show the message in the console.
	fprintf(stderr, "error: %s\n", msg);
	
	// terminate.
	exit(1);
}

XTBSysString XTBChooseOpenFile(const std::wstring& typeName, const std::wstring& ext){
	// ask for the file-name in the console.
	wprintf(L"*DIALOG* Open File Request\n");
	wprintf(L"Type path for %ls (*.%ls):\n", typeName.c_str(), ext.c_str());
	char buf1[4096];
	
	// TODO: more safe way instead of gets.
	gets(buf1);
	
	// return the file name.
	return buf1;
}

extern "C"
int main(int argc, char *argv[]){
	// save the command-line for XTBGetAppDir.
	g_argv0=argv[0];
	return Main(argc, argv);
}

FILE *XTBOpen(const XTBSysString& str, const char *mode){
	return fopen(str.c_str(), mode);
}


XTBSysString XTBGetAppDir(){
	// this is much like `dirname`!!
	string::size_type pos=MIN(g_argv0.rfind("/"), g_argv0.rfind("\\"));
	if(pos==string::npos)
		// failed. fall back.
		return "";
	else
		// dirname could be extracted.
		return g_argv0.substr(0, pos);
}

std::vector<XTBSysString> XTBScanDirectory(const XTBSysString& path){
	// SEE OS X'S XTBScanDirectory.
	DIR *dir=opendir(path.c_str());
	struct dirent *ent;
	
	
	std::vector<XTBSysString>  ret;
	string filePath;
	
	if(!dir)
		return ret;
	
	while((ent=readdir(dir))){
		if(ent->d_name[0]=='.')
			continue;
		filePath=path;
		filePath+="/";
		filePath+=ent->d_name;
		ret.push_back(filePath);
	}
	
	closedir(dir);
	
	return ret;
}

XTBSysString XTBGetStandardUserDefaultsPath(){
	// preference file as a hidden file in the home directory.
	return string(getenv("HOME"))+"/.xtbook.plist";
}

XTBSysString XTBGetStandardHistoryPath(){
	return string(getenv("HOME"))+"/.xtbook_history";
}

XTBSysString XTBSysStringFromWString(const std::wstring& s){
	return twW2M(s);
}
std::wstring XTBWStringFromSysString(const XTBSysString& s){
	return twM2W(s);
}

static void XTBRunStartupItem(const XTBSysString& path){
	if(system(("\""+path+"\"").c_str())){
		XTBLog("Error in startup item \"%s\".",
			   (path).c_str());
	}
}

#endif

#pragma mark -
#pragma mark Common Decent

void XTBErrorExit(const char *format, ...){
	// format the string.
	va_list list;
	char buf[1024];
	va_start(list, format);
	vsprintf(buf, format, list);
	
	// log the reason.
	XTBLog("Fatal error: %s\n", buf);
	
	// show the message and terminate.
	_errorExit(buf);
	
	// oops, isn't this the dead code?
	va_end(list);
}


std::string XTBReadAllContents(XTBSysString path){
	std::string str;
	
	// open the path.
	FILE *f=XTBOpen(path, "rb");
	char buf[1024];
	size_t count;
	if(!f){
		// oops, file not found!
		XTBThrowException(L"XTBFileNotFoundException", XTBWStringFromSysString(path).c_str(), NULL);
	}
	
	// read the next bytes.
	while((count=fread(buf, 1, 1024, f))){
		// add to the result.
		str.append(buf, count);
	}
	
	// close the handle and return the result.
	fclose(f);
	return str;
}

bool XTBDoesPathExist(const XTBSysString& path){
	// try opening the path.
	FILE *f=XTBOpen(path, "rb");
	if(f){
		// succeeded to open the path; path does exist. close the file.
		fclose(f);
		return true;
	}
	// failed to open the path; path doesn't exist.
	return false;
}
XTBSysString XTBGetResourceDirectory(XTBResourceDirectory dir){
	switch(dir){
		case XTBResourceDirectoryLanguages:
			return XTBGetAppDir()+XTBPathSeparator+XTBLanguagesDirName;
		case XTBResourceDirectoryDictionaries:
			return XTBGetAppDir()+XTBPathSeparator+XTBDictionariesDirName;
		case XTBResourceDirectoryInternalDictionaries:
			return XTBGetAppDir()+XTBPathSeparator+XTBInternalDictionariesDirName;
		case XTBResourceDirectoryStartupItems:
			return XTBGetAppDir()+XTBPathSeparator+XTBStartupItemsDirName;
	}
}

void XTBRunStartupItems(){
	std::vector<XTBSysString> pathes=XTBScanResourceDirectory(XTBResourceDirectoryStartupItems);
	for(std::vector<XTBSysString>::iterator it=pathes.begin();it!=pathes.end();it++)
		XTBRunStartupItem(*it);
}

void _XTBLog(const char *functionName, const char *file, int lineNumber, const char *str, ...){
	// format the message.
	va_list list;
	char buf[2048];
	va_start(list, str);
	vsprintf(buf, str, list);
	va_end(list);
	
	
	// get the current time.
	time_t t;
	struct tm tm;
	
	time(&t);
	tm=*localtime(&t);
	
	// convert the time to the string.
	std::string timeStr=asctime(&tm);
	
	// remove '\n' in the end of the result of asctime().
	timeStr.resize(timeStr.size()-1);
	
	// output the log.
	printf("%s XTBook %s(%s:%d): %s\n", timeStr.c_str(), functionName, file, lineNumber,
		   buf);
	fflush(stdout);
}

void XTBDumpBytes(const std::string& str){
	for(size_t i=0;i<str.size();i+=16){
		printf("%08x", (int)i);
		for(size_t j=i;j<str.size() && j<i+16;j++){
			if(((j&0xf)>0) && ((j&3)==0))
				printf(" ");
			unsigned int d=(uint8_t)str[j];
			printf(" %02x", d);
		}
		printf("\n");
	}
}

void XTBDumpWString(const std::wstring& s){
	std::string ss;
	ss=twW2M(s);
	puts(ss.c_str());
}
void XTBDumpWPtr(const wchar_t *s){
	XTBDumpWString(s);
}


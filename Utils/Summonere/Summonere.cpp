//
//  Summonere.cpp
//  
//
//  Created by Kawada Tomoaki on 8/15/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include <windows.h>
#include <string>
#include <vector>
#include <algorithm>
#include <shellapi.h>

#define PluginDirectoryName L"Summonere Plugins"

static HWND g_mainWindow;

static std::wstring currentModuleFilename(){
	wchar_t path[4096];
	GetModuleFileName(NULL, path, 4096);
	return path;
}

static std::wstring stringByRemovingLastPathComponent(const std::wstring& s){
	std::wstring::size_type pos=s.find_last_of(L"/\\");
	if(pos==std::wstring::npos)
		return std::wstring();
	return s.substr(0, pos);
}

static std::vector<std::wstring> filenamesInPath(const std::wstring& path){
	WIN32_FIND_DATA fd;
	HANDLE h;
	std::vector<std::wstring>  ret;
	std::wstring filePath;
	
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
				filePath=fd.cFileName;
				ret.push_back(filePath);
			}
		}else{
			// usual file.
			filePath=fd.cFileName;
			ret.push_back(filePath);
		}
		
		// iterate!
	}while(FindNextFile(h, &fd));
	
	// close the handle.
	FindClose(h);
	
	return ret;
}

typedef int (WINAPI *CeOpenerPluginInitProc)(HWND mainWindow);
typedef int (WINAPI *CeOpenerPluginTerminateProc)(HWND mainWindow);

bool runCeOpenerPlugin(HMODULE module){
	
	CeOpenerPluginInitProc initFunc;
	CeOpenerPluginTerminateProc terminateFunc;
	
	initFunc=(CeOpenerPluginInitProc)GetProcAddress(module, L"pluginInit");
	terminateFunc=(CeOpenerPluginTerminateProc)GetProcAddress(module, L"pluginTerminate");
	
	if(initFunc==NULL)
		return false;
	if(terminateFunc==NULL)
		return false;
	
	if((*initFunc)(g_mainWindow)){
		//(*terminateFunc)(g_mainWindow);
	}
	
	return true;
}

static bool loadPlugin(const std::wstring& path){
	
	HMODULE module=LoadLibrary(path.c_str());
	bool loaded=false;
	
	loaded=loaded||runCeOpenerPlugin(module);
	
	FreeLibrary(module);
	
	return loaded;
}

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPTSTR lpCmd, int nShow){
	g_mainWindow=GetActiveWindow();
	
	std::wstring thisPath=currentModuleFilename();
	std::wstring startupItemsPath=stringByRemovingLastPathComponent(thisPath);
	std::wstring appPath=stringByRemovingLastPathComponent(startupItemsPath);
	if(appPath.empty()){
		MessageBox(g_mainWindow, L"Couldn't locate XTBook.", NULL, MB_ICONERROR|MB_SETFOREGROUND);
		return 1;
	}
	
	std::wstring pluginsPath=appPath+L"\\"+PluginDirectoryName;
	
	std::vector<std::wstring> pluginFiles=filenamesInPath(pluginsPath);
	
	std::sort(pluginFiles.begin(), pluginFiles.end());
	
	int count=0;
	
	for(std::vector<std::wstring>::iterator it=pluginFiles.begin();
		it!=pluginFiles.end();it++){
		
		std::wstring path;
		
		path=pluginsPath+L'\\';
		path+=*it;
		
		if(loadPlugin(path))
			count++;
		
	}
	
	char buf[256];
	wchar_t buf2[256];
	
	sprintf(buf, "%d %s %s loaded.", count,
			(count==1)?"plugin":"plugins",
			(count==1)?"was":"were");
	mbstowcs(buf2, buf, 256);
	
	//MessageBox(g_mainWindow,  buf2, L"Summonere", MB_ICONINFORMATION);
	
	Sleep(4000);
	
	return 0;
	
}


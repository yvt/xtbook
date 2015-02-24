//
//  Hyperblaster.cpp
//  
//
//  Created by Kawada Tomoaki on 8/15/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include <windows.h>
#include <string>
#include <vector>
#include <algorithm>
#include <tchar.h>

#define DBT_DEVNODES_CHANGED 0x0007

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

static bool isStorageCardAvailable(){
	HANDLE h;
	WIN32_FIND_DATA fd;
	h=FindFirstFileExW(L"\\Storage Card",
					   FindExInfoStandard, &fd,
					   FindExSearchNameMatch,
					   NULL, 0);
	if(h==INVALID_HANDLE_VALUE){
		return false;
	}
	FindClose(h);
	return true;
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

DWORD GetMultiStringLength(LPCTSTR String)
{
	DWORD Count;
	Count = 0;
	while(*String != _T('\0'))
	{
		while(*String != _T('\0'))
		{
			String++;
			Count++;
		}
		String++;
		Count++;
	}
	return Count;
}

BOOL EnableSDHCSupport()
{
	HKEY hKey;
	DWORD dw;
	RegCreateKeyEx(HKEY_LOCAL_MACHINE, _T("Drivers\\SDCARD\\ClientDrivers\\Class\\SDMemory_Class\\High_Capacity"), 0, NULL, REG_OPTION_NON_VOLATILE, 0, NULL, &hKey, &dw);
	if(!hKey)
		return FALSE;
	if(dw == REG_OPENED_EXISTING_KEY)
	{
		RegCloseKey(hKey);
		return TRUE;
	}
	dw = 0x00000040;
	if(RegSetValueEx(hKey, _T("BlockTransferSize"), 0, REG_DWORD, (BYTE*)&dw, sizeof(DWORD)) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return FALSE;
	}
	if(RegSetValueEx(hKey, _T("DisablePowerManagement"), 0, REG_SZ, (BYTE*)_T(""), sizeof(TCHAR) * ((DWORD)_tcslen(_T("")) + 1)) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return FALSE;
	}
	if(RegSetValueEx(hKey, _T("Dll"), 0, REG_SZ, (BYTE*)_T("SDMemory.dll"), sizeof(TCHAR) * ((DWORD)_tcslen(_T("SDMemory.dll")) + 1)) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return FALSE;
	}
	if(RegSetValueEx(hKey, _T("IClass"), 0, REG_MULTI_SZ, (BYTE*)_T("{A4E7EDDA-E575-4252-9D6B-4195D48BB865}\0{8DD679CE-8AB4-43c8-A14A-EA4963FAA715}\0"), sizeof(TCHAR) * (GetMultiStringLength(_T("{A4E7EDDA-E575-4252-9D6B-4195D48BB865}\0{8DD679CE-8AB4-43c8-A14A-EA4963FAA715}\0")) + 1)) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return FALSE;
	}
	if(RegSetValueEx(hKey, _T("Prefix"), 0, REG_SZ, (BYTE*)_T("DSK"), sizeof(TCHAR) * ((DWORD)_tcslen(_T("DSK")) + 1)) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return FALSE;
	}
	if(RegSetValueEx(hKey, _T("Profile"), 0, REG_SZ, (BYTE*)_T("SDMemory"), sizeof(TCHAR) * ((DWORD)_tcslen(_T("SDMemory")) + 1)) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return FALSE;
	}
	RegCloseKey(hKey);
	PostMessage(HWND_BROADCAST, WM_DEVICECHANGE, DBT_DEVNODES_CHANGED, 0);
	return TRUE;
}



int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPTSTR lpCmd, int nShow){
	if(isStorageCardAvailable())
		return 0;
	
	if(!EnableSDHCSupport())
		return 1;
	
	while(!isStorageCardAvailable()){
		Sleep(500);
		if(GetKeyState(VK_ESCAPE)&0x80)
			return 1;
	}
	
	
	return 0;
	
}


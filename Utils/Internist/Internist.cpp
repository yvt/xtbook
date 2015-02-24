//
//  Internist.cpp
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

#define APPSTR L"\x30a2\x30d7\x30ea"

static HWND g_parentWindow;

static const std::wstring g_pathForStorageCard=L"\\Storage Card";
static const std::wstring g_pathForStorageCardApp=L"\\Storage Card\\" APPSTR;
static const std::wstring g_pathForStorageCardDic=L"\\Storage Card\\XTBook Dictionaries";

static const std::wstring g_pathForInternalFlash=L"\\NAND3";
static const std::wstring g_pathForInternalFlashApp=L"\\NAND3\\" APPSTR;

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

static std::wstring pathForXTBook(){
	
	std::wstring thisPath=currentModuleFilename();
	std::wstring startupItemsPath=stringByRemovingLastPathComponent(thisPath);
	std::wstring appPath=stringByRemovingLastPathComponent(startupItemsPath);
	if(appPath.find(L"XTBook")!=std::wstring::npos)
		return appPath;
	
	std::vector<std::wstring> files=filenamesInPath(g_pathForStorageCardApp);
	std::sort(files.begin(), files.end());
	for(std::vector<std::wstring>::reverse_iterator it=files.rbegin();it!=files.rend();it++){
		const std::wstring& file=*it;
		if(file.find(L"XTBook")==0){
			return g_pathForStorageCardApp+L"\\"+file;
		}
	}
	return std::wstring();
}

static std::wstring versionForXTBook(){
	std::wstring path=pathForXTBook();
	std::wstring::size_type pos=path.rfind(L'\\');
	return path.substr(pos+1);
}

static bool isXTBookInstalled(){
	std::vector<std::wstring> files=filenamesInPath(g_pathForInternalFlashApp);
	for(std::vector<std::wstring>::iterator it=files.begin();it!=files.end();it++){
		const std::wstring& file=*it;
		if(file.find(L"XTBook")==0){
			return true;
		}
	}
	return false;
}

static bool deletePath(const std::wstring& path){
	SHFILEOPSTRUCT op;
	memset(&op, 0, sizeof(SHFILEOPSTRUCT));
	op.hwnd=g_parentWindow;
	op.wFunc=FO_DELETE;
	op.pFrom=path.c_str();
	op.pTo=NULL;
	op.fFlags=FOF_NOCONFIRMATION;
	return !SHFileOperation(&op);
}

static bool deleteExistingInstallation(){
	std::vector<std::wstring> files=filenamesInPath(g_pathForInternalFlashApp);
	for(std::vector<std::wstring>::iterator it=files.begin();it!=files.end();it++){
		const std::wstring& file=*it;
		if(file.find(L"XTBook")==0){
			if(!deletePath(g_pathForInternalFlashApp+L"\\"+file))
				return false;
		}
	}
	return true;
}

static void copyRecursive(const std::wstring& fromPath,
						  const std::wstring& toPath){
	CreateDirectory(toPath.c_str(), NULL);
	
	WIN32_FIND_DATA fd;
	HANDLE h;
	
	h=FindFirstFileExW((fromPath+L"\\*").c_str(),
					   FindExInfoStandard,
					   &fd, FindExSearchNameMatch,
					   NULL, 0);
	if(h==INVALID_HANDLE_VALUE){
		return;
	}
	
	do{
		// is it a directory?
		if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
			if(fd.cFileName[0]!=L'.'){
				// recurse.
				std::wstring srcFile=fromPath;
				std::wstring toFile=toPath;
				std::wstring file=fd.cFileName;
				srcFile+=L'\\';
				toFile+=L'\\';
				srcFile+=file;
				toFile+=file;
				if(file==L"Dictionaries")
					continue;
				if(file==L"Help")
					continue;
				copyRecursive(srcFile, toFile);
			}
		}else{
			if(fd.cFileName[0]==L'.')
				continue;
			// usual file.
			std::wstring srcFile=fromPath;
			std::wstring toFile=toPath;
			std::wstring file=fd.cFileName;
			srcFile+=L'\\';
			toFile+=L'\\';
			srcFile+=file;
			
			if(file==L"Preferences.plist")
				continue;
			if(file==L"Internist.exe")
				continue;
			
			
			// remove postfix .installed
			if(file.size()>=10 && 
			   file.find(L".installed")==file.size()-10){
				file.resize(file.size()-10);
			}
			toFile+=file;
			
			CopyFile(srcFile.c_str(),
					 toFile.c_str(),
					 FALSE);
			
		}
		
		// iterate!
	}while(FindNextFile(h, &fd));
	
	// close the handle.
	FindClose(h);

}

static void installXTBook(const std::wstring& fromPath,
						  const std::wstring& toPath){
	copyRecursive(fromPath, toPath);
}

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPTSTR lpCmd, int nShow){
	
	HWND parentWindow=GetForegroundWindow();
	g_parentWindow=parentWindow;
	std::wstring appPath;
	std::wstring destPath;
	
	// find XTBook.
	while(true){
		appPath=pathForXTBook();
	
		if(appPath.empty()){
			if(MessageBox(parentWindow, L"Couldn't locate XTBook.", NULL, MB_ICONERROR|MB_RETRYCANCEL|MB_SETFOREGROUND)
			   ==IDCANCEL){
				return 1;
			}
		}
		break;
	}
	
	destPath=g_pathForInternalFlashApp+L"\\";
	destPath+=versionForXTBook();
	
	bool overwriting=isXTBookInstalled();
	
	if(overwriting){
		if(MessageBox(parentWindow, L"XTBook is already installed.\n"
					  L"Overwrite existing installation?\n\n"
					  L"Tap OK to start install.", NULL, MB_ICONWARNING|MB_OKCANCEL|MB_DEFBUTTON2|MB_SETFOREGROUND)
		   !=IDOK){
			return 0;
		}
		if(!deleteExistingInstallation()){
			MessageBox(parentWindow, L"Error while deleteing existing installation.\n"
					   L"Aborting.", NULL, MB_ICONERROR|MB_SETFOREGROUND);
			return 2;
		}
	}else{
		if(MessageBox(parentWindow, L"Are you sure to install XTBook?\n\n"
					  L"Tap OK to start install.", NULL, MB_ICONQUESTION|MB_OKCANCEL|MB_SETFOREGROUND)
		   !=IDOK){
			return 0;
		}
	}
	std::wstring msg;
	msg=L"Installing from "+appPath+L"\n";
	msg+=L"to ";
	msg+=destPath;
	msg+=L".";
	if(MessageBox(parentWindow, msg.c_str(), NULL,MB_OKCANCEL)
	   !=IDOK){
		return 0;
	}
	
	CreateDirectory(g_pathForInternalFlashApp.c_str(), NULL);
	
	installXTBook(appPath, destPath);
	
	CreateDirectory(g_pathForStorageCardDic.c_str(), NULL);
	
	MessageBox(parentWindow, L"Installation done.\n"
			   L"Close currently running XTBook.", L"Internist", MB_ICONINFORMATION|MB_OKCANCEL|MB_SETFOREGROUND);
	
	
	
	
	return 0;
}

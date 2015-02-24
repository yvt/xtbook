
/*
 *  main.cpp
 *  XTBook
 *
 *  Created by Nexhawks on 12/22/10.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#include "global.h"
#include "platform.h"
#include <tcw/twApp.h>
#include <tcw/twEvent.h>
#include <tcw/twException.h>
#include "XTBRootWindow.h"
#include "XTBSkin.h"
#include "XTBHGGothic.h"
#include "XTBHGGothicAA.h"
#include "i18n.h"
#include "XTBUserDefaults.h"
#include <stdarg.h>
#include <tcw/twStrConv.h>
#include <tcw/twMsgBox.h>
#include <wchar.h>
#include "TPList/TPLAutoReleasePtr.h"
#include "TPList/TPLString.h"
#include <assert.h>
#include "commondlg.h"
#include "XTBCoreDictionaryTypeRegister.h"
#include "XTBDictionary.h"
#include "TPList/TPLNumber.h"
using namespace std;

int Main(int argc, char *argv[])
{
	// Initialize TCW.
	try{
		twInitStruct initStruct;
		initStruct.title=L"XTBook";
		initStruct.scrSize=twSize(480, 320);
		initStruct.resizable=true;
#ifdef WIN32
		// Tell TCW to use full-screen mode in Sharp Brain(TM).
	
			initStruct.scrSize=twSize(440, 290);
		
		
#endif
		twInit(initStruct);
		atexit(twExit);
	}catch(const exception& ex){
		XTBErrorExit("Error while initializing tcw: %s", ex.what());
	}catch(...){
		XTBErrorExit("Error while initializing tcw.");
	}
	
	// Setup the initial desktop. This shows the application icon of XTBook.
	try{
		twSetDesktop(new XTBRootWindow());
	}catch(const exception& ex){
		XTBErrorExit("Failed to setup the root window: %s", ex.what());
	}catch(...){
		XTBErrorExit("Failed to setup the root window.");
	}
	
	// Enter the main loop.
	try{
		tw_event->mainLoop();
	}catch(const exception& ex){
		XTBErrorExit("Unhandled exception: %s", ex.what());
	}catch(...){
		XTBErrorExit("Unhandled exception.");
	}
	
	XTBUserDefaults::standardUserDefaults()->synchronize();
	
	twExit();
	
	return(0);
}

/** Used to tell the error message from errorExit to showError. */
static std::wstring g_errorMsg;

/** Displays the error message, and then terminates the application. */
static void showError(){
	twMsgBox(NULL,
			g_errorMsg, twMBB_ok, XTBLocalizedString(L"XTBInitError") );
	abort();
}

/** Called by initThreadSafeComponents. Causes the main thread to display the
 * error message, terminating the application. */
static void errorExit(const std::wstring& msg){
	XTBLog("Error while initialization: %s\n", twW2M(msg).c_str());
	g_errorMsg=msg;
	tw_event->invokeOnMainThread(new twNoArgumentGlobalFunctionInvocation(showError));
	
	
}

/** Short version. */
std::wstring XTBShortVersionInformation(){
	return L"XTBook-0.2.6"; // removed russian(setSoftKeyboardVisible) from XTBBrowser
	//return L"XTBook-0.2.6-russian";
}

/** Full version information. */
std::wstring XTBLongVersionInformation(){
	return XTBShortVersionInformation()+L" Copyright 2011-2012 Nexhawks, all rights reserved.\n"
	L"Visit http://www.nexhawks.net/ for more informations.";
}

void initThreadSafeComponents();
void initThreadUnsafeComponents();

void initThreadSafeComponents(){
	
	
	
	XTBSetProgressText(XTBLocalizedString(L"XTBLoadLocale"));
	try{
		XTBUserDefaults *def=XTBUserDefaults::standardUserDefaults();
		
		TPLString *locale=def->stringForKey(XTBUserDefaultsKeyLocale, twW2M(XTBDefaultLocale).c_str());
		assert(locale);
		if(locale){
			XTBSetLocale(twM2W(locale->UTF8String()));
		}
	}catch(const exception& ex){
		XTBLog("Locale was reseted because of exception \"%s\".", ex.what());
		// reset locale
		XTBUserDefaults *def=XTBUserDefaults::standardUserDefaults();
		def->setObject(TPLAutoReleasePtr<TPLString>(new TPLString(twW2M(XTBDefaultLocale).c_str())), 
					   XTBUserDefaultsKeyLocale);
	}catch(...){
		XTBLog("Locale was reseted because of exception.");
		// reset locale
		XTBUserDefaults *def=XTBUserDefaults::standardUserDefaults();
		def->setObject(TPLAutoReleasePtr<TPLString>(new TPLString(twW2M(XTBDefaultLocale).c_str())), 
					   XTBUserDefaultsKeyLocale);
	}
	
	std::wstring curStep;
	
	try{
		curStep=XTBLocalizedString(L"XTBInitSkinFailed");
		XTBSetProgressText(XTBLocalizedString(L"XTBInitSkin"));
		
		delete tw_curSkin;
		tw_curSkin=new XTBSkin();
		
		curStep=XTBLocalizedString(L"XTBStartupItemsFailed");
		XTBSetProgressText(XTBLocalizedString(L"XTBInitStartupItems"));
		
		XTBRunStartupItems();
	
		curStep=XTBLocalizedString(L"XTBInitDictionaryTypeFailed");
		XTBSetProgressText(XTBLocalizedString(L"XTBInitDictionaryType"));
		
		XTBRegisterCoreDictionaryTypes();
		
		curStep=XTBLocalizedString(L"XTBInitDictionaryFailed");
		XTBSetProgressText(XTBLocalizedString(L"XTBInitDictionary"));
		
		XTBDictionary::scanDictionaries();
		
	}catch(const exception& ex){
		errorExit(curStep+L"\n"+twM2W(ex.what()));
		return;
	}catch(...){
		errorExit(curStep);
		return;
	}
	
	XTBSetProgressText(XTBLocalizedString(L"XTBSavePreference"));
	{
		XTBUserDefaults *def=XTBUserDefaults::standardUserDefaults();
		def->synchronize();
	}
}
void initThreadUnsafeComponents(){
	
	XTBLog("initialization started. (appDir=\"%s\")", twW2M(XTBWStringFromSysString(XTBGetAppDir())).c_str());
	
	try{
		XTBSetLocale(XTBDefaultLocale);
		
	}catch(const exception& ex){
		XTBErrorExit("Failed to initialize the locale: %s", ex.what());
		return;
	}catch(...){
		XTBErrorExit("Failed to initialize the locale.");
		return;
	}
	
	
	const twFont *oldFont=tw_defFont;
	try{
		tw_defFont=new XTBHGGothicAA();
	}catch(const exception& ex){
		errorExit(XTBLocalizedString(L"XTBInitFontFailed")+L"\n"+twM2W(ex.what()));
		return;
	}catch(...){
		errorExit(XTBLocalizedString(L"XTBInitFontFailed"));
		return;
	}
	delete oldFont;
	
	try{
		XTBUserDefaults::standardUserDefaults();
	}catch(const exception& ex){
		errorExit(XTBLocalizedString(L"XTBInitPreferenceFailed")+L"\n"+twM2W(ex.what()));
		return;
	}catch(...){
		errorExit(XTBLocalizedString(L"XTBInitPreferenceFailed"));
		return;
	}
	
	// set full-screen.
	
	XTBUserDefaults *def=XTBUserDefaults::standardUserDefaults();
	if(def->numberForKey(XTBUserDefaultsKeyFullScreen, "false")->boolValue()){
		tw_app->setFullScreen(true);
		tw_app->setAutoScrSize();
	}
}



//
//  XTBEventManager.cpp
//  XTBook
//
//  Created by Kawada Tomoaki on 5/29/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "XTBEventManager.h"
#include <exception>
#include "platform.h"
#include <tcw/twStrConv.h>
#include <tcw/twApp.h>

XTBEventManager::XTBEventManager(){
	
}

XTBEventManager::~XTBEventManager(){
	
}

bool XTBEventManager::dealEvent(SDL_Event &ev){
	try{
		return twSDLEvent::dealEvent(ev);
	}catch(const std::exception& ex){
		XTBLog("exception '%s' while processing an event.",
			   ex.what());
	}catch(const std::wstring& str){
		XTBLog("exception '%s' while processing an event.",
			   twW2M(str).c_str());
	}catch(const std::string& str){
		XTBLog("exception '%s' while processing an event.",
			   str.c_str());
	}catch(const wchar_t *str){
		XTBLog("exception '%s' while processing an event.",
			   twW2M(str).c_str());
	}catch(const char *str){
		XTBLog("exception '%s' while processing an event.",
			   str);
	}catch(...){
		XTBLog("unknown exception while processing an event.");
	}
	return true;
}

twEvent *twCreateEvent(){
	return new XTBEventManager();
}

twApp *twCreateApp(){
	twApp *app=new twApp();
#ifdef _WIN32_WCE
	app->setAutoSize(twSize(480, 320));
#endif
	return app;
}

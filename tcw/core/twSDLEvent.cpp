/*
 *  twSDLEvent.cpp
 *  
 *
 *  Created by Nexhawks on 10/01/17.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#include <cstring>
#include <tcw/twApp.h>
#include <tcw/twSDLEvent.h>
#include <tcw/twWnd.h>
#include <tcw/twException.h>
#include <tcw/twTimer.h>
#include <tcw/twInvocation.h>
#include <cassert>
#include <tcw/twSDLSemaphore.h>
#include <tcw/twLock.h>
#include <exception>

#ifdef WIN32
#include <windows.h>
#endif

using namespace std;

#define twSE_VALIDATE		(SDL_USEREVENT+0x0001)
#define twSE_QUIT			(SDL_USEREVENT+0x0002)
#define twSE_INVOKE			(SDL_USEREVENT+0x0003)
#define twSE_FIRETIMER		(SDL_USEREVENT+0x0004)


twSDLEvent::twSDLEvent(){
	SDL_EnableKeyRepeat(400, 80);
	removedTimersSemaphore=new twSDLSemaphore();
}

twSDLEvent::~twSDLEvent(){
	delete removedTimersSemaphore;
}

wstring twSDLEvent::convertKey(const SDL_keysym& s) const{
	SDLKey sym=s.sym;
	switch(sym){
		case SDLK_BACKSPACE:
			return L"BackSpace";
		case SDLK_TAB:
			return L"Tab";
		case SDLK_CLEAR:
			return L"Clear";
		case SDLK_RETURN:
			return L"Return";
		case SDLK_PAUSE:
			return L"Pause";
		case SDLK_ESCAPE:
			return L"Escape";
		case SDLK_SPACE:
			return L" ";
		case SDLK_EXCLAIM:
			return L"!";
		case SDLK_QUOTEDBL:
			return L"\"";
		case SDLK_HASH:
			return L"#";
		case SDLK_DOLLAR:
			return L"$";
		case SDLK_AMPERSAND:
			return L"&";
		case SDLK_QUOTE:
			return L"'";
		case SDLK_LEFTPAREN:
			return L"(";
		case SDLK_RIGHTPAREN:
			return L")";
		case SDLK_ASTERISK:
			return L"*";
		case SDLK_PLUS:
			return L"+";
		case SDLK_COMMA:
			return L",";
		case SDLK_MINUS:
			return L"-";
		case SDLK_PERIOD:
			return L".";
		case SDLK_SLASH:
			return L"/";
		case SDLK_0:
			return L"0";
		case SDLK_1:
			return L"1";
		case SDLK_2:
			return L"2";
		case SDLK_3:
			return L"3";
		case SDLK_4:
			return L"4";
		case SDLK_5:
			return L"5";
		case SDLK_6:
			return L"6";
		case SDLK_7:
			return L"7";
		case SDLK_8:
			return L"8";
		case SDLK_9:
			return L"9";
		case SDLK_COLON:
			return L":";
		case SDLK_SEMICOLON:
			return L";";
		case SDLK_LESS:
			return L"<";
		case SDLK_EQUALS:
			return L"=";
		case SDLK_GREATER:
			return L">";
		case SDLK_QUESTION:
			return L"?";
		case SDLK_AT:
			return L"@";
		case SDLK_LEFTBRACKET:
			return L"[";
		case SDLK_BACKSLASH:
			return L"\\";
		case SDLK_RIGHTBRACKET:
			return L"]";
		case SDLK_CARET:
			return L"^";
		case SDLK_UNDERSCORE:
			return L"_";
		case SDLK_BACKQUOTE:
			return L"`";
		case SDLK_a:
			return L"a";
		case SDLK_b:
			return L"b";
		case SDLK_c:
			return L"c";
		case SDLK_d:
			return L"d";
		case SDLK_e:
			return L"e";
		case SDLK_f:
			return L"f";
		case SDLK_g:
			return L"g";
		case SDLK_h:
			return L"h";
		case SDLK_i:
			return L"i";
		case SDLK_j:
			return L"j";
		case SDLK_k:
			return L"k";
		case SDLK_l:
			return L"l";
		case SDLK_m:
			return L"m";
		case SDLK_n:
			return L"n";
		case SDLK_o:
			return L"o";
		case SDLK_p:
			return L"p";
		case SDLK_q:
			return L"q";
		case SDLK_r:
			return L"r";
		case SDLK_s:
			return L"s";
		case SDLK_t:
			return L"t";
		case SDLK_u:
			return L"u";
		case SDLK_v:
			return L"v";
		case SDLK_w:
			return L"w";
		case SDLK_x:
			return L"x";
		case SDLK_y:
			return L"y";
		case SDLK_z:
			return L"z";
		case SDLK_DELETE:
			return L"Delete";
		case SDLK_KP0:
			return L"KeyPad0";
		case SDLK_KP1:
			return L"KeyPad1";
		case SDLK_KP2:
			return L"KeyPad2";
		case SDLK_KP3:
			return L"KeyPad3";
		case SDLK_KP4:
			return L"KeyPad4";
		case SDLK_KP5:
			return L"KeyPad5";
		case SDLK_KP6:
			return L"KeyPad6";
		case SDLK_KP7:
			return L"KeyPad7";
		case SDLK_KP8:
			return L"KeyPad8";
		case SDLK_KP9:
			return L"KeyPad9";
		case SDLK_KP_PERIOD:
			return L".";
		case SDLK_KP_DIVIDE:
			return L"/";
		case SDLK_KP_MULTIPLY:
			return L"*";
		case SDLK_KP_MINUS:
			return L"-";
		case SDLK_KP_PLUS:
			return L"+";
		case SDLK_KP_ENTER:
			return L"Return";
		case SDLK_KP_EQUALS:
			return L"=";
		case SDLK_UP:
			return L"Up";
		case SDLK_DOWN:
			return L"Down";
		case SDLK_RIGHT:
			return L"Right";
		case SDLK_LEFT:
			return L"Left";
		case SDLK_INSERT:
			return L"Insert";
		case SDLK_HOME:
			return L"Home";
		case SDLK_END:
			return L"End";
		case SDLK_PAGEUP:
			return L"PageUp";
		case SDLK_PAGEDOWN:
			return L"PageDown";
		case SDLK_F1:
			return L"F1";
		case SDLK_F2:
			return L"F2";
		case SDLK_F3:
			return L"F3";
		case SDLK_F4:
			return L"F4";
		case SDLK_F5:
			return L"F5";
		case SDLK_F6:
			return L"F6";
		case SDLK_F7:
			return L"F7";
		case SDLK_F8:
			return L"F8";
		case SDLK_F9:
			return L"F9";
		case SDLK_F10:
			return L"F10";
		case SDLK_F11:
			return L"F11";
		case SDLK_F12:
			return L"F12";
		case SDLK_F13:
			return L"F13";
		case SDLK_F14:
			return L"F14";
		case SDLK_F15:
			return L"F15";
		case SDLK_NUMLOCK:
			return L"NumLock";
		case SDLK_CAPSLOCK:
			return L"CapsLock";
		case SDLK_SCROLLOCK:
			return L"ScrolLock";
		case SDLK_RSHIFT:
		case SDLK_LSHIFT:
			return L"Shift";
		case SDLK_RCTRL:
		case SDLK_LCTRL:
			return L"Ctrl";
		case SDLK_RALT:
		case SDLK_LALT:
			return L"Alt";
		case SDLK_RMETA:
#if SDLK_LMETA!=SDLK_RMETA
		case SDLK_LMETA:
#endif
			return L"Meta";
#if SDLK_RSUPER!=SDLK_RMETA
		case SDLK_RSUPER:
#if SDLK_RSUPER!=SDLK_LSUPER
		case SDLK_LSUPER:
#endif
#endif
			return L"Super";
		case SDLK_MODE:
			return L"Mode";
		case SDLK_HELP:
			return L"Help";
		case SDLK_PRINT:
			return L"Print";
		case SDLK_SYSREQ:
			return L"SystemRequest";
		case SDLK_BREAK:
			return L"Break";
		case SDLK_MENU:
			return L"Menu";
		case SDLK_POWER:
			return L"Power";
		default:
			wchar_t ch[2]={(wchar_t)sym,0};
			return ch;
	}
}

twMouseButton twSDLEvent::convertButton(Uint8 b) const{
	if(b==SDL_BUTTON_LEFT){
		return twMB_left;
	}else if(b==SDL_BUTTON_MIDDLE){
		return twMB_middle;
	}else if(b==SDL_BUTTON_RIGHT){
		return twMB_right;
	}else if(b==4){
		return twMB_wheelUp;
	}else if(b==5){
		return twMB_wheelDown;
	}else{
		return (twMouseButton)b;
	}
}

static SDLKey g_lastKeyEvent = SDLK_UNKNOWN;

bool twSDLEvent::dealEvent(SDL_Event& ev){
	if(ev.type==SDL_KEYDOWN){
		if(ev.key.keysym.sym != g_lastKeyEvent){
			keyDown(convertKey(ev.key.keysym));
			keyPress((wchar_t)ev.key.keysym.unicode);
			g_lastKeyEvent=ev.key.keysym.sym;
		}
		return true;
	}else if(ev.type==SDL_KEYUP){
		keyUp(convertKey(ev.key.keysym));
		if(ev.key.keysym.sym == g_lastKeyEvent){
			g_lastKeyEvent=(SDLKey)-1232;
		}
	}else if(ev.type==SDL_MOUSEMOTION){
		//m_moved=true;
		m_movePos=twPoint(ev.motion.x, ev.motion.y);
		mouseMove(m_movePos);
	}else if(ev.type==SDL_MOUSEBUTTONDOWN){
		mouseDown(twPoint(ev.button.x, ev.button.y), convertButton(ev.button.button));
	}else if(ev.type==SDL_MOUSEBUTTONUP){
		mouseUp(twPoint(ev.button.x, ev.button.y), convertButton(ev.button.button));
	}else if(ev.type==SDL_QUIT){
		return false;
	}else if(ev.type==twSE_QUIT){
		return false;
	}else if(ev.type==twSE_VALIDATE){
		validate();
	}else if(ev.type==SDL_VIDEORESIZE){
		tw_app->setScrSize(twSize(ev.resize.w, ev.resize.h));
		twInvalidateRect(twRect(twPoint(0, 0), tw_app->getScrSize()));
		validate();
		twInvalidateRect(twRect(twPoint(0, 0), tw_app->getScrSize()));
		validate();
	}else if(ev.type==SDL_VIDEOEXPOSE){
		twInvalidateRect(twRect(twPoint(0, 0), tw_app->getScrSize()));
	}else if(ev.type==twSE_INVOKE){
		twInvocation *invocation=(twInvocation *)(ev.user.data1);
		invocation->invoke();
		delete invocation;
	}else if(ev.type==twSE_FIRETIMER){
		twSDLEvent::timerInfo *info=(twSDLEvent::timerInfo *)(ev.user.data1);
		{
			twLock removedTimersLock(removedTimersSemaphore);
			if(removedTimers.find(info)!=removedTimers.end())
				return true;
		}
		
		if(!info->event)
			return true;
		if(!info->timer)
			return true;
		
		twTimer *timer=info->timer;
		if(!timer)
			return true;
		if(timer->event!=this)
			return true;
		fireTimer(timer);
	}
	return true;
}

void twSDLEvent::doEvents(int timeout){
	Uint32 ot=SDL_GetTicks();
	SDL_Event ev;
	g_lastKeyEvent=(SDLKey)-1232;
	while(SDL_PollEvent(&ev)){
		if(!dealEvent(ev))
			return;
		if(SDL_GetTicks()>ot+timeout)
			return;
	}
	{
		twLock removedTimersLock(removedTimersSemaphore);
		removedTimers.clear();
	}
}

bool twSDLEvent::waitEvent(){
	SDL_Event ev;
	SDL_WaitEvent(&ev);
	m_moved=false;
	g_lastKeyEvent=(SDLKey)-1232;
	if(!dealEvent(ev))
		return false;
	g_lastKeyEvent=(SDLKey)-1232;
	while(SDL_PollEvent(&ev))
		if(!dealEvent(ev))
			return false;
	if(m_moved){
		//mouseMove(m_movePos);
		// no need to use this mechanism
	}	
	{
		twLock removedTimersLock(removedTimersSemaphore);
		removedTimers.clear();
	}
	return true;
}

void twSDLEvent::postQuitMsg(){
	SDL_Event ev;
	memset(&ev, 0, sizeof(ev));
	ev.type=twSE_QUIT;
	SDL_PushEvent(&ev);
}

void twSDLEvent::postValidateMsg(){
	SDL_Event ev;
	//puts("valid2");
	memset(&ev, 0, sizeof(ev));
	ev.type=twSE_VALIDATE;
	SDL_PushEvent(&ev);
}

Uint32 SDLTimerProc(Uint32 interval, void *param){
	twSDLEvent::timerInfo *info=(twSDLEvent::timerInfo *)param;
	twTimer *timer=info->timer;
	SDL_Event ev;
	memset(&ev, 0, sizeof(ev));
	ev.type=twSE_FIRETIMER;
	ev.user.data1=info;
	
	SDL_PushEvent(&ev);
	return timer->getInterval();
}

void twSDLEvent::addTimer(twTimer *timer){
	{
		timerInfo info;
		info.timer=timer;
		info.event=this;
		timers.push_back(info);
	}
	{
		timerInfo& info=*(timers.rbegin());
		info.timerId=SDL_AddTimer(timer->getInterval(), SDLTimerProc, &info);
	}
}

void twSDLEvent::removeTimer(twTimer *timer){
	for(list<timerInfo>::iterator it=timers.begin();it!=timers.end();it++){
		timerInfo& info=*it;
		if(info.timer==timer){
			{
				twLock removedTimersLock(removedTimersSemaphore);
				removedTimers.insert(&info);
			}
			SDL_RemoveTimer(info.timerId);
			memset(&info, 0, sizeof(timerInfo));
			timers.erase(it);
			return;
		}
	}
	throw twException(L"timer to remove not found.");
}

void twSDLEvent::updateTimer(twTimer *timer){
	// no need to update.
}

void twSDLEvent::invokeOnMainThread(twInvocation *invocation){
	SDL_Event ev;
	memset(&ev, 0, sizeof(ev));
	ev.type=twSE_INVOKE;
	ev.user.data1=invocation;
	SDL_PushEvent(&ev);
}

void twThreadStarted();

__attribute((weak))
void twThreadStarted(){}

int BackgroundInvokeProc(void *data){
	twThreadStarted();
	twInvocation *inv=(twInvocation *)data;
	twSDLEvent *event=static_cast<twSDLEvent *> (tw_event);
	
#ifdef _WIN32_WCE
	CeSetThreadPriority(GetCurrentThread(),
						255);
#endif
	
	inv->invoke();
	delete inv;
	
	SDL_Delay(10);
	event->invokeOnMainThread(new twOneArgumentMemberFunctionInvocation<twSDLEvent, 
					   Uint32>(event, &twSDLEvent::recycleThread, SDL_ThreadID()));
	return 0;
}
void twSDLEvent::invokeInBackground(twInvocation *invocation){
	threads.push_back(SDL_CreateThread(BackgroundInvokeProc, invocation));
}
void twSDLEvent::recycleThread(Uint32 threadId){
	for(std::list<SDL_Thread *>::iterator it=threads.begin();it!=threads.end();it++){
		if(SDL_GetThreadID(*it)==threadId){
			SDL_WaitThread(*it, NULL);
			threads.erase(it);
			break;
		}
	}
}
void twSDLEvent::sleep(int ticks){
	SDL_Delay(ticks);
}


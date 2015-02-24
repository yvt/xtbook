/*
 *  twSDLEvent.h
 *  
 *
 *  Created by Nexhawks on 10/01/17.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#pragma once

#include <SDL/SDL.h>
#include <tcw/twEvent.h>
#include <tcw/twTypes.h>
#include <wchar.h>
#include <string>
#include <list>
#include <set>

class twTimer;
class twSDLSemaphore;

class twSDLEvent : public twEvent{
	friend Uint32 SDLTimerProc(Uint32 interval, void *param);
	friend int BackgroundInvokeProc(void *data);
public:
	struct timerInfo{
		twTimer *timer;
		SDL_TimerID timerId;
		twSDLEvent *event;
	};
protected:
	bool m_moved;
	twPoint m_movePos;
	std::list<timerInfo> timers;
	std::list<SDL_Thread *> threads;
	std::set<timerInfo *> removedTimers;
	twSDLSemaphore *removedTimersSemaphore;
	virtual std::wstring convertKey(const SDL_keysym&) const;
	virtual twMouseButton convertButton(Uint8) const;
	virtual bool dealEvent(SDL_Event& ev);
	
	virtual void addTimer(twTimer *);
	virtual void removeTimer(twTimer *);
	virtual void updateTimer(twTimer *);
	
	void recycleThread(Uint32);
public:
	twSDLEvent();
	virtual ~twSDLEvent();
	
	virtual void doEvents(int timeout);
	virtual bool waitEvent();
	virtual void postQuitMsg();
	virtual void postValidateMsg();
	virtual void invokeOnMainThread(twInvocation *);
	virtual void invokeInBackground(twInvocation *);
	virtual void sleep(int);
	
};


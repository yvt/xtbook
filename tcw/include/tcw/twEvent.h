/*
 *  twEvent.h
 *  
 *
 *  Created by Nexhawks on 10/01/17.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#pragma once

#include <tcw/twTypes.h>
#include <wchar.h>
#include <string>

enum twMouseButton{
	twMB_left=0,
	twMB_middle,
	twMB_right,
	twMB_wheelUp,
	twMB_wheelDown
};

class twTimer;
class twInvocation;
class twEvent{
	friend class twTimer;
protected:
	void validate();
	void keyDown(const std::wstring&);
	void keyPress(wchar_t);
	void keyUp(const std::wstring&);
	void mouseDown(const twPoint&, twMouseButton button);
	void mouseMove(const twPoint&);
	void mouseUp(const twPoint&, twMouseButton button);
	
	virtual void addTimer(twTimer *)=0;
	virtual void removeTimer(twTimer *)=0;
	virtual void updateTimer(twTimer *)=0;
	void fireTimer(twTimer *);
	
public:
	twEvent();
	virtual ~twEvent();
	
	virtual void mainLoop();
	virtual void doEvents(int timeout)=0;
	virtual bool waitEvent()=0;
	virtual void postQuitMsg()=0;
	virtual void postValidateMsg()=0;
	virtual void invokeOnMainThread(twInvocation *)=0;
	virtual void invokeDelayed(twInvocation *, int);
	virtual void invokeInBackground(twInvocation *)=0;
	virtual void sleep(int)=0;
};

extern twEvent *tw_event;

/** override this in order to create your own twEvent. */
twEvent *twCreateEvent();

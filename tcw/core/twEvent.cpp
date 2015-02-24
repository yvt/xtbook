/*
 *  twEvent.cpp
 *  
 *
 *  Created by Nexhawks on 10/01/17.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#include <tcw/twEvent.h>
#include <tcw/twWnd.h>
#include <tcw/twTimer.h>
#include <tcw/twSDLEvent.h>
#include <cassert>
#include <tcw/twTimerWithInvocation.h>

twEvent *tw_event;
extern twWnd *tw_capture;
extern bool tw_capClient;
extern twWnd *tw_over;

__attribute((weak))
twEvent *twCreateEvent(){
	return new twSDLEvent();
}

void twInitEvent();
void twExitEvent();

void twInitEvent(){
	tw_over=NULL;
	tw_event=twCreateEvent();
}
void twExitEvent(){
	delete tw_event;
}

void twEvent::validate(){
	twValidate();
}
void twEvent::keyDown(const std::wstring& nm){
	if(tw_capture)
		tw_capture->onKeyDown(nm);
	else if(twGetDesktop())
		twGetDesktop()->onKeyDown(nm);
}
void twEvent::keyPress(wchar_t ch){
	if(tw_capture)
		tw_capture->onKeyPress(ch);
	else if(twGetDesktop())
		twGetDesktop()->onKeyPress(ch);
}
void twEvent::keyUp(const std::wstring& nm){
	if(tw_capture)
		tw_capture->onKeyUp(nm);
	else if(twGetDesktop())
		twGetDesktop()->onKeyUp(nm);
}
void twEvent::mouseDown(const twPoint& pt, twMouseButton button){
	if(tw_capture)
		tw_capture->onMouseDown(pt-tw_capture->getWndRect().loc(), button);
	else if(twGetDesktop())
		twGetDesktop()->onMouseDown(pt-twGetDesktop()->getWndRect().loc(), button);
}
void twEvent::mouseMove(const twPoint& pt){
	//printf("0x%08x %s\n", tw_capture, tw_capClient?"true":"false");
	if(tw_capture)
		tw_capture->onMouseMove(pt-tw_capture->getWndRect().loc());
	else if(twGetDesktop()){
		twWnd *cHover=twGetDesktop()->hitTest(pt-twGetDesktop()->getWndRect().loc());
		if(cHover!=tw_over){
			if(tw_over)
				tw_over->clientMouseLeave(pt-tw_over->getWndRect().loc()-tw_over->getClientRect().loc());
			if(cHover)
				cHover->clientMouseEnter(pt-cHover->getWndRect().loc()-cHover->getClientRect().loc());
			tw_over=cHover;
		}
		twGetDesktop()->onMouseMove(pt-twGetDesktop()->getWndRect().loc());
	}
}
void twEvent::mouseUp(const twPoint& pt, twMouseButton button){
	if(tw_capture)
		tw_capture->onMouseUp(pt-tw_capture->getWndRect().loc(), button);
	else if(twGetDesktop())
		twGetDesktop()->onMouseUp(pt-twGetDesktop()->getWndRect().loc(), button);
}
void twEvent::mainLoop(){
	while(waitEvent());
}
void twEvent::fireTimer(twTimer *tm){
	if(!tm->isValid())
		return;
	tm->fire();
}	

twEvent::twEvent(){
	
}
twEvent::~twEvent(){
	
}

class twOneTimeTimerWithInvocation : public twTimerWithInvocation{
protected:
	virtual void fire(){
		invalidate();
		twTimerWithInvocation::fire();
		delete this;
	}
public:
	twOneTimeTimerWithInvocation(twInvocation *invocation)
	:twTimerWithInvocation(invocation){}
};
void twEvent::invokeDelayed(twInvocation *invocation, int delay){
	twOneTimeTimerWithInvocation *timer;
	timer=new twOneTimeTimerWithInvocation(invocation);
	timer->setInterval(delay);
	timer->addToEvent(this);
}

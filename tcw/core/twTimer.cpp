/*
 *  twTimer.cpp
 *  Evaluator
 *
 *  Created by Nexhawks on 12/29/10.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#include <tcw/twTimer.h>
#include <tcw/twException.h>
#include <tcw/twEvent.h>

twTimer::twTimer(){
	event=NULL;
	interval=1000;
}
twTimer::~twTimer(){
	invalidate();
}

void twTimer::addToEvent(twEvent *ev){
	if(event)
		throw twException(L"the timer is already added to an event.");
	event=ev;
	event->addTimer(this);
}
void twTimer::invalidate(){
	if(!event)
		return;
	event->removeTimer(this);
	event=NULL;
}
void twTimer::setInterval(int iv){
	interval=iv;
	if(event)
		event->updateTimer(this);
}

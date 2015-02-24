/*
 *  twTimer.h
 *  Evaluator
 *
 *  Created by Nexhawks on 12/29/10.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#pragma once
#include <tcw/twEvent.h>

class twTimer{
	friend class twEvent;
	friend class twSDLEvent;
protected:
	twEvent *event;
	int interval;
	virtual void fire()=0;
	
public:
	
	bool isValid() const{return event;}
	
	void addToEvent(twEvent *ev);
	void invalidate();
	void setInterval(int);
	int getInterval() const{return interval;};
	
	twTimer();
	virtual ~twTimer();
};	

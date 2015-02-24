/*
 *  twTimerWithInvocation.h
 *  XTBook
 *
 *  Created by Nexhawks on 2/12/11.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#pragma once
#include <tcw/twTimer.h>

class twTimerWithInvocation : public twTimer{
protected:
	twInvocation *invocation;
	virtual void fire();
public:
	twTimerWithInvocation(twInvocation *);
	virtual ~twTimerWithInvocation();
};

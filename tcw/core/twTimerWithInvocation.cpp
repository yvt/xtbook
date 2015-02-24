/*
 *  twTimerWithInvocation.cpp
 *  XTBook
 *
 *  Created by Nexhawks on 2/12/11.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#include <tcw/twTimerWithInvocation.h>
#include <tcw/twInvocation.h>

twTimerWithInvocation::twTimerWithInvocation(twInvocation * inv){
	invocation=inv;
}

twTimerWithInvocation::~twTimerWithInvocation(){
	delete invocation;
}

void twTimerWithInvocation::fire(){
	invocation->invoke();
}


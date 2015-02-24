/*
 *  twSDLSemaphore.h
 *  XTBook
 *
 *  Created by Nexhawks on 2/20/11.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#include <tcw/twSemaphore.h>
#include <tcw/twSDLEvent.h>

#pragma once

class twSDLSemaphore : public twSemaphore{
protected:
	SDL_sem *m_sem;
public:
	twSDLSemaphore(int iValue=1);
	virtual ~twSDLSemaphore();
	virtual void wait();
	virtual void post();	
};

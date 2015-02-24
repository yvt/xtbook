/*
 *  twSemaphore.h
 *  XTBook
 *
 *  Created by Nexhawks on 2/20/11.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#pragma once

#include <tcw/twLockable.h>

class twSemaphore : public twLockable{
public:
	static twSemaphore *create(int);
	virtual ~twSemaphore(){}
	virtual void wait()=0;
	virtual void post()=0;
	virtual void lock();
	virtual void unlock();
};

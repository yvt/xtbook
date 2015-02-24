/*
 *  twLock.h
 *  XTBook
 *
 *  Created by Nexhawks on 2/20/11.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#pragma once

#include <tcw/twLockable.h>

class twLock{
protected:
	twLockable *m_lockable;
public:
	twLock(twLockable *lockable){
		m_lockable=lockable;
		m_lockable->lock();
	}
	~twLock(){
		m_lockable->unlock();
	}
};

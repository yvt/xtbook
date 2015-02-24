/*
 *  twLockable.h
 *  XTBook
 *
 *  Created by Nexhawks on 2/20/11.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#pragma once

class twLockable{
public:
	virtual ~twLockable(){}
	virtual void lock()=0;
	virtual void unlock()=0;
};

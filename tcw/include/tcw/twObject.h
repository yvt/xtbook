/*
 *  twObject.h
 *  XTBook
 *
 *  Created by Nexhawks on 2/20/11.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#pragma once

#include <list>

class twInvocation;
class twSDLSemaphore;

class twObject{
protected:
	std::list<twInvocation *> m_invocations;
	twSDLSemaphore *m_invocationSemaphore;
	void invalidateAllAssociatedInvocations();
public:
	twObject();
	virtual ~twObject();
	void associateToInvocation(twInvocation *);
	void detachFromInvocation(twInvocation *);
};

/*
 *  twObject.cpp
 *  XTBook
 *
 *  Created by Nexhawks on 2/20/11.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#include <tcw/twObject.h>
#include <tcw/twSDLSemaphore.h>
#include <tcw/twInvocation.h>

twObject::twObject(){
	m_invocationSemaphore=new twSDLSemaphore();
}	

twObject::~twObject(){
	invalidateAllAssociatedInvocations();
	
	delete m_invocationSemaphore;
}

void twObject::invalidateAllAssociatedInvocations(){
	m_invocationSemaphore->wait();
	for(std::list<twInvocation *>::iterator it=m_invocations.begin();it!=m_invocations.end();it++){
		//printf("invalidate 0x%08x\n", (int)(*it));
		(*it)->m_valid=false;
	}
	m_invocationSemaphore->post();
}

void twObject::associateToInvocation(twInvocation *inv){
	m_invocationSemaphore->wait();
	//printf("associate 0x%08x\n", (int)inv);
	m_invocations.push_back(inv);
	m_invocationSemaphore->post();
}
void twObject::detachFromInvocation(twInvocation *inv){
	m_invocationSemaphore->wait();
	for(std::list<twInvocation *>::iterator it=m_invocations.begin();it!=m_invocations.end();it++){
		if(*it==inv){
			//printf("detouch 0x%08x\n", (int)inv);
			m_invocations.erase(it);
			break;
		}
	}
	m_invocationSemaphore->post();
}

/*
 *  twSDLSemaphore.cpp
 *  XTBook
 *
 *  Created by Nexhawks on 2/20/11.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#include <tcw/twSDLSemaphore.h>
#include <tcw/twException.h>
#include <cassert>
twSDLSemaphore::twSDLSemaphore(int initialValue){
	m_sem=SDL_CreateSemaphore(initialValue);
	if(!m_sem){
		throw twException(L"twSDLSemaphore: SDL_CreateSemaphore failed");
	}
}
twSDLSemaphore::~twSDLSemaphore(){
	SDL_DestroySemaphore(m_sem);
}
void twSDLSemaphore::wait(){
	//printf("wait: 0x%08x\n", (int)this);
	assert(this);
	assert(m_sem);
	SDL_SemWait(m_sem);
}
void twSDLSemaphore::post(){
	//printf("post: 0x%08x\n", (int)this);
	assert(this);
	assert(m_sem);
	SDL_SemPost(m_sem);
}

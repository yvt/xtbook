/*
 *  twSemaphore.cpp
 *  XTBook
 *
 *  Created by Nexhawks on 2/20/11.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#include <tcw/twSemaphore.h>
#include <tcw/twSDLSemaphore.h>

twSemaphore *twSemaphore::create(int i){
	return new twSDLSemaphore(i);
}

void twSemaphore::lock(){
	wait();
}

void twSemaphore::unlock(){
	post();
}

/*
 *  twInvocation.cpp
 *  
 *
 *  Created by Nexhawks on 2/12/11.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#include <tcw/twInvocation.h>

twInvocation::~twInvocation(){
	if(m_associatedObject && m_valid){
		m_associatedObject->detachFromInvocation(this);
	}
}

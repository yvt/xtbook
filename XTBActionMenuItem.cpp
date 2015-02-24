//
//  XTBActionMenuItem.cpp
//  XTBook
//
//  Created by Kawada Tomoaki on 5/19/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "XTBActionMenuItem.h"

XTBActionMenuItem::XTBActionMenuItem(){
	m_enable=true;
}

bool XTBActionMenuItem::isSelectable() const{
	return m_enable;
}

//
//  XTBActionMenuItem.h
//  XTBook
//
//  Created by Kawada Tomoaki on 5/19/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#pragma once

#include "XTBMenuItem.h"

class XTBActionMenuItem: public XTBMenuItem{
private:
protected:
	bool m_enable;
public:
	XTBActionMenuItem();
	virtual bool isSelectable() const;
	
	bool isEnable() const{return m_enable;}
	void setEnable(bool v){m_enable=v;}
};


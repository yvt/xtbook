/*
 *  twTopWnd.h
 *  
 *
 *  Created by Nexhawks on 10/01/30.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#pragma once
#include <tcw/twWnd.h>

class twTopWnd : public twWnd{
public:
	twTopWnd();
	
	virtual void clientPaint(const twPaintStruct&);
	virtual bool clientHitTest(const twPoint&) const;
};

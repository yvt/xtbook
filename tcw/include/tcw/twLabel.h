/*
 *  twLabel.h
 *  
 *
 *  Created by Nexhawks on 10/01/19.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#pragma once

#include <tcw/twWnd.h>

class twLabel : public twWnd{
public:
	twLabel();
	virtual ~twLabel();
	
	virtual void clientPaint(const twPaintStruct&);
	virtual bool clientHitTest(const twPoint&) const;
	
};

//
//  XTBOverlay.h
//  XTBook
//
//  Created by Nexhawks on 4/21/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#pragma once

#include <tcw/twDialog.h>

class XTBOverlay: public twDialog{
public:
	XTBOverlay();
	virtual ~XTBOverlay();
	virtual void clientPaint(const twPaintStruct&);
	virtual bool clientHitTest(const twPoint&) const;
};


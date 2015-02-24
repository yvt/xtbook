//
//  XTBSearchErrorView.h
//  XTBook
//
//  Created by Kawada Tomoaki on 6/1/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#pragma once

#include <tcw/twWnd.h>

/** display an informational text to indicate
 * the error while search. */
class XTBSearchErrorView: public twWnd{
public:
	XTBSearchErrorView();
	virtual ~XTBSearchErrorView();
	
	virtual void clientPaint(const twPaintStruct&);
	virtual bool clientHitTest(const twPoint&) const;
};

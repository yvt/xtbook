//
//  twDimmer.h
//  XTBook
//
//  Created by Nexhawks on 4/20/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#pragma once

#include <tcw/twWnd.h>
#include <tcw/twInvocation.h>

enum twDimmerStyle{
	twDS_dark=0,
	twDS_normal
};

class twDimmer: public twWnd{
protected:
	twDC *m_dimmedDC;
	twWnd *m_oldDesktop;
	twInvocation *m_invocation;
	twDimmerStyle m_style;
	twWnd *m_hidingWindow;
public:
	twDimmer(twDimmerStyle style, twWnd *oldDesktop, twInvocation *invocation);
	virtual ~twDimmer();
	
	virtual void clientPaint(const twPaintStruct&);
	virtual bool clientHitTest(const twPoint&) const;
	
	virtual void clientMouseDown(const twPoint&, twMouseButton);
	
	static twDimmer *openDimmer(twDimmerStyle, twInvocation *clickCallback=NULL);
	
	void setHidingWindow(twWnd *w){
		m_hidingWindow=w;
	}
};

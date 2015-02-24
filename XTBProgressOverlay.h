//
//  XTBProgressOverlay.h
//  XTBook
//
//  Created by Nexhawks on 4/21/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#pragma once

#include "XTBOverlay.h"
#include <tcw/twSemaphore.h>
#include <tcw/twInvocation.h>
#include <tcw/twTimer.h>

class XTBProgressOverlay: public XTBOverlay{
protected:
	static twDC *m_hourglassIcon;
	static twDC *getHourglassIcon();
	
	twSemaphore *m_msgMutex;
	twTimer *m_timer;
	std::wstring m_msg;
	float m_percentage;
	volatile bool m_modified;
	int m_segmentPosition;
private:
	void startAnimation();
	void stopAnimation();
	void timerFired();
public:
	XTBProgressOverlay();
	~XTBProgressOverlay();
	virtual void clientPaint(const twPaintStruct&);
	void setMessage(const std::wstring&);
	void setPercentage(float);
	
	virtual void show();
	virtual void hide();
};


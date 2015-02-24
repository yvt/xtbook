//
//  XTBDialogSheet.h
//  XTBook
//
//  Created by 河田 智明 on 8/2/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#pragma once

#include <tcw/twDialog.h>
#include <tcw/twTimerWithInvocation.h>
#include <tcw/twEvent.h>


class XTBDialogSheet: public twDialog{
	twTimerWithInvocation *m_appearAnimationTimer;
	twTicks m_appearStartTime;
	twTicks m_appearDuration;
	int m_startY;
	int m_endY;
	int m_paddingHeight;
	int m_lastSelectedIndex;
	int m_currentIndex;
	
	
	twRect finalRect() const;
	void startAppearAnimation();
	void stopAppearAnimation();
	void updateAppearAnimation();
	
protected:
	virtual twPoint getStartupPos(const twSize&,
								  const twRect&);
	
	virtual twSize startupSize() const=0;
	virtual twColor paddingColor() const{return 0xffffff;}
public:
	XTBDialogSheet();
	virtual ~XTBDialogSheet();
	
	virtual void willAppear();
	virtual void didDisappear();
	
	virtual void clientPaint(const twPaintStruct&);
	
};

/*
 *  XTBScrollView.h
 *  XTBook
 *
 *  Created by Nexhawks on 2/19/11.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#include <tcw/twWnd.h>
#include <tcw/twApp.h>

#pragma once
enum XTBScrollViewAnimationPhase{
	XTBScrollViewAnimationPhaseNone=0,
	XTBScrollViewAnimationPhaseScroll,
	XTBScrollViewAnimationPhaseMove,
	XTBScrollViewAnimationPhaseSpring	
};

enum{
	XTBScrollViewMouseHistoryMaxCount=3
};

class XTBScrollView : public twWnd{
private:
	twSize m_contentSize;
	twPoint m_contentLocation;
	XTBScrollViewAnimationPhase m_animationPhaseX;
	twTicks m_animationStartX;
	XTBScrollViewAnimationPhase m_animationPhaseY;
	twTicks m_animationStartY;
	twTimer *m_animationTimer;
	twPoint m_animationInitialLocation;
	twPoint m_animationFinalLocation;
	twTicks m_scrollDuration;
	twTicks m_bounceDuration;
	twTicks m_springDuration;
	twTicks m_moveDuration;
	twPoint m_oldMousePos[XTBScrollViewMouseHistoryMaxCount];
	twPoint m_firstMousePos;
	twPoint m_firstLocation;
	twTicks m_oldMouseTime[XTBScrollViewMouseHistoryMaxCount];
	int m_mouseHistoryCount;
	twTicks m_touchDelay;
	float m_velX, m_velY;
	bool m_drag;
	int m_touchSession;
	bool m_waitingTouch;
	bool m_touching;
	bool m_wasScrolling; // this prevents "tapping".
	void touchTimerFired(int);
	void timerFired();
	void startAnimation();
	void stopAnimation();

	bool recordMousePos(twPoint);
	
	bool m_clientDragging;
	bool m_waitingForDrag;
	twTicks m_dragDueTime;
	
protected:
	
	
	virtual bool isScrollableX() const;
	virtual bool isScrollableY() const;
	
public:
	XTBScrollView();
	virtual ~XTBScrollView();
	
	virtual void setRect(const twRect&);
	virtual void clientPaint(const twPaintStruct&);
	virtual bool clientHitTest(const twPoint&) const;
	virtual void clientMouseDown(const twPoint&, twMouseButton);
	virtual void clientMouseMove(const twPoint&);
	virtual void clientMouseUp(const twPoint&, twMouseButton);
	
	virtual void setContentLocation(const twPoint&);
	twPoint getContentLocation() const;
	twPoint getFinalContentLocation() const;
	twPoint getMaxContentLocation() const;
	virtual void setContentSize(const twSize&);
	twSize getContentSize() const;
	virtual void setContentLocationAnimated(const twPoint&);
	
	virtual void setScrollDuration(twTicks ticks){
		m_scrollDuration=ticks;
	}
	twTicks getScrollDuration() const{
		return m_scrollDuration;
	}
	
	// messages
	
	virtual void clientTouchStarted(const twPoint&, twMouseButton);
	virtual void clientTouchEnded(const twPoint&, twMouseButton);
	virtual void clientTouchCancelled(const twPoint&, twMouseButton);
	
	virtual void clientDragStarted(const twPoint&, twMouseButton);
	virtual void clientDragMoved(const twPoint&, twMouseButton);
	virtual void clientDragEnded(const twPoint&, twMouseButton);
	
	
	bool isAnimating() const;
	
	virtual void command(int);
	
	void readyForDrag(twTicks timeout);
};

/*
 *  XTBScrollView.cpp
 *  XTBook
 *
 *  Created by Nexhawks on 2/19/11.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#include "XTBScrollView.h"
#include <tcw/twInvocation.h>
#include <tcw/twTimerWithInvocation.h>
#include <tcw/twEvent.h>
#include <cmath>
#include <cassert>

using namespace std;

XTBScrollView::XTBScrollView(){
	m_contentSize=twSize(0, 0);
	m_contentLocation=twPoint(0, 0);
	m_animationPhaseX=XTBScrollViewAnimationPhaseNone;
	m_animationPhaseY=XTBScrollViewAnimationPhaseNone;
	m_animationTimer=NULL;
	m_scrollDuration=1300;
	m_bounceDuration=400;
	m_springDuration=300;
	m_moveDuration=500;
	m_drag=false;
	m_touching=false;
	m_touchSession=0;
	m_waitingTouch=false;
	m_touchDelay=200;
	
	twInvocation *invocation=new twNoArgumentMemberFunctionInvocation<XTBScrollView>
	(this, &XTBScrollView::timerFired);
	m_animationTimer=new twTimerWithInvocation(invocation);
	m_animationTimer->setInterval(10);
	
	m_waitingForDrag=false;
	m_clientDragging=false;
}

XTBScrollView::~XTBScrollView(){
	delete m_animationTimer;
}

twPoint XTBScrollView::getMaxContentLocation() const{
	twRect rt=getClientRect();
	twPoint value;
	if(m_contentSize.w>rt.w)
		value.x=m_contentSize.w-rt.w;
	else
		value.x=0;
	if(m_contentSize.h>rt.h)
		value.y=m_contentSize.h-rt.h;
	else
		value.y=0;
	return value;
}
bool XTBScrollView::isScrollableX() const{
	return getMaxContentLocation().x>0;
}
bool XTBScrollView::isScrollableY() const{
	return getMaxContentLocation().y>0;
}

void XTBScrollView::startAnimation(){
	if(m_animationTimer->isValid())
		return;
	
	m_animationTimer->addToEvent(tw_event);
}

void XTBScrollView::stopAnimation(){
	m_animationTimer->invalidate();
}

void XTBScrollView::setRect(const twRect& rt){
	twPoint maxContentLocation=getMaxContentLocation();
	twPoint contentLocation=getContentLocation();
	if(contentLocation.x>maxContentLocation.x){
		contentLocation.x=maxContentLocation.x;
	}	
	if(contentLocation.y>maxContentLocation.y){
		contentLocation.y=maxContentLocation.y;
	}	
	setContentLocation(contentLocation);
	twWnd::setRect(rt);
}
void XTBScrollView::clientPaint(const twPaintStruct&){
}
bool XTBScrollView::clientHitTest(const twPoint&) const{
	return true;
}
bool XTBScrollView::recordMousePos(twPoint pt){
	
	if(m_oldMousePos[0]==pt && m_mouseHistoryCount>0){
		// no movement.
		m_oldMouseTime[0]=twGetTicks();
		return false;
	}
	
	// move old histories
	for(int i=XTBScrollViewMouseHistoryMaxCount-1;i>0;i--){
		m_oldMousePos[i]=m_oldMousePos[i-1];
		m_oldMouseTime[i]=m_oldMouseTime[i-1];
	}
	m_oldMousePos[0]=pt;
	m_oldMouseTime[0]=twGetTicks();
	m_mouseHistoryCount++;
	if(m_mouseHistoryCount>XTBScrollViewMouseHistoryMaxCount)
		m_mouseHistoryCount=XTBScrollViewMouseHistoryMaxCount;
	
	return true;
}
void XTBScrollView::clientMouseDown(const twPoint& mousePos, twMouseButton mouseButton){
	if(mouseButton==twMB_wheelUp){
		twPoint contentLocation=getContentLocation();
		twPoint maxContentLocation=getMaxContentLocation();
		
		contentLocation.y-=128;
		if(contentLocation.y<0)
			contentLocation.y=0;
		if(contentLocation.y>maxContentLocation.y)
			contentLocation.y=maxContentLocation.y;
		stopAnimation();
		setContentLocation(contentLocation);
		
		return;
	}
	if(mouseButton==twMB_wheelDown){
		twPoint contentLocation=getContentLocation();
		twPoint maxContentLocation=getMaxContentLocation();
		
		contentLocation.y+=128;
		if(contentLocation.y<0)
			contentLocation.y=0;
		if(contentLocation.y>maxContentLocation.y)
			contentLocation.y=maxContentLocation.y;
		stopAnimation();
		setContentLocation(contentLocation);
		
		return;
	}
	if(mouseButton==twMB_left){
		m_wasScrolling=false;
		if(m_animationPhaseX!=XTBScrollViewAnimationPhaseNone && m_animationPhaseX!=XTBScrollViewAnimationPhaseSpring){
			if(ABS(m_animationFinalLocation.x-m_contentLocation.x)>30)
				m_wasScrolling=true;
		}
		if(m_animationPhaseY!=XTBScrollViewAnimationPhaseNone && m_animationPhaseY!=XTBScrollViewAnimationPhaseSpring){
			if(ABS(m_animationFinalLocation.y-m_contentLocation.y)>30)
			m_wasScrolling=true;
		}
		
		stopAnimation();
		
		m_animationPhaseX=XTBScrollViewAnimationPhaseNone;
		m_animationPhaseY=XTBScrollViewAnimationPhaseNone;
		
		if(m_waitingForDrag && twGetTicks()<m_dragDueTime){
			// begin client drag.
			m_clientDragging=true;
			m_waitingForDrag=false;
			clientDragStarted(mousePos, mouseButton);
			twSetCapture(this);
			return;
		}
		
		twPoint maxContentLocation=getMaxContentLocation();
		twPoint contentLocation=getContentLocation();
		if(contentLocation.x>maxContentLocation.x){
			contentLocation.x=maxContentLocation.x;
		}	
		if(contentLocation.y>maxContentLocation.y){
			contentLocation.y=maxContentLocation.y;
		}	
		if(contentLocation.x<0)
			contentLocation.x=0;
		if(contentLocation.y<0)
			contentLocation.y=0;
		
		
		setContentLocation(contentLocation);
		m_animationInitialLocation=contentLocation;
		m_animationFinalLocation=contentLocation;
		
		m_mouseHistoryCount=0;
		recordMousePos(mousePos);
		m_firstMousePos=mousePos;
		m_firstLocation=getContentLocation();
		m_drag=true;
		m_touching=false;
		m_waitingTouch=true;
		m_touchSession=rand()+1;
		
		tw_event->invokeDelayed(new twOneArgumentMemberFunctionInvocation<XTBScrollView, int>(this, &XTBScrollView::touchTimerFired, m_touchSession),
								m_touchDelay);
		
		
	}
	twSetCapture(this);
}
void XTBScrollView::clientMouseMove(const twPoint& mousePos){
	if(m_clientDragging){
		clientDragMoved(mousePos, twMB_left);
		return;
	}
	if(m_drag){
		
		twPoint maxContentLocation=getMaxContentLocation();
		twPoint contentLocation=m_firstLocation;
		
		if(ABS(m_firstMousePos.x-mousePos.x)<7 &&
		   ABS(m_firstMousePos.y-mousePos.y)<7){
			if(m_waitingTouch || m_touching){
				return;
			}
		}
		
		if(m_waitingTouch){
			m_waitingTouch=false;
		}
		if(m_touching){
			clientTouchCancelled(mousePos, twMB_left);
			m_touching=false;
		}
		
		contentLocation.x-=mousePos.x-m_firstMousePos.x;
		contentLocation.y-=mousePos.y-m_firstMousePos.y;
#if 0
		// this violates Apple's patent
		if(contentLocation.x>maxContentLocation.x){
			contentLocation.x=(maxContentLocation.x+contentLocation.x)/2;
		}	
		if(contentLocation.y>maxContentLocation.y){
			contentLocation.y=(maxContentLocation.y+contentLocation.y)/2;
		}	
		if(contentLocation.x<0)
			contentLocation.x/=2;
		if(contentLocation.y<0)
			contentLocation.y/=2;
#else
		
		if(contentLocation.x>maxContentLocation.x){
			contentLocation.x=maxContentLocation.x;
		}
		if(contentLocation.y>maxContentLocation.y){
			contentLocation.y=maxContentLocation.y;
		}
		if(contentLocation.x<0)
			contentLocation.x = 0;
		if(contentLocation.y<0)
			contentLocation.y = 0;
#endif
		
		if(!isScrollableX())
			contentLocation.x=0;
		if(!isScrollableY())
			contentLocation.y=0;
		
		setContentLocation(contentLocation);
		m_animationInitialLocation=contentLocation;
		
		float velX, velY;
		size_t historyIndex=m_mouseHistoryCount-1;
		twPoint oldMousePos=m_oldMousePos[historyIndex];
		twTicks oldMouseTime=m_oldMouseTime[historyIndex];
		
		velX=(float)(mousePos.x-oldMousePos.x)/(float)(twGetTicks()-oldMouseTime+1)*1000.f;
		velY=(float)(mousePos.y-oldMousePos.y)/(float)(twGetTicks()-oldMouseTime+1)*1000.f;
		
		if(!isScrollableX())
			velX=0.f;
		if(!isScrollableY())
			velY=0.f;
		
		m_velX=velX; m_velY=velY;
		
		float scrollDuration=(float)m_scrollDuration/1000.f;
		
		m_animationFinalLocation.x=(int)((float)contentLocation.x-velX*scrollDuration/2.f);
		m_animationFinalLocation.y=(int)((float)contentLocation.y-velY*scrollDuration/2.f);
		
		
		recordMousePos(mousePos);
	}
}
void XTBScrollView::clientMouseUp(const twPoint& mousePos, twMouseButton mouseButton){
	if(mouseButton==twMB_left && m_clientDragging){
		clientDragEnded(mousePos, mouseButton);
		m_clientDragging=false;
		
	}
	if(mouseButton==twMB_left && m_drag){
		
		twPoint maxContentLocation=getMaxContentLocation();
		m_animationStartX=twGetTicks();
		m_animationStartY=twGetTicks();
		m_drag=false;
		
		if(m_waitingTouch){
			if(!m_wasScrolling){
				clientTouchStarted(mousePos, twMB_left);
				twReleaseCapture();
				clientTouchEnded(mousePos, twMB_left);
			}
			m_waitingTouch=false;
			m_touching=false;
			return;
		}
		if(m_touching){
			twReleaseCapture();
			m_touching=false;
			clientTouchEnded(mousePos, twMB_left);
			return;
		}
		
		if((twGetTicks()-m_oldMouseTime[0])<200 &&
		   m_mouseHistoryCount==XTBScrollViewMouseHistoryMaxCount){
			// initate animated scroll
			if(m_animationInitialLocation.x<0 || m_animationInitialLocation.x>maxContentLocation.x){
				m_animationPhaseX=XTBScrollViewAnimationPhaseSpring;
				startAnimation();
			}else if(ABS(m_animationFinalLocation.x-m_animationInitialLocation.x)>4){
				m_animationPhaseX=XTBScrollViewAnimationPhaseScroll;
				startAnimation();
			}
			if(m_animationInitialLocation.y<0 || m_animationInitialLocation.y>maxContentLocation.y){
				m_animationPhaseY=XTBScrollViewAnimationPhaseSpring;
				startAnimation();
			}else if(ABS(m_animationFinalLocation.y-m_animationInitialLocation.y)>4){
				m_animationPhaseY=XTBScrollViewAnimationPhaseScroll;
				startAnimation();
			}
		}else{
			// initate spring scroll
			if(m_animationInitialLocation.x<0 || m_animationInitialLocation.x>maxContentLocation.x){
				m_animationPhaseX=XTBScrollViewAnimationPhaseSpring;
				startAnimation();
			}
			if(m_animationInitialLocation.y<0 || m_animationInitialLocation.y>maxContentLocation.y){
				m_animationPhaseY=XTBScrollViewAnimationPhaseSpring;
				startAnimation();
			}
		}
		
	}
	twReleaseCapture();
}

void XTBScrollView::touchTimerFired(int session){
	if(session!=m_touchSession)
		return;
	
	if(!m_waitingTouch)
		return;
	
	if(m_touching)
		return;
	
	m_touching=true;
	m_waitingTouch=false;
	clientTouchStarted(m_firstMousePos, twMB_left);
	
}
	

void XTBScrollView::setContentLocation(const twPoint& newLocation){
	twPoint delta=newLocation-m_contentLocation;
	if(delta.x==0 && delta.y==0)
		return;
	for(std::list<twWnd *>::iterator it=m_children.begin();it!=m_children.end();it++){
		twWnd *child=*it;
		twRect oldRect=child->getRect();
		oldRect.x-=delta.x;
		oldRect.y-=delta.y;
		child->setRect(oldRect);
	}
	m_contentLocation=newLocation;
}
void XTBScrollView::setContentLocationAnimated(const twPoint& newLocation){
	twPoint maxContentLocation=getMaxContentLocation();
	twPoint contentLocation=newLocation;
	if(contentLocation.x>maxContentLocation.x){
		contentLocation.x=maxContentLocation.x;
	}	
	if(contentLocation.y>maxContentLocation.y){
		contentLocation.y=maxContentLocation.y;
	}	
	if(contentLocation.x<0)
		contentLocation.x=0;
	if(contentLocation.y<0)
		contentLocation.y=0;
	startAnimation();
	m_animationPhaseX=XTBScrollViewAnimationPhaseMove;
	m_animationPhaseY=XTBScrollViewAnimationPhaseMove;
	m_animationStartX=twGetTicks();
	m_animationStartY=twGetTicks();
	m_animationInitialLocation=getContentLocation();
	m_animationFinalLocation=contentLocation;
}
twPoint XTBScrollView::getContentLocation() const{
	return m_contentLocation;
}
twPoint XTBScrollView::getFinalContentLocation() const{
	twPoint finalLocation=m_contentLocation;
	twPoint maxLocation=getMaxContentLocation();
	if(m_animationPhaseX!=XTBScrollViewAnimationPhaseNone){
		finalLocation.x=m_animationFinalLocation.x;
		if(finalLocation.x<0)
			finalLocation.x=0;
		if(finalLocation.x>maxLocation.x)
			finalLocation.x=maxLocation.x;
	}
	if(m_animationPhaseY!=XTBScrollViewAnimationPhaseNone){
		finalLocation.y=m_animationFinalLocation.y;
		if(finalLocation.y<0)
			finalLocation.y=0;
		if(finalLocation.y>maxLocation.y)
			finalLocation.y=maxLocation.y;
	}
	return finalLocation;
}

void XTBScrollView::setContentSize(const twSize& newsize){
	m_contentSize=newsize;
	twPoint maxContentLocation=getMaxContentLocation();
	twPoint contentLocation=getContentLocation();
	if(contentLocation.x>maxContentLocation.x){
		contentLocation.x=maxContentLocation.x;
	}	
	if(contentLocation.y>maxContentLocation.y){
		contentLocation.y=maxContentLocation.y;
	}	
	setContentLocation(contentLocation);
}
twSize XTBScrollView::getContentSize() const{
	return m_contentSize;
}


void XTBScrollView::timerFired(){
	twPoint contentLocation=getContentLocation();
	twPoint maxContentLocation=getMaxContentLocation();
	bool stillAnimatingX=false;
	bool stillAnimatingY=false;
	twTicks tick=twGetTicks();
	
	if(m_animationPhaseX==XTBScrollViewAnimationPhaseScroll){
		float time=(float)(tick-m_animationStartX); // milliseconds
		float per=time/(float)m_scrollDuration;
		contentLocation.x=m_animationInitialLocation.x;
		if(per<1.f)
			stillAnimatingX=true;
		else
			per=1.f;
		contentLocation.x+=(float)(m_animationFinalLocation.x-m_animationInitialLocation.x)*(1.f-(1.f-per)*(1.f-per));
		// do bounce scroll
		if(contentLocation.x<0){
			float initialLocation=(float)(m_animationInitialLocation.x);
			float finalLocation=(float)(m_animationFinalLocation.x);
			float hitLocation=0.f;
			float hitInvPer=finalLocation-hitLocation;
			hitInvPer/=finalLocation-initialLocation;
			hitInvPer=fabsf(sqrtf(hitInvPer));
			
			float hitTime=(1.f-hitInvPer)*(float)m_scrollDuration;
			float hitVel=m_velX*hitInvPer;
			float timeSinceHit=time-hitTime;
			
			if(timeSinceHit<0.f)	// should not happen
				timeSinceHit=0.f; 
			if(timeSinceHit>m_bounceDuration){
				// already converged
				contentLocation.x=(int)hitLocation;
				stillAnimatingX=false;
			}else{
				// bounce
				float bouncePer=timeSinceHit/(float)m_bounceDuration;
				bouncePer*=(bouncePer-1.f)*(1.f-bouncePer)*(1.f-bouncePer)*.5f;
				
				float distance;
				distance=hitVel*(float)m_bounceDuration/1000.f;
				distance*=bouncePer;
				distance+=hitLocation;
				contentLocation.x=(int)distance;
				stillAnimatingX=true;
			}
		}
		else if(contentLocation.x>maxContentLocation.x){
			float initialLocation=(float)(m_animationInitialLocation.x);
			float finalLocation=(float)(m_animationFinalLocation.x);
			float hitLocation=maxContentLocation.x;
			float hitInvPer=finalLocation-hitLocation;
			hitInvPer/=finalLocation-initialLocation;
			hitInvPer=fabsf(sqrtf(hitInvPer));
			
			float hitTime=(1.f-hitInvPer)*(float)m_scrollDuration;
			float hitVel=m_velX*hitInvPer;
			float timeSinceHit=time-hitTime;
			
			if(timeSinceHit<0.f)	// should not happen
				timeSinceHit=0.f; 
			if(timeSinceHit>m_bounceDuration){
				// already converged
				contentLocation.x=(int)hitLocation;
				stillAnimatingX=false;
			}else{
				// bounce
				float bouncePer=timeSinceHit/(float)m_bounceDuration;
				bouncePer*=(bouncePer-1.f)*(1.f-bouncePer)*(1.f-bouncePer)*.5f;
				
				float distance;
				distance=hitVel*(float)m_bounceDuration/1000.f;
				distance*=bouncePer;
				distance+=hitLocation;
				contentLocation.x=(int)distance;
				stillAnimatingX=true;
			}
		}
	}else if(m_animationPhaseX==XTBScrollViewAnimationPhaseMove){
		float time=(float)(tick-m_animationStartX); // milliseconds
		float per=time/(float)m_moveDuration;
		contentLocation.x=m_animationInitialLocation.x;
		if(per<1.f)
			stillAnimatingX=true;
		else
			per=1.f;
		if(per<.5f)
			per=per*per*2.f;
		else
			per=1.f-(1.f-per)*(1.f-per)*2.f;
		contentLocation.x+=(float)(m_animationFinalLocation.x-m_animationInitialLocation.x)*per;
	}else if(m_animationPhaseX==XTBScrollViewAnimationPhaseSpring){
		float time=(float)(tick-m_animationStartX); // milliseconds
		float per=time/(float)m_springDuration;
		contentLocation.x=m_animationInitialLocation.x;
		if(per<1.f)
			stillAnimatingX=true;
		else
			per=1.f;
		per=1.f-(1.f-per)*(1.f-per);
		contentLocation.x+=(float)(MAX(MIN(m_animationInitialLocation.x, maxContentLocation.x), 0)
								   -m_animationInitialLocation.x)*per;
	}
	
	
	
	if(m_animationPhaseY==XTBScrollViewAnimationPhaseScroll){
		float time=(float)(tick-m_animationStartY); // milliseconds
		float per=time/(float)m_scrollDuration;
		contentLocation.y=m_animationInitialLocation.y;
		if(per<1.f)
			stillAnimatingY=true;
		else
			per=1.f;
		contentLocation.y+=(float)(m_animationFinalLocation.y-m_animationInitialLocation.y)*(1.f-(1.f-per)*(1.f-per));
		// do bounce scroll
		if(contentLocation.y<0){
			float initialLocation=(float)(m_animationInitialLocation.y);
			float finalLocation=(float)(m_animationFinalLocation.y);
			float hitLocation=0.f;
			float hitInvPer=finalLocation-hitLocation;
			hitInvPer/=finalLocation-initialLocation;
			hitInvPer=fabsf(sqrtf(hitInvPer));
			
			float hitTime=(1.f-hitInvPer)*(float)m_scrollDuration;
			float hitVel=m_velY*hitInvPer;
			float timeSinceHit=time-hitTime;
			
			if(timeSinceHit<0.f)	// should not happen
				timeSinceHit=0.f; 
			if(timeSinceHit>m_bounceDuration){
				// already converged
				contentLocation.y=(int)hitLocation;
				stillAnimatingY=false;
			}else{
				// bounce
				float bouncePer=timeSinceHit/(float)m_bounceDuration;
				bouncePer*=(bouncePer-1.f)*(1.f-bouncePer)*(1.f-bouncePer)*.5f;
				
				float distance;
				distance=hitVel*(float)m_bounceDuration/1000.f;
				distance*=bouncePer;
				distance+=hitLocation;
				contentLocation.y=(int)distance;
				stillAnimatingY=true;
			}
		}
		else if(contentLocation.y>maxContentLocation.y){
			float initialLocation=(float)(m_animationInitialLocation.y);
			float finalLocation=(float)(m_animationFinalLocation.y);
			float hitLocation=maxContentLocation.y;
			float hitInvPer=finalLocation-hitLocation;
			hitInvPer/=finalLocation-initialLocation;
			hitInvPer=fabsf(sqrtf(hitInvPer));
			
			float hitTime=(1.f-hitInvPer)*(float)m_scrollDuration;
			float hitVel=m_velY*hitInvPer;
			float timeSinceHit=time-hitTime;
			
			if(timeSinceHit<0.f)	// should not happen
				timeSinceHit=0.f; 
			if(timeSinceHit>m_bounceDuration){
				// already converged
				contentLocation.y=(int)hitLocation;
				stillAnimatingY=false;
			}else{
				// bounce
				float bouncePer=timeSinceHit/(float)m_bounceDuration;
				bouncePer*=(bouncePer-1.f)*(1.f-bouncePer)*(1.f-bouncePer)*.5f;
				
				float distance;
				distance=hitVel*(float)m_bounceDuration/1000.f;
				distance*=bouncePer;
				distance+=hitLocation;
				contentLocation.y=(int)distance;
				stillAnimatingY=true;
			}
		}
	}else if(m_animationPhaseY==XTBScrollViewAnimationPhaseMove){
		float time=(float)(tick-m_animationStartY); // milliseconds
		float per=time/(float)m_moveDuration;
		contentLocation.y=m_animationInitialLocation.y;
		if(per<1.f)
			stillAnimatingY=true;
		else
			per=1.f;
		if(per<.5f)
			per=per*per*2.f;
		else
			per=1.f-(1.f-per)*(1.f-per)*2.f;
		contentLocation.y+=(float)(m_animationFinalLocation.y-m_animationInitialLocation.y)*per;
	}else if(m_animationPhaseY==XTBScrollViewAnimationPhaseSpring){
		float time=(float)(tick-m_animationStartY); // milliseconds
		float per=time/(float)m_springDuration;
		contentLocation.y=m_animationInitialLocation.y;
		if(per<1.f)
			stillAnimatingY=true;
		else
			per=1.f;
		per=1.f-(1.f-per)*(1.f-per)*(1.f-per);
		contentLocation.y+=(float)(MAX(MIN(m_animationInitialLocation.y, maxContentLocation.y), 0)
								   -m_animationInitialLocation.y)*per;
	}
	
	setContentLocation(contentLocation);
	
	if(!(stillAnimatingX || stillAnimatingY))
		stopAnimation();
	
}

bool XTBScrollView::isAnimating() const{
	if(m_animationPhaseX!=XTBScrollViewAnimationPhaseNone)
		return true;
	if(m_animationPhaseY!=XTBScrollViewAnimationPhaseNone)
		return true;
	
	return false;
}

void XTBScrollView::readyForDrag(twTicks timeout){
	m_waitingForDrag=true;
	m_dragDueTime=twGetTicks()+timeout;
}

#pragma mark -
#pragma mark Event Handler

void XTBScrollView::clientTouchStarted(const twPoint&, twMouseButton){
}
void XTBScrollView::clientTouchEnded(const twPoint&, twMouseButton){
}
void XTBScrollView::clientTouchCancelled(const twPoint&, twMouseButton){
}

void XTBScrollView::clientDragStarted(const twPoint &, twMouseButton){
	
}
void XTBScrollView::clientDragMoved(const twPoint &, twMouseButton){
	
}
void XTBScrollView::clientDragEnded(const twPoint &, twMouseButton){
	
}

void XTBScrollView::command(int id){
	if(getParent())
		getParent()->command(id);
}





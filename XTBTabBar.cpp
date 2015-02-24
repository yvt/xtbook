/*
 *  XTBTabBar.cpp
 *  XTBook
 *
 *  Created by Nexhawks on 2/21/11.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#include "XTBTabBar.h"
#include "XTBTabPage.h"
#include <tcw/twBaseSkin.h>
#include <tcw/twTimer.h>
#include <tcw/twTimerWithInvocation.h>
#include <tcw/twInvocation.h>
#include <tcw/twEvent.h>
#include "xpms/ConfigButtonIcon.xpm"
//#include "xpms/RemoveIcon.xpm"
#include "xpms/ConfigButtonIconPress.xpm"
//#include "xpms/RemoveIconPress.xpm"
//#include "xpms/TabBarBg.xpm"
#include "xpms/TabBarTab.xpm"
#include <tcw/twSDLDC.h>
#include <math.h>
#include <assert.h>
#include <tcw/twFont.h>
#include "xpms/TabCloseButton.xpm"

#pragma mark - Utils

static std::wstring ellipsisStringToFitWithin(const std::wstring& str, const twFont *fnt, int maxWidth){
	std::wstring s;
	if(fnt->measure(str).w<=maxWidth)
		return str;
	
	const std::wstring ellipsisBack=L"...";
	int width=0;
	maxWidth-=fnt->measure(ellipsisBack).w;
	
	
	for(std::wstring::size_type n=0;n<str.size();n++){
		int newWidth=width+fnt->measure(std::wstring(str, n, 1)).w;
		if(newWidth>maxWidth)
			return s+ellipsisBack;
		s+=str[n];
		width=newWidth;
	}
	return str;
}

#pragma mark - Resources

twDC *XTBTabBar::m_tabBarBg=NULL;
twDC *XTBTabBar::getTabBarBg(){
	if(!m_tabBarBg){
		//m_tabBarBg=twSDLDC::loadFromXPM(TabBarBg);
	}
	return m_tabBarBg;
}

twDC *XTBTabBar::m_tabBarTab=NULL;
twDC *XTBTabBar::getTabBarTab(){
	if(!m_tabBarTab){
		m_tabBarTab=twSDLDC::loadFromXPM(TabBarTab);
	}
	return m_tabBarTab;
}

twDC *XTBTabBar::m_configButtonIcon=NULL;
twDC *XTBTabBar::getConfigButtonIcon(){
	if(!m_configButtonIcon){
		m_configButtonIcon=twSDLDC::loadFromXPM(ConfigButtonIcon);
	}
	return m_configButtonIcon;
}

twDC *XTBTabBar::m_configButtonIconPress=NULL;
twDC *XTBTabBar::getConfigButtonIconPress(){
	if(!m_configButtonIconPress){
		m_configButtonIconPress=twSDLDC::loadFromXPM(ConfigButtonIconPress);
	}
	return m_configButtonIconPress;
}

twDC *XTBTabBar::m_removeIcon=NULL;
twDC *XTBTabBar::getRemoveIcon(){
	assert(false);
	return NULL;
	/*
	if(!m_removeIcon){
		m_removeIcon=twSDLDC::loadFromXPM(RemoveIcon);
	}
	return m_removeIcon;*/
}

twDC *XTBTabBar::m_removeIconPress=NULL;
twDC *XTBTabBar::getRemoveIconPress(){
	assert(false);
	return NULL;
	/*
	if(!m_removeIconPress){
		m_removeIconPress=twSDLDC::loadFromXPM(RemoveIconPress);
	}
	return m_removeIconPress;*/
}

twDC *XTBTabBar::m_tabCloseButton=NULL;
twDC *XTBTabBar::getTabCloseButton(){
	if(!m_tabCloseButton){
		m_tabCloseButton=twSDLDC::loadFromXPM(TabCloseButton);
	}
	return m_tabCloseButton;
}

#pragma mark -
#pragma mark Lifecycle

XTBTabBar::XTBTabBar(){
	m_selectedIndex=0;
	m_dragMode=XTBTabBarDragModeNone;
	
	twInvocation *invocataion;
	invocataion=new twNoArgumentMemberFunctionInvocation<XTBTabBar>(this, &XTBTabBar::timerFired);
	m_animationTimer=new twTimerWithInvocation(invocataion);
	m_animationTimer->setInterval(10);
}
XTBTabBar::~XTBTabBar(){
	delete m_animationTimer;
}

#pragma mark -
#pragma mark Responder

void XTBTabBar::renderTab(int index, twDC *dc){
	twRect rt=rectForTabPageAtIndex(index);
	twWndStatus status=getStatus();
	bool pressed=false;
	if(index!=m_selectedIndex){
		status.focused=false;
		
	}else{
		pressed=true;
		if(m_dragMode==XTBTabBarDragModeMove){
			if(indexForPos(m_dragTabPos)==-1){
				return;
			}
		}
	}
	if(index==XTBTabBarConfigButton){
		if(m_dragMode==XTBTabBarDragModeConfigButton){
			if(m_hoverConfigButton)
				pressed=true;
		}else if(m_dragMode==XTBTabBarDragModeMove){
			if(indexForPos(m_dragTabPos)==-1){
				pressed=true;
			}
		}
	}
	/*
	if(getStatus().focused && index==m_selectedIndex){
		//tw_curSkin->drawFocusRect(dc, twRect(rt.x+3, rt.y+3, 10, 9));
	}*/
	if(index==XTBTabBarConfigButton){
		// removing tab with dragging it left is no longer supported.
		twDC *icon;
		/*if(m_dragMode==XTBTabBarDragModeMove){
			icon=(pressed?getRemoveIconPress():getRemoveIcon());
		}else{*/
			icon=(pressed?getConfigButtonIconPress():getConfigButtonIcon());
		//}
		dc->bitBlt(icon, twPoint(rt.x+1, rt.y+1), twRect(0, 0, 16, 16));
	}else{
		twRect srcRect;
		twDC *srcImg=getTabBarTab();
		
		srcRect.y=0; srcRect.h=16;
		
		int tabPos=m_tabPages[index]->getFinalTabPos();
		int selTabPos;
		if(m_dragMode==XTBTabBarDragModeMove)
			selTabPos=indexForPos(m_dragTabPos);
		else
			selTabPos=indexForPos(m_tabPages[m_selectedIndex]->getTabPos());
		//printf("%d: %d\n", index, tabPos);
		if(index==m_selectedIndex){
			selTabPos=tabPos;
		}
		if(tabPos<selTabPos){
			srcRect.x=3; 
			srcRect.w=7;
			dc->bitBlt(srcImg, twPoint(rt.x-5, rt.y), srcRect);
			if(tabPos==selTabPos-1){
				srcRect.x=22; 
				srcRect.w=7;
				dc->bitBlt(srcImg, twPoint(rt.x+rt.w-2, rt.y), srcRect);
			}else{
				int index2=index+1;
				if(index2==m_selectedIndex)
					index2++;
				if(index2<m_tabPages.size()){
					if(m_tabPages[index2]->getTabPos()>m_tabPages[index]->getTabPos()+1.1f){
						srcRect.x=22; 
						srcRect.w=7;
						dc->bitBlt(srcImg, twPoint(rt.x+rt.w-2, rt.y), srcRect);
					}
				}else{
					srcRect.x=22; 
					srcRect.w=7;
					dc->bitBlt(srcImg, twPoint(rt.x+rt.w-2, rt.y), srcRect);
				}
			}
		}else if(tabPos>selTabPos){
			if(tabPos==selTabPos+1){
				srcRect.x=3; 
				srcRect.w=7;
				dc->bitBlt(srcImg, twPoint(rt.x-5, rt.y), srcRect);
			}else{
				int index2=index-1;
				if(index2==m_selectedIndex)
					index2--;
				if(index2>=0 && m_tabPages[index2]->getTabPos()<m_tabPages[index]->getTabPos()-1.1f){
					srcRect.x=3; 
					srcRect.w=7;
					dc->bitBlt(srcImg, twPoint(rt.x-5, rt.y), srcRect);
				}else{
					srcRect.x=22; 
					srcRect.w=7;
					dc->bitBlt(srcImg, twPoint(rt.x-2, rt.y), srcRect);
				}
			}
			if(tabPos==m_tabPages.size()-1){
				srcRect.x=22; 
				srcRect.w=7;
				dc->bitBlt(srcImg, twPoint(rt.x+rt.w-2, rt.y), srcRect);
			}else{
				int index2=index+1;
				if(index2==m_selectedIndex)
					index2++;
				if(index2<m_tabPages.size()){
					if(m_tabPages[index2]->getTabPos()>m_tabPages[index]->getTabPos()+1.1f){
						srcRect.x=22; 
						srcRect.w=7;
						dc->bitBlt(srcImg, twPoint(rt.x+rt.w-2, rt.y), srcRect);
					}
				}else{
					srcRect.x=22; 
					srcRect.w=7;
					dc->bitBlt(srcImg, twPoint(rt.x+rt.w-2, rt.y), srcRect);
				}
			}
		}else{
			//assert(index==m_selectedIndex);
			srcRect.x=35; 
			srcRect.w=7;
			dc->bitBlt(srcImg, twPoint(rt.x-5, rt.y), srcRect);
		
			srcRect.x=54; 
			srcRect.w=7;
			dc->bitBlt(srcImg, twPoint(rt.x+rt.w-2, rt.y), srcRect);
		
			for(int x=rt.x+2;x<rt.w+rt.x-2;x+=12){
				dc->bitBlt(getTabBarTab(), twPoint(x, 0), twRect(42, 0, std::min(rt.w+rt.x-2-x, 12), 16));
			}
		}
		
		int closeWidth=0;
		
		// draw close button.
		if(countOfTabPages()>1){
			dc->bitBlt(getTabCloseButton(), twPoint(rt.x+3, rt.y),
					   twRect((index==m_selectedIndex)?12:0,
							  (m_dragMode==XTBTabBarDragModeClose
							   &&m_closeDragTabIndex==index
							   &&m_hoverClose)?16:0, 12, 16));
			closeWidth+=16;
		}
		
		// now render the text.
		std::wstring text=m_tabPages[index]->caption();
		const twFont *font=getFont();
		int maxWidth=rt.w-4-closeWidth;
		text=ellipsisStringToFitWithin(text, font, maxWidth);
		twSize siz=font->measure(text);
		twPoint pt(rt.x+(rt.w-siz.w)/2, rt.y+(rt.h-siz.h)/2+1);
		if(pt.x<rt.x+closeWidth)
			pt.x=rt.x+closeWidth;
		
		font->render(dc, twRGB(192, 192, 192), pt+twPoint(0, 1), text);
		
		font->render(dc, twRGB(0, 0, 0), pt+twPoint(0, 0), text);
		
	}
	
}
void XTBTabBar::clientPaint(const twPaintStruct& p){
	twDC *dc=p.dc;
	twRect rt(twPoint(0, 0), p.bound);
	//dc->fillRect(tw_curSkin->getCtrlColor(), rt);
	for(int x=0;x<rt.w;x+=12){
		dc->bitBlt(getTabBarTab(), twPoint(x, 0), twRect(10, 0, 12, 16));
	}
	
	renderTab(XTBTabBarConfigButton, dc);
	for(int i=0;i<m_tabPages.size();i++)
		if(i!=m_selectedIndex)
			renderTab(i, dc);
	
	if(m_tabPages.size())
		renderTab(m_selectedIndex, dc);
}
bool XTBTabBar::clientHitTest(const twPoint& point) const{
	return true;
}

void XTBTabBar::clientKeyDown(const std::wstring& keyName){
}

void XTBTabBar::clientMouseDown(const twPoint& mouseLocation, twMouseButton mouseButton){
	twTicks tick=twGetTicks();
	
	if(mouseButton==twMB_left){
		int closeIndex=closeIndexAtPoint(mouseLocation);
		if(closeIndexAtPoint(mouseLocation)!=XTBTabBarNotFound){
			m_dragMode=XTBTabBarDragModeClose;
			m_closeDragTabIndex=closeIndex;
			m_hoverClose=true;
			invalidate();
			twSetCapture(this);
			return;
		}
		
		
		int index=indexAtPoint(mouseLocation);
		select(index);
		m_firstMousePos=mouseLocation;
		if(index!=XTBTabBarNotFound && index!=XTBTabBarConfigButton){
			m_dragMode=XTBTabBarDragModePress;
			m_firstTabPos=getSelectedTabPage()->getTabPos();
			m_dragTabPos=m_firstTabPos;
		}else if(index==XTBTabBarConfigButton){
			m_dragMode=XTBTabBarDragModeConfigButton;
			m_hoverConfigButton=true;
		}
		invalidate();
	}
	
	if(twGetTicks()>tick+80){
		clientMouseUp(mouseLocation, mouseButton);
		return;
	}
	twSetCapture(this);
}
void XTBTabBar::clientMouseMove(const twPoint& mouseLocation){
	if(m_dragMode==XTBTabBarDragModePress){
		if(ABS(mouseLocation.x-m_firstMousePos.x)>6){
			m_dragMode=XTBTabBarDragModeMove;
		}
	}else if(m_dragMode==XTBTabBarDragModeMove){
		int deltaX=mouseLocation.x-m_firstMousePos.x;
		m_dragTabPos=m_firstTabPos+(float)deltaX/(float)getTabSize();
		
		relayoutTabPages();
		invalidate();
	}else if(m_dragMode==XTBTabBarDragModeConfigButton){
		bool newHover=indexAtPoint(mouseLocation)==XTBTabBarConfigButton;
		if(newHover!=m_hoverConfigButton){
			m_hoverConfigButton=newHover;
			invalidate();
		}
	}else if(m_dragMode==XTBTabBarDragModeClose){
		bool newHover=closeIndexAtPoint(mouseLocation)==
		m_closeDragTabIndex;
		if(newHover!=m_hoverClose){
			m_hoverClose=newHover;
			invalidate();
		}
	}
}
void XTBTabBar::clientMouseUp(const twPoint& mouseLocation, twMouseButton mouseButton){
	if(m_dragMode==XTBTabBarDragModeMove){
		XTBTabPage *tabPage=getSelectedTabPage();
		int newIndex=indexForPos(m_dragTabPos);
		if(newIndex==-1){
			removeTabPageAtIndex(m_selectedIndex);
		}else{
			m_tabPages.erase(m_tabPages.begin()+m_selectedIndex);
			m_selectedIndex=newIndex;
			if(m_selectedIndex==m_tabPages.size())
				m_tabPages.push_back(tabPage);
			else
				m_tabPages.insert(m_tabPages.begin()+indexForPos(m_dragTabPos), tabPage);
			tabPage->moveTabPos(m_dragTabPos, m_selectedIndex);
		}
		m_dragMode=XTBTabBarDragModeNone;
		relayoutTabPages();
		invalidate();
	}else if(m_dragMode==XTBTabBarDragModeConfigButton){
		if(m_hoverConfigButton){
			sendCmdToParent();
		}
		invalidate();
	}else if(m_dragMode==XTBTabBarDragModeClose){
		if(m_hoverClose){
			removeTabPageAtIndex(m_closeDragTabIndex);
			relayoutTabPages();
		}
		invalidate();
	}
	m_dragMode=XTBTabBarDragModeNone;
	twReleaseCapture();
}

#pragma mark -
#pragma mark Tab Management

int XTBTabBar::indexForPos(float pos) const{
	// !!!: removing tab with dragging it left is no longer supported.
	int minValue=(m_tabPages.size()>1)?0:0;
	return MIN(MAX((int)floorf(pos+.65f), minValue), (int)(m_tabPages.size()-1));
}

int XTBTabBar::indexForTabPage(XTBTabPage *tabPage) const{
	for(int n=0;n<m_tabPages.size();n++)
		if(m_tabPages[n]==tabPage)
			return n;
	return XTBTabBarNotFound;
}

void XTBTabBar::addTabPage(XTBTabPage *tabPage){
	m_tabPages.push_back(tabPage);
	if(m_tabPages.size()==1){
		m_selectedIndex=0;
		getSelectedTabPage()->enter();
	}
	tabPage->setTabPos(m_tabPages.size()-1, false);
	invalidate();
}
void XTBTabBar::select(XTBTabPage *tabPage){
	int index=indexForTabPage(tabPage);
	if(index!=XTBTabBarNotFound)
		this->select(index);
}
void XTBTabBar::select(int index){
	if(m_selectedIndex==index)
		return;
	if(index==XTBTabBarConfigButton){
		return;
	}
	if(index==XTBTabBarNotFound){
		return;
	}
	getSelectedTabPage()->leave();
	m_selectedIndex=index;
	getSelectedTabPage()->enter();
	invalidate();
}
XTBTabPage *XTBTabBar::getSelectedTabPage() const{
	if(m_tabPages.size()==0)
		return NULL;
	if(m_selectedIndex==XTBTabBarConfigButton)
		return NULL;
	return m_tabPages[m_selectedIndex];
}	
twRect XTBTabBar::rectForTabPageAtIndex(int index) const{
	if(index==XTBTabBarConfigButton)
		return twRect(0, 0, 16, 16);
	
	XTBTabPage *tabPage=m_tabPages[index];
	float pos=tabPage->getTabPos();
	if(index==m_selectedIndex && m_dragMode==XTBTabBarDragModeMove)
		pos=m_dragTabPos;
	int tabSize=getTabSize();
	return twRect(29+(int)((float)tabSize*pos), 0, tabSize, 16);
}
twRect XTBTabBar::rectForTabCloseButtonAtIndex(int index) const{
	
	if(countOfTabPages()==1){
		// cannot close without multiple tabs.
		return twRect(0,0,0,0);
	}
	
	twRect tabPageRect=rectForTabPageAtIndex(index);
	return twRect(tabPageRect.x+3, 0, 12, 16);
}
twRect XTBTabBar::rectForConfigButton() const{
	return rectForTabPageAtIndex(XTBTabBarConfigButton);
}
int XTBTabBar::getTabSize() const{
	twRect rt=getRect();
	if(m_tabPages.size()==0)
		return 19;
	return MIN((rt.w-42)/m_tabPages.size(), 150);
}
void XTBTabBar::removeTabPageAtIndex(int index){
	if(index==m_selectedIndex){
		getSelectedTabPage()->leave();
		delete m_tabPages[index];
		m_tabPages.erase(m_tabPages.begin()+index);
		if(m_tabPages.size()){
			if(index>=m_tabPages.size())
				index=m_tabPages.size()-1;
			m_selectedIndex=index;
			getSelectedTabPage()->enter();
		}
	}else{
		delete m_tabPages[index];
		m_tabPages.erase(m_tabPages.begin()+index);
		if(m_selectedIndex>index){
			m_selectedIndex--;
		}
	}
	invalidate();
}
void XTBTabBar::removeTabPage(XTBTabPage *tabPage){
	int index=indexForTabPage(tabPage);
	if(index!=XTBTabBarNotFound)
		this->removeTabPageAtIndex(index);
}

int XTBTabBar::indexAtPoint(const twPoint& pt) const{
	if(rectForTabPageAtIndex(XTBTabBarConfigButton) && pt)
		return XTBTabBarConfigButton;
	for(int n=0;n<m_tabPages.size();n++)
		if(rectForTabPageAtIndex(n) && pt)
			return n;
	return XTBTabBarNotFound;
}

int XTBTabBar::closeIndexAtPoint(const twPoint& pt) const{
	for(int n=0;n<m_tabPages.size();n++)
		if(rectForTabCloseButtonAtIndex(n) && pt)
			return n;
	return XTBTabBarNotFound;
}



void XTBTabBar::relayoutTabPages(){
	int i=0;
	for(int n=0;n<m_tabPages.size();n++){
		if(m_dragMode==XTBTabBarDragModeMove){
			if(n==m_selectedIndex){
				continue;
			}
			if(i==indexForPos(m_dragTabPos))
				i++;
		}
		m_tabPages[n]->setTabPos(i, true);
		i++;
	}
	if(doesNeedAnimation())
		startAnimation();
}

#pragma mark -
#pragma mark Animation Management

void XTBTabBar::timerFired(){
	invalidate();
	
	if(!doesNeedAnimation())
		stopAnimation();
}
void XTBTabBar::startAnimation(){
	if(m_animationTimer->isValid())
		return;
	m_animationTimer->addToEvent(tw_event);
}
void XTBTabBar::stopAnimation(){
	m_animationTimer->invalidate();
}
bool XTBTabBar::doesNeedAnimation(){
	for(int n=0;n<m_tabPages.size();n++)
		if(m_tabPages[n]->isAnimating())
			return true;
	return false;
}

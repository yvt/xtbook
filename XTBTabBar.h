/*
 *  XTBTabBar.h
 *  XTBook
 *
 *  Created by Nexhawks on 2/21/11.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#pragma once

#include <tcw/twWnd.h>
#include <vector>
class XTBTabPage;

enum{
	XTBTabBarNotFound=-2,
	XTBTabBarConfigButton=-1
};

enum XTBTabBarDragMode{
	XTBTabBarDragModeNone=0,
	XTBTabBarDragModePress,
	XTBTabBarDragModeMove,
	XTBTabBarDragModeConfigButton,
	XTBTabBarDragModeClose
};

class XTBTabBar : public twWnd{
protected:
	static twDC *m_tabBarBg;
	static twDC *getTabBarBg();
	static twDC *m_tabBarTab;
	static twDC *getTabBarTab();
	static twDC *m_configButtonIcon;
	static twDC *getConfigButtonIcon();
	static twDC *m_configButtonIconPress;
	static twDC *getConfigButtonIconPress();
	static twDC *m_removeIcon;
	static twDC *getRemoveIcon();
	static twDC *m_removeIconPress;
	static twDC *getRemoveIconPress();
	static twDC *m_tabCloseButton;
	static twDC *getTabCloseButton();
	
	std::vector<XTBTabPage *> m_tabPages;
	int m_selectedIndex;
	twTimer *m_animationTimer;
	XTBTabBarDragMode m_dragMode;
	twPoint m_firstMousePos;
	float m_firstTabPos;
	float m_dragTabPos;
	bool m_hoverConfigButton;
	
	int m_closeDragTabIndex;
	bool m_hoverClose;
	
	void removeTabPageAtIndex(int);
	int indexForTabPage(XTBTabPage *) const;
	void timerFired();
	void startAnimation();
	void stopAnimation();
	bool doesNeedAnimation();
	twRect rectForTabPageAtIndex(int) const;
	twRect rectForTabCloseButtonAtIndex(int) const;
	int getTabSize() const;
	void renderTab(int index, twDC *);
	int indexAtPoint(const twPoint&) const;
	int indexForPos(float) const;
	int closeIndexAtPoint(const twPoint&) const;
public:
	XTBTabBar();
	virtual ~XTBTabBar();
	virtual void clientPaint(const twPaintStruct&);
	virtual bool clientHitTest(const twPoint&) const;
	
	virtual void clientKeyDown(const std::wstring&);
	
	virtual void clientMouseDown(const twPoint&, twMouseButton);
	virtual void clientMouseMove(const twPoint&);
	virtual void clientMouseUp(const twPoint&, twMouseButton);
	
	void addTabPage(XTBTabPage *);
	void select(XTBTabPage *);
	void select(int);
	XTBTabPage *getSelectedTabPage() const;
	void removeTabPage(XTBTabPage *);
	void relayoutTabPages();
	
	int countOfTabPages() const{
		return (int)m_tabPages.size();
	}
	
	twRect rectForConfigButton() const;
};


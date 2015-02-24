/*
 *  XTBMainWindow.cpp
 *  XTBook
 *
 *  Created by Nexhawks on 2/12/11.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#include "XTBMainWindow.h"
#include <tcw/twMsgBox.h>
#include "XTBTabPage.h"
#include <tcw/twBaseSkin.h>
#include "i18n.h"
#include "XTBMenu.h"
#include "XTBButtonMenuItem.h"
#include "XTBSeparatorMenuItem.h"
#include <memory>
#include "XTBBrowser.h"
#include "XTBPreferenceWindow.h"

static void invalidateDesktop(){
	twGetDesktop()->invalidate();
}

void runMainWindow(){
	twWnd *oldDesktop=twGetDesktop();
	twSetDesktop(new XTBMainWindow());
	delete oldDesktop;
	tw_event->invokeOnMainThread(new twNoArgumentGlobalFunctionInvocation(invalidateDesktop));
	
}

XTBMainWindow::XTBMainWindow(){
	m_tabBar.setParent(this);
	m_tabBar.setId(XTBMainWindowTabBarId);
	m_tabBar.show();
	m_tabBar.addTabPage(new XTBTabPage(this));
	
	setRect(twRect(twPoint(0, 0), tw_app->getScrSize()));
}
XTBMainWindow::~XTBMainWindow(){
	
}


void XTBMainWindow::setRect(const twRect& rt){
	m_tabBar.setRect(twRect(0, rt.h-16, rt.w, 16));
	twWnd::setRect(rt);
	if(m_tabBar.getSelectedTabPage())
		m_tabBar.getSelectedTabPage()->layout();
	
}

void XTBMainWindow::clientPaint(const twPaintStruct&p){/*
	twDC *dc=p.dc;
	twColor color=tw_curSkin->getCtrlColor();
	twRect rt(twPoint(0, 0), p.bound);*/
	//dc->drawRect(color, twRect(rt.x, rt.y, rt.w-1, rt.h-1));
	//dc->drawLine(color, twPoint(17, 1), twPoint(17, rt.h-2));
}

twRect XTBMainWindow::getRectForBrowser(){
	twRect rt=getRect();
	return twRect(0, 0, rt.w, rt.h-16);
}

void XTBMainWindow::command(int id){
	if(id==XTBMainWindowTabBarId){
		XTBMenu menu;
		XTBButtonMenuItem *mi;
		menu.setTargetRect(m_tabBar.rectForConfigButton()+m_tabBar.getWndRect().loc()+m_tabBar.getClientRect().loc());
		
		
		XTBBrowser *browser=m_tabBar.getSelectedTabPage()->getBroser();
		
		menu.addMenuItem(new XTBButtonMenuItem(XTBLocalizedString(L"XTBMainWindowMenuQuit")));
		
		
		menu.addMenuItem(new XTBSeparatorMenuItem());
		
		menu.addMenuItem(new XTBButtonMenuItem(XTBLocalizedString(L"XTBMainWindowMenuPreferences")));
		
		
		menu.addMenuItem(new XTBSeparatorMenuItem());
		
		mi=new XTBButtonMenuItem(XTBLocalizedString(L"XTBMainWindowMenuGoBack"));
		if(!browser->canGoBack())
			mi->setEnable(false);
		menu.addMenuItem(mi);
		
		mi=new XTBButtonMenuItem(XTBLocalizedString(L"XTBMainWindowMenuGoForward"));
		if(!browser->canGoForward())
			mi->setEnable(false);
		menu.addMenuItem(mi);
		
		
		menu.addMenuItem(new XTBSeparatorMenuItem());
		
		mi=new XTBButtonMenuItem(XTBLocalizedString(L"XTBMainWindowMenuGoPreviousPage"));
		if(!browser->canGoPreviousPage())
			mi->setEnable(false);
		menu.addMenuItem(mi);
		
		mi=new XTBButtonMenuItem(XTBLocalizedString(L"XTBMainWindowMenuGoNextPage"));
		if(!browser->canGoNextPage())
			mi->setEnable(false);
		menu.addMenuItem(mi);
		
		
		menu.addMenuItem(new XTBSeparatorMenuItem());
		
		mi=new XTBButtonMenuItem(XTBLocalizedString(L"XTBMainWindowMenuCloseTab"));
		if(m_tabBar.countOfTabPages()==1)
			mi->setEnable(false);
		menu.addMenuItem(mi);
		
		menu.addMenuItem(new XTBButtonMenuItem(XTBLocalizedString(L"XTBMainWindowMenuNewTab")));
		
		
		menu.showModal(this);
		
		switch(menu.getLastSelection()){
			
			case 0:
				exit(0);
				break;
			case 1: break; // separator.
			case 2:
				// preference.
				{
					XTBPreferenceWindow pref;
					pref.showModal(NULL);
				}
				break;
			case 3: break; // separator.
			case 4:
				browser->goBack();
				break;
			case 5:
				browser->goForward();
				break;
			case 6: break; // separator.
			case 7:
				browser->goPreviousPage();
				break;
			case 8:
				browser->goNextPage();
				break;
			case 9: break; // sparator.
			case 10:
				m_tabBar.removeTabPage(m_tabBar.getSelectedTabPage());
				m_tabBar.relayoutTabPages();
				break;
			case 11:
				{
					XTBTabPage *newTabPage=new XTBTabPage(this);
					m_tabBar.addTabPage(newTabPage);
					m_tabBar.select(newTabPage);
				}
				break;
		}
		
	}else if(id==XTBMainWindowBrowserId){
		m_tabBar.invalidate();
	}
}

bool XTBMainWindow::canOpenNewTab() const{
	if(m_tabBar.countOfTabPages()>=3)
		return false;
	return true;
}
void XTBMainWindow::openLinkInNewTab(const std::wstring& url){
	XTBTabPage *tabPage=new XTBTabPage(this);
	m_tabBar.addTabPage(tabPage);
	tabPage->getBroser()->navigate(url);
}

void XTBMainWindow::clientKeyDown(const std::wstring& name){
	XTBTabPage *tabPage=m_tabBar.getSelectedTabPage();
	if(tabPage->getBroser()->getStatus().focused==false){
		tabPage->getBroser()->focus();
		tabPage->getBroser()->clientKeyDown(name);
	}
	
	
	if(name==L"F2"){
		if(m_tabBar.countOfTabPages()>=1){
			m_tabBar.select(0);
		}
	}else if(name==L"F3"){
		if(m_tabBar.countOfTabPages()>=2){
			m_tabBar.select(1);
		}
	}else if(name==L"F4"){
		if(m_tabBar.countOfTabPages()>=3){
			m_tabBar.select(2);
		}
	}else if(name==L"F5"){
		if(m_tabBar.countOfTabPages()>=4){
			m_tabBar.select(3);
		}
	}else if(name==L"Escape"){
		command(XTBMainWindowTabBarId);
	}
}

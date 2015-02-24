//
//  XTBMenu.cpp
//  XTBook
//
//  Created by Kawada Tomoaki on 5/18/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include <algorithm>
#include "XTBMenu.h"
#include "XTBMenuItem.h"
#include <tcw/twDC.h>
#include <tcw/twInvocation.h>
#include <assert.h>

XTBMenu::XTBMenu(){
	m_selection=XTBMenuNoIndex;
	m_targetRect=twRect(0,0,0,0);
	m_drag=false;
	removeAllMenuItems();
	
	m_keyboardEnabled=false;
	m_keyboardSelection=-1;
}

XTBMenu::~XTBMenu(){
	removeAllMenuItems();
}

void XTBMenu::addMenuItem(XTBMenuItem *item){
	m_items.push_back(item);
	
	twRect oldRect=getRect();
	oldRect.w=std::max(oldRect.w, item->getMinWidth()+
					   getMenuItemOffset().x*2);
	oldRect.h+=item->getHeight();
	setRect(oldRect);
}

void XTBMenu::removeAllMenuItems(){
	for(std::vector<XTBMenuItem *>::iterator it=
		m_items.begin();it!=m_items.end();it++){
		delete *it;
	}
	m_items.clear();
	setRect(twRect(0,0,getMenuItemOffset().x*2,
				   getMenuItemOffset().y*2));
}

void XTBMenu::disableKeyboard(){
	if(!m_keyboardEnabled)
		return;
	m_keyboardEnabled=false;
	invalidate();
}

void XTBMenu::enableKeyboard(){
	if(m_keyboardEnabled)
		return;
	if(!m_hasSelectable)
		return;
	m_keyboardEnabled=true;
	if(m_keyboardSelection==-1){
		// find first selectable.
		for(int i=0;i<(int)m_items.size();i++){
			if(m_items[i]->isSelectable()){
				m_keyboardSelection=i;
				break;
			}
		}
		assert(m_keyboardSelection!=-1);
	}
	invalidate();
}

void XTBMenu::clientPaint(const twPaintStruct& p){
	XTBOverlay::clientPaint(p);
	twDC *dc=p.dc;
	twRect rect=getClientRect();
	twRect rt=rect;
	rt.w-=getMenuItemOffset().x*2;
	rt+=getMenuItemOffset();
	m_hasSelectable=false;
	for(std::vector<XTBMenuItem *>::iterator it=
		m_items.begin();it!=m_items.end();it++){
		XTBMenuItem *item=*it;
		rt.h=item->getHeight();
		item->render(dc, rt, 
					 ((int(it-m_items.begin())==m_selection) && m_hover) ||
					 (m_keyboardEnabled && m_keyboardSelection==int(it-m_items.begin())));
		rt.y+=rt.h;
		if(item->isSelectable())
			m_hasSelectable=true;
	}
}

void XTBMenu::clientKeyDown(const std::wstring &key){
	if(key==L"Up" || key==L"KeyPad8"){
		if(m_keyboardEnabled){
			assert(m_hasSelectable);
			do{
				m_keyboardSelection--;
				if(m_keyboardSelection<0)
					m_keyboardSelection=(int)m_items.size()-1;
			}while(!m_items[m_keyboardSelection]->isSelectable());
			invalidate();
		}
		enableKeyboard();
	}else if(key==L"Down" || key==L"KeyPad2"){
		if(m_keyboardEnabled){
			assert(m_hasSelectable);
			do{
				m_keyboardSelection++;
				if(m_keyboardSelection>=(int)m_items.size())
					m_keyboardSelection=0;
			}while(!m_items[m_keyboardSelection]->isSelectable());
			invalidate();
		}
		enableKeyboard();
	}else if(key==L"Escape"){
		m_selection=XTBMenuNoIndex;
		endDialog();
	}else if(key==L"Return" || key==L"Enter"){
		if(m_keyboardEnabled){
			m_selection=m_keyboardSelection;
			endDialog();
		}
	}
}

void XTBMenu::clientMouseDown(const twPoint& pt, twMouseButton button){
	if(button!=twMB_left)
		return;
	disableKeyboard();
	twRect rect=getClientRect();
	twRect rt=rect;
	m_selection=XTBMenuNoIndex;
	twSetCapture(this);
	rt.w-=getMenuItemOffset().x*2;
	rt+=getMenuItemOffset();
	m_drag=true;
	for(std::vector<XTBMenuItem *>::iterator it=
		m_items.begin();it!=m_items.end();it++){
		XTBMenuItem *item=*it;
		
		rt.h=item->getHeight();
		if((rt&&pt) && item->isSelectable()){
			
			m_selection=it-m_items.begin();
			m_selectionRect=rt;
			m_hover=true;
			invalidate();
			break;
		}
		rt.y+=rt.h;
	}
}
void XTBMenu::clientMouseMove(const twPoint& pt){
	if(m_selection!=XTBMenuNoIndex && m_drag){
		bool oldHover=m_hover;
		m_hover=(m_selectionRect&&pt);
		if(oldHover!=m_hover)
			invalidate();
	}
}
void XTBMenu::clientMouseUp(const twPoint& pt, twMouseButton button){
	if(button!=twMB_left)
		return;
	twReleaseCapture();
	m_drag=false;
	if(m_selection!=XTBMenuNoIndex){
		if(m_hover){
			tw_event->invokeDelayed(new twNoArgumentMemberFunctionInvocation<XTBMenu>
									(this, &XTBMenu::endDialog), 100);
			
		}else{
			m_selection=XTBMenuNoIndex;
		}
		
		//invalidate();
	}
}

void XTBMenu::backgroundTouched(){
	m_selection=XTBMenuNoIndex;
	endDialog();
}

twPoint XTBMenu::getMenuItemOffset() const{
	return twPoint(2, 2);
}

twPoint XTBMenu::getStartupPos(const twSize& siz,
					  const twRect& desktop){
	twPoint pt;
	if(m_targetRect.y+m_targetRect.h+siz.h<=desktop.h){
		// place bottom.
		pt.y=m_targetRect.y+m_targetRect.h;
	}else if(m_targetRect.y-siz.h>=0){
		// place top.
		pt.y=m_targetRect.y-siz.h;
	}else if(desktop.h-m_targetRect.y-m_targetRect.h>
			 m_targetRect.y){
		// place bottom, touching the bottom screen border.
		pt.y=desktop.h-siz.h;
	}else{
		// place top, touching the top screen border.
		pt.y=0;
	}
	
	if(m_targetRect.x+siz.w<=desktop.w){
		// place so that their left borders connect.
		pt.x=m_targetRect.x;
	}else if(m_targetRect.x+m_targetRect.w-siz.w>=0){
		// place so that their right borders connect.
		pt.x=m_targetRect.x+m_targetRect.w-siz.w;
	}else{
		// place right, touching the right screen border.
		pt.x=desktop.w-siz.w;
	}
	
	return pt;
}


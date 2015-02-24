/*
 *  twDialog.cpp
 *  
 *
 *  Created by Nexhawks on 10/01/30.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#include <tcw/twDialog.h>
#include <tcw/twEvent.h>
#include <tcw/twDimmer.h>
#include <tcw/twInvocation.h>

twDialog::twDialog(){
	m_res=twDR_invalid;

	twWndStyle style=getStyle();
	style.border=twBS_dialog;
	setStyle(style);
	
	m_needsDimming=false;
	m_dimmer=NULL;
	m_dimmerStyle=twDS_dark;
}

twDialog::~twDialog(){	
}

twPoint twDialog::getStartupPos(const twSize& siz,
								const twRect& desk){
	return twPoint((desk.w-siz.w)/2,
				   (desk.h-siz.h)/2);
}

twDlgResult twDialog::showModal(twWnd *parent){
	twReleaseCapture();
	twWnd *oldDesktop;
	
	m_mode=twDM_modal;
	
	if(m_needsDimming){
		m_dimmer=twDimmer::openDimmer(m_dimmerStyle, new twNoArgumentMemberFunctionInvocation<twDialog>(this, &twDialog::backgroundTouched));
		oldDesktop=getParent();
		setParent(m_dimmer);
	}
	
	m_res=twDR_invalid;
	setModalParent(parent);
	if(twWnd *w=twGetDesktop()){
		twRect r1=w->getRect();
		twRect r2=getRect();
		twPoint pos=getStartupPos(r2.size(), r1);
		setRect(twRect(pos.x, pos.y, r2.w, r2.h ));
	}
	show();
	focus();
	while(tw_event->waitEvent()){
		if(m_res!=twDR_invalid)
			break;
	}
	hide();
	if(m_needsDimming){
		setParent(oldDesktop);
		delete m_dimmer;
		m_dimmer=NULL;
	}
	return m_res;
}

void twDialog::showWithCompletionHandler(twInvocation *handler){
	twReleaseCapture();
	m_handler=handler;
	m_mode=twDM_handler;
	
	if(m_needsDimming){
		m_dimmer=twDimmer::openDimmer(m_dimmerStyle, new twNoArgumentMemberFunctionInvocation<twDialog>(this, &twDialog::backgroundTouched));
		m_oldDesktop=getParent();
		setParent(m_dimmer);
	}
	
	m_res=twDR_invalid;
	
	if(twWnd *w=twGetDesktop()){
		twRect r1=w->getRect();
		twRect r2=getRect();
		setRect(twRect( (r1.w-r2.w)/2, (r1.h-r2.h)/2, r2.w, r2.h ));
	}
	
	show();
	focus();
}

void twDialog::endDialog(){
	endDialog(twDR_none);
}

void twDialog::endDialog(twDlgResult res){
	m_res=res;
	hide();
	if(m_mode==twDM_handler){
		if(m_needsDimming){
			setParent(m_oldDesktop);
			delete m_dimmer;
			m_dimmer=NULL;
		}
		if(m_handler){
			m_handler->invoke();
			delete m_handler;
			m_handler=NULL;
		}
	}
}	

twDlgResult twDialog::getResult(){
	return m_res;
}

void twDialog::backgroundTouched(){
	
}


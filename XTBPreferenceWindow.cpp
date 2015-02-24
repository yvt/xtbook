//
//  XTBPreferenceWindow.cpp
//  XTBook
//
//  Created by Kawada Tomoaki on 8/2/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "XTBPreferenceWindow.h"
#include <tcw/twBaseSkin.h>
#include "XTBListView.h"
#include "XTBPreferenceLocaleDataSource.h"
#include <tcw/twEvent.h>
#include <tcw/twInvocation.h>
#include "XTBUserDefaults.h"
#include "TPList/TPLAutoReleasePtr.h"
#include "TPList/TPLString.h"
#include <tcw/twStrConv.h>
#include "TPList/TPLNumber.h"
#include "XTBScrollView.h"

enum{
	XTBPreferenceWindowLocalesViewId=1,
	XTBPreferenceWindowSingleTouhLinkId,
	XTBPreferenceWindowFullScreenId,
	XTBPreferenceWindowSinglePageId,
	XTBPreferenceWindowIgnoreTemplatesId
};

XTBPreferenceWindow::XTBPreferenceWindow(){
	
	XTBUserDefaults *def=XTBUserDefaults::standardUserDefaults();
	
	m_scrollView=new XTBScrollView();
	m_scrollView->setRect(twRect(0, 0, 318, 218));
	m_scrollView->setParent(this);
	m_scrollView->setContentSize(twSize(318, 278));
	m_scrollView->show();
	
	m_localesLabel=new twLabel();
	m_localesLabel->setRect(twRect(20, 20, 60, 20));
	m_localesLabel->setTitle(XTBLocalizedString(L"XTBPreferenceLocale"));
	m_localesLabel->setParent(m_scrollView);
	m_localesLabel->show();
	
	m_localesView=new XTBListView(XTBListViewStyleSubtitle);
	{
		twWndStyle style=m_localesView->getStyle();
		style.border=twBS_panel;
		m_localesView->setStyle(style);
	}
	m_localesView->setRect(twRect(80, 20, 220, 120));
	m_localesDataSource=new XTBPreferenceLocaleDataSource();
	m_localesView->setDataSource(m_localesDataSource);
	{
		int location=m_localesDataSource->selectedIndex()*32;
		int maxLocation=m_localesView->getMaxContentLocation().y;
		if(location>maxLocation)
			location=maxLocation;
		m_localesView->setContentLocation(twPoint(0, location));
	}
	m_localesView->setId(XTBPreferenceWindowLocalesViewId);
	m_localesView->setParent(m_scrollView);
	m_localesView->show();
	
	m_singleTouchLinkCheckBox=new twCheckBox();
	m_singleTouchLinkCheckBox->setTitle(XTBLocalizedString(L"XTBPreferenceSingleTouchLink"));
	m_singleTouchLinkCheckBox->setCheckState(def->numberForKey(XTBUserDefaultsKeySingleTouchLink, "false")->boolValue());
	m_singleTouchLinkCheckBox->setId(XTBPreferenceWindowSingleTouhLinkId);
	m_singleTouchLinkCheckBox->setRect(twRect(30, 150, 220, 20));
	m_singleTouchLinkCheckBox->setParent(m_scrollView);
	m_singleTouchLinkCheckBox->show();
	
	m_fullScreenCheckBox=new twCheckBox();
	m_fullScreenCheckBox->setTitle(XTBLocalizedString(L"XTBPreferenceFullScreen"));
	m_fullScreenCheckBox->setCheckState(def->numberForKey(XTBUserDefaultsKeyFullScreen, "false")->boolValue());
	m_fullScreenCheckBox->setId(XTBPreferenceWindowFullScreenId);
	m_fullScreenCheckBox->setRect(twRect(30, 170, 220, 20));
	m_fullScreenCheckBox->setParent(m_scrollView);
#ifdef EV_PLATFORM_MACOSX
	{
		twWndStyle style=m_fullScreenCheckBox->getStyle();
		style.enable=false;
		m_fullScreenCheckBox->setStyle(style);
	}
#endif
	m_fullScreenCheckBox->show();
	
	m_singlePageCheckBox=new twCheckBox();
	m_singlePageCheckBox->setTitle(XTBLocalizedString(L"XTBPreferenceSinglePage"));
	m_singlePageCheckBox->setCheckState(def->numberForKey(XTBUserDefaultsKeySinglePage, "false")->boolValue());
	m_singlePageCheckBox->setId(XTBPreferenceWindowSinglePageId);
	m_singlePageCheckBox->setRect(twRect(30, 190, 220, 20));
	m_singlePageCheckBox->setParent(m_scrollView);
	m_singlePageCheckBox->show();
	
	m_ignoreTemplateCheckBox=new twCheckBox();
	m_ignoreTemplateCheckBox->setTitle(XTBLocalizedString(L"XTBPreferenceIgnoreTemplates"));
	m_ignoreTemplateCheckBox->setCheckState(def->numberForKey(XTBUserDefaultsKeyIgnoreTemplates, "false")->boolValue());
	m_ignoreTemplateCheckBox->setId(XTBPreferenceWindowIgnoreTemplatesId);
	m_ignoreTemplateCheckBox->setRect(twRect(30, 210, 220, 20));
	m_ignoreTemplateCheckBox->setParent(m_scrollView);
	m_ignoreTemplateCheckBox->show();
	

	
	
	m_label=new twLabel();
	m_label->setTitle( XTBLocalizedString(L"XTBPreferenceNoteNeedsRestart"));
	m_label->setRect(twRect(14, 245,280, 20));
	m_label->setParent(m_scrollView);
	m_label->show();
	
	m_localesView->focus();
	
	m_fullScreenModified=false;
}

XTBPreferenceWindow::~XTBPreferenceWindow(){
	m_localesView->setDataSource(NULL);
	delete m_localesDataSource;
	delete m_localesView;
	delete m_singlePageCheckBox;
	delete m_singleTouchLinkCheckBox;
	delete m_ignoreTemplateCheckBox;
	delete m_fullScreenCheckBox;
	delete m_localesLabel;
	delete m_scrollView;
}

twSize XTBPreferenceWindow::startupSize() const{
	return twSize(320,200);
}

twColor XTBPreferenceWindow::paddingColor() const{
	return tw_curSkin->getCtrlColor();
}

void XTBPreferenceWindow::clientPaint(const twPaintStruct &p){
	twRect r(twPoint(0,0),startupSize());
	twDC *dc=p.dc;
	//const twFont *font=getFont();
	dc->fillRect(tw_curSkin->getCtrlColor(), r);
	
	
	XTBDialogSheet::clientPaint(p);
}

void XTBPreferenceWindow::backgroundTouched(){
	if(m_fullScreenModified){
		bool check=m_fullScreenCheckBox->getCheckState();
		tw_app->setFullScreen(check);
		if(check)
			tw_app->setAutoScrSize();
		else
			tw_app->setScrSize(twSize(440, 290));
	}
	endDialog();
}

void XTBPreferenceWindow::command(int id){
	if(id==XTBPreferenceWindowLocalesViewId){
		tw_event->invokeDelayed(new twNoArgumentMemberFunctionInvocation<XTBPreferenceWindow>(this, &XTBPreferenceWindow::localeSelected), 100);
		
	}else if(id==XTBPreferenceWindowSingleTouhLinkId){
		TPLAutoReleasePtr<TPLNumber> num(new TPLNumber((bool)m_singleTouchLinkCheckBox->getCheckState()));
		XTBUserDefaults *def=XTBUserDefaults::standardUserDefaults();
		def->setObject(num, XTBUserDefaultsKeySingleTouchLink);
		def->synchronize();
	}else if(id==XTBPreferenceWindowFullScreenId){
		TPLAutoReleasePtr<TPLNumber> num(new TPLNumber((bool)m_fullScreenCheckBox->getCheckState()));
		XTBUserDefaults *def=XTBUserDefaults::standardUserDefaults();
		def->setObject(num, XTBUserDefaultsKeyFullScreen);
		def->synchronize();
		
		m_fullScreenModified=true;
		
		
	}else if(id==XTBPreferenceWindowSinglePageId){
		TPLAutoReleasePtr<TPLNumber> num(new TPLNumber((bool)m_singlePageCheckBox->getCheckState()));
		XTBUserDefaults *def=XTBUserDefaults::standardUserDefaults();
		def->setObject(num, XTBUserDefaultsKeySinglePage);
		def->synchronize();
	}else if(id==XTBPreferenceWindowIgnoreTemplatesId){
		TPLAutoReleasePtr<TPLNumber> num(new TPLNumber((bool)m_ignoreTemplateCheckBox->getCheckState()));
		XTBUserDefaults *def=XTBUserDefaults::standardUserDefaults();
		def->setObject(num, XTBUserDefaultsKeyIgnoreTemplates);
		def->synchronize();
	}
}

void XTBPreferenceWindow::localeSelected(){
	int oldSel=m_localesDataSource->selectedIndex();
	int newSel=m_localesView->getLastActiveRow();
	
	if(oldSel==newSel)
		return;
	
	m_localesDataSource->setSelectedIndex(newSel);
	XTBSetLocale(m_localesDataSource->localeForIndex(newSel));
	
	m_localesView->invalidateItem(oldSel);
	m_localesView->invalidateItem(newSel);
	
	XTBUserDefaults *def=XTBUserDefaults::standardUserDefaults();
	def->setObject(TPLAutoReleasePtr<TPLString>(new TPLString(twW2M(m_localesDataSource->localeForIndex(newSel)).c_str())), 
				   XTBUserDefaultsKeyLocale);
	def->synchronize();
}

void XTBPreferenceWindow::clientKeyDown(const std::wstring &key){
	if(key==L"Escape"){
		endDialog();
	}
}
/*
 *  XTBTestWindow.cpp
 *  XTBook
 *
 *  Created by Nexhawks on 2/20/11.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#include "XTBTestWindow.h"
#include <tcw/twSDLDC.h>
#include "XTBTextFieldTest.h"
#include "XTBScrollViewTest.h"
#include "XTBViewerTest.h"
#include "XTBListViewTest.h"
#include "commondlg.h"
#include <tcw/twMsgBox.h>
#include "platform.h"
#include "XTBDrawTest.h"

class XTBTestWindowListViewDataSource: public XTBListViewDataSource{
public:
	virtual int rowCount(){
		return 8;
	}
	virtual std::wstring stringAtRow(int row){
		if(row==0)
			return L"Progress Overlay";
		if(row==1)
			return L"Text Field";
		if(row==2)
			return L"Scroll View";
		if(row==3)
			return L"Viewer";
		if(row==4)
			return L"List View";
		if(row==5)
			return L"Message Box";
		if(row==6)
			return L"File Chooser";
		if(row==7)
			return L"Draw";
		abort();
	}
	virtual std::wstring detailStringAtRow(int row){
		if(row==0)
			return L"XTBInvokeWithProgressOverlay()";
		if(row==1)
			return L"XTBTextFieldTest";
		if(row==2)
			return L"XTBScrollViewTest";
		if(row==3)
			return L"XTBViewerTest";
		if(row==4)
			return L"XTBListViewTest";
		if(row==5)
			return L"twMsgBox()";
		if(row==6)
			return L"XTBChooseOpenFile()";
		if(row==7)
			return L"XTBDrawText";
		abort();
	}

};

XTBTestWindow::XTBTestWindow():
m_listView(XTBListViewStyleSubtitle){
	setNeedsDimming(true);
	twWndStyle style=getStyle();
	style.border=twBS_dialog;
	setStyle(style);
	setTitle(L"Test Suite for Debugging Purpose");
	setRect(twRect(0, 0, 200, 160));
	
	twRect cli=getClientRect();
	
	m_listView.setRect(twRect(0, 0, cli.w, cli.h));
	m_dataSource=new XTBTestWindowListViewDataSource();
	m_listView.setDataSource(m_dataSource);
	m_listView.setId(1);
	m_listView.setParent(this);
	{
		twWndStyle style=m_listView.getStyle();
		style.border=twBS_panel;
		m_listView.setStyle(style);
	}
	
	m_listView.show();
}

XTBTestWindow::~XTBTestWindow(){
}

static volatile inline unsigned long long rdtsc() {
    return 0;
	/*unsigned long long ret;
    __asm__ volatile ("rdtsc" : "=A" (ret));
    return ret;*/
}
void XTBTestWindow::doFontRenderPerformanceTest(){
	XTBSetProgressText(L"Intermediate...");
	tw_event->sleep(2000);
	XTBSetProgressText(L"Progress...");
	for(int n=0;n<200;n++){
		XTBSetProgressPercentage((float)n/200.f);
		tw_event->sleep(10);
	}
	/*
	unsigned long long ot;
	unsigned long long  tm;
	ot=rdtsc();
	for(int i=0;i<50000;i++){
		getFont()->render(getClientDC(), 0, twPoint(0, 0),
						  L"ああああああああああ");
	}
	tm=rdtsc()-ot;
	printf("test1: %lld clocks\n", tm);
	
	SDL_Surface *sf;
	twSize sz=getFont()->measure(L"ああああああああああ");
	sf=SDL_CreateRGBSurface(SDL_SWSURFACE, sz.w, sz.h, 16, 0xf, 0xf0, 0xf00, 0);
	sf=SDL_DisplayFormat(sf);
	twSDLDC dc(sf, twPoint(0, 0), true);
	getFont()->render(&dc, 0, twPoint(0, 0),
					  L"ああああああああああ");
	
	ot=rdtsc();
	for(int i=0;i<50000;i++){
		getClientDC()->bitBlt(&dc, twPoint(0, 0), twRect(0, 0, sz.w, sz.h));
	}
	tm=rdtsc()-ot;
	printf("test2: %lld clocks\n", tm);*/
}

void XTBTestWindow::command(int index){
	if(index==1){
		int row=m_listView.getLastActiveRow();
		if(row==0){
			
			
			XTBInvokeWithProgressOverlay(new twNoArgumentMemberFunctionInvocation<XTBTestWindow>(this, &XTBTestWindow::doFontRenderPerformanceTest));
			//doFontRenderPerformanceTest();
		}else if(row==1){
			XTBTextFieldTest test;
			test.showModal(this);
			invalidate();
		}else if(row==2){
			XTBScrollViewTest test;
			test.showModal(this);
			invalidate();
		}else if(row==3){
			XTBViewerTest test;
			test.showModal(this);
			invalidate();
		}else if(row==4){
			XTBListViewTest test;
			test.showModal(this);
			invalidate();
		}else if(row==5){
			twMsgBox(this, L"Hello World!", twMBB_yesNoCancel, 
					 L"twMsgBox");
		}else if(row==6){
			XTBChooseOpenFile(L"Text File", L"txt");
		}else if(row==7){
			XTBDrawTest test;
			test.showModal(this);
			invalidate();
		}
	}
	
}

void XTBTestWindow::backgroundTouched(){
	focus();
	if(getStatus().focused)
		endDialog();
}


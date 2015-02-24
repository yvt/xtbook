//
//  XTBFootnoteView.cpp
//  XTBook
//
//  Created by Kawada Tomoaki on 8/3/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "XTBFootnoteView.h"
#include <tcw/twApp.h>
#include "TWiki/TWHTMLBodyElement.h"
#include "TWViewer.h"

twSize XTBFootnoteView::startupSize() const{
	twSize scrSize=tw_app->getScrSize();
	return twSize((scrSize.w*2)/3, scrSize.h/3);
}

XTBFootnoteView::XTBFootnoteView(TWHTMLElement *b){
	m_footnoteBody=b;
	
	// we are free to disconnect footnote body from
	// the original dom.
	m_footnoteView=new TWHTMLBodyElement();
	m_footnoteView->addChild(m_footnoteBody);
	
	// disable "display: none".
	m_footnoteBody->setStyleAttr(L"");
	
	// create viewer.
	m_viewer=new TWViewer();
	m_viewer->setRect(rectForViewer());
	m_viewer->setHTMLDocument(m_footnoteView);
	m_viewer->setParent(this);
	m_viewer->show();
}

XTBFootnoteView::~XTBFootnoteView(){
	// disconnect footnote body from the footnote view
	// and return it to the original dom.
	// footnote view is destroyed by viewer.
	m_footnoteView->removeChild(m_footnoteBody);
	
	// restore the "display: none".
	m_footnoteBody->setStyleAttr(L"display: none");
	
	delete m_viewer;
}

void XTBFootnoteView::backgroundTouched(){
	
	
	endDialog();
}

twRect XTBFootnoteView::rectForViewer() const{
	twSize startSize=startupSize();
	twRect rt;
	rt.x=0; rt.y=0;
	rt.w=startSize.w-4;
	rt.h=startSize.h-4;
	return rt;
}

void XTBFootnoteView::command(int wndId){
	
}

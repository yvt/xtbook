//
//  XTBListView.cpp
//  XTBook
//
//  Created by Nexhawks on 4/16/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "XTBListView.h"
#include "XTBListViewDataSource.h"
#include <assert.h>
#include <tcw/twInvocation.h>
#include <tcw/twBaseSkin.h>

#pragma mark Lifetime

XTBListView::XTBListView(XTBListViewStyle style):
XTBViewer(rowHeightForStyle(style)){
	setAsync(false);
	m_dataSource=NULL;
	m_rowHeight=rowHeightForStyle(style);
	m_style=style;
	m_activeRow=XTBListViewNotFound;
	m_lastActiveRow=XTBListViewNotFound;
	m_keyboardMode=false;
	
	setContentSize(twSize(1, 0));
}

int XTBListView::rowHeightForStyle(XTBListViewStyle style) const{
	if(style==XTBListViewStyleDefault)
		return 24;
	if(style==XTBListViewStyleValue1)
		return 24;
	if(style==XTBListViewStyleValue2)
		return 24;
	if(style==XTBListViewStyleSubtitle)
		return 32;
	assert(false);
	return 0;
}

#pragma mark -
#pragma mark Data Source

void XTBListView::setDataSource(XTBListViewDataSource *dataSource){
	m_dataSource=dataSource;
	reloadData();
}

void XTBListView::reloadData(){
	if(!m_dataSource){
		// empty.
		setContentSize(twSize(1, 0));
		return;
	}
	
	setContentSize(twSize(1, m_dataSource->rowCount()*m_rowHeight));
	
	invalidateAllTiles();
}

void XTBListView::invalidateItem(int row){
	if(row==XTBListViewNotFound)
		return;
	invalidateTile(row);
}

int XTBListView::rowAtPoint(const twPoint & pt) const{
	twPoint localPoint=pt;
	localPoint+=getContentLocation();
	if(localPoint.y<0)
		return XTBListViewNotFound;
	if(localPoint.y>=getContentSize().h)
		return XTBListViewNotFound;
	return localPoint.y/m_rowHeight;
}

twRect XTBListView::rectForRow(int row) const{
	twRect rt;
	rt.x=0; rt.y=row*m_rowHeight;
	rt.w=getClientRect().w;
	rt.h=m_rowHeight;
	rt-=getContentLocation();
	return rt;
}

#pragma mark -
#pragma mark Item

void XTBListView::paintContents(twDC *dc, twRect rt){
	int startRow=rt.y/m_rowHeight;
	int endRow=(rt.y+rt.h+m_rowHeight-1)/m_rowHeight;
	const twFont *font=getFont();
	assert(m_dataSource!=NULL);
	
	for(int row=startRow;row<endRow;row++){
		
		assert(row>=0);
		assert(row<m_dataSource->rowCount());
		
		twRect itemRect;
		itemRect.x=0; itemRect.y=row*m_rowHeight;
		itemRect.w=rt.w; itemRect.h=m_rowHeight;
		
		twColor bgColor;
		twColor fgColor;
		twColor detailColor;
		
		//printf("rendering %d (act=%d)\n", row, m_activeRow);
		if(row==m_activeRow){
			bgColor=tw_curSkin->getSelectionColor();
			fgColor=tw_curSkin->getSelectionTextColor();
			detailColor=fgColor;
		}else{
			bgColor=tw_curSkin->getWndColor();
			fgColor=tw_curSkin->getWndTextColor();
			detailColor=twRGB(0, 80, 200);
		}
		
		dc->fillRect(bgColor, itemRect);
		
		XTBListViewItemState state;
		state=m_dataSource->stateAtRow(row);
		
		dc->drawLine(tw_curSkin->getCtrlColor(), 
					 twPoint(itemRect.x, itemRect.y+itemRect.h-1),  twPoint(itemRect.x+itemRect.w, itemRect.y+itemRect.h-1));
		
		if(state==XTBListViewItemStateChecked){
			int left=itemRect.x+2;
			int top=itemRect.y+itemRect.h/2;
			dc->drawLine(detailColor, twPoint(left, top+1), twPoint(left+2, top+3));
			dc->drawLine(detailColor, twPoint(left+5, top-5), twPoint(left+2, top+3));
		}
		
		if(m_style==XTBListViewStyleDefault){
			std::wstring text=m_dataSource->stringAtRow(row);
			twSize size=font->measure(text);
			font->render(dc, fgColor, 
						 twPoint(itemRect.x+10, itemRect.y
								 +(itemRect.h-size.h)/2),
						 m_dataSource->stringAtRow(row));
		}else if(m_style==XTBListViewStyleValue1){
			int columnWidth=itemRect.w/4;
			std::wstring text=m_dataSource->stringAtRow(row);
			std::wstring detail=m_dataSource->detailStringAtRow(row);
			twSize size=font->measure(text);
			
			font->render(dc, fgColor, 
						 twPoint(itemRect.x+columnWidth-size.w, 
								 itemRect.y
								 +(itemRect.h-size.h)/2),
						 text);
			
			font->render(dc, detailColor, 
						 twPoint(itemRect.x+columnWidth+5, itemRect.y
								 +(itemRect.h-size.h)/2),
						 detail);
		}else if(m_style==XTBListViewStyleValue2){
			//int columnWidth=itemRect.w/3;
			std::wstring text=m_dataSource->stringAtRow(row);
			std::wstring detail=m_dataSource->detailStringAtRow(row);
			twSize size=font->measure(detail);
			
			font->render(dc, fgColor, 
						 twPoint(itemRect.x+10, itemRect.y
								 +(itemRect.h-size.h)/2),
						 text);
			
			font->render(dc, detailColor, 
						 twPoint(itemRect.x+itemRect.w-size.w-5, itemRect.y
								 +(itemRect.h-size.h)/2),
						 detail);
		}else if(m_style==XTBListViewStyleSubtitle){
			std::wstring text=m_dataSource->stringAtRow(row);
			std::wstring detail=m_dataSource->detailStringAtRow(row);
			twSize size=font->measure(text);
			font->render(dc, fgColor, 
						 twPoint(itemRect.x+10, itemRect.y
								 +(itemRect.h/2-size.h)/2),
						 text);
			
			font->render(dc, detailColor, 
						 twPoint(itemRect.x+10, itemRect.y+itemRect.h/2
								 +(itemRect.h/2-size.h)/2),
						 detail);
		}
		
		if(m_keyboardMode && m_keyboardPos==row){
			dc->dimRect(itemRect);
		}
		
	}
}

#pragma mark -
#pragma mark Event Handler

void XTBListView::clientTouchStarted(const twPoint& pt, twMouseButton){
	//puts("touchStart");
	m_activeRow=rowAtPoint(pt);
	invalidateItem(m_activeRow);
	m_deactivateSession=rand();
}
void XTBListView::deactivateRow(int session){
	if(session!=m_deactivateSession)
		return;
	int oldActiveRow=m_activeRow;
	
	m_activeRow=XTBListViewNotFound;
	invalidateItem(oldActiveRow);
	
}
void XTBListView::clientTouchEnded(const twPoint& pt, twMouseButton){
	int oldActiveRow=m_activeRow;
	//puts("touchEnd");
	if(oldActiveRow==XTBListViewNotFound)
		return;
	
	twRect itemRect=rectForRow(oldActiveRow);
	
	m_lastActiveRow=m_activeRow;
	
	
	
	sendCmdToParent();
	
	itemTouched(pt-itemRect.loc(), oldActiveRow);
	
	tw_event->invokeDelayed(new twOneArgumentMemberFunctionInvocation<XTBListView, int>(this, &XTBListView::deactivateRow, m_deactivateSession), 100);
}
void XTBListView::clientTouchCancelled(const twPoint& pt, twMouseButton){
	//puts("touchCancel");
	int oldActiveRow=m_activeRow;
	if(oldActiveRow==XTBListViewNotFound)
		return;
	m_activeRow=XTBListViewNotFound;
	
	invalidateItem(oldActiveRow);
}

void XTBListView::fillTopMargin(twDC *dc, twRect rt) const{
	if(rt.h>1){
		dc->fillRect(tw_curSkin->getWndColor(), twRect(rt.x, rt.y, rt.w, rt.h-1));
	}
	dc->fillRect(tw_curSkin->getCtrlColor(), twRect(rt.x, rt.y+rt.h-1, rt.w, 1));
}
void XTBListView::fillBottomMargin(twDC *dc, twRect rt) const{
	dc->fillRect(tw_curSkin->getWndColor(), rt);
	for(int y=m_rowHeight-1;y<rt.h;y+=m_rowHeight){
		dc->fillRect(tw_curSkin->getCtrlColor(), twRect(rt.x, rt.y+y, rt.w, 1));
	}
}

void XTBListView::willAppear(){
	
	XTBViewer::willAppear();
}

void XTBListView::didDisappear(){
	
	XTBViewer::didDisappear();
}

#pragma mark -
#pragma mark Keyboard Mode

void XTBListView::clientEnter(){
	XTBViewer::clientEnter();
	
}

void XTBListView::clientLeave(){
	// disable keyboard mode.
	if(m_keyboardMode){
		m_keyboardMode=false;
		invalidateItem(m_keyboardPos);
	}
	XTBViewer::clientLeave();
	
}


void XTBListView::scrollToKeyboardPos(){
	twPoint pos=getContentLocation();
	twRect rt=rectForRow(m_keyboardPos);
	twRect rt2=getClientRect();
	if(rt.y<0){
		pos.y+=rt.y;
	}
	if(rt.y+rt.h>rt2.h){
		pos.y+=(rt.y+rt.h)-rt2.h;
	}
	setContentLocationAnimated(pos);
}

void XTBListView::clientMouseDown(const twPoint &pt, twMouseButton bt){
	// disable keyboard mode.
	if(m_keyboardMode){
		m_keyboardMode=false;
		invalidateItem(m_keyboardPos);
	}
	
	XTBViewer::clientMouseDown(pt, bt);
}

void XTBListView::clientKeyDown(const std::wstring &key){
	// enable keyboard mode.
	if(!m_dataSource)
		return;
	if(!m_keyboardMode){
		// initial position.
		m_keyboardMode=true;
		m_keyboardPos=getContentLocation().y/m_rowHeight;
		if(m_keyboardPos<0)
			m_keyboardPos=0;
		if(m_keyboardPos>=m_dataSource->rowCount())
			m_keyboardPos=m_dataSource->rowCount()-1;
		invalidateItem(m_keyboardPos);
		scrollToKeyboardPos();
		return;
	}
	
	if(key==L"Up"||key==L"KeyPad8"){
		if(m_keyboardPos>0){
			m_keyboardPos--;
			invalidateItem(m_keyboardPos);
			invalidateItem(m_keyboardPos+1);
			scrollToKeyboardPos();
		}
	}else if(key==L"Down"||key==L"KeyPad2"){
		if(m_keyboardPos<m_dataSource->rowCount()-1){
			m_keyboardPos++;
			invalidateItem(m_keyboardPos);
			invalidateItem(m_keyboardPos-1);
			scrollToKeyboardPos();
		}
	}else if(key==L"Return"){
		// simulate touch.
		m_activeRow=m_keyboardPos;
		invalidateItem(m_activeRow);
		m_deactivateSession=rand();
		m_lastActiveRow=m_activeRow;
		sendCmdToParent();
		itemTouched(twPoint(0,0), m_activeRow);
		tw_event->invokeDelayed(new twOneArgumentMemberFunctionInvocation<XTBListView, int>(this, &XTBListView::deactivateRow, m_deactivateSession), 100);
	}
}

#pragma mark -
#pragma mark Default Event Handler

void XTBListView::itemTouched(const twPoint &, int row){
	
}


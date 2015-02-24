//
//  XTBStartPageList.cpp
//  XTBook
//
//  Created by Kawada Tomoaki on 6/24/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "XTBStartPageList.h"

#include "xpms/StartPageListTop.xpm"
#include "xpms/StartPageListBottom.xpm"

#include "XTBListView.h"
#include "XTBStartPageListDataSource.h"

#include <tcw/twSDLDC.h>

#include "XTBMenu.h"
#include "XTBButtonMenuItem.h"
#include "XTBSeparatorMenuItem.h"

#include <tcw/twMsgBox.h>

#pragma mark - Assets

twDC *XTBStartPageList::m_startPageListTopImage=NULL;

twDC *XTBStartPageList::getStartPageListTopImage(){
	if(m_startPageListTopImage==NULL){
		m_startPageListTopImage=twSDLDC::loadFromXPM(StartPageListTop);
	}
	return m_startPageListTopImage;
}

twDC *XTBStartPageList::m_startPageListBottomImage=NULL;

twDC *XTBStartPageList::getStartPageListBottomImage(){
	if(m_startPageListBottomImage==NULL){
		m_startPageListBottomImage=twSDLDC::loadFromXPM(StartPageListBottom);
	}
	return m_startPageListBottomImage;
}


#pragma mark - Lifetime

XTBStartPageList::XTBStartPageList(){
	m_listView=new XTBListView(XTBListViewStyleSubtitle);
	m_listView->setParent(this);
	m_listView->show();
	
	m_isEditing=false;
	m_reloadDataDeferred=false;
	m_selectAllowed=true;
}

XTBStartPageList::~XTBStartPageList(){
	delete m_listView;
}

#pragma mark - Responder

void XTBStartPageList::clientPaint(const twPaintStruct &p){
	twDC *dc=p.dc;
	twRect rt=p.boundRect;
	twRect r;
	const twColor marginColor=twRGB(255, 255, 255);
	const twColor borderColor=twRGB(136, 136, 136);
	
	rt.h+=7;
	
	// fill header.
	r=twRect(rt.x, rt.y, rt.w, 25);
	dc->bitBlt(getStartPageListTopImage(), 
			   twPoint(r.x, r.y), 
			   twRect(0, 0, 16, 25));
	dc->bitBlt(getStartPageListTopImage(), 
			   twPoint(r.x+r.w-16, r.y), 
			   twRect(32, 0, 16, 25));
	for(int x=0;x<r.w-32;x+=16){
		dc->bitBlt(getStartPageListTopImage(), 
				   twPoint(r.x+16+x, r.y), 
				   twRect(16, 0, 
						  std::min(16, rt.w-32-x)
						  , 25));
	}
	
	// draw the title.
	std::wstring title=getTitle();
	const twFont *fnt=getFont();
	twPoint pt(r.x+14, r.y+10);
	title=XTBFormat(title, XTBStringWithInt(m_listView->getDataSource()->rowCount()).c_str(), NULL);
	fnt->render(dc, twRGB(230, 230, 230), 
				twPoint(pt.x, pt.y+1), 
				title);
	fnt->render(dc, twRGB(0, 0, 0), 
				twPoint(pt.x, pt.y), 
				title);
	
	// fill footer.
	r=twRect(rt.x, rt.y+rt.h-25, rt.w, 25);
	dc->bitBlt(getStartPageListBottomImage(), 
			   twPoint(r.x, r.y), 
			   twRect(0, 0, 16, 25));
	dc->bitBlt(getStartPageListBottomImage(), 
			   twPoint(r.x+r.w-16, r.y), 
			   twRect(32, 0, 16, 25));
	for(int x=0;x<r.w-32;x+=16){
		dc->bitBlt(getStartPageListBottomImage(), 
				   twPoint(r.x+16+x, r.y), 
				   twRect(16, 0, 
						  std::min(16, rt.w-32-x)
						  , 25));
	}
	
	// fill left margin and border.
	r=twRect(rt.x, rt.y+25, 8, rt.h-50);
	dc->fillRect(marginColor, twRect(r.x, r.y, r.w-1, r.h));
	dc->fillRect(borderColor, twRect(r.x+r.w-1, r.y,
									 1, r.h));
	
	// fill right margin and border.
	r=twRect(rt.x+rt.w-8, rt.y+25, 8, rt.h-50);
	dc->fillRect(marginColor, twRect(r.x+1, r.y, r.w-1, r.h));
	dc->fillRect(borderColor, twRect(r.x, r.y,
									 1, r.h));
	
	// contents is filled by m_listView.
	// we don't need to fill.
	// do not do it; it is slow.
	
}

bool XTBStartPageList::clientHitTest(const twPoint &pt) const{
	return true;
}

twRect XTBStartPageList::rectForListView() const{
	twRect rt(twPoint(0, 0), getClientRect().size());
	return twRect(rt.x+8, rt.y+25,
				  rt.w-8*2,
				  rt.h-25*2+7);
}

void XTBStartPageList::setRect(const twRect &rt){
	twWnd::setRect(rt);
	m_listView->setRect(rectForListView());
}

void XTBStartPageList::selected(){
	m_selectAllowed=true;
	// rect of row.
	twRect listRect=rectForListView();
	int row=m_listView->getLastActiveRow();
	twRect rt(0, row*32, listRect.w, 32);
	rt.y-=m_listView->getContentLocation().y;
	
	XTBStartPageListDataSource *dataSource=(XTBStartPageListDataSource *)m_listView->getDataSource();
	
	XTBMenu menu;
	menu.setTargetRect(rt+m_listView->getWndRect().loc());
	
	menu.addMenuItem(new XTBButtonMenuItem(XTBLocalizedString(L"XTBStartPageListOpen")));
	
	if(dataSource->isClearable() || dataSource->isRemovable()){
		menu.addMenuItem(new XTBSeparatorMenuItem());
		if(dataSource->isRemovable()){
			menu.addMenuItem(new XTBButtonMenuItem(XTBLocalizedString(L"XTBStartPageListRemove")));
		}else if(dataSource->isClearable()){
			menu.addMenuItem(new XTBButtonMenuItem(XTBLocalizedString(L"XTBStartPageListClear")));
		}
	}
	
	menu.showModal(this);
	
	if(menu.getLastSelection()==0){
		// notify the parent the event.
		sendCmdToParent();
	}else if(menu.getLastSelection()==2){
		if(dataSource->isRemovable()){
			std::set<int> inds;
			inds.insert(row);
			dataSource->removeItemsAtRows(inds);
			reloadData();
		}else if(dataSource->isClearable()){
			if(twMsgBox(NULL, XTBLocalizedString(L"XTBStartPageListClearConfirm"), twMBB_okCancel, 
						L"XTBook", true)==twDR_ok){
				dataSource->clearAllItems();
				reloadData();
			}
		}
	}
}

void XTBStartPageList::command(int){
	// list view was selected.
	if(m_isEditing){
		// toggle the check.
		XTBStartPageListDataSource *dataSource;
		dataSource=static_cast<XTBStartPageListDataSource *>(m_listView->getDataSource());
		
		if(dataSource){
			int row=m_listView->getLastActiveRow();
			dataSource->toggleCheckAtRow(row);
			
			m_listView->invalidateItem(row);
		}
	}else{
		
		
		if(!m_selectAllowed)
			return;
		m_selectAllowed=false;
		tw_event->invokeDelayed(new twNoArgumentMemberFunctionInvocation<XTBStartPageList>(this, &XTBStartPageList::selected), 100);
	
	}
}

int XTBStartPageList::getLastActiveRow() const{
	return m_listView->getLastActiveRow();
}

#pragma mark - Data

void XTBStartPageList::setDataSource(XTBStartPageListDataSource *source){
	m_listView->setDataSource(source);
}

void XTBStartPageList::reloadData(){
	
	if(!isVisible()){
		m_reloadDataDeferred=true;
		return;
	}
	
	reloadDataSource();
}

void XTBStartPageList::willAppear(){
	
	if(m_reloadDataDeferred){
		m_reloadDataDeferred=false;
		reloadDataSource();
	}
	
	twWnd::willAppear();
}

void XTBStartPageList::reloadDataSource(){
	XTBStartPageListDataSource *dataSource;
	dataSource=static_cast<XTBStartPageListDataSource *>(m_listView->getDataSource());
	if(dataSource){
		dataSource->reloadData();
		m_listView->reloadData();
	}
}


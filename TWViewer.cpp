/*
 *  TWViewer.cpp
 *  XTBook
 *
 *  Created by Nexhawks on 2/23/11.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */
/*
 * This file is part of XTBook.
 *
 * XTBook is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * XTBook is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with XTBook.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "TWViewer.h"
#include "TWRenderDCTcw.h"
#include "TWRenderManagerTcw.h"
#include <tcw/twFont.h>
#include <tcw/twWnd.h>
#include <tcw/twLock.h>
#include "TWiki/TWRenderView.h"
#include "TWiki/TWRenderBoxModelObject.h"
#include "TWiki/TWHTMLBodyElement.h"
#include "TWiki/TWRenderFlowInline.h"
#include "TWiki/TWRenderInline.h"
#include <assert.h>
#include "TWiki/TWUtils.h"
#include "commondlg.h"
#include <tcw/twInvocation.h>
#include <algorithm>
#include "TWiki/TWDiagnostics.h"
#include "i18n.h"
#include <tcw/twTimerWithInvocation.h>
#include <tcw/twStrConv.h>

TWViewer::TWViewer():
m_htmlDocument(NULL),
m_renderView(NULL),
m_imageManager(),
m_fontManager(new TWRenderManagerTcw(tw_defFont, &m_imageManager)),
m_renderViewSemaphore(new twSDLSemaphore()),
m_layouted(false),
m_contentWidth(0),
m_needsLayout(true),
m_selecting(false){
	//setContentSize(twSize(0, 2048));
	setScrollDuration(600);
	m_drag=false;
	m_holdTimer=NULL;
	m_lastInvalidTouch=0;
	//setAsync(false);
}
TWViewer::~TWViewer(){
    // wait for rendering completion
    m_renderViewSemaphore->wait();
    
    
    if(m_renderView)
        delete m_renderView;
	
	if(m_htmlDocument)
		delete m_htmlDocument;
    
    delete m_fontManager;
    delete m_renderViewSemaphore;
}

#pragma mark -
#pragma mark Properties

void TWViewer::setHTMLDocument(TWHTMLBodyElement *htmlDocument){
	{
		twLock lock(m_renderViewSemaphore);
		m_htmlDocument=htmlDocument;
		
		if(m_renderView)
			delete m_renderView;
		
		if(m_htmlDocument)
			m_htmlDocument->calcStyle();
		m_renderView=NULL;
		
		setContentSize(twSize(1, 1));
		m_layouted=false;
		m_needsLayout=true;
	}
    
    startLayout();
}

TWHTMLBodyElement *TWViewer::htmlDocument() const{
    return m_htmlDocument;
}

#pragma mark -
#pragma mark Layout

void TWViewer::doLayout(){
	twLock lock(m_renderViewSemaphore);
	
	XTBSetProgressText(XTBLocalizedString(L"TWViewerBuildingRenderTree"));
	
	if(m_renderView){
		delete m_renderView;
		m_renderView=NULL;
	}
	if(m_htmlDocument){
#if 0
		puts("Here's size of DOM tree...");
		puts("----");
		TWDiagnostics::dumpNodeMemoryUsage(m_htmlDocument);
		puts("----");
		puts("");
#endif
		/*
		while(1){
			if(m_renderView)
				delete m_renderView;*/
		m_renderView=dynamic_cast<TWRenderView *>(TWRenderObject::objectWithNode(m_htmlDocument));
		assert(m_renderView);

		XTBSetProgressText(XTBLocalizedString(L"TWViewerComputingStyles"));
		
		
		m_htmlDocument->setAttribute(L"style", L"width: "+
									 TWUtils::stringWithIntValue(getClientRect().w)+L"px;"
									 +L"background-color: white;"
									 +L"padding: 4px;");
		
		
		m_htmlDocument->calcStyle();
		m_renderView->calcStyle();
		
		XTBSetProgressText(XTBLocalizedString(L"TWViewerLayouting"));
		
		
		m_renderView->layout(m_fontManager);
		
		XTBSetProgressText(XTBLocalizedString(L"TWViewerComputingCoordinates"));
		
		
		m_renderView->calcScreenRect();
		

		
		//}
		TWRect rt=m_renderView->screenRect();
		if(rt.h==0)
			rt.h=1;
#if 0
		puts("Here's size of Render tree...");
		puts("----");
		TWDiagnostics::dumpNodeMemoryUsage(m_renderView);
		puts("----");
		puts("");
#endif
		
		XTBSetProgressText(XTBLocalizedString(L"TWViewerIndexingDOM"));
		
		m_ids.clear();
		m_domIds.clear();
		searchNodeForIds(m_renderView);
		searchNodeForDomIds(m_htmlDocument);
		m_renderView->calcChildrenFlowIndex();
		
		m_renderView->validateLayout();
		
		setContentSize(twSize(rt.w, rt.h));
	}else{
		setContentSize(twSize(0, 0));
	}
		
	m_layouted=true;
	m_needsLayout=false;
	m_contentWidth=getClientRect().w;
	
}

bool TWViewer::shouldLayoutAync() const{
	return m_htmlDocument!=NULL;
}

void TWViewer::startLayout(){
	if(getClientRect().w<=0)
		return;
	if(!m_needsLayout)
		return;
	if(shouldLayoutAync()){
		XTBInvokeWithProgressOverlay(new twNoArgumentMemberFunctionInvocation<TWViewer>(this, &TWViewer::doLayout));
	}else{
		doLayout();
	}
    invalidateAllTiles();
}

void TWViewer::setRect(const twRect &rt){
   
	XTBViewer::setRect(rt);
	if(getClientRect().w!=m_contentWidth){
		 m_layouted=false;
		m_needsLayout=true;
	}
	
   
    startLayout();
}

#pragma mark -
#pragma mark Renderer

void TWViewer::paintContents(twDC *dc, twRect rt){
    if(!m_layouted)
        return;
	if(!m_renderView)
		return;
    twLock lock(m_renderViewSemaphore);
	
	m_imageManager.resetUsageCount();

    TWRenderDCTcw dc2(dc,m_fontManager);
    m_renderView->render(&dc2);
	
	m_imageManager.evictNotUsedImages();
}

void TWViewer::fillTopMargin(twDC *dc, twRect rt) const{
	dc->fillRect(twRGB(128, 128, 128), rt);
}
void TWViewer::fillBottomMargin(twDC *dc, twRect rt) const{
	int maxPageBottomMargin=std::max(getClientRect().h-getContentSize().h, 0);
	
	if(rt.h<=maxPageBottomMargin){
		dc->fillRect(twRGB(255, 255, 255), rt);
	}else{
		dc->fillRect(twRGB(255, 255, 255), twRect(rt.x, rt.y, rt.w, maxPageBottomMargin));
		
		twRect leftMargin(rt.x, rt.y+maxPageBottomMargin, rt.w, rt.h-maxPageBottomMargin);
		dc->fillRect(twRGB(128, 128, 128), leftMargin);
	}
}

#pragma mark - Heading Locator
const std::wstring g_idAttrName=L"id";

void TWViewer::searchNodeForIds(TWRenderObject *obj){
	if(obj->isText())
		return;
	if(obj->node()){
		TWHTMLElement *elm=obj->node();
		if(elm->hasAttribute(g_idAttrName)){
			m_ids[elm->getAttribute(g_idAttrName)]=obj;
		}
	}
	for(TWNode *child=obj->firstChild();child;child=child->nextSibling()){
		TWRenderObject *childObj;
		childObj=static_cast<TWRenderObject *>(child);
		searchNodeForIds(childObj);
	}
}

void TWViewer::searchNodeForDomIds(TWHTMLElement *elm){
	if(elm->hasAttribute(g_idAttrName)){
		m_domIds[elm->getAttribute(g_idAttrName)]=elm;
	}
	for(TWNode *child=elm->firstChild();child;child=child->nextSibling()){
		TWHTMLElement *childObj;
		childObj=static_cast<TWHTMLElement *>(child);
		searchNodeForDomIds(childObj);
	}
}

twRect TWViewer::rectForId(const std::wstring &id) const{
	std::map<std::wstring, TWRenderObject *>::const_iterator it;
	it=m_ids.find(id);
	if(it==m_ids.end()){
		return twRect(0,0,0,0);
	}else{
		TWRenderObject *obj=it->second;
		TWRenderBox *box=dynamic_cast<TWRenderBox *>(obj);
		if(box){
			TWRect r=box->screenRect();
			return twRect(r.x, r.y, r.w, r.h);
		}else{
			return twRect(0,0,0,0);
		}
	}
}

TWRenderObject *TWViewer::renderObjectForId(const std::wstring &id) const{
	std::map<std::wstring, TWRenderObject *>::const_iterator it;
	it=m_ids.find(id);
	if(it==m_ids.end()){
		return NULL;
	}else{
		return it->second;
	}
}

TWHTMLElement *TWViewer::htmlElementForId(const std::wstring &id) const{
	std::map<std::wstring, TWHTMLElement *>::const_iterator it;
	it=m_domIds.find(id);
	if(it==m_domIds.end()){
		return NULL;
	}else{
		return it->second;
	}
}

#pragma mark -
#pragma mark Interaction

void TWViewer::clientTouchStarted(const twPoint& pt, twMouseButton mb){
	if(mb!=twMB_left)
		return;
	
	TWRenderBoxModelObject *renderer=static_cast<TWRenderBoxModelObject *>
	(activeRenderObjectAtPoint(pt+getContentLocation()));
	if(renderer==NULL){
		m_lastInvalidTouch=twGetTicks();
		return;
	}
	
	m_lastInvalidTouch=0;
	
	m_drag=true;
	m_dragElement=elementForRenderer(renderer);
	TWRect r=renderer->boundaryRect().sanitize();
	m_dragRect=twRect(r.x, r.y, r.w, r.h);
	
	invalidate();
	
	m_holded=false;
	m_holdPos=pt;
	m_holdTimer=new twTimerWithInvocation(new twNoArgumentMemberFunctionInvocation<TWViewer>(this, &TWViewer::holded));
	m_holdTimer->setInterval(500); // TODO: make hold duration customizable
	m_holdTimer->addToEvent(tw_event);
	
}
void TWViewer::clientTouchEnded(const twPoint& pt, twMouseButton mb){
	if(mb!=twMB_left)
		return;
	if(twGetTicks()>m_lastInvalidTouch+300){
		// too long invalid touch.
		m_lastInvalidTouch=0;
		//XTBLog("reject");
	}else{
		readyForDrag(400);
		//XTBLog("wait");
	}
	if(!m_drag)
		return;
	if(m_holdTimer){
		delete m_holdTimer;
		m_holdTimer=NULL;
	}
	
	if(!m_holded)
	tw_event->invokeOnMainThread(new twNoArgumentMemberFunctionInvocation<TWViewer>
							(this,&TWViewer::dragEnded));
}
void TWViewer::clientTouchCancelled(const twPoint& pt, twMouseButton mb){
	if(mb!=twMB_left)
		return;
	if(m_holdTimer){
		delete m_holdTimer;
		m_holdTimer=NULL;
	}
	m_drag=false;
	invalidate();
}

void TWViewer::clientDragStarted(const twPoint &pt, twMouseButton){
	unsigned int i;
	twPoint p=pt+getContentLocation();
	i=m_renderView->flowIndexAtPoint(TWPoint(p.x, p.y));
	if(i==(unsigned int)-1)
		return;
	m_selecting=true;
	m_selectionStart=i;
	m_selectionEnd=i;
	invalidate();
}

void TWViewer::clientDragMoved(const twPoint &pt, twMouseButton){
	unsigned int i;
	if(!m_selecting)
		return;
	twPoint p=pt+getContentLocation();
	i=m_renderView->flowIndexAtPoint(TWPoint(p.x, p.y));
	if(i==(unsigned int)-1)
		return;
	if(m_selectionEnd!=i){
		m_selectionEnd=i;
		invalidate();
	}
}

void TWViewer::clientDragEnded(const twPoint &pt, twMouseButton){
	if(!m_selecting)
		return;
	m_lastEventType=TWViewerEventTypeTextSelected;
	sendCmdToParent();
	m_selecting=false;
	invalidate();
}

std::wstring TWViewer::getLastSelectedText() const{
	std::wstring selstr;
	m_renderView->captureSelectionString(selstr, 
										 std::min(m_selectionStart, m_selectionEnd), 
										 std::max(m_selectionStart, m_selectionEnd));
	return selstr;
}

struct TWViewerTouchStartRenderObjectCallbackArgs{
	TWViewer *viewer;
	TWRenderObject *renderer;
	TWViewerTouchStartRenderObjectCallbackArgs(TWViewer *_viewer):
	viewer(_viewer), renderer(NULL){}
};

void TWViewer::touchStartRenderObjectCallback(TWViewerTouchStartRenderObjectCallbackArgs *args,
											  TWRenderBoxModelObject *obj) const{
	TWHTMLElement *element=elementForRenderer(obj);
	if(!element){
		return;
	}
	if(!isElementActive(element))
		return;
	args->renderer=obj;
}
bool TWViewer::isElementActive(TWHTMLElement *element) const{
	switch(element->type()){
		case TWHTMLElementTypeA:
			if(element->hasAttribute(L"href")){
				return true;
			}
			return false;
		default:
			return false;
	}
}
void touchStartRenderObjectCallbackDriver(void *data, TWNode *node){
	TWViewerTouchStartRenderObjectCallbackArgs *args=static_cast<TWViewerTouchStartRenderObjectCallbackArgs *>(data);
	args->viewer->touchStartRenderObjectCallback(args, static_cast<TWRenderBoxModelObject *>(node));
	
}
TWRenderObject *TWViewer::activeRenderObjectAtPoint(const twPoint& pt) const{
	twLock lock(m_renderViewSemaphore);
	TWViewerTouchStartRenderObjectCallbackArgs args((TWViewer *)this);
	if(m_renderView)
		m_renderView->accumulateRenderObjectsAtPoint(TWPoint(pt.x, pt.y), touchStartRenderObjectCallbackDriver, 
													 &args);
	return args.renderer;
}
TWHTMLElement *TWViewer::elementForRenderer(TWRenderObject *renderer) const{
	if(TWRenderFlowInline *flowInline=dynamic_cast<TWRenderFlowInline *>(renderer)){
		return flowInline->renderer()->node();
	}
	return renderer->node();
}

void TWViewer::clientPaint(const twPaintStruct& ps){
	XTBViewer::clientPaint(ps);
	twDC *dc=ps.dc;
	if(m_drag){
		twRect dragRect=m_dragRect;
		twPoint scrPos=getContentLocation();
		dragRect-=scrPos;
		
		dc->dimRect(twRect(dragRect.x-2, dragRect.y,
						   dragRect.w+4, dragRect.h));
		
		dc->dimRect(twRect(dragRect.x-1, dragRect.y-1,
						   dragRect.w+2, 1));
		dc->dimRect(twRect(dragRect.x, dragRect.y-2,
						   dragRect.w, 1));
		
		dc->dimRect(twRect(dragRect.x-1, dragRect.y+dragRect.h,
						   dragRect.w+2, 1));
		dc->dimRect(twRect(dragRect.x, dragRect.y+dragRect.h+1,
						   dragRect.w, 1));
		

		
	}
	
	if(m_selecting){
		TWRenderDCTcw selDC(dc, m_fontManager);
		
		unsigned int selBegin=m_selectionStart;
		unsigned int selEnd=m_selectionEnd;
		
		if(selBegin>selEnd)
			std::swap(selBegin, selEnd);
		
		selDC.setSelectionShift(getContentLocation());
		
		TWRect dragRect(0,0,0,0);
		
		if(selBegin!=selEnd)
			dragRect=m_renderView->drawSelection(&selDC, selBegin, selEnd);
		m_dragRect=twRect(dragRect.x, dragRect.y,
						  dragRect.w, dragRect.h);
	}
	/*
	twRect rt(twPoint(0, 0), getClientRect().size());
	dc->dimRect(twRect(0, 0, rt.w, 1));
	dc->dimRect(twRect(0, rt.h-1, rt.w, 1));*/
}

void TWViewer::dragEnded(){
	if(!m_dragElement)
		return;
	if(!m_drag)
		return;
	if(m_holded)
		m_lastEventType=TWViewerEventTypeLinkHolded;
	else
		m_lastEventType=TWViewerEventTypeLinkTouched;
	sendCmdToParent();
	m_drag=false;
	invalidate();
}

void TWViewer::holded(){
	assert(m_holdTimer!=NULL);
	
	m_holded=true;
	
	delete m_holdTimer;
	m_holdTimer=NULL;
	
	clientMouseUp(m_holdPos, twMB_left);

	
	tw_event->invokeOnMainThread(new twNoArgumentMemberFunctionInvocation<TWViewer>
								 (this,&TWViewer::dragEnded));
}

twRect TWViewer::getLastTouchedRect() const{
	twRect dragRect=m_dragRect;
	twPoint scrPos=getContentLocation();
	dragRect-=scrPos;
	return dragRect;
}

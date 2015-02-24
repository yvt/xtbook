/*
 *  XTBViewer.cpp
 *  XTBook
 *
 *  Created by Nexhawks on 2/20/11.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#include "XTBViewer.h"
#include <tcw/twLock.h>
#include <tcw/twEvent.h>
#include <tcw/twSDLDC.h>
#include <tcw/twInvocation.h>
#include <cassert>
#include "platform.h"
#include <tcw/twStrConv.h>

XTBViewer::XTBViewer(int tileSize){
	m_nowProcessing=false;
	m_tileSize=tileSize;
	m_tileBegin=0;
	m_currentSession=0;
	m_tilesLock=new twSDLSemaphore();
	m_processLock=new twSDLSemaphore(XTBViewerProcessLockInitialValue);
	m_async=true;
	m_processingDeferred=false;
}
XTBViewer::~XTBViewer(){
	for(int n=0;n<XTBViewerProcessLockInitialValue;n++)
		m_processLock->wait();
	
	{
		twLock tilesLock(m_tilesLock);
		
		// clear tiles.
		while(!m_tiles.empty()){
			delete m_tiles.front();
			m_tiles.pop_front();
		}
		
	}
	
	delete m_processLock;
	delete m_tilesLock;
	
}
void XTBViewer::fillCheckerBoard(twDC *dc, twRect rt) const{
	bool b1=false;
	const int checkerBoardSize=16;
	for(int x=0;x<rt.w;x+=checkerBoardSize){
		bool b2=b1;
		for(int y=0;y<rt.h;y+=checkerBoardSize){
			int w=MIN(rt.w-x, checkerBoardSize);
			int h=MIN(rt.h-y, checkerBoardSize);
			dc->fillRect(b2?twRGB(255, 255, 255):
						 twRGB(192, 192, 192), 
						 twRect(rt.x+x, rt.y+y, w, h));
			b2=!b2;
		}
		b1=!b1;
	}
}
void XTBViewer::fillTopMargin(twDC *dc, twRect rt) const{
	dc->fillRect(twRGB(128, 128, 128), rt);
}
void XTBViewer::fillBottomMargin(twDC *dc, twRect rt) const{
	dc->fillRect(twRGB(128, 128, 128), rt);
}
void XTBViewer::clientPaint(const twPaintStruct& p){
	twDC *dc=p.dc;
	std::list<twDC *> tiles;
	{
		twLock tilesLock(m_tilesLock);
		tiles=m_tiles;
	}
	twRect rt=getClientRect();
	int i=m_tileBegin;
	int scr=getContentLocation().y;
	for(std::list<twDC *>::iterator it=tiles.begin();it!=tiles.end();it++){
		int tileHeight=tileHeightAtIndex(i);
		if(*it)
			dc->bitBlt(*it, twPoint(0, i*m_tileSize-scr), 
					   twRect(0, m_tileSize*i, rt.w, tileHeight));
		else{
			fillCheckerBoard(dc, twRect(0, i*m_tileSize-scr,
										rt.w, tileHeight));
		}
		i++;
	}
	
	int topBound=-scr;
	if(topBound>0)
		fillTopMargin(dc, twRect(0, 0, rt.w, topBound));
	
	int bottomBound=getContentSize().h-scr;
	if(bottomBound<rt.h)
		fillBottomMargin(dc, twRect(0, bottomBound, rt.w, rt.h-bottomBound));
		
	// draw scroll bar.
	if(getMaxContentLocation().y>0){
		int scrollBarHeight=rt.h-2;
		int trackBarHeight;
		trackBarHeight=rt.h*scrollBarHeight/getContentSize().h;
		
		if(scr<0)
			trackBarHeight+=scr;
		if(scr>getMaxContentLocation().y)
			trackBarHeight+=getMaxContentLocation().y-scr;
		if(trackBarHeight<4)
			trackBarHeight=4;
		
		twRect trackBarRect;
		trackBarRect.x=getClientRect().w-5;
		trackBarRect.w=4;
		
		trackBarRect.y=getContentLocation().y*(scrollBarHeight-trackBarHeight)/getMaxContentLocation().y;
		if(trackBarRect.y<0)
			trackBarRect.y=0;
		
		trackBarRect.h=trackBarHeight;
		if(trackBarRect.y+trackBarRect.h>scrollBarHeight)
			trackBarRect.y=scrollBarHeight-trackBarHeight;
		
		trackBarRect.y+=1;
		
		// round rect
		
		dc->dimRect(twRect(trackBarRect.x, trackBarRect.y+1,
						   trackBarRect.w, trackBarRect.h-2));
		dc->dimRect(twRect(trackBarRect.x+1, trackBarRect.y,
						   trackBarRect.w-2, 1));
		dc->dimRect(twRect(trackBarRect.x+1, 
						   trackBarRect.y+trackBarRect.h-1,
						   trackBarRect.w-2, 1));
		
	}
}
void XTBViewer::setContentLocation(const twPoint& newLocation){
	XTBScrollView::setContentLocation(newLocation);
	updateTiles();
	processInvalidTiles();
}
void XTBViewer::setContentSize(const twSize& newSize){
	XTBScrollView::setContentSize(twSize(getClientRect().w, newSize.h));
}

void XTBViewer::setRect(const twRect& newRect){
	twRect oldRect=getRect();
	XTBScrollView::setRect(newRect);
	setContentSize(getContentSize());
	if(newRect.w!=oldRect.w)
		invalidateAllTiles();
}

bool XTBViewer::isScrollableX() const{
	return false;
	
}

bool XTBViewer::isScrollableY() const{
	return true;
	
}

void XTBViewer::willAppear(){
	if(m_processingDeferred){
		m_processingDeferred=false;
		
		beginProcessing(true);
	}
	XTBScrollView::willAppear();
}

void XTBViewer::didDisappear(){
	XTBScrollView::didDisappear();
	
	// don't invalidate when different desktop
	twWnd *w=getParent();
	while(w->getParent())
		w=w->getParent();
	if(twGetDesktop()==w)
		invalidateAllTiles();
}

#pragma mark -
#pragma mark Tile Management

void XTBViewer::updateTiles(){
	twLock tilesLock(m_tilesLock);
	
	twPoint contentLocation=getContentLocation();
	twPoint finalLocation=getFinalContentLocation();
	int currentTileBegin=m_tileBegin;
	int currentTileEnd=currentTileBegin+m_tiles.size();
	int limitValidateHeight=getClientRect().h*5;
	if(isAnimating()){
		if(finalLocation.y>contentLocation.y+limitValidateHeight)
			finalLocation.y=contentLocation.y+limitValidateHeight;
		if(finalLocation.y<contentLocation.y-limitValidateHeight)
			finalLocation.y=contentLocation.y-limitValidateHeight;
	}else{
		finalLocation=contentLocation;
	}
	int newTileBegin=MAX(MIN(contentLocation.y, finalLocation.y)/m_tileSize, 0);
	int newTileEnd=MIN((MAX(contentLocation.y, finalLocation.y)+getClientRect().h+m_tileSize-1)/m_tileSize,
					   (getContentSize().h+m_tileSize-1)/m_tileSize);
	
	if(newTileBegin>=currentTileEnd || newTileEnd<=currentTileBegin){
		// completely other area
		while(!m_tiles.empty()){
			delete m_tiles.front();
			m_tiles.pop_front();
		}
		
		// add new tiles
		currentTileBegin=newTileBegin;
		currentTileEnd=currentTileBegin;
		m_tileBegin=newTileBegin;
		while(currentTileEnd<newTileEnd){
			m_tiles.push_back(NULL);
			currentTileEnd++;
		}
	}else{
		// some removed
		int updateTileBegin=MAX(currentTileBegin, newTileBegin);
		int updateTileEnd=MIN(currentTileEnd, newTileEnd);
		while(currentTileBegin<updateTileBegin){
			delete m_tiles.front();
			m_tiles.pop_front();
			currentTileBegin++;
			m_tileBegin++;
		}
		while(currentTileEnd>updateTileEnd){
			delete m_tiles.back();
			m_tiles.pop_back();
			currentTileEnd--;
		}
		
		// add new tiles
		while(currentTileBegin>newTileBegin){
			m_tiles.push_front(NULL);
			currentTileBegin--;
			m_tileBegin--;
		}
		while(currentTileEnd<newTileEnd){
			m_tiles.push_back(NULL);
			currentTileEnd++;
		}
	}
	
	
	
	invalidate();
}
void XTBViewer::processInvalidTiles(){
	if(m_nowProcessing)
		return;
	if(numberOfInvalidTiles()){
		// begin render
		beginProcessing();
	}
}
int XTBViewer::numberOfInvalidTiles(){
	twLock tilesLock(m_tilesLock);
	int invalidCount=0;
	if(getContentSize().h==0)
		return 0;
	for(std::list<twDC *>::iterator it=m_tiles.begin();it!=m_tiles.end();it++){
		if(*it==NULL)
			invalidCount++;
	}
	return invalidCount;
}
void XTBViewer::beginProcessing(bool forced){
	if(m_nowProcessing)
		return;
	
	if((isVisible()==false) && (forced==false)){
		m_processingDeferred=true;
		return;
	}
	
	m_nowProcessing=true;
	if(m_async){
		m_processLock->wait();
		tw_event->invokeInBackground(new twOneArgumentMemberFunctionInvocation<XTBViewer, 
									 int>(this, &XTBViewer::renderThread, m_currentSession));
	}else{
		renderAll();
	}
}
void XTBViewer::invalidateAllTiles(){
	{
		twLock tilesLock(m_tilesLock);
		
		// clear tiles.
		while(!m_tiles.empty()){
			delete m_tiles.front();
			m_tiles.pop_front();
		}
		
	}
	
	// next session.
	m_currentSession++;
	m_nowProcessing=false;
	
	// update.
	updateTiles();
	processInvalidTiles();
}
void XTBViewer::invalidateTile(int tileIndex){
	{
		twLock tilesLock(m_tilesLock);
		int index=m_tileBegin;
		std::list<twDC *>::iterator it=m_tiles.begin();
		while(it!=m_tiles.end()){
			if(index==tileIndex){
				delete *it;
				*it=NULL;
				break;
			}
			it++;
			index++;
		}
	}
	
	processInvalidTiles();
}

#pragma mark -
#pragma mark Validation

int XTBViewer::firstInvalidTileIndex(){
	twLock tilesLock(m_tilesLock);
	int i=m_tileBegin;
	for(std::list<twDC *>::iterator it=m_tiles.begin();it!=m_tiles.end();it++){
		if(*it==NULL)
			return i;
		i++;
	}
	return XTBViewerNotFound;
}

void XTBViewer::updateTileAtIndex(int index, twDC *tile, int session){
	if(session!=m_currentSession)
		return;
	twLock tilesLock(m_tilesLock);
	int i=m_tileBegin;
	//printf("tile %d updated\n", index);
	try{
		for(std::list<twDC *>::iterator it=m_tiles.begin();it!=m_tiles.end();it++){
			if(i==index){
				if(*it) // is already valid?
					delete *it; // remove old one and update.
				*it=tile;
				tw_event->invokeOnMainThread(new twOneArgumentMemberFunctionInvocation
											 <XTBViewer, int>(this, (void(XTBViewer::*)(int))&XTBViewer::remoteInvalidate, index));
				return;
			}	
			i++;
		}
	}catch(...){
		XTBLog("error in invokeOnMainThread.");
	} // exception in invokeOnMainThread.
	// update failed.
	delete tile;
}

int XTBViewer::tileHeightAtIndex(int ind) const{
	return MIN(m_tileSize, getContentSize().h-ind*m_tileSize);
}

twDC *XTBViewer::generateTileAtIndex(int ind){
	twRect rt=getClientRect();
	twSDLDC *dc;
	dc=new twSDLDC(twSize(rt.w, tileHeightAtIndex(ind)), twPoint(0, -ind*m_tileSize));
	try{
		if(tileHeightAtIndex(ind))
			paintContents(dc,dc->getBounds());
	}catch(const std::wstring& str){
		XTBLog("exception while filling the tile at index %d: %s", ind, twW2M(str).c_str());
	}catch(const std::exception& ex){
		XTBLog("exception while filling the tile at index %d: %s", ind, ex.what());
	}catch(...){
		XTBLog("exception while filling the tile at index %d.", ind);
	}
	return dc;
}

void XTBViewer::renderAll(){
	while(true){
		int processedTileIndex=firstInvalidTileIndex();
		if(processedTileIndex==XTBViewerNotFound){
			// no tile is invalid; bail out.
			break;
		}
		
		twDC *dc=generateTileAtIndex(processedTileIndex);
		assert(dc);
		
		updateTileAtIndex(processedTileIndex, dc, m_currentSession);
		
	}
	m_nowProcessing=false;
}

void XTBViewer::renderThread(int session){
	
	while(session==m_currentSession){
		int processedTileIndex=firstInvalidTileIndex();
		if(processedTileIndex==XTBViewerNotFound){
			// no tile is invalid; bail out.
			break;
		}
		
		twDC *dc=generateTileAtIndex(processedTileIndex);
		assert(dc);
		
		updateTileAtIndex(processedTileIndex, dc, session);
		
	}
	
	if(session==m_currentSession){
		m_nowProcessing=false;
		tw_event->invokeOnMainThread(new twNoArgumentMemberFunctionInvocation<XTBViewer>(this, &XTBViewer::remoteProcess));
	}
	m_processLock->post();
}

void XTBViewer::remoteInvalidate(int tileId){
	invalidateClientRect(twRect(0, tileId*m_tileSize-getContentLocation().y, getClientRect().w, m_tileSize));
}
void XTBViewer::remoteProcess(){
	processInvalidTiles();
}

void XTBViewer::setAsync(bool value){
	m_async=value;
}

#pragma mark -
#pragma mark Default Contents

void XTBViewer::paintContents(twDC *dc, twRect rt){
	twPoint p1, p2;
	p1=rt.topLeft();
	p2=rt.bottomRight();
	for(int x=p1.x/10*10;x<p2.x;x+=10)
		for(int y=p1.y/10*10;y<p2.y;y+=1)
			dc->fillRect((((x+y)/10)&1)?twRGB(255, 255, 255):
						 twRGB(192, 192, 192), twRect(x, y, 10, 10));
	
}



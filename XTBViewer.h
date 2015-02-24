/*
 *  XTBViewer.h
 *  XTBook
 *
 *  Created by Nexhawks on 2/20/11.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#include "XTBScrollView.h"
#include <tcw/twDC.h>
#include <tcw/twSDLSemaphore.h>

#pragma once

enum{
	XTBViewerNotFound=-1,
	XTBViewerProcessLockInitialValue=2
};

class XTBViewer : public XTBScrollView {
private:
	volatile int m_currentSession;
	// session is the number that changes each time this was reloaded, or 
	// horizonally resized.
	// updating tiles without correct session will be rejected.
	// note that m_nowProcessing meaning the process with m_currentSession
	// is running, so if the process had the wrong session number, 
	// m_nowProcessing would be false.
	
	int m_tileSize;
	int m_tileBegin;
	std::list<twDC *> m_tiles;
	bool m_nowProcessing;
	twSDLSemaphore *m_tilesLock;
	twSDLSemaphore *m_processLock;
	bool m_async;

	bool m_processingDeferred; // because it's hidden
	
	int tileHeightAtIndex(int) const;
	twDC *generateTileAtIndex(int);
	int firstInvalidTileIndex();
	void updateTileAtIndex(int index, twDC *, int session);
	void updateTiles();
	void processInvalidTiles();
	void renderThread(int session);
	void beginProcessing(bool forced=false);
	void renderAll();
	
	void remoteInvalidate(int tileId);
	void remoteProcess();
	
	virtual void fillTopMargin(twDC *, twRect) const;
	virtual void fillBottomMargin(twDC *, twRect) const;
	virtual void fillCheckerBoard(twDC *, twRect) const;
    
protected:
	
	virtual void paintContents(twDC *, twRect);
	
	virtual bool isScrollableX() const;
	virtual bool isScrollableY() const;
	

public:
	XTBViewer(int tileSize=128);
	virtual ~XTBViewer();
	virtual void clientPaint(const twPaintStruct&);
	virtual void setContentLocation(const twPoint&);
	virtual void setContentSize(const twSize&);
	virtual void setRect(const twRect&);
	int numberOfInvalidTiles();
	void invalidateAllTiles();
	void setAsync(bool);
	void invalidateTile(int);
	
	
	virtual void willAppear();
	virtual void didDisappear();
	
};

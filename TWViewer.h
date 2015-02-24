/*
 *  TWViewer.h
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

#include <vector>
#include "XTBViewer.h"
#include "TWiki/TWTypes.h"
#include "TWViewerImageManager.h"
#include <tcw/twTimer.h>

class twSDLSemaphore;
class TWHTMLElement;
class TWRenderObject;
class TWHTMLBodyElement;
class TWRenderView;
class TWRenderDC;
class TWRenderManager;
class TWRenderBoxModelObject;
class TWNode;

struct TWViewerTouchStartRenderObjectCallbackArgs;

enum TWViewerEventType{
	TWViewerEventTypeLinkTouched=0,
	TWViewerEventTypeLinkHolded,
	TWViewerEventTypeTextSelected
};

void touchStartRenderObjectCallbackDriver(void *data, TWNode *node);
	
class TWViewer : public XTBViewer{
	friend void touchStartRenderObjectCallbackDriver(void *data, TWNode *node);
private:
    TWHTMLBodyElement *m_htmlDocument;
    TWRenderView *m_renderView;
    TWRenderManager *m_fontManager;
	TWViewerImageManager m_imageManager;
    volatile bool m_layouted;
	bool m_needsLayout;
	int m_contentWidth;
	
	bool m_drag;
	TWHTMLElement *m_dragElement;
	twRect m_dragRect;
	
	bool m_selecting;
	unsigned int m_selectionStart;
	unsigned int m_selectionEnd;
	
	bool m_holded;
	twTimer *m_holdTimer;
	twPoint m_holdPos;
	
	twTicks m_lastInvalidTouch;
	
	std::map<std::wstring, TWRenderObject *> m_ids;
	std::map<std::wstring, TWHTMLElement *> m_domIds;
    
    twSDLSemaphore *m_renderViewSemaphore;
    void doLayout();
    void startLayout();
    
    virtual void paintContents(twDC *, twRect);
	
	bool isElementActive(TWHTMLElement *) const;
	TWHTMLElement *elementForRenderer(TWRenderObject *) const;
	TWRenderObject *activeRenderObjectAtPoint(const twPoint&) const;
	
	bool shouldLayoutAync() const;
	
	void dragEnded();
	void holded();
	
	void searchNodeForIds(TWRenderObject *);
	void searchNodeForDomIds(TWHTMLElement *);
	
	TWViewerEventType m_lastEventType;
	
protected:
	void touchStartRenderObjectCallback(TWViewerTouchStartRenderObjectCallbackArgs *,
										TWRenderBoxModelObject *) const;
	
	virtual void fillTopMargin(twDC *, twRect) const;
	virtual void fillBottomMargin(twDC *, twRect) const;
    
public:
	TWViewer();
	virtual ~TWViewer();
    
	// will be deleted in the destructor
    void setHTMLDocument(TWHTMLBodyElement *);
    TWHTMLBodyElement *htmlDocument() const;
    
    virtual void setRect(const twRect&);
	
	virtual void clientTouchStarted(const twPoint&, twMouseButton);
	virtual void clientTouchEnded(const twPoint&, twMouseButton);
	virtual void clientTouchCancelled(const twPoint&, twMouseButton);
	
	virtual void clientDragStarted(const twPoint&, twMouseButton);
	virtual void clientDragMoved(const twPoint&, twMouseButton);
	virtual void clientDragEnded(const twPoint&, twMouseButton);
	
	virtual void clientPaint(const twPaintStruct&);
	
	TWHTMLElement *getLastTouchedElement() const{return m_dragElement;}
	twRect getLastTouchedRect() const;
	bool getHolded() const{return m_holded;}
	TWViewerEventType getLastEventType() const{return m_lastEventType;}
	std::wstring getLastSelectedText() const;
	
	twRect rectForId(const std::wstring&) const;
	
	TWRenderObject *renderObjectForId(const std::wstring&) const;
	
	TWHTMLElement *htmlElementForId(const std::wstring&) const;
	
	
};

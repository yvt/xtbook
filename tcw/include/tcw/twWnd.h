/*
 *  twWindow.h
 *  
 *
 *  Created by Nexhawks on 10/01/13.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#pragma once

#include <list>
#include <wchar.h>
#include <string>
#include <tcw/twTypes.h>
#include <tcw/twDC.h>
#include <tcw/twFont.h>
#include <tcw/twEvent.h>
#include <tcw/twObject.h>

struct twPaintStruct{
	twDC *dc;
	twSize bound;
	twRect boundRect;
	twRect paintRect;
};

enum twBorderStyle{
	twBS_none=0,
	twBS_solid,
	twBS_panel,
	twBS_dialog,
	twBS_overlapped
};

struct twWndStyle{
	twWndStyle();
	bool enable;
	bool visible;
	twBorderStyle border;
	
	void validate() const;
};

struct twWndStatus{
	twWndStatus();
	twRect rect;
	bool enable;
	bool visible;
	bool focused;
	bool hover;
	bool defWnd;
};

enum twWndDrag{
	twWD_none=0,
	twWD_move,
	twWD_up,
	twWD_down,
	twWD_left,
	twWD_right,
	twWD_invalid
};

class twWnd : public twObject{
protected:
	int m_index;
	int m_id;
	twWnd *m_parent;
	std::wstring m_title;
	std::list<twWnd *> m_children;
	twRect m_rect;
	twWndStyle m_style;
	const twFont *m_font;
	twWnd *m_def;
	std::list<twWnd *> m_modals;
	twWnd *m_modalParent;
	bool m_keyEventHandled;
private:
	twWndDrag m_drag;
	twPoint m_dragPos;
public:
	twWnd();
	virtual ~twWnd();
	
	virtual void setParent(twWnd *newParent);
	twWnd *getParent() const;
	
	virtual void setModalParent(twWnd *newParent);
	twWnd *getModalParent() const;
	
	virtual void setTitle(const std::wstring&);
	const std::wstring& getTitle() const;
	
	virtual void setId(int i);
	int getId() const;
	
	virtual void setRect(const twRect&);
	const twRect& getRect() const;	// parent
	
	virtual void setStyle(const twWndStyle&);
	const twWndStyle& getStyle() const;
	
	virtual void setDefault(twWnd *);
	twWnd *getDefault() const;
	
	twWndStatus getStatus() const;
	
	virtual void setFont(const twFont *);
	const twFont *getFont() const;
	
	void enumChildren(void (*func)(twWnd *)) const;
	twRect getWndRect() const;		// screen
	twRect getClientRect() const;	// non-client
	twRect getVisibleWndRect() const; // screen, clipped
	void invalidate() const;
	void invalidateClientRect() const;
	void invalidateClientRect(const twRect&) const;
	void focus();
	void blur();
	twSize getBorderSize() const;
	twWnd *getRootWindow() const;
	
	void paint(const twRect&); // screen
	twWnd * hitTest(const twPoint&);
	const twWnd * hitTest(const twPoint&) const;
	
	virtual void show();
	virtual void hide();
	bool isVisible() const;
	
	twDC *getDC();
	twDC *getClientDC();
	twDC *getDC(const twRect&); // screen
	twDC *getClientDC(const twRect&); // screen
	
	void onKeyDown(const std::wstring&);
	void onKeyPress(wchar_t);
	void onKeyUp(const std::wstring&);
	void onMouseDown(const twPoint&, twMouseButton);
	void onMouseMove(const twPoint&);
	void onMouseUp(const twPoint&, twMouseButton);
	void transferFocus();
	
	void sendCmdToParent() const;
	
	twSize getMinSize() const;
	
	void clientKeyEventHandled();
	
	// as message in windows
	
	virtual void clientKeyDown(const std::wstring&);
	virtual void clientKeyPress(wchar_t);
	virtual void clientKeyUp(const std::wstring&);
	virtual void clientMouseDown(const twPoint&, twMouseButton);
	virtual void clientMouseMove(const twPoint&);
	virtual void clientMouseUp(const twPoint&, twMouseButton);
	virtual void clientMouseEnter(const twPoint&);
	virtual void clientMouseLeave(const twPoint&);
	virtual void clientPaint(const twPaintStruct&);
	virtual void clientEnter();
	virtual void clientLeave();
	virtual bool clientHitTest(const twPoint&) const;
	virtual void command(int);
	
	virtual void willAppear();
	virtual void didDisappear();
	
protected:
	
	void enter();
	void leave();
};

void twInvalidateRect(const twRect&);
void twSetDesktop(twWnd *);
twWnd *twGetDesktop();
void twValidate();
void twSetCapture(twWnd *, bool client=true);
void twReleaseCapture();

extern const twFont *tw_defFont;

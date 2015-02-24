//
//  XTBListView.h
//  XTBook
//
//  Created by Nexhawks on 4/16/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#pragma once

#include "XTBViewer.h"
#include <vector>

class XTBListViewDataSource;

enum XTBListViewStyle{
	XTBListViewStyleDefault=0,
	XTBListViewStyleValue1,
	XTBListViewStyleValue2,
	XTBListViewStyleSubtitle
};

enum{
	XTBListViewNotFound=-1
};

class XTBListView: public XTBViewer{
private:
	XTBListViewStyle m_style;
	XTBListViewDataSource *m_dataSource;
	int m_rowHeight;
	int m_activeRow;
	int m_lastActiveRow;
	int m_deactivateSession;
	
	bool m_keyboardMode;
	int m_keyboardPos;
	
	int rowHeightForStyle(XTBListViewStyle) const;
	int rowAtPoint(const twPoint&) const;
	twRect rectForRow(int) const;
	void deactivateRow(int session);
	
	void scrollToKeyboardPos();
	
protected:
	virtual void paintContents(twDC *, twRect);
	
	virtual void fillTopMargin(twDC *, twRect) const;
	virtual void fillBottomMargin(twDC *, twRect) const;

public:
	XTBListView(XTBListViewStyle style=XTBListViewStyleDefault);
	
	virtual void clientTouchStarted(const twPoint&, twMouseButton);
	virtual void clientTouchEnded(const twPoint&, twMouseButton);
	virtual void clientTouchCancelled(const twPoint&, twMouseButton);
	
	virtual void clientMouseDown(const twPoint&, twMouseButton);
	
	virtual void clientKeyDown(const std::wstring&);
	virtual void clientEnter();
	virtual void clientLeave();
	
	/** it's up to caller to release data source. */
	void setDataSource(XTBListViewDataSource *);
	XTBListViewDataSource *getDataSource() const{
		return m_dataSource;
	}
	void reloadData();
	void invalidateItem(int);
	int getLastActiveRow() const{
		return m_lastActiveRow;
	}
	
	virtual void itemTouched(const twPoint&, int row);
	
	virtual void willAppear();
	virtual void didDisappear();
};

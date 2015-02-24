//
//  XTBStartPageList.h
//  XTBook
//
//  Created by Kawada Tomoaki on 6/24/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#pragma once

#include <tcw/twWnd.h>

class XTBListView;
class XTBStartPageListDataSource;

class XTBStartPageList: public twWnd{

	static twDC *m_startPageListTopImage;
	static twDC *m_startPageListBottomImage;
	
	static twDC *getStartPageListTopImage();
	static twDC *getStartPageListBottomImage();
	
	XTBListView *m_listView;
	bool m_isEditing;
	
	bool m_reloadDataDeferred;
	
	twRect rectForListView() const;
	
	void reloadDataSource();
	
	bool m_selectAllowed;
	
	void selected();
	
public:
	
	XTBStartPageList();
	virtual ~XTBStartPageList();
	
	virtual void clientPaint(const twPaintStruct&);
	virtual bool clientHitTest(const twPoint&) const;
	
	virtual void setRect(const twRect&);
	virtual void command(int);
	
	void setDataSource(XTBStartPageListDataSource *);
	
	int getLastActiveRow() const;
	
	void reloadData();
	
	virtual void willAppear();
};


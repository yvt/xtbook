//
//  XTBContentsChooser.h
//  XTBook
//
//  Created by Kawada Tomoaki on 8/1/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#pragma once

#include <tcw/twDialog.h>
#include <tcw/twTimerWithInvocation.h>
#include <tcw/twEvent.h>
#include "XTBDialogSheet.h"

class XTBListView;
class XTBArticle;
class XTBContentsDataSource;

class XTBContentsChooser: public XTBDialogSheet{

	int m_lastSelectedIndex;
	int m_currentIndex;
	
	XTBListView *m_listView;
	XTBContentsDataSource *m_dataSource;
	
	bool m_selectAllowed;
	
	void selected();
protected:
	
	virtual twSize startupSize() const;
	
public:
	XTBContentsChooser(XTBArticle *, int);
	virtual ~XTBContentsChooser();
	
	virtual void backgroundTouched();
	
	virtual void command(int);
	
	int lastSelectedIndex() const{return m_lastSelectedIndex;}
};


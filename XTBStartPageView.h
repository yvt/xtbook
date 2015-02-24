//
//  XTBStartPageView.h
//  XTBook
//
//  Created by Kawada Tomoaki on 6/24/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#pragma once

#include <tcw/twWnd.h>
#include <tcw/twButton.h>

class XTBStartPageList;
class XTBStartPageBookmarkDataSource;
class XTBStartPageHistoryDataSource;

enum{
	XTBStartPageViewBookmarkId=1,
	XTBStartPageViewHistoryId,
	XTBStartPageViewAboutId
};

class XTBStartPageView: public twWnd{
	
	static twDC *getLogoTexted();
	
	XTBStartPageBookmarkDataSource *m_bookmarkData;
	XTBStartPageHistoryDataSource *m_historyData;
	
	XTBStartPageList *m_bookmarkList;
	XTBStartPageList *m_historyList;
	
	twButton *m_aboutButton;
	
	std::wstring m_lastSelectedUrl;
	
	bool m_canSendCmd;
	
	void sendCmd();
	
	int logoHeight() const;
	
public:
	XTBStartPageView();
	virtual ~XTBStartPageView();
	
	virtual void setRect(const twRect&);
	
	virtual void command(int);
	
	const std::wstring& getLastSelectedUrl() const{
		return m_lastSelectedUrl;
	}
	
	XTBStartPageList *bookmarList() const{return m_bookmarkList;}
	XTBStartPageList *historyList() const{return m_historyList;}
	
	virtual void clientPaint(const twPaintStruct&);
};

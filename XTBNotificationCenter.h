//
//  XTBNotificationCenter.h
//  XTBook
//
//  Created by Kawada Tomoaki on 6/25/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#pragma once

#include <tcw/twObject.h>
#include <tcw/twInvocation.h>

#include <set>
#include <map>
typedef twInvocation *XTBNotificationHandler;


struct XTBNotification{
	std::wstring name;
	std::set<XTBNotificationHandler> handlers;
	
	XTBNotification();
	~XTBNotification();
};



class XTBNotificationCenter: public twObject{
	std::map<std::wstring, XTBNotification> m_notifications;
public:
	XTBNotificationCenter();
	virtual ~XTBNotificationCenter();
	
	XTBNotificationHandler registerHandler(const std::wstring& notificationName,
						 twInvocation *invocation);
	void unregisterHandler(const std::wstring& notificationName,
						   XTBNotificationHandler handler);
	
	void sendNotification(const std::wstring&);
	
	static XTBNotificationCenter *defaultNotificationCenter();
};


#define XTBBookmarkModifiedNotification	L"XTBBookmarkModifiedNotification"
#define XTBHistoryModifiedNotification	L"XTBHistoryModifiedNotification"
#define XTBDisplayTitleChangedNotification	L"XTBDisplayTitleChangedNotification"


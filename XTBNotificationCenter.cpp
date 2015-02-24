//
//  XTBNotificationCenter.cpp
//  XTBook
//
//  Created by Kawada Tomoaki on 6/25/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "XTBNotificationCenter.h"

XTBNotification::XTBNotification(){}

XTBNotification::~XTBNotification(){}

XTBNotificationCenter::XTBNotificationCenter(){
	
}

XTBNotificationCenter::~XTBNotificationCenter(){
	
}

XTBNotificationHandler XTBNotificationCenter::registerHandler(const std::wstring &notificationName, twInvocation *invocation){
	XTBNotification& n=m_notifications[notificationName];
	n.handlers.insert(invocation);
	return invocation;
}

void XTBNotificationCenter::unregisterHandler(const std::wstring& notificationName,
											  XTBNotificationHandler invocation){
	XTBNotification& n=m_notifications[notificationName];
	std::set<XTBNotificationHandler>::iterator it;
	it=n.handlers.find(invocation);
	if(it!=n.handlers.end()){
		n.handlers.erase(it);
		
	}
	delete invocation;
}

void XTBNotificationCenter::sendNotification(const std::wstring & notificationName){
	XTBNotification& n=m_notifications[notificationName];
	std::set<XTBNotificationHandler> invalidHandlers;
	std::set<XTBNotificationHandler>::iterator it;
	for(it=n.handlers.begin();it!=n.handlers.end();it++){
		twInvocation *i=*it;
		if(!i->m_valid){
			invalidHandlers.insert(i);
			continue;
		}
		i->invoke();
	}
	for(it=invalidHandlers.begin();it!=invalidHandlers.end();
		it++){
		std::set<XTBNotificationHandler>::iterator it2;
		it2=n.handlers.find(*it);
		if(it2!=n.handlers.end()){
			n.handlers.erase(it2);
		}

	}
}

static XTBNotificationCenter *m_defaultNotificationCenter=NULL;

XTBNotificationCenter *XTBNotificationCenter::defaultNotificationCenter(){
	if(m_defaultNotificationCenter==NULL){
		m_defaultNotificationCenter=new XTBNotificationCenter();
	}
	return m_defaultNotificationCenter;
}

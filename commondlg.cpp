//
//  commondlg.cpp
//  XTBook
//
//  Created by Nexhawks on 4/21/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "commondlg.h"
#include "XTBProgressOverlay.h"
#include <tcw/twApp.h>
#include <tcw/twEvent.h>
#include <assert.h>
#include <tcw/twSDLSemaphore.h>
#include "XTBException.h"
#include <tcw/twStrConv.h>

static XTBProgressOverlay *g_progressOverlay=NULL;
static twSDLSemaphore *g_progressWaitDisplaySemaphore=NULL;

static void endProgressOverlay(){
	assert(g_progressOverlay);
	g_progressOverlay->endDialog();
}

static void progressOverlayBackgroundThread(twInvocation *inv){
	
	g_progressWaitDisplaySemaphore->lock();
	g_progressWaitDisplaySemaphore->unlock();
	
	// wait for the progress overlay to open completely
	//tw_event->sleep(100);
	
	try{
		inv->invoke();
		
	}catch(const XTBException& ex){
		XTBLog("exception thrown in a task with a progress overlay: %s", twW2M(ex.description()).c_str());
	}catch(const std::exception& ex){
		XTBLog("exception thrown in a task with a progress overlay: %s", ex.what());
	}
	
	// close the progress overlay remotely
	tw_event->invokeOnMainThread(new twNoArgumentGlobalFunctionInvocation(endProgressOverlay));

	delete inv;
	
}


void XTBInvokeWithProgressOverlay(twInvocation *backgroundInvocation){
	
	if(g_progressOverlay){
		// already running! we cannot nest the progress overlay.
		// use the existing progress overlay.
		
		backgroundInvocation->invoke();
		delete backgroundInvocation;
		
		return;
	}
	
	if(!g_progressWaitDisplaySemaphore){
		g_progressWaitDisplaySemaphore=new twSDLSemaphore();
	}
	
	g_progressWaitDisplaySemaphore->lock();
	
	tw_event->invokeInBackground(new twOneArgumentGlobalFunctionInvocation<twInvocation *>(progressOverlayBackgroundThread, backgroundInvocation));
	
	g_progressOverlay=new XTBProgressOverlay();
	g_progressWaitDisplaySemaphore->unlock();
	g_progressOverlay->showModal(NULL);
	
	delete g_progressOverlay;
	g_progressOverlay=NULL;
	
}
void XTBSetProgressText(const std::wstring& msg){
	if(g_progressOverlay){
		g_progressOverlay->setMessage(msg);
		
	}
}
void XTBSetProgressPercentage(float per){
	if(g_progressOverlay){
		g_progressOverlay->setPercentage(per);
		
	}
}


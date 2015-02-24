//
//  XTBEventManager.h
//  XTBook
//
//  Created by Kawada Tomoaki on 5/29/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#pragma once

#include <tcw/twEvent.h>
#include <tcw/twSDLEvent.h>

class XTBEventManager: public twSDLEvent{
protected:
	virtual bool dealEvent(SDL_Event& ev);
public:
	XTBEventManager();
	virtual ~XTBEventManager();
};

twEvent *twCreateEvent();

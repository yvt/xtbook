//
//  XTBMenuItem.h
//  XTBook
//
//  Created by Kawada Tomoaki on 5/19/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#pragma once

#include <string>
#include <tcw/twObject.h>
#include <tcw/twFont.h>
#include <tcw/twDC.h>

class XTBMenuItem: public twObject{
private:
	
protected:
	
	const twFont *m_font;
public:
	XTBMenuItem();
	virtual ~XTBMenuItem();
	
	virtual int getHeight() const=0;
	virtual int getMinWidth() const=0;
	
	virtual bool isSelectable() const;
	virtual void render(twDC *, const twRect&, bool selected);
	
};

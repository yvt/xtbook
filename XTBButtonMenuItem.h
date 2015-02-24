//
//  XTBButtonMenuItem.h
//  XTBook
//
//  Created by Kawada Tomoaki on 5/19/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#pragma once

#include "XTBActionMenuItem.h"

class XTBButtonMenuItem: public XTBActionMenuItem{
private:
protected:
	std::wstring m_text;
public:
	XTBButtonMenuItem();
	XTBButtonMenuItem(const std::wstring&);
	
	virtual int getHeight() const;
	virtual int getMinWidth() const;
	
	virtual void render(twDC *, const twRect&, bool selected);
};

//
//  XTBLabelMenuItem.h
//  XTBook
//
//  Created by Kawada Tomoaki on 5/27/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#pragma once

#include "XTBMenuItem.h"

class XTBLabelMenuItem: public XTBMenuItem{
private:
protected:
	std::wstring m_text;
public:
	XTBLabelMenuItem();
	XTBLabelMenuItem(const std::wstring&);
	
	virtual int getHeight() const;
	virtual int getMinWidth() const;
	
	virtual void render(twDC *, const twRect&, bool selected);
};


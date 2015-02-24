/*
 *  twVGAFont.h
 *  
 *
 *  Created by Nexhawks on 10/01/16.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#pragma once

#include <tcw/twSDLDC.h>
#include <tcw/twFont.h>
#include <wchar.h>
#include <string>

class twVGAFont : public twFont{
protected:
	twSDLDC *fnt;
	mutable twColor ocolor;
	
	virtual void setColor(twColor col) const;
	
public:
	twVGAFont();
	virtual ~twVGAFont();
	virtual void render(twDC *, twColor, const twPoint&, const std::wstring&) const;
	virtual twSize measure(const std::wstring&) const;
};


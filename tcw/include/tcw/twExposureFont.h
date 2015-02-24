/*
 *  twExposureFont.h
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

class twExposureFont : public twFont{
protected:
	twSDLDC *fnt;
	twRect glyphs[65536];
	mutable Uint32 ocolor;
	int size;
	
	virtual void setColor(Uint32 col) const;
	
public:
	twExposureFont(const std::wstring& nm);
	virtual ~twExposureFont();
	virtual void render(twDC *, twColor, const twPoint&, const std::wstring&) const;
	virtual twSize measure(const std::wstring&) const;
};

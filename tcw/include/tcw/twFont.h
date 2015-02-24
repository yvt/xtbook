/*
 *  twFont.h
 *  
 *
 *  Created by Nexhawks on 10/01/16.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#pragma once

#include <tcw/twTypes.h>
#include <tcw/twDC.h>
#include <wchar.h>
#include <string>

class twFont{
protected:
	
public:
	twFont();
	virtual ~twFont();
	virtual void render(twDC *, twColor, const twPoint&, const std::wstring&) const=0;
	virtual twSize measure(const std::wstring&) const=0;
	
	std::wstring wrapString(const std::wstring&, int maxWidth) const;
};

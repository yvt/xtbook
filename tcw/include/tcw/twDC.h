/*
 *  twDC.h
 *  
 *
 *  Created by Nexhawks on 10/01/13.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#pragma once

#include <tcw/twTypes.h>

class twDC{
protected:
	
	twRect m_clipRect;
	bool hasClip(); // false if nothing will be drawn because of clipRect
	
public:
	
	twDC();
	virtual ~twDC();
	
	virtual void resetClipRect();
	virtual void setClipRect(const twRect&);
	const twRect& getClipRect() const;
	void addClipRect(const twRect&);
	
	virtual void clear(twColor);
	virtual void drawPoint(twColor, const twPoint&);
	virtual void drawLine(twColor, const twPoint&, const twPoint&);
	virtual void drawRect(twColor, const twRect&);
	virtual void drawEllipse(twColor, const twRect&);
	virtual void fillRect(twColor, const twRect&)=0;
	virtual void fillEllipse(twColor, const twRect&);
	virtual void fillTriangle(twColor, const twPoint&, const twPoint&, const twPoint&);
	virtual void bitBlt(const twDC *, const twPoint& dest, const twRect& src);
	virtual void stretchBlt(const twDC *, const twRect& dest, const twRect& src);
	
	virtual void dimRect(const twRect&)=0;
};

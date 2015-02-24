/*
 *  twSDLDC.h
 *  
 *
 *  Created by Nexhawks on 10/01/14.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#pragma once

#include <tcw/twDC.h>
#include <SDL/SDL.h>
#include <wchar.h>
#include <string>
#include <tcw/twTimer.h>

class twSDLDC : public twDC{
protected:
	SDL_Surface *m_surf;
	twPoint m_origin;
	bool m_local;
	
	twRect defaultClipRectRaw();
	
	void stretchLine(uint16_t *dest, unsigned int pixels,
					uint16_t *src1, uint16_t *src2,
					int srcX, int srcDeltaX, int srcYFrac) const;
	void stretchLine(uint16_t *dest, unsigned int pixels,
					uint16_t *src, 
					int srcX, int srcDeltaX) const;
	
	
public:
	twSDLDC(SDL_Surface *surface, const twPoint& origin, bool localize=false);
	twSDLDC(const twSize&, const twPoint& origin=twPoint(0, 0));
	virtual ~twSDLDC();
	
	static twSDLDC *loadFrom(const std::wstring&);
	static twSDLDC *loadFromXPM(const char **);
	
	virtual void resetClipRect();
	virtual void setClipRect(const twRect&);
	
	virtual void fillRect(twColor, const twRect&);
	
	virtual void bitBlt(const twDC *, const twPoint& dest, const twRect& src);
	virtual void stretchBlt(const twDC *dc, const twRect& dest, const twRect& src);
	
	virtual twRect getBounds();
	virtual SDL_Surface *getSurface();
	
	virtual void dimRect(const twRect&);
	
	/** convert surface into screen format, making blit faster. */
	void optimize();
};

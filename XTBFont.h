/*
 *  XTBFont.h
 *  XTBook
 *
 *  Created by Nexhawks on 2/11/11.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#pragma once

#include <stdint.h>
#include <tcw/twFont.h>

class twSDLDC;

class XTBFont : public twFont{
protected:
	unsigned char *fontData;
	unsigned int *indexMap;
	unsigned char *widthMap;
	
	int topMargin;
	int actualLineHeight;
	
	virtual void drawGlyph(twSDLDC *, uint16_t color, twPoint pt, uint16_t chr) const=0;
	virtual void bitmapForGlyph(uint8_t *, uint16_t)const=0;
	
	void drawGlyphScaled(twSDLDC *, uint16_t, twRect, uint16_t, int baseWidth, int alpha) const;
	
	int widthForScaledGlyph(int baseWidth, int height) const;
	
	void drawGlyphScaledSpan(uint16_t *destSpan,
							 const uint8_t *srcLine1,
							 const uint8_t *srcLine2,
							 int pixels,
							 int startSrcX,
							 int deltaSrcX,
							 int fSrcY, // [0-0xffff]
							 uint16_t nativeColor,
							 int alpha,
							 int lastSrcX) const;
	
	void drawGlyphScaledSpan(uint16_t *destSpan,
							 const uint8_t *srcLine,
							 int pixels,
							 int startSrcX,
							 int deltaSrcX,
							 uint16_t nativeColor,
							 int alpha,
							 int lastSrcX) const;
	
public:
	XTBFont(unsigned char *fontData, unsigned int *indexMap, unsigned char *widthMap);
	virtual void render(twDC *, twColor, const twPoint&, const std::wstring&) const;
	virtual twSize measure(const std::wstring&) const;

	/** advanced rendering.
	 * @param alpha alpha value in range [0-255]. 
	 * @size height of character. */
	void render(twDC *, twColor, const twPoint&, const std::wstring&, int size, int alpha) const; 

	twSize measure(const std::wstring&, int size) const;
	
};


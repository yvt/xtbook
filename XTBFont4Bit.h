/*
 *  XTBFont4Bit.h
 *  XTBook
 *
 *  Created by Nexhawks on 2/11/11.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#pragma once
#include "XTBFont.h"

class XTBFont4Bit : public XTBFont{
protected:
	void drawBlock(twSDLDC *, uint16_t color, twPoint pt, const unsigned char *data) const;
	virtual void drawGlyph(twSDLDC *, uint16_t color, twPoint pt, uint16_t chr) const;
	virtual void bitmapForGlyph(uint8_t *, uint16_t) const;
	void placeBlock(uint8_t *dest, size_t pitch, const unsigned char *data, int rows) const;
public:
	XTBFont4Bit(unsigned char *fontData, unsigned int *indexMap, unsigned char *widthMap);
};



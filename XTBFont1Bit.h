/*
 *  XTBFont1Bit.h
 *  XTBook
 *
 *  Created by Nexhawks on 2/11/11.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#pragma once
#include "XTBFont.h"

class XTBFont1Bit : public XTBFont{
protected:
	virtual void drawBlock(twSDLDC *, uint16_t color, twPoint pt, const unsigned char *data) const;
	virtual void drawGlyph(twSDLDC *, uint16_t color, twPoint pt, uint16_t chr) const;
public:
	XTBFont1Bit(unsigned char *fontData, unsigned int *indexMap, unsigned char *widthMap);
};


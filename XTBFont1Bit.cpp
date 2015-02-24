/*
 *  XTBFont1Bit.cpp
 *  XTBook
 *
 *  Created by Nexhawks on 2/11/11.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#include "XTBFont1Bit.h"
#include <tcw/twSDLDC.h>

XTBFont1Bit::XTBFont1Bit(unsigned char *fontData, unsigned int *indexMap, unsigned char *widthMap)
:XTBFont(fontData, indexMap, widthMap){
}

void XTBFont1Bit::drawBlock(twSDLDC *dc, uint16_t color, twPoint pt, const unsigned char *data) const{
	SDL_Surface *surface=dc->getSurface();
	uint16_t *pixels=(uint16_t *)(surface->pixels);
	int pitch=surface->pitch/2;
	
	twRect clipRect=dc->getClipRect();
	twRect bounds=dc->getBounds();
	clipRect.x-=bounds.x;
	clipRect.y-=bounds.y;
	clipRect.w+=clipRect.x;
	clipRect.h+=clipRect.y;
	
	if(clipRect.x<0){
		clipRect.w-=clipRect.x;
		clipRect.x=0;
	}
	if(clipRect.y<0){
		clipRect.h-=clipRect.y;
		clipRect.y=0;
	}
	if(clipRect.w>surface->w){
		clipRect.w=surface->w;
	}
	if(clipRect.h>surface->h){
		clipRect.h=surface->h;
	}
	
	// cull out the invisible block
	if(pt.x<=clipRect.x-6 || pt.y<=clipRect.y-12 || pt.x>=clipRect.w || pt.y>=clipRect.h)
		return;
	
	if(pt.x>=clipRect.x && pt.y>=clipRect.y && pt.x+6<=clipRect.w && pt.y+12<=clipRect.h){
		// if the block is completely visible...
		pixels+=pt.x+pt.y*pitch;
		register unsigned char lastData=*data;
		register int bitPos=0;
		for(int h=0;h<12;h++){
			register uint16_t *pixels2=pixels;
			for(int x=0;x<6;x++){
				if(lastData&1)
					*pixels2=color;
				bitPos++;
				if(bitPos==8){
					// next data!
					bitPos=0;
					data++;
					lastData=*data;
				}else{
					lastData>>=1;
				}
				pixels2++;
			}
			pixels+=pitch;
		}
	}else{
		// or, if the block is partially visible... clip.
		pixels+=pt.x+pt.y*pitch;
		register unsigned char lastData=*data;
		register int bitPos=0;
		register int px, py;
		py=pt.y;
		for(int h=0;h<12;h++){
			register uint16_t *pixels2=pixels;
			px=pt.x;
			for(int x=0;x<6;x++){
				if(lastData&1){
					if(px>=clipRect.x && py>=clipRect.y && px<clipRect.w && py<clipRect.h)
						*pixels2=color;
				}
				bitPos++;
				if(bitPos==8){
					// next data!
					bitPos=0;
					data++;
					lastData=*data;
				}else{
					lastData>>=1;
				}
				pixels2++;
				px++;
			}
			py++;
			pixels+=pitch;
		}
	}
	
}

void XTBFont1Bit::drawGlyph(twSDLDC *dc, uint16_t color, twPoint pt, uint16_t chr) const{
	if(indexMap[chr]==0xffffffffU){
		// the glyph isn't available.
		return;
	}
	
	int blockCount=(widthMap[chr]+5)/6;
	unsigned char *data=fontData+indexMap[chr];
	
	for(int block=0;block<blockCount;block++){
		drawBlock(dc, color, twPoint(pt.x, pt.y), data);
		pt.x+=6;
		data+=9;
	}	
}

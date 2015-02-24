/*
 *  XTBFont4Bit.cpp
 *  XTBook
 *
 *  Created by Nexhawks on 2/11/11.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#include "XTBFont4Bit.h"
#include <tcw/twSDLDC.h>

XTBFont4Bit::XTBFont4Bit(unsigned char *fontData, unsigned int *indexMap, unsigned char *widthMap)
:XTBFont(fontData, indexMap, widthMap){
}

#define doDraw()											\
do{														\
register int alphaValue=lastData&0xf;				\
if(alphaValue==0)									\
break;											\
register unsigned int tmp1=*pixels2;				\
register unsigned int tmp2, tmp3=0;					\
tmp2=(tmp1&0xF81F)*(0x10-alphaValue);				\
tmp2+=((unsigned int)color&0xF81F)*alphaValue;		\
tmp3=(tmp2>>4)&0xF81F;								\
tmp2=(tmp1&0x7E0)*(0x10-alphaValue);				\
tmp2+=((unsigned int)color&0x7E0)*alphaValue;		\
tmp3|=(tmp2>>4)&0x7FE0;								\
*pixels2=tmp3;										\
}while(0)



void XTBFont4Bit::drawBlock(twSDLDC *dc, uint16_t color, twPoint pt, const unsigned char *data) const{
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
				doDraw();
				bitPos++;
				if(bitPos==2){
					// next data!
					bitPos=0;
					data++;
					lastData=*data;
				}else{
					lastData>>=4;
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
				
				if(px>=clipRect.x && py>=clipRect.y && px<clipRect.w && py<clipRect.h)
					doDraw();
				
				bitPos++;
				if(bitPos==2){
					// next data!
					bitPos=0;
					data++;
					lastData=*data;
				}else{
					lastData>>=4;
				}
				pixels2++;
				px++;
			}
			py++;
			pixels+=pitch;
		}
	}
	
}

void XTBFont4Bit::drawGlyph(twSDLDC *dc, uint16_t color, twPoint pt, uint16_t chr) const{
	if(indexMap[chr]==0xffffffffU){
		// the glyph isn't available.
		return;
	}
	int w=widthMap[chr];
	int blockCount=0;
	while(w>0){
		blockCount++;
		w-=6;
	} // blockCount=(w+5)/6
	unsigned char *data=fontData+indexMap[chr];
	
	for(int block=0;block<blockCount;block++){
		drawBlock(dc, color, twPoint(pt.x, pt.y), data);
		pt.x+=6;
		data+=36;
	}	
}

void XTBFont4Bit::bitmapForGlyph(uint8_t *glyphBuf, uint16_t chr) const{
	uint8_t *buf=glyphBuf;
	
	int w=widthMap[chr];
	int blockCount=0;
	while(w>0){
		blockCount++;
		w-=6;
	} // blockCount=(w+5)/6
	
	w=widthMap[chr];
	
	unsigned char *data=fontData+indexMap[chr];
	int leftW=w;
	
	for(int block=0;block<blockCount;block++){
		placeBlock(buf, w, data, std::min(6, leftW));
		data+=36;
		buf+=6;
		leftW-=6;
	}
	
	
}

void XTBFont4Bit::placeBlock(uint8_t *dest, size_t pitch,
							 const unsigned char *data, int rows) const{
	
	for(int h=0;h<12;h++){
		uint8_t *dest2=dest;
		const unsigned char *data2=data;
		register unsigned char lastData=*data2;
		register int bitPos=0;
		register int x;
		for(x=0;x<rows;x++){
			{
				register uint8_t px=lastData;
				px=px&0xf;
				px|=px<<4;
				*(dest2++)=px;
			}
			bitPos++;
			if(bitPos==2){
				bitPos=0;
				data2++;
				lastData=*data2;
			}else{
				lastData>>=4;
			}
		}
		
		dest+=pitch;
		data+=3;
		
	}
}





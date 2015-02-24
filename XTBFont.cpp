/*
 *  XTBFont.cpp
 *  XTBook
 *
 *  Created by Nexhawks on 2/11/11.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#include "XTBFont.h"
#include <tcw/twSDLDC.h>
#include <tcw/twException.h>
#include "XTBException.h"
#include <assert.h>
#ifdef EV_PLATFORM_WIN32
#include <malloc.h>
#endif

XTBFont::XTBFont(unsigned char *_fontData, unsigned int *_indexMap, unsigned char *_widthMap){
	fontData=_fontData;
	indexMap=_indexMap;
	widthMap=_widthMap;
	topMargin=0;
	actualLineHeight=12;
}
void XTBFont::render(twDC *dc, twColor color, const twPoint& pos, const std::wstring& str) const{
	if(!dc)
		return;
	twSDLDC *sdlDC=dynamic_cast<twSDLDC *>(dc);
	if(sdlDC==NULL){
		XTBThrowException(L"XTBFontNotSDLDCException", NULL);
	}
	
	SDL_Surface *surf=sdlDC->getSurface();
	if(surf->format->BitsPerPixel!=16){
		XTBThrowException(L"XTBFontNot16BitColorDepthException", NULL);
		throw twException(L"XTBFont supports only 16-bit color depth.");
	}
	
	SDL_LockSurface(surf);
	
	uint16_t nativeColor=(uint16_t)SDL_MapRGB(surf->format, twGetR(color), twGetG(color), twGetB(color));
	
	wchar_t br=0;
	twPoint pt=pos;
	twRect rt=sdlDC->getBounds();
	pt.x-=rt.x;
	pt.y-=rt.y;
	
	pt.y-=topMargin;
	
	for(int n=0;n<str.length();n++){
		
		if(br==0){
			if(str[n]==13 || str[n]==10)
				br=str[n];
		}
		
		if(str[n]==br){
			pt.x=pos.x-rt.x;
			pt.y+=actualLineHeight;
			continue;
		}else if(str[n]==13 || str[n]==10){
			continue;
		}	
		
		uint16_t charCode=(uint16_t)str[n];
		drawGlyph(sdlDC, nativeColor, pt, charCode);
		pt.x+=widthMap[charCode];
		if(charCode<128){
			if(widthMap[charCode]>1)
				pt.x--;
		}
	}
	
	SDL_UnlockSurface(surf);
}
twSize XTBFont::measure(const std::wstring& str) const{
	wchar_t br=0;
	twPoint pt(0,actualLineHeight);
	int mw=0;
	
	for(int n=0;n<str.length();n++){
		
		if(br==0){
			if(str[n]==13 || str[n]==10)
				br=str[n];
		}
		
		if(str[n]==br){
			pt.x=0;
			pt.y+=actualLineHeight;
			continue;
		}else if(str[n]==13 || str[n]==10){
			continue;
		}	
		
		uint16_t charCode=(uint16_t)str[n];
		pt.x+=widthMap[charCode];
		if(charCode<128){
			if(widthMap[charCode]>1)
				pt.x--;
		}
		if(pt.x>mw)
			mw=pt.x;
		
	}
	
	return twSize(mw, pt.y);
}

int XTBFont::widthForScaledGlyph(int baseWidth, int height) const{
	return baseWidth*height/actualLineHeight;
}

/* scaled version. */
void XTBFont::render
(twDC *dc, twColor color, const twPoint& pos,
 const std::wstring& str, int size, int alpha) const{
	
	if(size==actualLineHeight && alpha>=255){
		// fast renderer.
		render(dc, color, pos ,str);
		return;
	}
	
	if(size>300)
		size=300;
	
	if(!dc)
		return;
	twSDLDC *sdlDC=dynamic_cast<twSDLDC *>(dc);
	if(sdlDC==NULL){
		XTBThrowException(L"XTBFontNotSDLDCException", NULL);
	}
	
	SDL_Surface *surf=sdlDC->getSurface();
	if(surf->format->BitsPerPixel!=16){
		XTBThrowException(L"XTBFontNot16BitColorDepthException", NULL);
		throw twException(L"XTBFont supports only 16-bit color depth.");
	}
	
	SDL_LockSurface(surf);
	
	uint16_t nativeColor=(uint16_t)SDL_MapRGB(surf->format, twGetR(color), twGetG(color), twGetB(color));
	
	wchar_t br=0;
	twPoint pt=pos;
	twRect rt=sdlDC->getBounds();
	pt.x-=rt.x;
	pt.y-=rt.y;
	
	int charHeight=(12*size)/actualLineHeight;
	
	// offset of topMargin.
	pt.y-=topMargin*size/actualLineHeight;
	
	for(int n=0;n<str.length();n++){
		
		if(br==0){
			if(str[n]==13 || str[n]==10)
				br=str[n];
		}
		
		if(str[n]==br){
			pt.x=pos.x-rt.x;
			pt.y+=size;
			continue;
		}else if(str[n]==13 || str[n]==10){
			continue;
		}	
		
		uint16_t charCode=(uint16_t)str[n];
		int baseWidth=widthMap[charCode];
		int scaledWidth=widthForScaledGlyph(baseWidth, size);
		
		drawGlyphScaled(sdlDC, nativeColor,
						twRect(pt.x, pt.y, scaledWidth, charHeight),
						charCode, baseWidth, alpha);
		
		pt.x+=scaledWidth;
		if(charCode<128){
			if(scaledWidth>1 && scaledWidth>=baseWidth)
				pt.x--;
		}
	}
	
	SDL_UnlockSurface(surf);

}

void XTBFont::drawGlyphScaled(twSDLDC *sdlDC, uint16_t nativeColor,
							  twRect dest, uint16_t charCode,
							  int baseWidth, int alpha) const{
	
	twRect clipRect=sdlDC->getClipRect();
	twRect bounds=sdlDC->getBounds();
	clipRect.x-=bounds.x;
	clipRect.y-=bounds.y;
	clipRect.w+=clipRect.x;
	clipRect.h+=clipRect.y;
	
	// clipped out?
	if(!(dest&&clipRect))
		return ;
	
	SDL_Surface *surf=sdlDC->getSurface();
	// surf is already locked.
	assert(surf->pixels!=NULL);
	
	if(baseWidth==0)
		return;
	if(indexMap[charCode]==0xffffffffU)
		return;
	
	if(alpha<=0)
		return;
	if(alpha>256)
		alpha=256;
	
	
	int startDestX;
	int endDestX;
	int startSrcX;
	int deltaSrcX;
	
	int startDestY;
	int endDestY;
	int startSrcY;
	int deltaSrcY;
	
	// calculate parameters.
	startDestX=dest.x;
	endDestX=dest.x+dest.w;
	startSrcX=0;
	deltaSrcX=(baseWidth<<16)/dest.w;
	
	startDestY=dest.y;
	endDestY=dest.y+dest.h;
	startSrcY=0;
	deltaSrcY=(12<<16)/dest.h;
	
	// clip.
	if(startDestX<clipRect.x){
		startSrcX+=(clipRect.x-startDestX)*deltaSrcX;
		startDestX=clipRect.x;
	}
	if(startDestY<clipRect.y){
		startSrcY+=(clipRect.y-startDestY)*deltaSrcY;
		startDestY=clipRect.y;
	}
	if(endDestX>clipRect.x+clipRect.w)
		endDestX=clipRect.x+clipRect.w;
	if(endDestY>clipRect.y+clipRect.h)
		endDestY=clipRect.y+clipRect.h;
	
	if(startSrcX+deltaSrcX*(endDestX-startDestX-1)>((baseWidth-1)<<16)){
		int over=startSrcX+deltaSrcX*(endDestX-startDestX-1);
		over-=((baseWidth-1)<<16);
		endDestX-=(over+deltaSrcX-1)/deltaSrcX;
	}
	if(startSrcY+deltaSrcY*(endDestY-startDestY-1)>(11<<16)){
		int over=startSrcY+deltaSrcY*(endDestY-startDestY-1);
		over-=(11<<16);
		endDestY-=(over+deltaSrcY-1)/deltaSrcY;
	}
	
	if(startDestX>=endDestX)
		return;
	if(startDestY>=endDestY)
		return;
	
	assert(startDestX>=clipRect.x);
	assert(startDestY>=clipRect.y);
	assert(endDestX<=clipRect.x+clipRect.w);
	assert(endDestY<=clipRect.y+clipRect.h);
	assert(startSrcX>=0);
	assert(startSrcY>=0);
	assert(startSrcX+deltaSrcX*(endDestX-startDestX-1)<=((baseWidth-1)<<16));
	assert(startSrcY+deltaSrcY*(endDestY-startDestY-1)<=(11<<16));
	
	int srcY=startSrcY;
	uint16_t *destPixels=(uint16_t *)surf->pixels;
	size_t pitchPixels=surf->pitch/2; // ???: surf->pitch/2 ?
	
	destPixels+=startDestX;
	destPixels+=startDestY*pitchPixels;
	
	int pixelsPerSpan=endDestX-startDestX;
	
	uint8_t *bmp=(uint8_t *)alloca(baseWidth*12);
	
	bitmapForGlyph(bmp, charCode);
	
	for(int destY=startDestY;destY<endDestY;destY++){
		
		int iSrcY=srcY>>16;
		int fSrcY=srcY&0xffff;
		
		// draw scanline.
		if(iSrcY==11)
			drawGlyphScaledSpan(destPixels,
								bmp+11*baseWidth,
								pixelsPerSpan,
								startSrcX,
								deltaSrcX,
								nativeColor,
								alpha,
								baseWidth-1);
		else
			drawGlyphScaledSpan(destPixels,
								bmp+iSrcY*baseWidth,
								bmp+(iSrcY+1)*baseWidth,
								pixelsPerSpan,
								startSrcX,
								deltaSrcX,
								fSrcY,
								nativeColor,
								alpha,
								baseWidth-1);
		
		
		// next line;
		srcY+=deltaSrcY;
		destPixels+=pitchPixels;
	}
	
}

#define doDraw()											\
do{														\
if(alphaValue<=0x8)									\
break;											\
alphaValue-=0x8;\
alphaValue<<=1; if(alphaValue>0x3f)alphaValue=0x3f;  \
register unsigned int tmp1=*destSpan;				\
register unsigned int tmp2, tmp3=0;					\
tmp2=(tmp1&0xF81F)*(0x40-alphaValue);				\
tmp2+=((unsigned int)color&0xF81F)*alphaValue;		\
tmp3=(tmp2>>6)&0xF81F;								\
tmp2=(tmp1&0x7E0)*(0x40-alphaValue);				\
tmp2+=((unsigned int)color&0x7E0)*alphaValue;		\
tmp3|=(tmp2>>6)&0x7E0;								\
*destSpan=tmp3;										\
}while(0)

void XTBFont::drawGlyphScaledSpan(uint16_t *destSpan,
								  const uint8_t *srcLine,
								  int pixels,
								  int startSrcX,
								  int deltaSrcX,
								  uint16_t color,
								  int alpha,
								  int lastSrcX) const{
	while(pixels--){
		register int alphaValue;
		{
			register int iSrcX=startSrcX>>16;
			
			// don't interpolate on the very right because
			// no more pixels in the right.
			if(iSrcX==lastSrcX)
				alphaValue=srcLine[iSrcX];
			else{
				alphaValue=srcLine[iSrcX];
				
				register int fSrcX=startSrcX&0xffff;
				register int rightPixelAlpha=srcLine[++iSrcX];
				rightPixelAlpha-=alphaValue;
				alphaValue+=(rightPixelAlpha*fSrcX)>>16;
			}
	
		}
	
		// apply global alpha, and shift (6bit).
		alphaValue=(alphaValue*alpha)>>10;
		
		doDraw();
		
		
		
		
		startSrcX+=deltaSrcX;
		destSpan++;
	}
}

void XTBFont::drawGlyphScaledSpan(uint16_t *destSpan,
								  const uint8_t *srcLine1,
								  const uint8_t *srcLine2,
								  int pixels,
								  int startSrcX,
								  int deltaSrcX,
								  int fSrcY,
								  uint16_t color,
								  int alpha,
								  int lastSrcX) const{
	while(pixels--){
		register int alphaValue;
		{
			register int iSrcX=startSrcX>>16;
			register int alphaValueBottom;
			
			// don't interpolate horizonally on the very right
			// because no more pixels in the right.
			
			if(iSrcX==lastSrcX){
				alphaValue=srcLine1[iSrcX];
				alphaValueBottom=srcLine2[iSrcX];
			}else{
				alphaValue=srcLine1[iSrcX];
				alphaValueBottom=srcLine2[iSrcX];
				
				register int fSrcX=startSrcX&0xffff;
				register int rightPixelAlpha=srcLine1[++iSrcX];
				rightPixelAlpha-=alphaValue;
				alphaValue+=(rightPixelAlpha*fSrcX)>>16;
				
				rightPixelAlpha=srcLine2[iSrcX];
				rightPixelAlpha-=alphaValueBottom;
				alphaValueBottom+=(rightPixelAlpha*fSrcX)>>16;
			}
			
			{
				alphaValueBottom-=alphaValue;
				alphaValue+=(alphaValueBottom*fSrcY)>>16;
			}
			
		}
		
		// apply global alpha, and shift (6bit).
		alphaValue=(alphaValue*alpha)>>10;
		
		doDraw();
		
		startSrcX+=deltaSrcX;
		destSpan++;
	}
}

twSize XTBFont::measure(const std::wstring& str, int size) const{
	wchar_t br=0;
	twPoint pt(0,size);
	int mw=0;
	
	if(size==actualLineHeight)
		// fast measure.
		return measure(str);
	
	for(int n=0;n<str.length();n++){
		
		if(br==0){
			if(str[n]==13 || str[n]==10)
				br=str[n];
		}
		
		if(str[n]==br){
			pt.x=0;
			pt.y+=size;
			continue;
		}else if(str[n]==13 || str[n]==10){
			continue;
		}	
		
		uint16_t charCode=(uint16_t)str[n];
		int baseWidth=widthMap[charCode];
		int scaledWidth=widthForScaledGlyph(baseWidth, size);
		pt.x+=scaledWidth;
		if(charCode<128){
			if(scaledWidth>1 && scaledWidth>=baseWidth)
				pt.x--;
		}
		if(pt.x>mw)
			mw=pt.x;
		
	}
	
	return twSize(mw, pt.y);
}


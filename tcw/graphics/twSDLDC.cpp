/*
 *  twSDLDC.cpp
 *  
 *
 *  Created by Nexhawks on 10/01/14.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#define USE_SDL_IMAGE		0
#ifdef DEBUG
#define DEBUG_TRANSFER		0
#define SIMUL_TRANSFER		0
#else
#define DEBUG_TRANSFER		0
#define SIMUL_TRANSFER		0
#endif

#if USE_SDL_IMAGE
#include <SDL/SDL_image.h>
#endif
#include <tcw/twException.h>
#include <tcw/twSDLDC.h>
#include <tcw/twStrConv.h>

#include <assert.h>

#include <algorithm>

twSDLDC::twSDLDC(SDL_Surface *surface, const twPoint& orig, bool localize)
:m_surf(surface), m_origin(orig), twDC(){
	if(m_surf==NULL)
		throw twException(L"twSDL: null surface");
	m_local=localize;
	resetClipRect();
}

twSDLDC::twSDLDC(const twSize& sz, const twPoint& orig){
	SDL_Surface *screenSuface=SDL_GetVideoSurface();
	const SDL_PixelFormat& fmt = *(screenSuface->format);
	m_surf=SDL_CreateRGBSurface(SDL_SWSURFACE,sz.w,sz.h,
								fmt.BitsPerPixel,
								fmt.Rmask,fmt.Gmask,fmt.Bmask,fmt.Amask );
	m_local=true;
	m_origin=orig;
	resetClipRect();
}

twSDLDC::~twSDLDC(){
	if(m_local)
		SDL_FreeSurface(m_surf);
}

twSDLDC *twSDLDC::loadFrom(const std::wstring& str){
	char bf[256];
	twW2M(bf, str.c_str());
	
	SDL_Surface *surf;
#if USE_SDL_IMAGE
	surf=IMG_Load(bf);
#else
	surf=SDL_LoadBMP(bf);
#endif
	
	if(surf==NULL){
		throw twException(L"loadFrom: IMG_Load/SDL_LoadBMP failed");
	}
	
	twSDLDC *dc;
	dc=new twSDLDC(surf, twPoint(0, 0));
	dc->m_local=true;
	
	return dc;
	
}

twRect twSDLDC::defaultClipRectRaw(){
	twRect rt;
	rt.x=0; rt.y=0;
	rt.w=m_surf->w; rt.h=m_surf->h;
	return rt;
}

void twSDLDC::resetClipRect(){
	twRect rt=defaultClipRectRaw();
	setClipRect(rt-m_origin);
}

void twSDLDC::setClipRect(const twRect& rt){
	
	twRect rawRect;
	twRect surfRect;
	
	rawRect=rt+m_origin;
	surfRect=defaultClipRectRaw();
	
	rawRect=rawRect&surfRect;
	
	SDL_Rect rt2;
	rt2.x=rawRect.x;
	rt2.y=rawRect.y;
	rt2.w=rawRect.w; rt2.h=rawRect.h;
	SDL_SetClipRect(m_surf, &rt2);
	m_clipRect.x=rt2.x-m_origin.x;
	m_clipRect.y=rt2.y-m_origin.y;
	m_clipRect.w=rt2.w;
	m_clipRect.h=rt2.h;
	//fillRect(rand(), rt);
	//printf("(%d, %d)-Step(%d, %d)\n", rt.x, rt.y, rt.w, rt.h);
}
#if DEBUG_TRANSFER
static uint64_t total=0;

static void DTCountTransfer(uint64_t bytes){
	total+=bytes;
	fprintf(stderr, "transfers total: %llu KiB\n", total>>10);
}
#endif
#if SIMUL_TRANSFER
static volatile Uint32 STLastTime=0;
static volatile uint64_t STLastBytes=0;
static volatile uint64_t STTotalBytes=0;
static SDL_semaphore *STSemaphore=NULL;

static Uint32 transferTimeForBytes(uint64_t bytes){
	return bytes/8000; 
}
static void STSimulateTransfer(uint64_t bytes){
	if(!STSemaphore)
		STSemaphore=SDL_CreateSemaphore(1);
	SDL_SemWait(STSemaphore);
	STTotalBytes+=bytes;
	Uint32 finalTransferTime=transferTimeForBytes(STTotalBytes-STLastBytes);
	finalTransferTime+=STLastTime;
	
	if(SDL_GetTicks()<finalTransferTime){
		Uint32 delayTime;
		delayTime=finalTransferTime-SDL_GetTicks();
		if(delayTime<10){
			SDL_SemPost(STSemaphore);
			return;
		}
		fprintf(stderr, "STSimulateTransfer: give delay %dms\n", (int)delayTime);
		SDL_Delay(delayTime);
		STLastTime=SDL_GetTicks();
		STLastBytes=STTotalBytes;
	}else{
		STLastTime=SDL_GetTicks();
		STLastBytes=STTotalBytes;
	}
	SDL_SemPost(STSemaphore);
}
#endif

void DebugTransfer(uint64_t bytes);

void DebugTransfer(uint64_t bytes){
#if DEBUG_TRANSFER
	DTCountTransfer(bytes);
#endif
#if SIMUL_TRANSFER
	STSimulateTransfer(bytes);
#endif
}

void twSDLDC::fillRect(twColor col, const twRect& rt){
	Uint32 color;
	twRect rawRect;
	twRect surfRect;
	
	rawRect=rt+m_origin;
	surfRect=defaultClipRectRaw();
	
	rawRect=rawRect&surfRect;
	
	SDL_Rect rt2;
	
	rt2.x=rawRect.x;
	rt2.y=rawRect.y;
	rt2.w=rawRect.w; rt2.h=rawRect.h;
	color=SDL_MapRGB(m_surf->format,
					 twGetR(col), twGetG(col), twGetB(col));
	
	SDL_FillRect(m_surf, &rt2, color);

	DebugTransfer(rt2.w*rt2.h*2);

	//total+=rt2.w*rt2.h;
	//fprintf(stderr, "fillRect: (%d,%d)-step(%d,%d) area: %llu\n", rt2.x, rt2.y, rt2.w, rt2.h, total);
}

void twSDLDC::dimRect(const twRect& rt){
	twRect rawRect;
	twRect surfRect;
	
	assert(m_surf->format->BitsPerPixel==16);
	
	if(!(rt&&m_clipRect))
		return;
	
	rawRect=(rt&m_clipRect)+m_origin;
	surfRect=defaultClipRectRaw();
	
	rawRect=rawRect&surfRect;
	
	SDL_LockSurface(m_surf);
	
	uint8_t *bytes=(uint8_t *)m_surf->pixels;
	bytes+=m_surf->pitch*rawRect.y;
	bytes+=2*rawRect.x;
	
	for(int leftLines=rawRect.h;leftLines;leftLines--){
		
		if((rawRect.x&1)==0 && (rawRect.w&1)==0){
			// even (aligned)
			register uint32_t *doublePixels=(uint32_t *)bytes;
			register uint32_t *lastDoublePixels=doublePixels+(rawRect.w>>1);
			
			for(;doublePixels!=lastDoublePixels;doublePixels++){
				register uint32_t value=*doublePixels;
				value=value&0xF7DEF7DE; // 11110111110111101111011111011110
				value>>=1;
				*doublePixels=value;
			}
		}else{
			// odd (maybe unaligned)
			register uint16_t *pixels=(uint16_t *)bytes;
			register uint16_t *lastPixels=pixels+rawRect.w;
			
			for(;pixels!=lastPixels;pixels++){
				register uint16_t value=*pixels;
				value=value&0xF7DE; // 1111011111011110
				value>>=1;
				*pixels=value;
			}
		}
		
		bytes+=m_surf->pitch;
	}
	
	SDL_UnlockSurface(m_surf);
	
	DebugTransfer(rawRect.w*rawRect.h*4);
}

void twSDLDC::bitBlt(const twDC *dc, const twPoint& dest, const twRect& src){
	const twSDLDC *sdldc=dynamic_cast<const twSDLDC *>(dc);
	if(sdldc==NULL){
		throw twException(L"bitBlt: dc must be able to be casted to (const twSDLDC *)");
	}
	SDL_Rect rt1, rt2;
	rt1.x=src.x+sdldc->m_origin.x;
	rt1.y=src.y+sdldc->m_origin.y;
	rt1.w=src.w; rt1.h=src.h;
	rt2.x=dest.x+m_origin.x;
	rt2.y=dest.y+m_origin.y;
	rt2.w=src.w; rt2.h=src.h;
	SDL_BlitSurface(sdldc->m_surf, &rt1, m_surf, &rt2);

	DebugTransfer(rt2.w*rt2.h*4);

}


void twSDLDC::stretchBlt(const twDC *dc, const twRect& dest, const twRect& src){
	const twSDLDC *sdldc=dynamic_cast<const twSDLDC *>(dc);
	if(sdldc==NULL){
		throw twException(L"stretchBlt: dc must be able to be casted to (const twSDLDC *)");
	}
	
	if(m_surf->format->BitsPerPixel!=16){
		throw twException(L"stretchBlt: this surface must be 16-bit. try optimizing it.");
	}
	
	if(sdldc->m_surf->format->BitsPerPixel!=16){
		throw twException(L"stretchBlt: given surface must be 16-bit. try optimizing it.");
	}
	
	SDL_Surface *srcSurf=sdldc->m_surf;
	
	twRect dest1;
	twRect src1;
	twRect destClip=m_clipRect+m_origin;
	dest1=dest+m_origin;
	src1=src+sdldc->m_origin;
	
	// poorly, inversed destination rectangle isn't supported.
	if(dest1.w<=0 || dest1.h<=0){
		return;
	}
	
	// is clipped, cull it out.
	if(dest1.x>=destClip.x+destClip.w)
		return;
	if(dest1.y>=destClip.y+destClip.h)
		return;
	if(dest1.x+dest1.w<=destClip.x)
		return;
	if(dest1.y+dest1.h<=destClip.y)
		return;
	if(std::max(src1.x, src1.x+src1.w)<=0)
		return;
	if(std::max(src1.y, src1.y+src1.h)<=0)
		return;
	if(std::min(src1.x, src1.x+src1.w)>=(int)srcSurf->w)
		return;
	if(std::min(src1.y, src1.y+src1.h)>=(int)srcSurf->h)
		return;
	
	int destStartY, destEndY;
	int srcY, srcDeltaY;
	
	int destStartX, destEndX;
	int srcX, srcDeltaX;
	
	// calculate vertical offsets.
	destStartY=dest1.y;
	destEndY=dest1.y+dest1.h;
	
	srcY=(src1.y)<<16;
	srcDeltaY=(src1.h<<16)/dest1.h;
	
	// calculate horizonal offsets.
	destStartX=dest1.x;
	destEndX=dest1.x+dest1.w;
	
	srcX=(src1.x)<<16;
	srcDeltaX=(src1.w<<16)/dest1.w;
	
	// clip, in dest coord.
	if(destStartX<0){
		srcX-=srcDeltaX*destStartX;
		destStartX=0;
	}
	if(destStartY<0){
		srcY-=srcDeltaY*destStartY;
		destStartY=0;
	}
	if(destEndX>destClip.x+destClip.w)
		destEndX=destClip.x+destClip.w;
	if(destEndY>destClip.y+destClip.h)
		destEndY=destClip.y+destClip.h;
	
	// clip, in src coord.
	if(srcX<0){
		int shift=(srcDeltaX-1-srcX)/srcDeltaX;
		srcX+=srcDeltaX*shift;
		destStartX+=srcX;
	}
	if(srcY<0){
		int shift=(srcDeltaY-1-srcY)/srcDeltaY;
		srcY+=srcDeltaY*shift;
		destStartY+=srcY;
	}
	if(srcX>(srcSurf->w-1)<<16){
		int shift=srcX-((srcSurf->w-1)<<16);
		shift=(shift+srcDeltaX-1)/srcDeltaX;
		srcX-=shift*srcDeltaX;
		destStartX+=shift;
	}
	if(srcY>(srcSurf->h-1)<<16){
		int shift=srcY-((srcSurf->h-1)<<16);
		shift=(shift+srcDeltaY-1)/srcDeltaY;
		srcX-=shift*srcDeltaY;
		destStartY+=shift;
	}
	if(srcX+srcDeltaX*(destEndX-destStartX-1)<0){
		int shift;
		shift=(srcDeltaX-1-srcX+srcDeltaX*(destEndX-destStartX-1))
		/srcDeltaX;
		destEndX-=shift;
	}
	if(srcY+srcDeltaY*(destEndY-destStartY-1)<0){
		int shift;
		shift=(srcDeltaY-1-srcY+srcDeltaY*(destEndY-destStartY-1))
		/srcDeltaY;
		destEndY-=shift;
	}
	if(srcX+srcDeltaX*(destEndX-destStartX-1)>(srcSurf->w-1)<<16){
		int shift;
		shift=srcX+srcDeltaX*(destEndX-destStartX-1)-((srcSurf->w-1)<<16);
		shift=(shift+srcDeltaX-1)/srcDeltaX;
		destEndX-=shift;
	}
	if(srcY+srcDeltaY*(destEndY-destStartY-1)>(srcSurf->h-1)<<16){
		int shift;
		shift=srcY+srcDeltaY*(destEndY-destStartY-1)-((srcSurf->h-1)<<16);
		shift=(shift+srcDeltaY-1)/srcDeltaY;
		destEndY-=shift;
	}
	
	if(destStartX>=destEndX)
		return;
	
	if(destStartY>=destEndY)
		return;
	
	assert(srcX>=0);
	assert(srcX<=((srcSurf->w-1)<<16));
	assert(srcX+srcDeltaX*(destEndX-destStartX-1)>=0);
	assert(srcX+srcDeltaX*(destEndX-destStartX-1)<=((srcSurf->w-1)<<16));
	assert(srcY>=0);
	assert(srcY<=((srcSurf->h-1)<<16));
	assert(srcY+srcDeltaY*(destEndY-destStartY-1)>=0);
	assert(srcY+srcDeltaY*(destEndY-destStartY-1)<=((srcSurf->h-1)<<16));
	assert(destStartX>=0);
	assert(destEndX<=m_surf->w);
	assert(destStartY>=0);
	assert(destEndY<=m_surf->h);
	
	uint16_t *pixels=(uint16_t *)(m_surf->pixels);
	uint16_t *srcPixels;
	pixels+=destStartX;
	pixels+=destStartY*(m_surf->pitch>>1);
	srcPixels=(uint16_t *)(srcSurf->pixels);
	for(;destStartY<destEndY;destStartY++,srcY+=srcDeltaY){
		assert(srcY>=0);
		assert(srcY<=((srcSurf->h-1)<<16));
		
		uint16_t *curSrcPixels1=srcPixels;
		curSrcPixels1+=(srcY>>16)*(srcSurf->w);
		
		uint16_t *curSrcPixels2=curSrcPixels1;
		curSrcPixels2+=srcSurf->w;
		
		if(srcY==((srcSurf->h-1)<<16)){
			stretchLine(pixels, destEndX-destStartX, 
						curSrcPixels1, srcX, srcDeltaX);
		}else{
			stretchLine(pixels, destEndX-destStartX, 
						curSrcPixels1, curSrcPixels2,
						srcX, srcDeltaX, srcY&0xffff);
		}
		
		pixels+=m_surf->pitch>>1;
	}
}

static unsigned int Lerp16(unsigned int color1,
					   unsigned int color2,
					   unsigned int per){ // per: 6-bit
	unsigned int colora, colorb;
	unsigned int invPer=0x40-per;
	{
		register unsigned int color1a, color2a;
		color1a=color1&0xf81f;
		color2a=color2&0xf81f;
		color1a*=invPer;
		color2a*=per;
		colora=((color1a+color2a)>>6)&0xf81f;
	}
	{
		register unsigned int color1b, color2b;
		color1b=color1&0x7e0;
		color2b=color2&0x7e0;
		color1b*=invPer;
		color2b*=per;
		colorb=((color1b+color2b)>>6)&0x7e0;
	}
	return colora|colorb;
}

void twSDLDC::stretchLine(uint16_t *dest, unsigned int pixels,
						  uint16_t *src, 
						  int srcX, int srcDeltaX)const{
	assert(srcX>=0);
	assert(srcX+srcDeltaX*(pixels-1)>=0);
	
	while(pixels--){
		// sample.
		unsigned int iSrcX=srcX>>16;
		unsigned int fSrcX=srcX&0xffff;
		fSrcX>>=10;
		
		unsigned int color;
		if(fSrcX){
			color=Lerp16(src[iSrcX], src[iSrcX+1], 
						 fSrcX);
		}else{
			color=src[iSrcX];
		}
		
		// write.
		*(dest++)=color;
		
		srcX+=srcDeltaX;
	}
}

void twSDLDC::stretchLine(uint16_t *dest, unsigned int pixels,
						  uint16_t *src1, uint16_t *src2, 
						  int srcX, int srcDeltaX,
						  int srcYFrac)const{
	assert(srcX>=0);
	assert(srcX+srcDeltaX*(pixels-1)>=0);
	
	srcYFrac>>=10;
	while(pixels--){
		// sample.
		unsigned int iSrcX=srcX>>16;
		unsigned int fSrcX=srcX&0xffff;
		fSrcX>>=10;
		
		unsigned int color;
		if(fSrcX){
			unsigned int color1, color2;
			color1=Lerp16(src1[iSrcX], src2[iSrcX], srcYFrac);
			color2=Lerp16(src1[iSrcX+1], src2[iSrcX+1], srcYFrac);
			
			color=Lerp16(color1, color2, 
						 fSrcX);
		}else{
			color=Lerp16(src1[iSrcX], src2[iSrcX], srcYFrac);
		}
		
		// write.
		*(dest++)=color;
		
		srcX+=srcDeltaX;
	}
}



twRect twSDLDC::getBounds(){
	return twRect(0, 0, m_surf->w, m_surf->h)-m_origin;
}

SDL_Surface *twSDLDC::getSurface(){
	return m_surf;
}

void twSDLDC::optimize(){
	if(SDL_GetVideoSurface()==m_surf){
		throw twException(L"Screen DC cannot be optimized.");
	}
	SDL_Surface *newSurf=SDL_DisplayFormat(m_surf);
	SDL_FreeSurface(m_surf);
	m_surf=newSurf;
}


/*
 *  twExposureFont.cpp
 *  
 *
 *  Created by Nexhawks on 10/01/16.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <tcw/twExposureFont.h>
#include <tcw/twStrConv.h>
#include <tcw/twException.h>

using namespace std;

void twExposureFont::setColor(Uint32 col) const{
	col&=0xffffff;
	if(col==ocolor)
		return;
	
	SDL_Surface *surf=fnt->getSurface();
	SDL_LockSurface(surf);
	
	Uint32 *ptr=(Uint32 *)surf->pixels;
	int n, cnt;
	cnt=surf->w*surf->h;
	for(n=0;n<cnt;n++){
		ptr[n]&=0xff000000;
		ptr[n]|=col;
	}
	
	SDL_UnlockSurface(surf);
	ocolor=col;
}

twExposureFont::twExposureFont(const std::wstring& name){
	char buf[256];
	char fn[256];
	std::wstring str2;
	twW2M(buf, name.c_str());
	
	str2=name+L".png";
	fnt=twSDLDC::loadFrom(str2);
	
	FILE *f;
	int ch;
	twRect rt;
	memset(glyphs, 0, sizeof(glyphs));
	sprintf(fn, "%s.cfg", buf);
	f=fopen(fn, "r");
	if(f==NULL){
		delete fnt;
		throw twException(L"twExposureFont: couldn't load config");
	}
	size=0;
	while(fscanf(f, "%d%d%d%d%d", &ch, &rt.x, &rt.y, &rt.w, &rt.h)!=EOF){
		glyphs[ch]=rt;
		if(rt.h>size)
			size=rt.h;
	}
	ocolor=0xffffff;
}

twExposureFont::~twExposureFont(){
	delete fnt;
}

void twExposureFont::render(twDC *dc, twColor col, const twPoint& pos, const wstring& str) const{
	twPoint pt=pos;
	int n;
	wchar_t br=0;
	
	setColor(col);
	
	for(n=0;n<str.length();n++){
		twRect glyph=glyphs[int(str[n])&0xffff];
		if(!br){
			if(str[n]==13 || str[n]==10)
				br=str[n];
		}
		if(str[n]==br){
			pt.x=pos.x;
			pt.y+=size;
		}
		
		dc->bitBlt(fnt, pt, glyph);
		
		pt.x+=glyph.w;
		
	}
}

twSize twExposureFont::measure(const std::wstring& str) const{
	twPoint pt(0, size);
	int n;
	int mw=0;
	wchar_t br=0;
	for(n=0;n<str.length();n++){
		twRect glyph=glyphs[int(str[n])&0xffff];
		if(!br){
			if(str[n]==13 || str[n]==10)
				br=str[n];
		}
		if(str[n]==br){
			pt.x=0;
			pt.y+=size;
		}
		
		pt.x+=glyph.w;
		if(pt.x>mw)
			mw=pt.x;
		
	}
	return twSize(mw, pt.y);
}


/*
 *  twDC.cpp
 *  
 *
 *  Created by Nexhawks on 10/01/13.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#include <algorithm>
#include <tcw/twDC.h>
#include <tcw/twException.h>

using namespace std;

#define MIN(a, b) (((a)<(b))?(a):(b))
#define MAX(a, b) (((a)>(b))?(a):(b))

bool twDC::hasClip(){
	return m_clipRect.w && m_clipRect.h;
}

twDC::twDC(){

	resetClipRect();
}

twDC::~twDC(){
	
}

void twDC::resetClipRect(){
	setClipRect(twRect(-4096, -4096, 8192, 8192));
}
void twDC::setClipRect(const twRect& rt){
	m_clipRect=rt;
}
const twRect& twDC::getClipRect() const{
	return m_clipRect;
}
void twDC::addClipRect(const twRect& rt){
	setClipRect(m_clipRect&rt);
}
void twDC::clear(twColor col){
	if(!hasClip())
		return;
	fillRect(col, m_clipRect);
}
void twDC::drawLine(twColor col, const twPoint& p1, const twPoint& p2){
	if(!hasClip())
		return;
	if(p1.x==p2.x)
		fillRect(col, twRect(p1.x, MIN(p1.y, p2.y),
							 1, MAX(p1.y, p2.y)-MIN(p1.y, p2.y)+1));
	else if(p1.y==p2.y)
		fillRect(col, twRect(MIN(p1.x, p2.x), p1.y,
							 MAX(p1.x, p2.x)-MIN(p1.x, p2.x)+1, 1));
	else{
		int x1, x2, dx;
		int y1, y2, y3;
		int cx, ox;
		x1=p1.x<<18; x2=p2.x<<18;
		y1=p1.y; y2=p2.y;
		if(y1>y2){
			swap(x1, x2);
			swap(y1, y2);
		}
		dx=(x2-x1)/(y2-y1+1);
		if(y1<m_clipRect.y){
			x1+=dx*(m_clipRect.y-y1);
			y1=m_clipRect.y;
		}
		y3=y2;
		if(y2>m_clipRect.y+m_clipRect.h){
			y2=m_clipRect.y+m_clipRect.h;
		}
		ox=x1>>18;
		while(y1<=y2){
			x1+=dx;
			cx=x1>>18;
			if(y1==y3)
				cx=(x2>>18);
			
			int xx1, xx2;
			xx1=MIN(ox, cx); xx2=MAX(ox, cx);
			if(xx1==xx2)
				fillRect(col, twRect(xx1, y1, 1, 1));
			else
				fillRect(col, twRect(xx1, y1, xx2-xx1, 1));
			
			ox=cx;
			
			y1++;
		}
	}
}

void twDC::drawPoint(twColor col, const twPoint& pt){
	fillRect(col, twRect(pt.x, pt.y, 1, 1));
}

void twDC::drawRect(twColor col, const twRect& rt){
	if(!hasClip())
		return;
	fillRect(col, twRect(rt.x, rt.y, rt.w+1, 1));
	fillRect(col, twRect(rt.x, rt.y+rt.h, rt.w+1, 1));
	fillRect(col, twRect(rt.x, rt.y, 1, rt.h+1));
	fillRect(col, twRect(rt.x+rt.w, rt.y, 1, rt.h+1));
}

static inline unsigned int sqrt_long(unsigned int a){
	// http://www.finetune.co.jp/~lyuka/technote/fract/sqrt_hypot.html
	register int x;
	register int t;
	register int s;
	unsigned int scale;
	
	x = a;
	if (x > 1) {
		scale = 0;
		if (x < 0x8000) {
			x <<= 16;
			scale = 8;
			a = x;
		}
		x >>= 8;
		s = 8;
		for (t = 0x400000L; x < t; t >>= 2)
			s--;
		t = 88;
		t <<= s;
		x *= 22;
		s += 5;
		x >>= s;                      /* -3.1e-2 < err < +2.9e-2 */
		s = a;
		t += x;
		x = s;
		s /= t;
		s += t;
		s >>= 1;                      /* -4.8e-4 < err <= 0      */
		t = x;
		x /= s;
		x += s;
		x >>= 1;                      /* -1.2e-7 < err <= 0      */
		s = x;
		s++;
		s *= x;
		if (t > s)                    /* adjust LSB              */
			x++;
		if (scale) {
			x += 127;
			x >>= 8;
		}
	}
	return x;
}

void twDC::drawEllipse(twColor col, const twRect& rt){
	if(!hasClip())
		return;
	int vYpos, vYdelta;
	int y1, y2;
	
	// initialize transform
	vYpos=-(1<<29);
	vYdelta=(1<<30)/(rt.h+1);
	
	// calulate Y bound
	y1=rt.y; y2=rt.y+rt.h+1;
	
	// clip by Y
	if(y1<m_clipRect.y){
		vYpos+=vYdelta*(m_clipRect.y-y1);
		y1=m_clipRect.y;
	}	
	if(y2>m_clipRect.y+m_clipRect.h){
		y2=m_clipRect.y+m_clipRect.h;
	}
	
	int y;
	int ox1, ox2;
	
	if(y1>rt.y){
		// calculate first step
		// use pythagoras' theory
		unsigned int temp;
		vYpos-=vYdelta;
		temp=(1<<28)-(vYpos>>15)*(vYpos>>15);
		temp=sqrt_long(temp);
		
		// transform
		//      2x+w             w
		// xx1=------ - temp * -----
		//       2               2
		int xx1, xx2;
		xx1=(rt.x*2+rt.w-((rt.w*temp)>>14))>>1;
		xx2=(rt.x*2+rt.w+((rt.w*temp)>>14))>>1;
		ox1=xx1; ox2=xx2;
		vYpos+=vYdelta;
	}
	
	vYpos+=vYdelta>>1;
	
	for(y=y1;y<y2;y++){
		
		// use pythagoras' theory
		unsigned int temp;
		temp=(1<<28)-(vYpos>>15)*(vYpos>>15);
		temp=sqrt_long(temp);
		
		// transform
		//      2x+w             w
		// xx1=------ - temp * -----
		//       2               2
		int xx1, xx2;
		xx1=(rt.x*2+rt.w-((rt.w*temp+8192)>>14))>>1;
		xx2=(rt.x*2+rt.w+((rt.w*temp+8192)>>14)+1)>>1;
		
		if(y>rt.y){
			if(ox1==xx1)
				fillRect(col, twRect(xx1, y, 1, 1));
			else
				fillRect(col, twRect(MIN(xx1, ox1), y, MAX(xx1, ox1)-MIN(xx1, ox1), 1));
			if(ox2==xx2)
				fillRect(col, twRect(xx2, y, 1, 1));
			else
				fillRect(col, twRect(MIN(xx2, ox2), y, MAX(xx2, ox2)-MIN(xx2, ox2), 1));
		}
		if(y==rt.y || y==rt.y+rt.h){
			// close the figure
			fillRect(col, twRect(xx1, y, MAX(1, xx2-xx1), 1));
		}
		ox1=xx1; ox2=xx2;
		
		vYpos+=vYdelta;
	}
	
}

void twDC::fillEllipse(twColor col, const twRect& rt){
	if(!hasClip())
		return;
	int vYpos, vYdelta;
	int y1, y2;
	
	// initialize transform
	vYpos=-(1<<29);
	vYdelta=(1<<30)/(rt.h+1);
	
	// calulate Y bound
	y1=rt.y; y2=rt.y+rt.h+1;
	
	// clip by Y
	if(y1<m_clipRect.y){
		vYpos+=vYdelta*(m_clipRect.y-y1);
		y1=m_clipRect.y;
	}	
	if(y2>m_clipRect.y+m_clipRect.h){
		y2=m_clipRect.y+m_clipRect.h;
	}
	
	int y;
	
	vYpos+=vYdelta>>1;
	
	for(y=y1;y<y2;y++){
		
		// use pythagoras' theory
		unsigned int temp;
		temp=(1<<28)-(vYpos>>15)*(vYpos>>15);
		temp=sqrt_long(temp);
		
		// transform
		//      2x+w             w
		// xx1=------ - temp * -----
		//       2               2
		int xx1, xx2;
		xx1=(rt.x*2+rt.w-((rt.w*temp)>>14))>>1;
		xx2=(rt.x*2+rt.w+((rt.w*temp)>>14)+1)>>1;
	
		fillRect(col, twRect(xx1, y, xx2-xx1+1, 1));
		
		vYpos+=vYdelta;
	}
	
}

void twDC::fillTriangle(twColor col, const twPoint& pt1, const twPoint& pt2, const twPoint& pt3){
	if(!hasClip())
		return;
	int y1, y2;
	twPoint p1, p2, p3;
	
	// calculate y bound
	y1=MIN(MIN(pt1.y, pt2.y), pt3.y);
	y2=MAX(MAX(pt1.y, pt2.y), pt3.y);
	if(y1<m_clipRect.y){
		y1=m_clipRect.y;
	}	
	if(y2>m_clipRect.y+m_clipRect.h){
		y2=m_clipRect.y+m_clipRect.h;
	}
	
	p1=pt1;
	if(pt2.y<p1.y){
		p2=p1;
		p1=pt2;
	}else{
		p2=pt2;
	}
	if(pt3.y<p1.y){
		p3=p2;
		p2=p1;
		p1=pt3;
	}else if(pt3.y<p2.y){
		p3=p2;
		p2=pt3;
	}else{
		p3=pt3;
	}
	
	int y;
	for(y=y1;y<y2;y++){
		int x1, x2, per;
		
		// calculate side
		
		if(y==p1.y)
			x1=p1.x;
		else if(y==p3.y)
			x1=p3.x;
		else{
			per=((y-p1.y)<<16)/(p3.y-p1.y);
			x1=p1.x+(((p3.x-p1.x)*per)>>16);
		}
		
		// calculate another side
		
		if(y==p1.y)
			x2=p1.x;
		else if(y<p2.y){
			per=((y-p1.y)<<16)/(p2.y-p1.y);
			x2=p1.x+(((p2.x-p1.x)*per)>>16);
		}else if(y==p2.y){
			x2=p2.x;
		}else if(y<p3.y){
			per=((y-p2.y)<<16)/(p3.y-p2.y);
			x2=p2.x+(((p3.x-p2.x)*per)>>16);
		}else{
			x2=p3.x;
		}
		
		// fill
		
		fillRect(col, twRect(MIN(x1, x2), y, MAX(x1, x2)-MIN(x1, x2), 1));
		
	}
	
}

void twDC::bitBlt(const twDC *, const twPoint&, const twRect&){
	throw twException(L"bitBlt: not implemented");
}

void twDC::stretchBlt(const twDC *, const twRect&, const twRect&){
	throw twException(L"stretchBlt: not implemented");
}


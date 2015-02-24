/*
 *  twTypes.h
 *  
 *
 *  Created by Nexhawks on 10/01/13.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#pragma once

#include <stdint.h>
#include <wchar.h>

struct twPoint{
	int x, y;
	twPoint(){}
	twPoint(int xx, int yy){
		x=xx; y=yy;
	}
	twPoint operator +(const twPoint& p2) const{
		return twPoint(x+p2.x, y+p2.y);
	}	
	twPoint operator -(const twPoint& p2) const{
		return twPoint(x-p2.x, y-p2.y);
	}	
	twPoint operator *(const twPoint& p2) const{
		return twPoint(x*p2.x, y*p2.y);
	}	
	twPoint operator /(const twPoint& p2) const{
		return twPoint(x/p2.x, y/p2.y);
	}	
	void operator +=(const twPoint& p){
		x+=p.x; y+=p.y;
	}
	void operator -=(const twPoint& p){
		x-=p.x; y-=p.y;
	}
	void operator *=(const twPoint& p){
		x*=p.x; y*=p.y;
	}
	void operator /=(const twPoint& p){
		x/=p.x; y/=p.y;
	}
	bool operator ==(const twPoint& p) const{
		return x==p.x && y==p.y;
	}
	bool operator !=(const twPoint& p) const{
		return x!=p.x || y!=p.y;
	}
	static twPoint min(const twPoint& p1, const twPoint& p2){
		return twPoint((p1.x<p2.x)?p1.x:p2.x,
					   (p1.y<p2.y)?p1.y:p2.y);
	}
	static twPoint max(const twPoint& p1, const twPoint& p2){
		return twPoint((p1.x>p2.x)?p1.x:p2.x,
					   (p1.y>p2.y)?p1.y:p2.y);
	}
	static int dot(const twPoint& p1, const twPoint& p2){
		return p1.x*p2.x+p1.y*p2.y;
	}
};

struct twSize{
	int w, h;
	twSize(){}
	twSize(int xx, int yy){
		w=xx; h=yy;
	}
	twSize(const twPoint& p){
		w=p.x; h=p.y;
	}
	twSize operator +(const twSize& p2) const{
		return twSize(w+p2.w, h+p2.h);
	}	
	twSize operator -(const twSize& p2) const{
		return twSize(w-p2.w, h-p2.h);
	}	
	twSize operator *(const twSize& p2) const{
		return twSize(w*p2.w, h*p2.h);
	}	
	twSize operator /(const twSize& p2) const{
		return twSize(w/p2.w, h/p2.h);
	}	
	void operator +=(const twSize& p){
		w+=p.w; h+=p.h;
	}
	void operator -=(const twSize& p){
		w-=p.w; h-=p.h;
	}
	void operator *=(const twSize& p){
		w*=p.w; h*=p.h;
	}
	void operator /=(const twSize& p){
		w/=p.w; h/=p.h;
	}
	bool operator ==(const twSize& p) const{
		return w==p.w && h==p.h;
	}
	bool operator !=(const twSize& p) const{
		return w!=p.w || h!=p.h;
	}
};

struct twRect{
	int x, y, w, h;
	twRect(){}
	twRect(const twPoint& p, const twSize &s){
		x=p.x; y=p.y; w=s.w; h=s.h;
	}
	twRect(int xx, int yy, int ww, int hh){
		x=xx; y=yy; w=ww; h=hh;
	}
	static twRect emptyRect(){return twRect(0, 0, 0, 0);}
	twPoint topLeft() const{
		return twPoint(x, y);
	}
	twPoint topRight() const {
		return twPoint(x+w, y);
	}
	twPoint bottomLeft() const{
		return twPoint(x, y+h);
	}
	twPoint bottomRight() const{
		return twPoint(x+w, y+h);
	}
	twRect operator +(const twRect& r) const{
		twPoint p1, p2;
		p1=twPoint::min(topLeft(), r.topLeft());
		p2=twPoint::max(bottomRight(), r.bottomRight());
		return twRect(p1, p2-p1);
	}
	twRect operator |(const twRect& r) const{
		twPoint p1, p2;
		p1=twPoint::min(topLeft(), r.topLeft());
		p2=twPoint::max(bottomRight(), r.bottomRight());
		return twRect(p1, p2-p1);
	}
	twRect operator &(const twRect& r) const{
		twPoint p1, p2;
		p1=twPoint::max(topLeft(), r.topLeft());
		p2=twPoint::min(bottomRight(), r.bottomRight());
		return twRect(p1, twPoint::max(p2-p1, twPoint(0, 0)));
	}
	bool operator &&(const twRect& r) const{
		return (x<r.x+r.w && y<r.y+r.h && x+w>r.x && y+h>r.y);
	}
	operator bool() const{
		return w>0 && h>0;
	}
	void operator +=(const twRect& r){
		twPoint p1, p2;
		p1=twPoint::min(topLeft(), r.topLeft());
		p2=twPoint::max(bottomRight(), r.bottomRight());
		x=p1.x; y=p1.y;
		w=p2.x-p1.x; h=p2.y-p1.y;
	}
	void operator |=(const twRect& r){
		twPoint p1, p2;
		p1=twPoint::min(topLeft(), r.topLeft());
		p2=twPoint::max(bottomRight(), r.bottomRight());
		x=p1.x; y=p1.y;
		w=p2.x-p1.x; h=p2.y-p1.y;
	}
	void operator &=(const twRect& r){
		twPoint p1, p2;
		p1=twPoint::max(topLeft(), r.topLeft());
		p2=twPoint::min(bottomRight(), r.bottomRight());
		x=p1.x; y=p1.y;
		w=p2.x-p1.x; h=p2.y-p1.y;
		if(w<0)
			w=0;
		if(h<0)
			h=0;
	}
	twRect operator +(const twPoint& p) const{
		return twRect(x+p.x, y+p.y, w, h);
	}
	twRect operator -(const twPoint& p) const{
		return twRect(x-p.x, y-p.y, w, h);
	}
	twRect operator +(const twSize& p) const{
		return twRect(x, y, w+p.w, h+p.h);
	}
	twRect operator -(const twSize& p) const{
		return twRect(x, y, w-p.w, h-p.h);
	}
	bool operator &&(const twPoint& p) const{
		return p.x>=x && p.y>=y && p.x<x+w && p.y<y+h;
	}
	
	void operator +=(const twPoint& p){
		x+=p.x; y+=p.y;
	}
	void operator -=(const twPoint& p){
		x-=p.x; y-=p.y;
	}
	void operator +=(const twSize& p){
		w+=p.w; h+=p.h;
	}
	void operator -=(const twSize& p){
		w-=p.w; h-=p.h;
	}
	
	twPoint loc() const{
		return twPoint(x, y);
	}
	twSize size() const{
		return twSize(w, h);
	}
	
	twRect inflate(int amount) const{
		twRect rt=*this;
		rt.x-=amount; rt.y-=amount;
		rt.w+=amount*2;
		rt.h+=amount*2;
		return rt;
	}
	
};

typedef uint32_t twColor;

static inline twColor twRGB(uint8_t r, uint8_t g, uint8_t b){
	return ((uint8_t)(r)|((uint8_t)(g) << 8)|((uint8_t)(b) << 16));
}

static inline uint8_t twGetR(twColor col){
	return (col&0x0000ff)>>0;
}
static inline uint8_t twGetG(twColor col){
	return (col&0x00ff00)>>8;
}
static inline uint8_t twGetB(twColor col){
	return (col&0xff0000)>>16;
}

#ifndef MAX
#define MAX(a,b)			(((a)>(b))?(a):(b))
#endif
#ifndef MIN
#define MIN(a,b)			(((a)<(b))?(a):(b))
#endif
#ifndef ABS
#define ABS(a)				(((a)<0)?(-(a)):(a))
#endif


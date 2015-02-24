/*
 *  TWTypes.h
 *  XTBook
 *
 *  Created by Nexhawks on 2/23/11.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */
/*
 * This file is part of XTBook.
 *
 * XTBook is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * XTBook is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with XTBook.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#define TW_USE_BITFIELD		1
#if TW_USE_BITFIELD
#define TW_BITFIELD(s) :s
#endif

struct TWPoint{
	int x, y;
	TWPoint(){}
	TWPoint(int xx, int yy){
		x=xx; y=yy;
	}
	TWPoint operator +(const TWPoint& p2) const{
		return TWPoint(x+p2.x, y+p2.y);
	}	
	TWPoint operator -(const TWPoint& p2) const{
		return TWPoint(x-p2.x, y-p2.y);
	}	
	TWPoint operator *(const TWPoint& p2) const{
		return TWPoint(x*p2.x, y*p2.y);
	}	
	TWPoint operator /(const TWPoint& p2) const{
		return TWPoint(x/p2.x, y/p2.y);
	}	
	void operator +=(const TWPoint& p){
		x+=p.x; y+=p.y;
	}
	void operator -=(const TWPoint& p){
		x-=p.x; y-=p.y;
	}
	void operator *=(const TWPoint& p){
		x*=p.x; y*=p.y;
	}
	void operator /=(const TWPoint& p){
		x/=p.x; y/=p.y;
	}
	bool operator ==(const TWPoint& p) const{
		return x==p.x && y==p.y;
	}
	bool operator !=(const TWPoint& p) const{
		return x!=p.x || y!=p.y;
	}
	static TWPoint min(const TWPoint& p1, const TWPoint& p2){
		return TWPoint((p1.x<p2.x)?p1.x:p2.x,
					   (p1.y<p2.y)?p1.y:p2.y);
	}
	static TWPoint max(const TWPoint& p1, const TWPoint& p2){
		return TWPoint((p1.x>p2.x)?p1.x:p2.x,
					   (p1.y>p2.y)?p1.y:p2.y);
	}
	static int dot(const TWPoint& p1, const TWPoint& p2){
		return p1.x*p2.x+p1.y*p2.y;
	}
};

struct TWSize{
	int w, h;
	TWSize(){}
	TWSize(int xx, int yy){
		w=xx; h=yy;
	}
	TWSize(const TWPoint& p){
		w=p.x; h=p.y;
	}
	TWSize operator +(const TWSize& p2) const{
		return TWSize(w+p2.w, h+p2.h);
	}	
	TWSize operator -(const TWSize& p2) const{
		return TWSize(w-p2.w, h-p2.h);
	}	
	TWSize operator *(const TWSize& p2) const{
		return TWSize(w*p2.w, h*p2.h);
	}	
	TWSize operator /(const TWSize& p2) const{
		return TWSize(w/p2.w, h/p2.h);
	}	
	void operator +=(const TWSize& p){
		w+=p.w; h+=p.h;
	}
	void operator -=(const TWSize& p){
		w-=p.w; h-=p.h;
	}
	void operator *=(const TWSize& p){
		w*=p.w; h*=p.h;
	}
	void operator /=(const TWSize& p){
		w/=p.w; h/=p.h;
	}
	bool operator ==(const TWSize& p) const{
		return w==p.w && h==p.h;
	}
	bool operator !=(const TWSize& p) const{
		return w!=p.w || h!=p.h;
	}
};

struct TWRect{
	int x, y, w, h;
	TWRect(){}
	TWRect(const TWPoint& p, const TWSize &s){
		x=p.x; y=p.y; w=s.w; h=s.h;
	}
	TWRect(int xx, int yy, int ww, int hh){
		x=xx; y=yy; w=ww; h=hh;
	}
	TWPoint topLeft() const{
		return TWPoint(x, y);
	}
	TWPoint topRight() const {
		return TWPoint(x+w, y);
	}
	TWPoint bottomLeft() const{
		return TWPoint(x, y+h);
	}
	TWPoint bottomRight() const{
		return TWPoint(x+w, y+h);
	}
	TWRect operator +(const TWRect& r) const{
		TWPoint p1, p2;
		p1=TWPoint::min(topLeft(), r.topLeft());
		p2=TWPoint::max(bottomRight(), r.bottomRight());
		return TWRect(p1, p2-p1);
	}
	TWRect operator |(const TWRect& r) const{
		TWPoint p1, p2;
		p1=TWPoint::min(topLeft(), r.topLeft());
		p2=TWPoint::max(bottomRight(), r.bottomRight());
		return TWRect(p1, p2-p1);
	}
	TWRect operator &(const TWRect& r) const{
		TWPoint p1, p2;
		p1=TWPoint::max(topLeft(), r.topLeft());
		p2=TWPoint::min(bottomRight(), r.bottomRight());
		return TWRect(p1, TWPoint::max(p2-p1, TWPoint(0, 0)));
	}
	bool operator &&(const TWRect& r) const{
		return (x<r.x+r.w && y<r.y+r.h && x+w>r.x && y+h>r.y);
	}
	void operator +=(const TWRect& r){
		TWPoint p1, p2;
		p1=TWPoint::min(topLeft(), r.topLeft());
		p2=TWPoint::max(bottomRight(), r.bottomRight());
		x=p1.x; y=p1.y;
		w=p2.x-p1.x; h=p2.y-p1.y;
	}
	void operator |=(const TWRect& r){
		TWPoint p1, p2;
		p1=TWPoint::min(topLeft(), r.topLeft());
		p2=TWPoint::max(bottomRight(), r.bottomRight());
		x=p1.x; y=p1.y;
		w=p2.x-p1.x; h=p2.y-p1.y;
	}
	void operator &=(const TWRect& r){
		TWPoint p1, p2;
		p1=TWPoint::max(topLeft(), r.topLeft());
		p2=TWPoint::min(bottomRight(), r.bottomRight());
		x=p1.x; y=p1.y;
		w=p2.x-p1.x; h=p2.y-p1.y;
		if(w<0)
			w=0;
		if(h<0)
			h=0;
	}
	TWRect operator +(const TWPoint& p) const{
		return TWRect(x+p.x, y+p.y, w, h);
	}
	TWRect operator -(const TWPoint& p) const{
		return TWRect(x-p.x, y-p.y, w, h);
	}
	TWRect operator +(const TWSize& p) const{
		return TWRect(x, y, w+p.w, h+p.h);
	}
	TWRect operator -(const TWSize& p) const{
		return TWRect(x, y, w-p.w, h-p.h);
	}
	bool operator &&(const TWPoint& p) const{
		return p.x>=x && p.y>=y && p.x<x+w && p.y<y+h;
	}
	
	void operator +=(const TWPoint& p){
		x+=p.x; y+=p.y;
	}
	void operator -=(const TWPoint& p){
		x-=p.x; y-=p.y;
	}
	void operator +=(const TWSize& p){
		w+=p.w; h+=p.h;
	}
	void operator -=(const TWSize& p){
		w-=p.w; h-=p.h;
	}
	
	TWPoint loc() const{
		return TWPoint(x, y);
	}
	TWSize size() const{
		return TWSize(w, h);
	}
	
	
	TWRect sanitize() const{
		TWRect rt=*this;
		if(rt.w<0){
			rt.w=-rt.w;
			rt.x-=rt.w;
		}
		if(rt.h<0){
			rt.h=-rt.h;
			rt.y-=rt.h;
		}
		return rt;
	}
	
	TWRect inflate(int amount) const{
		TWRect rt=*this;
		rt.x-=amount; rt.y-=amount;
		rt.w+=amount*2;
		rt.h+=amount*2;
		return rt;
	}
	

};





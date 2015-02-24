//
//  TWRenderManagerTcw.cpp
//  XTBook
//
//  Created by Nexhawks on 3/13/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//
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

#include "TWRenderManagerTcw.h"
#include <tcw/twFont.h>
#include "TWRenderDCTcw.h"
#include "TWViewerImageManager.h"
#include <tcw/twDC.h>
#include "xpms/ImageNotLoaded.xpm"
#include <tcw/twSDLDC.h>
#include <tcw/twStrConv.h>
#include <exception>
#include "XTBFont.h"

static twDC *m_imageNotLoadedImage=NULL;
twDC *TWRenderManagerTcw::getImageNotLoadedImage(){
	if(!m_imageNotLoadedImage){
		m_imageNotLoadedImage=twSDLDC::loadFromXPM(ImageNotLoaded);
		static_cast<twSDLDC *>(m_imageNotLoadedImage)->optimize();
	}
	return m_imageNotLoadedImage;
}

TWRenderManagerTcw::TWRenderManagerTcw(const twFont *font,
									   TWViewerImageManager *mng){
    m_font=font;
	m_imageManager=mng;
	m_size=11;
	m_alpha=256;
	if(dynamic_cast<const XTBFont *>(m_font))
		m_isXTBFont=true;
	else
		m_isXTBFont=false;
}

TWRenderManagerTcw::~TWRenderManagerTcw(){

}

static twColor twColorFromStyleColor(const TWHTMLStyleColor& styleColor){
    return twRGB(styleColor.red, styleColor.green, styleColor.blue);
}


void TWRenderManagerTcw::setStyle(const TWHTMLStyleProxy &style){
	m_color=twColorFromStyleColor(style.color());
	m_weight=style.fontWeight();
	m_size=style.fontSize().toPixels(style.inheritedFontSize(), style);
	m_alpha=style.color().alpha;
	if(m_alpha==255)
		m_alpha=256;
	if(m_size<9)
		m_size=9;
	m_decoration=style.textDecoration();
}

TWSize TWRenderManagerTcw::sizeForString(std::wstring const &str){
	twSize size;
	if(m_isXTBFont)
		size=static_cast<const XTBFont *>(m_font)->measure(str, m_size);
	else
		size=m_font->measure(str);
	return TWSize(size.w, size.h);
}

void TWRenderManagerTcw::drawString(std::wstring const &str, const TWPoint &pt, TWRenderDC *renderDC){
    TWRenderDCTcw *renderDCTcw=static_cast<TWRenderDCTcw *>(renderDC);
	twDC *dc=renderDCTcw->internalDC();
	
	if(m_isXTBFont){
		static_cast<const XTBFont *>(m_font)->render(dc, m_color, twPoint(pt.x, pt.y), str,
					   m_size, m_alpha);
		if(m_weight>300){
			static_cast<const XTBFont *>(m_font)->render(dc, m_color, twPoint(pt.x, pt.y), str,
														 m_size, m_alpha);
		}
		if(m_weight>150){
			static_cast<const XTBFont *>(m_font)->render(dc, m_color, twPoint(pt.x+1, pt.y), str,
														 m_size, m_alpha);
		}
	}else{
		m_font->render(dc, m_color, twPoint(pt.x, pt.y), str);
		if(m_weight>300){
			m_font->render(dc, m_color, twPoint(pt.x, pt.y), str);
		}
		if(m_weight>150){
			m_font->render(dc, m_color, twPoint(pt.x+1, pt.y), str);
		}
	}
	
	if(m_decoration){
		TWSize sz=sizeForString(str);
		
		if(m_decoration&TWHTMLStyleTextDecorationUnderline){
			dc->fillRect(m_color, twRect(pt.x, pt.y+sz.h-1, sz.w, 1));
		}
		if(m_decoration&TWHTMLStyleTextDecorationOverline){
			dc->fillRect(m_color, twRect(pt.x, pt.y, sz.w, 1));
		}
		
		if(m_decoration&TWHTMLStyleTextDecorationLineThrough){
			dc->fillRect(m_color, twRect(pt.x, pt.y+sz.h/2, sz.w, 1));
		}
		
	}
	
}

TWSize TWRenderManagerTcw::sizeForImage(const std::wstring &url){
	XTBImageArticle *article=m_imageManager->imageArticleForUrl(url);
	if(!article){
		return TWSize(16,16);
	}
	twSize s(16,16);
	try{
		s=article->dimension();
	}catch(const std::wstring& err){
		XTBLog("Exception while getting dimension of \"%s\": %s", twW2M(url).c_str(),
			   twW2M(err).c_str());
	}catch(const std::string& err){
		XTBLog("Exception while getting dimension of \"%s\": %s", twW2M(url).c_str(),
			   (err).c_str());
	}catch(const std::exception& err){
		XTBLog("Exception while getting dimension of \"%s\": %s", twW2M(url).c_str(),
			   (err.what()));
	}catch(...){
		
	}
	
	return TWSize(s.w,s.h);
}

void TWRenderManagerTcw::drawImage(const std::wstring &url, 
								   const TWRect &pt, TWRenderDC *dc){
	// TODO: support scaling.
	XTBImageArticle *article=m_imageManager->imageArticleForUrl(url);
	twDC *imageDC=getImageNotLoadedImage();
	try{
		if(!article){
			imageDC=getImageNotLoadedImage();
		}else{
			imageDC=article->image()->dc();
		}
	}catch(const std::wstring& err){
		XTBLog("Exception while getting image of \"%s\": %s", twW2M(url).c_str(),
			   twW2M(err).c_str());
	}catch(const std::string& err){
		XTBLog("Exception while getting image of \"%s\": %s", twW2M(url).c_str(),
			   (err).c_str());
	}catch(const std::exception& err){
		XTBLog("Exception while getting image of \"%s\": %s", twW2M(url).c_str(),
			   (err.what()));
	}catch(...){
		
	}
	
	twSize s=imageDC->getClipRect().size();
	
	TWRenderDCTcw *renderDCTcw=static_cast<TWRenderDCTcw *>(dc);
	twDC *idc=renderDCTcw->internalDC();
	
	twRect outRect;
	
	if(imageDC==getImageNotLoadedImage()){
		outRect.w=std::min(pt.w, s.w);
		outRect.h=std::min(pt.h, s.h);
		
		outRect.x=(pt.w-outRect.w)/2+pt.x;
		outRect.y=(pt.h-outRect.h)/2+pt.y;
		
		idc->drawRect(0xb0b0b0, twRect(pt.x, pt.y, pt.w-1, pt.h-1));
	}else{
		outRect=twRect(pt.x, pt.y, pt.w, pt.h);
	}
	
	idc->stretchBlt(imageDC, outRect,
					twRect(0,0,s.w,s.h));
	//idc->bitBlt(imageDC, twPoint(pt.x, pt.y), twRect(0,0,s.w,s.h));
	
}

TWSize TWRenderManagerTcw::sizeForListMarker(){
	return TWSize(3,3);
}

void TWRenderManagerTcw::drawListMarker(const TWRect &rt, TWRenderDC * renderDC){
	TWRenderDCTcw *renderDCTcw=static_cast<TWRenderDCTcw *>(renderDC);
	twDC *dc=renderDCTcw->internalDC();
	
	twColor col=twRGB(0,82,140);
	dc->fillRect(col, twRect(rt.x, rt.y, rt.w, rt.h));
}



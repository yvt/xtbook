/*
 *  twApp.cpp
 *  
 *
 *  Created by Nexhawks on 10/01/13.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#include <tcw/twApp.h>
#include <tcw/twStrConv.h>
#include <tcw/twSDLDC.h>
#include <tcw/twWnd.h>

#ifdef WIN32
#include <windows.h>
#endif


using namespace std;

twApp *tw_app=NULL;

void twInitWnd();
void twInitSkin();
void twInitEvent();

void twExitWnd();
void twExitSkin();
void twExitEvent();

twApp::twApp(){
	setTitle(L"tw_app");
	
	const SDL_VideoInfo *info=SDL_GetVideoInfo();
	m_autoSize=twSize(info->current_w, info->current_h);
}

twApp::~twApp(){
	
}

void twApp::setTitle(const wstring &title){
	char buf[256];
	twW2M(buf, title.c_str());
	SDL_WM_SetCaption(buf, buf);
	m_appTitle=title;
}

const std::wstring& twApp::getTitle() const{
	return m_appTitle;
}

void twApp::setScrSize(const twSize& sz){
	int flags=SDL_SWSURFACE;
	if(m_resizable)
		flags|=SDL_RESIZABLE;
	if(m_fullScreen)
		flags|=SDL_FULLSCREEN;
	
	m_surf=SDL_SetVideoMode(sz.w, sz.h, 16, flags);
	m_scrSize=sz;
	if(twGetDesktop())
		twGetDesktop()->setRect(twRect(0, 0, sz.w, sz.h));
}

void twApp::setAutoScrSize(){
	int flags=SDL_SWSURFACE;
	if(m_resizable && !m_fullScreen)
		flags|=SDL_RESIZABLE;
	if(m_fullScreen)
		flags|=SDL_FULLSCREEN;
	
	m_surf=SDL_SetVideoMode(m_autoSize.w, m_autoSize.h, 16, flags);
	m_scrSize=twSize(m_surf->w, m_surf->h);
	if(twGetDesktop())
		twGetDesktop()->setRect(twRect(0, 0, m_scrSize.w, m_scrSize.h));
}

const twSize& twApp::getScrSize() const{
	return m_scrSize;
}

twDC *twApp::getDC(const twRect& rt){
	twSDLDC *dc;
	dc=new twSDLDC(m_surf, rt.loc());
	dc->setClipRect(rt-rt.loc());
	return dc;
}

void DebugTransfer(uint64_t bytes);

void twApp::update(const twRect&rt){
	/*SDL_Rect r;
	Uint32 col=SDL_MapRGB(m_surf->format, rand()%256, rand()%256, rand()%256);
	r.x=rt.x; r.y=rt.y; r.w=1; r.h=rt.h;
	SDL_FillRect(m_surf, &r, col);
	r.x=rt.x; r.y=rt.y; r.w=rt.w; r.h=1;
	SDL_FillRect(m_surf, &r, col);
	r.x=rt.x+rt.w-1; r.y=rt.y; r.w=1; r.h=rt.h;
	SDL_FillRect(m_surf, &r, col);
	r.x=rt.x; r.y=rt.y+rt.h-1; r.w=rt.w; r.h=1;
	SDL_FillRect(m_surf, &r, col);
	*/
	SDL_UpdateRect(m_surf, rt.x, rt.y, rt.w, rt.h);
	
	DebugTransfer(rt.w*rt.h*4);
}

twInitStruct::twInitStruct(){
	title=L"tw_app";
	scrSize=twSize(640, 480);
	resizable=true;
	fullScreen=false;
}

__attribute((weak))
twApp *twCreateApp(){
	return new twApp();
}

void twInit(const twInitStruct& str){
	if(tw_app)
		return;
	
#ifdef _WIN32_WCE
	CeSetThreadPriority(GetCurrentThread(),
						248);
#endif
	
	SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER);
	tw_app=twCreateApp();
	tw_app->setTitle(str.title);
	tw_app->m_resizable=str.resizable;
	tw_app->m_fullScreen=str.fullScreen;
	tw_app->setScrSize(str.scrSize);
	
	twInitWnd();
	twInitSkin();
	twInitEvent();
}

void twExit(){
	if(!tw_app)
		return;
	twExitWnd();
	twExitSkin();
	delete tw_app;
	// TODO: SDL_Exit();
	tw_app=NULL;
}

twDC *twLoadBitmap(const std::wstring& fn){
	return twSDLDC::loadFrom(fn);
}

twTicks twGetTicks(){
	return SDL_GetTicks();
}


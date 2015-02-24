/*
 *  twApp.h
 *  
 *
 *  Created by Nexhawks on 10/01/13.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#pragma once

#include <SDL/SDL.h>
#include <tcw/twTypes.h>
#include <wchar.h>
#include <string>
#include <tcw/twDC.h>

struct twInitStruct{
	std::wstring title;
	twSize scrSize;
	bool resizable;
	bool fullScreen;
	
	twInitStruct();
};

class twApp{
	friend void twInit(const twInitStruct& str);
	friend void twExit();
protected:
	SDL_Surface *m_surf;
	std::wstring m_appTitle;
	twSize m_scrSize;
	twSize m_autoSize;
	bool m_resizable;
	bool m_fullScreen;
	
	
	
public:
	
	twApp();
	~twApp();
	void setTitle(const std::wstring&);
	const std::wstring& getTitle() const;
	void setScrSize(const twSize&);
	void setAutoSize(const twSize&s){m_autoSize=s;}
	void setAutoScrSize();
	void setFullScreen(bool v){m_fullScreen=v;}
	bool isFullScreen() const{return m_fullScreen;}
	const twSize& getScrSize() const;
	twDC *getDC(const twRect& target);
	void update(const twRect&);
};

typedef Uint32 twTicks;

void twInit(const twInitStruct& str);
void twExit();
twTicks twGetTicks(); 

/** override this method in order to create your own twApp. */
twApp *twCreateApp();

twDC *twLoadBitmap(const std::wstring&);

extern twApp *tw_app;

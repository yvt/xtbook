/*
 *  twWindow.cpp
 *  
 *
 *  Created by Nexhawks on 10/01/13.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#include <vector>
#include <tcw/twWnd.h>
#include <tcw/twApp.h>
#include <tcw/twException.h>
#include <tcw/twVGAFont.h>
#include <tcw/twBaseSkin.h>
#include <tcw/twEvent.h>
#include <typeinfo>
#include <algorithm>
#include <assert.h>
//#define DEBUG_VALIDATE_TIME


using namespace std;

static int tw_maxWnds=1024;
static twWnd ** tw_wnds;
static bool tw_dirty;
static twRect tw_dirtyRect;
static twWnd *tw_desktop;
twWnd *tw_capture; // non-static because referenced by twEvent
bool tw_capClient; // non-static because referenced by twEvent
const twFont *tw_defFont;
twWnd *tw_over;

void twInitWnd();
void twExitWnd();

void twInitWnd(){
	int n;
	tw_wnds=new twWnd *[tw_maxWnds];
	for(n=0;n<tw_maxWnds;n++)
		tw_wnds[n]=NULL;
	tw_dirty=true;
	tw_dirtyRect=twRect(twPoint(0, 0), tw_app->getScrSize());
	tw_defFont=new twVGAFont();
	tw_capture=NULL;

}

void twExitWnd(){
	delete[] tw_wnds;
}

twWndStyle::twWndStyle(){
	enable=true;
	visible=false;
	border=twBS_none;
	
}

twWndStatus::twWndStatus(){
	
}

void twWndStyle::validate() const{
	if(border!=twBS_none && border!=twBS_solid &&
	   border!=twBS_panel && border!=twBS_dialog &&
	   border!=twBS_overlapped)
		throw twException(L"validate: invalid border");
}

static int twAllocWindow(){
	int n;
	for(n=0;n<tw_maxWnds;n++)
		if(!tw_wnds[n])
			return n;
	throw twException(L"twAllocWindow: couldn't allocate window");
}

twWnd::twWnd(){
	// register this window
	m_index=twAllocWindow();
	m_id=0;
	m_parent=NULL;
	m_title=L"";
	m_rect=twRect(0, 0, 0, 0);
	m_font=tw_defFont;
	m_drag=twWD_none;
	m_modalParent=NULL;
		m_def=NULL;
}

twWnd::~twWnd(){
	setParent(NULL);
	setModalParent(NULL);
	if(tw_over==this)
		tw_over=NULL;
}

void twWnd::setParent(twWnd *newParent){
	if(newParent==m_parent)
		return;
	if(m_parent){
		if(getStatus().focused && m_parent->m_children.size()>=2){
			list<twWnd *>::iterator it=m_parent->m_children.begin();
			it++;
			(*it)->focus();
		}
		
		m_parent->m_children.erase(find(m_parent->m_children.begin(), m_parent->m_children.end(),
								   this));
		
	}
	if(newParent)
		newParent->m_children.push_back(this);
	m_parent=newParent;
}

twWnd *twWnd::getParent() const{
	return m_parent;
}

void twWnd::setModalParent(twWnd *newParent){
	if(newParent==m_modalParent)
		return;
	if(m_modalParent)
		m_modalParent->m_modals.erase(find(m_modalParent->m_modals.begin(), m_modalParent->m_modals.end(),
										this));
	if(newParent)
		newParent->m_modals.push_back(this);
	m_modalParent=newParent;
}

twWnd *twWnd::getModalParent() const{
	return m_modalParent;
}

void twWnd::setTitle(const std::wstring& title){
	m_title=title;
	invalidate();
}
const wstring& twWnd::getTitle() const{
	return m_title;
}

void twWnd::setId(int i){
	m_id=i;
}
int twWnd::getId() const{
	return m_id;
}

void twWnd::setRect(const twRect& rt){
	invalidate();
	m_rect=rt;
	invalidate();
}
const twRect& twWnd::getRect() const{
	return m_rect;
}

void twWnd::setStyle(const twWndStyle& style){
	style.validate();
	this->m_style=style;
	invalidate();
}
const twWndStyle& twWnd::getStyle() const{
	return m_style;
}

void twWnd::setDefault(twWnd *w){
	m_def=w;
	invalidate();
}
twWnd *twWnd::getDefault() const{
	return m_def;
}

twWndStatus twWnd::getStatus() const{
	twWndStatus s;
	s.rect=m_rect;
	s.enable=m_style.enable;
	s.visible=m_style.visible;
	s.hover=(tw_over==this);
	if(m_parent){
		if(*(m_parent->m_children.begin())==this)
			s.focused=m_parent->getStatus().focused;
		else
			s.focused=false;
		s.defWnd=(m_parent->getDefault()==this);
	}else{
		s.focused=(this==tw_desktop);
		s.defWnd=false;
	}	
	return s;
}

void twWnd::setFont(const twFont *fnt){
	m_font=fnt;
	invalidate();
}

const twFont *twWnd::getFont() const{
	return m_font;
}

void twWnd::enumChildren(void (*func)(twWnd *)) const{
	if(!func)
		return;
	for(list<twWnd *>::const_iterator it=m_children.begin();it!=m_children.end();it++)
		(*func)(*it);
}

twRect twWnd::getWndRect() const{
	if(m_parent)
		return m_rect+m_parent->getWndRect().loc()+m_parent->getClientRect().loc();
	else
		return m_rect;
}

twRect twWnd::getVisibleWndRect() const{
	if(getStyle().visible==false && this!=tw_desktop){
		//fprintf(stderr, "%s is hidden\n", typeid(this).name());
		return twRect(-4096, -4096, 0, 0); // hidden
	}
	if(getParent()){
		twRect scrRect=getParent()->getVisibleWndRect();
		/*fprintf(stderr, "%s: (%d, %d)-(%d, %d)\n", typeid(this).name(),
				getWndRect().x, getWndRect().y, getWndRect().w, getWndRect().h);
		fprintf(stderr, "    %s: (%d, %d)-(%d, %d)\n", typeid(getParent()).name(),
				scrRect.x,scrRect.y, scrRect.w, scrRect.h);*/
		
		if(getWndRect() && scrRect){
			// visible
			return getWndRect() & scrRect;
		}else{
			//invisible 
			return twRect(-4096, -4096, 0, 0);
		}
	}else{
		twRect scrRect(twPoint(0, 0), tw_app->getScrSize());
		if(getWndRect() && scrRect){
			// visible
			return getWndRect() & scrRect;
		}else{
			//invisible 
			return twRect(-4096, -4096, 0, 0);
		}
	}
}

twRect twWnd::getClientRect() const{
	twRect rt=m_rect-m_rect.loc();
	if(m_style.border==twBS_none){
		return rt;
	}else if(m_style.border==twBS_solid){
		return tw_curSkin->getSolidBorderCliRect(rt);
	}else if(m_style.border==twBS_panel){
		return tw_curSkin->getPanelBorderCliRect(rt);
	}else if(m_style.border==twBS_dialog){
		return tw_curSkin->getDialogBorderCliRect(rt);
	}else if(m_style.border==twBS_overlapped){
		return tw_curSkin->getOverlappedBorderCliRect(rt);
	}
	return rt;
}

void twWnd::invalidate() const{
	twInvalidateRect(getVisibleWndRect());
}

void twWnd::invalidateClientRect() const{
	twRect rt=getClientRect();
	invalidateClientRect(twRect(0, 0, rt.w, rt.h));
}

void twWnd::invalidateClientRect(const twRect& rt) const{
	twRect r=(rt&(getClientRect()-getClientRect().loc()))+getWndRect().loc()+getClientRect().loc();
	twInvalidateRect(r&getVisibleWndRect());
}


void twWnd::focus(){
	if(!getStatus().enable)
		return;
	if(m_modals.size()){
		// modal dialog is shown
		assert((*m_modals.begin())!=this);
		(*m_modals.begin())->focus();
		return;
	}
	if(!m_parent)
		return;
	if(getStatus().focused)
		return;
	if(!getStatus().visible)
		return;
	
	m_parent->focus();
	twWnd *old=*(m_parent->m_children.begin());
	m_parent->m_children.erase(find(m_parent->m_children.begin(), m_parent->m_children.end(),
									this));
	m_parent->m_children.push_front(this);
	if(old)
		old->leave();//invalidate();
	enter();
}

void twWnd::blur(){
	if(!m_parent)
		return;
	
	if(m_parent->m_children.size()==1)
		return;
	
	if(*m_parent->m_children.begin()==this){
		m_parent->m_children.erase(find(m_parent->m_children.begin(), m_parent->m_children.end(),
										this));
		m_parent->m_children.push_back(this);
		
		leave();
		(*m_parent->m_children.begin())->enter();
	}else{
		m_parent->m_children.erase(find(m_parent->m_children.begin(), m_parent->m_children.end(),
										this));
		m_parent->m_children.push_back(this);
	}
}

twSize twWnd::getBorderSize() const{
	return getRect().size()-getClientRect().size();
}

void twWnd::paint(const twRect& rt){
	
	if(!(getWndRect() && rt))
		return; // out of clip rect
	
	if(rt.w==0 || rt.h==0)
		return;
	
	// render client region
	
	if((getClientRect()+getWndRect().loc()) && rt){
	
		twPaintStruct str;
		twDC *cdc=getClientDC(rt);
		twRect rt2=getClientRect();
		
		str.dc=cdc;
		str.bound=rt2.size();
		str.boundRect=twRect(twPoint(0, 0), rt2.size());
		str.paintRect=str.boundRect;
		str.paintRect=str.paintRect&(rt-(getClientRect().loc()+getWndRect().loc()));
		
		clientPaint(str);
		
		delete cdc;
		
		for(list<twWnd *>::reverse_iterator it=m_children.rbegin();it!=
			m_children.rend();it++){
			twWnd *w=*it;
			if(!w->getStyle().visible)
				continue;
			twRect crt;
			crt=w->getWndRect();//w->getRect()+getClientRect().loc()+getWndRect().loc();
			crt&=getClientRect()+getWndRect().loc();
			crt&=rt;
			w->paint(crt);
		}
		
	}
	
	
	
	// render border
	
	twDC *dc=getDC(rt);
	twWndStatus s=getStatus();
	twRect rr=m_rect-m_rect.loc();
	
	if(m_style.border==twBS_none){
	}else if(m_style.border==twBS_solid){
		tw_curSkin->drawSolidBorder(dc, rr, s, m_title);
	}else if(m_style.border==twBS_panel){
		tw_curSkin->drawPanelBorder(dc, rr, s, m_title);
	}else if(m_style.border==twBS_dialog){
		tw_curSkin->drawDialogBorder(dc, rr, s, m_title);
	}else if(m_style.border==twBS_overlapped){
		tw_curSkin->drawOverlappedBorder(dc, rr, s, m_title);
	}
	
	
	
	delete dc;
}

twWnd *twWnd::hitTest(const twPoint& pt){
	if(!m_style.visible)
		return NULL;
	twRect rt=m_rect-m_rect.loc();
	if(!(rt&&pt))
		return NULL;
	if(getClientRect()&&pt){
		for(list<twWnd *>::iterator it=m_children.begin();it!=
			m_children.end();it++){
			twWnd *w=*it;
			if(w->getRect() && pt-getClientRect().loc()){
				twPoint p2=pt-getClientRect().loc()-w->getRect().loc();
				if(w->hitTest(p2) && w->getStyle().visible){
					return w;
				}
			}
		}
		return clientHitTest(pt-getClientRect().loc())?this:NULL;
	}
	if(m_style.border==twBS_none){
		return NULL;
	}else if(m_style.border==twBS_solid){
		return tw_curSkin->hitTestSolidBorder(rt, pt)?this:NULL;
	}else if(m_style.border==twBS_panel){
		return tw_curSkin->hitTestPanelBorder(rt, pt)?this:NULL;
	}else if(m_style.border==twBS_dialog){
		return tw_curSkin->hitTestDialogBorder(rt, pt)?this:NULL;
	}else if(m_style.border==twBS_overlapped){
		return tw_curSkin->hitTestOverlappedBorder(rt, pt)?this:NULL;
	}
	return NULL;
}


const twWnd *twWnd::hitTest(const twPoint& pt) const{
	twRect rt=m_rect-m_rect.loc();
	if(!(rt&&pt))
		return NULL;
	if(getClientRect()&&pt){
		for(list<twWnd *>::const_iterator it=m_children.begin();it!=
			m_children.end();it++){
			twWnd *w=*it;
			if(w->getRect() && pt-getClientRect().loc()){
				twPoint p2=pt-getClientRect().loc()-w->getRect().loc();
				if(w->hitTest(p2) && w->getStyle().visible){
					return w;
				}
			}
		}
		return clientHitTest(pt-getClientRect().loc())?this:NULL;
	}
	if(m_style.border==twBS_none){
		return NULL;
	}else if(m_style.border==twBS_solid){
		return tw_curSkin->hitTestSolidBorder(rt, pt)?this:NULL;
	}else if(m_style.border==twBS_panel){
		return tw_curSkin->hitTestPanelBorder(rt, pt)?this:NULL;
	}else if(m_style.border==twBS_dialog){
		return tw_curSkin->hitTestDialogBorder(rt, pt)?this:NULL;
	}else if(m_style.border==twBS_overlapped){
		return tw_curSkin->hitTestOverlappedBorder(rt, pt)?this:NULL;
	}
	return NULL;
}

void twWnd::show(){
	twWndStyle style=getStyle();
	if(style.visible==false){
		if(getParent()){
			if(getParent()->isVisible()){
				willAppear();
			}
		}
	}
	style.visible=true;
	setStyle(style);
}

void twWnd::hide(){
	twRect rt=getVisibleWndRect();
	twWndStyle style=getStyle();
	bool doDisappear=false;
	if(isVisible()){
		doDisappear=true;
	}
	style.visible=false;
	setStyle(style);
	twInvalidateRect(rt);
	if(doDisappear){
		didDisappear();
	}
}

bool twWnd::isVisible() const{
	if(this==twGetDesktop())
		return true;
	else if(getParent()){
		return getStyle().visible && getParent()->isVisible();
	}else{
		return false;
	}
}

twDC *twWnd::getDC(){
	return tw_app->getDC(getWndRect());
}
twDC *twWnd::getClientDC(){
	return tw_app->getDC(getClientRect()+getWndRect().loc());
}
twDC *twWnd::getDC(const twRect& crt){
	twRect rt=getWndRect();
	twDC *dc=tw_app->getDC(rt);
	dc->addClipRect(crt-rt.loc());
	return dc;
}
twDC *twWnd::getClientDC(const twRect& crt){
	twRect rt=getClientRect()+getWndRect().loc();
	twDC *dc=tw_app->getDC(rt);
	dc->addClipRect(crt-rt.loc());
	return dc;
}

void twWnd::clientKeyEventHandled(){
	m_keyEventHandled=true;
}

void twWnd::onKeyDown(const wstring& str){
	bool useDef=true;
	
	m_keyEventHandled=false;
	clientKeyDown(str);
	if(m_keyEventHandled)
		return;
	if(m_children.size()){
		(*m_children.begin())->onKeyDown(str);
		if((*m_children.begin())==m_def)
			useDef=false;
	}else{
		if(str==L"Tab"){
			transferFocus();
		}
	}
	
	if(str==L"Return" && useDef && m_def)
		m_def->onKeyDown(str);
	
}
void twWnd::onKeyPress(wchar_t ch){
	m_keyEventHandled=false;
	clientKeyPress(ch);
	if(m_keyEventHandled)
		return;
	if(m_children.size()){
		(*m_children.begin())->onKeyPress(ch);
	}	
}
void twWnd::onKeyUp(const wstring& str){
	m_keyEventHandled=false;
	clientKeyUp(str);
	if(m_keyEventHandled)
		return;
	if(m_children.size()){
		(*m_children.begin())->onKeyUp(str);
	}	
}
void twWnd::onMouseDown(const twPoint& pt, twMouseButton button){
	focus();
	if(!getStatus().focused)
		return;
	if(((getClientRect()&&pt) && !tw_capture) || (tw_capture && tw_capClient)){
		for(list<twWnd *>::iterator it=m_children.begin();it!=
			m_children.end();it++){
			twWnd *w=*it;
			if(w->getRect() && pt-getClientRect().loc()){
				twPoint p2=pt-getClientRect().loc()-w->getRect().loc();
				if(w->hitTest(p2) && w->getStyle().visible){
					if(w->getStyle().enable){
						
						w->onMouseDown(p2, button);
					}
					return;
				}
			}
		}
		clientMouseDown(pt-getClientRect().loc(), button);
	}else{
		if(button==twMB_left){
			if(m_style.border==twBS_none ||
			   m_style.border==twBS_solid ||
			   m_style.border==twBS_panel){
				m_drag=twWD_invalid;
			}else if(m_style.border==twBS_dialog){
				if(pt.y<getClientRect().y)
					m_drag=twWD_move;
				else
					m_drag=twWD_invalid;
			}else if(m_style.border==twBS_overlapped){
				if(pt.y<4){
					m_drag=twWD_up;
				}else if(pt.x<4){
					m_drag=twWD_left;
				}else if(pt.x>=m_rect.w-4){
					m_drag=twWD_right;
				}else if(pt.y>=m_rect.h-4){
					m_drag=twWD_down;
				}else if(pt.y<getClientRect().y){
					m_drag=twWD_move;
				}else{
					m_drag=twWD_invalid;
				}
			}
		}else{
			m_drag=twWD_invalid;
		}
		m_dragPos=pt;
		twSetCapture(this, false);
	}
	/*if(m_children.size() && !tw_capture){
		(*m_children.begin())->onMouseDown(pt-(*m_children.begin())->getRect().loc(), button);
	}	*/
}
void twWnd::onMouseMove(const twPoint& pt){
	if(m_modals.size())
		return;
	if(((getClientRect()&&pt) && !tw_capture) || (tw_capture && tw_capClient)){
		if(!tw_capture){
			for(list<twWnd *>::iterator it=m_children.begin();it!=
				m_children.end();it++){
				twWnd *w=*it;
				if(w->getRect() && pt-getClientRect().loc()){
					twPoint p2=pt-getClientRect().loc()-w->getRect().loc();
					if(w->hitTest(p2) && w->getStyle().visible){
						if(w->getStyle().enable)
							w->onMouseMove(p2);
						return;
					}
				}
			}
		}
		
		clientMouseMove(pt-getClientRect().loc());
	}else if(m_drag!=twWD_none){
		
		int vl;
		twPoint sft(0, 0);
		if(m_drag==twWD_move){
			setRect(m_rect+(pt-m_dragPos));
			sft-=(pt-m_dragPos);
		}
		if(m_drag==twWD_up){
			vl=m_rect.h-(pt.y-m_dragPos.y);
			vl=max(vl, getMinSize().h);
			setRect(twRect(m_rect.x, m_rect.y+m_rect.h-vl,
						   m_rect.w, vl));
			sft.y-=(pt.y-m_dragPos.y);
		}
		if(m_drag==twWD_down){
			vl=m_rect.h+(pt.y-m_dragPos.y);
			vl=max(vl, getMinSize().h);
			setRect(twRect(m_rect.x, m_rect.y,
						   m_rect.w, vl));
		} 
		if(m_drag==twWD_left){
			vl=m_rect.w-(pt.x-m_dragPos.x);
			vl=max(vl, getMinSize().w);
			setRect(twRect(m_rect.x+m_rect.w-vl, m_rect.y,
						   vl, m_rect.h));
			sft.x-=(pt.x-m_dragPos.x);
		} 
		if(m_drag==twWD_right){
			vl=m_rect.w+(pt.x-m_dragPos.x);
			vl=max(vl, getMinSize().w);
			setRect(twRect(m_rect.x, m_rect.y,
						   vl, m_rect.h));
		}
		m_dragPos=pt+sft;
	}else{
		
	}
	/*if(m_children.size() && !tw_capture){
		(*m_children.begin())->onMouseMove(pt-(*m_children.begin())->getRect().loc());
	}*/
}
void twWnd::onMouseUp(const twPoint& pt, twMouseButton button){
	if(m_modals.size())
		return;
	if(((getClientRect()&&pt) && !tw_capture) || (tw_capture && tw_capClient)){
		clientMouseUp(pt-getClientRect().loc(), button);
		for(list<twWnd *>::iterator it=m_children.begin();it!=
			m_children.end();it++){
			twWnd *w=*it;
			if(w->getRect() && pt-getClientRect().loc()){
				twPoint p2=pt-getClientRect().loc()-w->getRect().loc();
				if(w->hitTest(p2) && w->getStyle().visible){
					if(w->getStyle().enable)
						w->onMouseUp(p2, button);
					return;
				}
			}
		}
	}else if(m_drag!=twWD_none){
		m_drag=twWD_none;
		twReleaseCapture();
	}
	/*if(m_children.size() && !tw_capture){
		(*m_children.begin())->onMouseUp(pt-(*m_children.begin())->getRect().loc(), button);
	}*/
}

void twWnd::sendCmdToParent() const{
	if(m_parent)
		m_parent->command(m_id);
}

twSize twWnd::getMinSize() const{
	return m_rect.size()-getClientRect().size();
}

void twWnd::clientKeyDown(const wstring& str){
}
void twWnd::clientKeyPress(wchar_t ch){
}
void twWnd::clientKeyUp(const wstring& str){
}
void twWnd::clientMouseDown(const twPoint& pt, twMouseButton button){
}
void twWnd::clientMouseMove(const twPoint& pt){
}
void twWnd::clientMouseUp(const twPoint& pt, twMouseButton button){
}
void twWnd::clientMouseEnter(const twPoint& pt){
}
void twWnd::clientMouseLeave(const twPoint& pt){
}
void twWnd::clientPaint(const twPaintStruct& str){
}
void twWnd::clientEnter(){
}
void twWnd::clientLeave(){	
}

bool twWnd::clientHitTest(const twPoint& pt) const{
	return false;
}

void twWnd::command(int){	
}

void twWnd::willAppear(){
	for(list<twWnd *>::iterator it=m_children.begin();it!=
		m_children.end();it++){
		twWnd *w=*it;
		if(w->getStyle().visible){
			w->willAppear();
		}
	}
}
void twWnd::didDisappear(){
	for(list<twWnd *>::iterator it=m_children.begin();it!=
		m_children.end();it++){
		twWnd *w=*it;
		if(w->getStyle().visible){
			w->didDisappear();
		}
	}
}

void twWnd::enter(){
	invalidate();
	clientEnter();
	if(m_children.size())
		(*m_children.begin())->enter();
}

void twWnd::leave(){
	invalidate();
	clientLeave();
	if(m_children.size())
		(*m_children.begin())->leave();
}

void twWnd::transferFocus(){
	twWnd *w=this;
	
	while(w=w->getParent(), w){
		if(w->m_children.size()>1)
			break;
	}
	if(w==twGetDesktop())
		return;
	if(w==NULL)
		return;
	(*(w->m_children.rbegin()))->focus();
}

twWnd *twWnd::getRootWindow() const{
	if(getParent())
		return getParent()->getRootWindow();
	else if(twGetDesktop()==this)
		return NULL;
	else
		return const_cast<twWnd *>(this);
}

void twInvalidateRect(const twRect& rt){
	//printf("invrect (%d, %d)-Step(%d, %d)\n", rt.x, rt.y, rt.w, rt.h);
	twRect scrRect(twPoint(0, 0), tw_app->getScrSize());
	if(!(scrRect && rt))
		return;
	/*if(rt.w<=0 || rt.h<=0)
		return;*/
	
	if(!tw_dirty){
		tw_dirty=true;
		tw_dirtyRect=rt&scrRect;
		
	}else{
		tw_dirtyRect+=rt&scrRect;
	}
	tw_event->postValidateMsg();
}

void twSetDesktop(twWnd *w){
	if(tw_desktop==w){
		w->invalidate();
		return;
	}
	twWnd *oldDesktop=tw_desktop;
	w->willAppear();
	tw_desktop=w;
	if(oldDesktop)
		oldDesktop->didDisappear();
	w->setRect(twRect(twPoint(0,0), tw_app->getScrSize()));
	w->invalidate();
}

twWnd *twGetDesktop(){
	return tw_desktop;
}

#ifdef DEBUG_VALIDATE_TIME
static volatile inline unsigned long long rdtsc() {
    unsigned long long ret;
    __asm__ volatile ("rdtsc" : "=A" (ret));
    return ret;
}
#endif

void twValidate(){
	if(!tw_dirty){
		//puts("undirty validate");
		return;
	}
	//puts("validate");
	
#ifdef DEBUG_VALIDATE_TIME
	unsigned long long ot=rdtsc();
#endif
	
	twDC *dc=tw_app->getDC(tw_dirtyRect);
	//dc->fillRect(0, tw_dirtyRect-tw_dirtyRect.loc());
	delete dc;
	if(tw_desktop){
		tw_desktop->paint(tw_dirtyRect);
	}else{
		//puts("warning: twValidate without desktop");
	}	

#ifdef DEBUG_VALIDATE_TIME
	unsigned long long tm=rdtsc();
	tm-=ot;
	fprintf(stderr, "validation time: %llu clock(s) area: (%d, %d)-step(%d, %d)\n", tm,
			tw_dirtyRect.x, tw_dirtyRect.y, tw_dirtyRect.w, tw_dirtyRect.h);
#endif
	
	tw_dirty=false;
	tw_app->update(tw_dirtyRect);
	
	
}

void twSetCapture(twWnd *wnd, bool client){
	tw_capture=wnd;
	tw_capClient=client;
}

void twReleaseCapture(){
	tw_capture=NULL;
}


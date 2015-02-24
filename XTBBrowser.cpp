/*
 *  XTBBrowser.cpp
 *  XTBook
 *
 *  Created by Nexhawks on 2/21/11.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#include "global.h"
#include <tcw/twSDLDC.h>
#include <tcw/twBaseSkin.h>
#include "XTBBrowser.h"
#include "XTBSearchBar.h"
#include "TWViewer.h"
#include "XTBTextField.h"
#include "XTBException.h"
#include "XTBSearchView.h"
#include "XTBStartPageView.h"
#include <memory>
#include <assert.h>
#include "XTBListView.h"
#include "XTBStartPageList.h"
#include "XTBContentsChooser.h"
#include "XTBTextField.h"

#include "TWiki/TWHTMLElement.h"
#include "TWiki/TWHTMLBuilder.h"
#include "TWiki/TWHTMLBodyElement.h"
#include "TWiki/TWReader.h"
#include "TWiki/TWParser.h"
#include "TWiki/TWParserConfig.h"
#include "TWiki/TWPreprocessor.h"
#include "TWiki/TWPreprocessorNode.h"
#include "TWiki/TWRenderObject.h"
#include <tcw/twStrConv.h>
#include <tcw/twMsgBox.h>
#include "TWiki/TWUtils.h"

#include "i18n.h"
#include "commondlg.h"

#include "XTBMenu.h"
#include "XTBButtonMenuItem.h"
#include "XTBSeparatorMenuItem.h"
#include "XTBLabelMenuItem.h"

#include "XTBDictionary.h"
#include "XTBArticle.h"

#include "xpms/SearchBarLeft.xpm"
#include "xpms/SearchBarRight.xpm"
#include "xpms/SearchBarForward.xpm"
#include "xpms/SearchBarBack.xpm"
#include "xpms/PageBar.xpm"

#include "TPList/TPLDictionary.h"
#include "XTBUserDefaults.h"
#include "TPList/TPLAutoReleasePtr.h"
#include "TPList/TPLNumber.h"

#include "XTBNotificationCenter.h"

#include "XTBFootnoteView.h"
#include "XTBHistory.h"
#include "XTBSoftKeyboard.h"

#pragma mark - Browser State

std::wstring XTBBrowserState::urlString() const{
	if(fragmentId.empty())
		return url.urlString();
	else
		return url.urlString()+L'#'+fragmentId;
}


TPLString *XTBBrowserState::tplUrlString() const{
	std::wstring s=urlString();
	return new TPLString(twW2M(s).c_str());
}

#pragma mark - Assets

twDC *XTBBrowser::m_searchBarBackImage=NULL;
twDC *XTBBrowser::m_searchBarForwardImage=NULL;
twDC *XTBBrowser::m_searchBarLeftImage=NULL;
twDC *XTBBrowser::m_searchBarRightImage=NULL;
twDC *XTBBrowser::m_pageBarImage=NULL;

twDC *XTBBrowser::getSearchBarBackImage(){
	if(!m_searchBarBackImage){
		m_searchBarBackImage=twSDLDC::loadFromXPM(SearchBarBack);
	}
	return m_searchBarBackImage;
}
twDC *XTBBrowser::getSearchBarForwardImage(){
	if(!m_searchBarForwardImage){
		m_searchBarForwardImage=twSDLDC::loadFromXPM(SearchBarForward);
	}
	return m_searchBarForwardImage;	
}
twDC *XTBBrowser::getSearchBarLeftImage(){
	if(!m_searchBarLeftImage){
		m_searchBarLeftImage=twSDLDC::loadFromXPM(SearchBarLeft);
	}
	return m_searchBarLeftImage;
}
twDC *XTBBrowser::getSearchBarRightImage(){
	if(!m_searchBarRightImage){
		m_searchBarRightImage=twSDLDC::loadFromXPM(SearchBarRight);
	}
	return m_searchBarRightImage;
}
twDC *XTBBrowser::getPageBarImage(){
	if(!m_pageBarImage){
		m_pageBarImage=twSDLDC::loadFromXPM(PageBar);
	}
	return m_pageBarImage;
}

#pragma mark - Lifetime

XTBBrowser::XTBBrowser(){
	m_delegate=NULL;
	
	m_searchBar=new XTBSearchBar();
	m_viewer=new TWViewer();
	m_searchView=new XTBSearchView();
	m_startPageView=new XTBStartPageView();
	m_softKeyboard=new XTBSoftKeyboard();
	
	m_softKeyboard->setParent(this);
	m_softKeyboard->setId(XTBBrowserSoftKeyboardId);
	
	m_searchBar->setParent(this);
	m_searchBar->getTextField()->setTitle(L"");
	m_searchBar->setId(XTBBrowserSearchBarId);
	m_searchBar->show();
	
	m_viewer->setParent(this);
	// m_viewer->show(); NOTE: m_startPageView is visible now.
	
	m_searchView->setParent(this);
	m_searchView->setId(XTBBrowserSearchViewId);
	// m_searchView->show(); NOTE: m_startPageView is ...
	
	m_startPageView->setParent(this);
	m_startPageView->setId(XTBBrowserStartPageViewId);
	m_startPageView->show();
	
	m_rootElement=NULL;
    
	m_viewer->setId(XTBBrowserViewerId);
	
	m_shownDocument=NULL;
	m_history.push_back(initialHistory());
	m_historyPos=m_history.begin();
	
	m_drag=XTBBrowserDragModeNone;
	m_hover=false;
}
XTBBrowser::~XTBBrowser(){
	if(m_shownDocument)
		delete m_shownDocument;
	delete m_searchBar;
	delete m_viewer;
	delete m_searchView;
	delete m_startPageView;

}

#pragma mark - Loading Article


XTBBrowserState XTBBrowser::initialHistory() const{
	XTBBrowserState state;
	state.page=0;
	state.contentLocation=twPoint(0, 0);
	return state;
}

XTBBrowserState XTBBrowser::stateForURLString(const std::wstring& urlString)const{
	XTBBrowserState state;
	std::wstring::size_type pos;
	std::wstring urlBody;
	state.contentLocation=twPoint(0, 0);
	state.page=0;
	
	if((pos=urlString.find(L'#'))!=std::wstring::npos){
		urlBody=urlString.substr(0, pos);
		state.fragmentId=urlString.substr(pos+1);
		
		if(urlBody.empty()){
			// on this page.
			urlBody=m_currentState.url.urlString();
		}
	}else{
		urlBody=urlString;
	}
	
	state.url=XTBURL(urlBody);

	
	return state;
}

struct XTBBrowserLoadPageResult{
	bool exceptionThrown;

	std::wstring msg;
	XTBBrowserLoadPageResult(){
		exceptionThrown=false;
	}
};

bool XTBBrowser::isStateNeedsReloading(const XTBBrowserState& state) const{
	const XTBBrowserState& oldState=m_currentState;
	if(oldState.url.type()!=state.url.type())
		return true;
	if(state.url.type()!=XTBURLTypeDocument){
		return oldState.url!=state.url;
	}else{
		if(oldState.url!=state.url)
			return true;
		
		
		XTBArticle *article=dynamic_cast<XTBArticle *>(m_shownDocument);
		
		// now, m_shownArticle must be the article for state.articleName
		// because oldState.articleName is m_shownArticle's name.
		// if not, something seems to be going wrong. (but it is not easy
		// to assert)
		if(article){
			if(isSinglePageMode()){
				return false;
			}
			int page1=(oldState.page!=XTBBrowserStatePageUnknown)?
			oldState.page:pageForFragmentId(oldState.fragmentId, article);
			int page2=(state.page!=XTBBrowserStatePageUnknown)?
			state.page:pageForFragmentId(state.fragmentId, article);
			return page1!=page2;
		}else{
			return false;
		}
	}
}

void XTBBrowser::setUrlType(XTBURLType urlType){
	if(urlType==XTBURLTypeDocument && !m_viewer->getStyle().visible)
		m_viewer->show();
	else if(urlType!=XTBURLTypeDocument && m_viewer->getStyle().visible)
		m_viewer->hide();
	
	if(urlType==XTBURLTypeSearch && !m_searchView->getStyle().visible)
		m_searchView->show();
	else if(urlType!=XTBURLTypeSearch && m_searchView->getStyle().visible)
		m_searchView->hide();
	
	if(urlType==XTBURLTypeHome && !m_startPageView->getStyle().visible)
		m_startPageView->show();
	else if(urlType!=XTBURLTypeHome && m_startPageView->getStyle().visible)
		m_startPageView->hide();
	
}

XTBSearchViewQuery XTBBrowser::queryForState(const XTBBrowserState& state) const{
	XTBSearchViewQuery query;
	query.key=state.url.articleName;
	return query;
}

bool XTBBrowser::displayURL(XTBBrowserState state){
	
	XTBBrowserLoadPageResult result;
	bool urlChanged=true;
	
	switch(state.url.type()){
		case XTBURLTypeSearch:
		case XTBURLTypeHome:
			m_viewer->setHTMLDocument(NULL);
			if(m_rootElement){
				delete m_rootElement;
				m_rootElement=NULL;
			}
			if(state.url.type()==XTBURLTypeSearch){
				XTBSearchViewQuery query=queryForState(state);
				if(query.key.empty())
					m_searchView->setQuery(query);
				else{
					m_searchView->setQueryDelayed(query);
					urlChanged=false;
				}
			}
			
			m_currentState=state;
			
			setUrlType(state.url.type());
			break;
		case XTBURLTypeDocument:
			//XTBLog("loading %s", twW2M(url).c_str());
			if(m_searchView->isVisible()){
				// erase search result
				//m_searchView->setQuery(XTBSearchViewQuery());
			}
			if(isStateNeedsReloading(state)){
				result.exceptionThrown=false;
				
				XTBInvokeWithProgressOverlay(new twTwoArgumentMemberFunctionInvocation
											 <XTBBrowser, XTBBrowserState *, 
											 XTBBrowserLoadPageResult *>(this, &XTBBrowser::loadPage,&state,
																		 &result));
				if(result.exceptionThrown){
					XTBLog("exception thrown while acecssing %s[page %d]: %s", twW2M(state.url.urlString()).c_str(), 
						   state.page, twW2M(result.msg).c_str());
					std::wstring msg=XTBLocalizedString(L"XTBBrowserFailedToLoad");
					if(!result.msg.empty()){
						msg+=L"\n\n";
						msg+=result.msg;
					}
					
					twWnd *parent=getRootWindow();
					twMsgBox(parent, msg, twMBB_ok, XTBLocalizedString(L"XTBError"), true);
					return false;
				}else{
					
					m_viewer->setHTMLDocument(static_cast<TWHTMLBodyElement *>(m_rootElement));
					
					if(!state.fragmentId.empty()){
						twRect rt=m_viewer->rectForId(state.fragmentId);
						m_viewer->setContentLocation(twPoint(0,rt.y));
					}
						
					setUrlType(state.url.type());
					
					
					break;
				}
			}else{
				if(!state.fragmentId.empty()){
					twRect rt=m_viewer->rectForId(state.fragmentId);
					m_viewer->setContentLocationAnimated(twPoint(0,rt.y));
				}
				m_currentState=state;
			}
			
			m_viewer->invalidate();
			
			break;
		case XTBURLTypeSystemCall:
			// shouldn't come here; ignore.
			return false;
		
	}
	
	if(urlChanged)
		sendCmdToParent();
	
	return true;
}

XTBDocument *XTBBrowser::documentForBrowserState(const XTBBrowserState& state) const{
	return state.url.document();
}

int XTBBrowser::pageForFragmentId(const std::wstring& fragmentId, const XTBArticle *article) const{
	if(fragmentId.empty())
		return 1;
	
	if(fragmentId[0]>=L'0' && fragmentId[0]<=L'9'){
		// direct specify
		int page=TWUtils::intValueFromString(fragmentId);
		int pageCount=article->pagesCount();
		
		// correct the page to fit in range
		if(page<1)
			page=1;
		if(page>pageCount)
			page=pageCount;
		
		return page;
	}
	
	return article->pageForFragmentId(fragmentId);
	
}

void XTBBrowser::loadPage(XTBBrowserState* state, XTBBrowserLoadPageResult *result){
	// initialize random seed for RandomSelection.
	srand(twGetTicks()); rand();rand();rand();
	try{
		XTBBrowserState nowState=m_currentState;
		
		XTBSetProgressText(XTBLocalizedString(L"XTBBrowserLoadingArticle"));
		
		if(nowState.url!=state->url){
			std::auto_ptr<XTBDocument> newArticle(documentForBrowserState(*state));
			
			if(!newArticle.get()){
				result->exceptionThrown=true;
				result->msg=XTBLocalizedString(L"XTBBrowserPageNotReturned");
				return;
			}
			
			if(m_shownDocument)
				delete m_shownDocument;
			
			m_shownDocument=newArticle.release();
		}
		
		XTBArticle *article;
		std::wstring html;
		if((article=dynamic_cast<XTBArticle *>(m_shownDocument))){
			
			if(isSinglePageMode()){
				html=article->htmlForPage(1);
				int pageCount=article->pagesCount();
				for(int i=2;i<=pageCount;i++)
					html+=article->htmlForPage(i);
			}else{
				if(state->page==XTBBrowserStatePageUnknown){
					int newPage=pageForFragmentId(state->fragmentId, article);
					state->page=newPage;
				}
				html=article->htmlForPage(state->page);
			}
			
		}else{
			html=L"<img src=\""+state->url.urlString()+L"\" />";
		}
		
		
		std::auto_ptr<TWHTMLBodyElement> rootElement(static_cast<TWHTMLBodyElement *>(TWHTMLElement::nodeWithName(L"body")));
		
		{
			TWHTMLBuilder builder(rootElement.get());
			
			{
				TWReader reader(html);
				XTBSetProgressText(XTBLocalizedString(L"XTBBrowserParsing"));
				builder.expand(&reader);
			}
		}
		
		m_currentState=*state;
		
		if(m_rootElement){
			delete m_rootElement;
		}
		m_rootElement=rootElement.release();
		
		
	
	}catch(const std::wstring& str){
		result->exceptionThrown=true;
		result->msg=str;
	}catch(const std::exception& ex){
		result->exceptionThrown=true;
		result->msg=twM2W(ex.what());
	}catch(const std::string& str){
		result->exceptionThrown=true;
		result->msg=twM2W(str);
	}catch(...){
		result->exceptionThrown=true;
		result->msg=L"";
	}
	
}

bool XTBBrowser::isSinglePageMode() const{
	if(XTBUserDefaults::standardUserDefaults()->numberForKey
	   (XTBUserDefaultsKeySinglePage)->boolValue()){
		return true;
	}
	return false;
}

#pragma mark - Navigation Controller (Low-level)

void XTBBrowser::updateHistory(){
	m_currentState=currentActualState();
	*m_historyPos=currentActualState();
}

XTBBrowserState XTBBrowser::currentActualState() const{
	XTBBrowserState state=m_currentState;
	state.contentLocation=m_viewer->getContentLocation();
	return state;
}

void XTBBrowser::advanceHistory(){
	// remove the following history items.
#if XTBBROWSER_HISTORY_VECTOR
	m_history.erase(m_historyPos+1, m_history.end());
	
	// create an new history.
	m_history.push_back(currentActualState());
	
	// move to the new history.
	m_historyPos=m_history.end()-1;
#else
	XTBBrowserStateList::iterator it, it2;
	it=m_historyPos; it++;
	while(it!=m_history.end()){
		it2=it; it2++;
		m_history.erase(it);
		it=it2;
	}
	
	// create an new history.
	m_history.push_back(currentActualState());
	
	// move to the new history.
	it=m_historyPos;
	
	it++; it2=it; it2++;
	assert(it2==m_history.end());
	m_historyPos=it;
	assert(it!=m_history.end());
#endif
}

bool XTBBrowser::isStateNeedsAdvancingHistory(const XTBBrowserState &state) const{
	const XTBBrowserState& oldState=m_currentState;
	
	if(oldState.url.type()!=state.url.type())
		return true;
	
	switch(oldState.url.type()){
		case XTBURLTypeHome:
		case XTBURLTypeSearch:
			return false;
		case XTBURLTypeDocument:
			if(oldState.url!=state.url)
				return true;
			if(oldState.fragmentId!=state.fragmentId)
				return true;
			return false;
		case XTBURLTypeSystemCall:
			assert(0);
			return false;
	}
	
	assert(0);
}

bool XTBBrowser::moveToHistoryPos(XTBBrowserStateList::iterator newHistoryPos){
	// if there's no movement, no-op.
	if(m_historyPos==newHistoryPos)
		return true;
	
	assert(newHistoryPos!=m_history.end());
	
	updateHistory();
	
	const XTBBrowserState& newState=*newHistoryPos;
	
	bool succeeded=displayURL(newState);
	
	if(succeeded){
		
		// move to the target history item.
		m_historyPos=newHistoryPos;
		
		// restore the scroll position;
		m_viewer->setContentLocation(newState.contentLocation);
		
		// update the search(address) bar.
		updateSearchBar();
		
	}else{
		
		// transition failed.
		// no need to move to the target history item because
		// the previous page will be preserved.
		
	}
	
	return succeeded;
}


void XTBBrowser::updateSearchBar(){
	m_searchBar->getTextField()->setTitle(currentActualState().urlString());
}

#pragma mark - Navigation Controller (High-level)

std::wstring XTBBrowser::displayTitle() const{
	const XTBBrowserState& state=currentActualState();
	switch(state.url.type()){
		case XTBURLTypeHome:
			return XTBLocalizedString(L"XTBBrowserTitleHome");
		case XTBURLTypeSearch:
			return XTBFormat(XTBLocalizedString(L"XTBBrowserTitleSearch"), state.url.urlString().c_str(), NULL);
		case XTBURLTypeDocument:
			XTBArticle *article;
			if((article=dynamic_cast<XTBArticle *>(m_shownDocument))!=NULL){
				std::wstring str=article->displayTitle();
				if(!str.empty())
					return str;
			}
			return state.url.urlString();
		case XTBURLTypeSystemCall:
			return L"XTBook";
	}
	assert(false);
	return L"";
}

void XTBBrowser::navigate(std::wstring const &url){
	XTBBrowserState state=stateForURLString(url);
	
	// if it is system call?
	if(state.url.type()==XTBURLTypeSystemCall){
		invokeSystemCall(state);
		return;
	}
	
	// if nothing changes, do not visit.
	if(state.url==m_currentState.url &&
	   state.fragmentId==m_currentState.fragmentId){
		// but... move to fragment.
		if(!m_currentState.fragmentId.empty()){
			twRect rt=m_viewer->rectForId(state.fragmentId);
			m_viewer->setContentLocationAnimated(twPoint(0,rt.y));
		}
		return;
	}
	
	// really exists?
	if(!state.url.isReachable()){
		// doesn't exist.
		std::wstring msg=XTBLocalizedString(L"XTBBrowserUrlNotFound");
		msg=XTBFormat(msg, url.c_str(), NULL);
		
		twWnd *parent=getRootWindow();
		twMsgBox(parent, msg, twMBB_ok, XTBLocalizedString(L"XTBError"), true);
		return;
	}
	
	// write history database.
	if(state.url.type()==XTBURLTypeDocument){
		XTBHistoryDB::visitPage(url);
	}
	
	// save the current history.
	updateHistory();
	
	// will we need to advance the history?
	bool shouldAdvanceHistory=isStateNeedsAdvancingHistory(state);
	
	// display the target.
	bool succeeded=displayURL(state);
	
	if(succeeded){
		
		//XTBLog("succeeded.");
		
		if(shouldAdvanceHistory){
			advanceHistory();
			//XTBLog("advanced history.");
		}
		
		updateHistory();
		
		updateSearchBar();
		
	}else{
	
		// don't move from the current history item.
		//XTBLog("failed.");
		
	}
	
	
}

bool XTBBrowser::canGoBack() const{
	
	return m_historyPos!=m_history.begin();
}

bool XTBBrowser::canGoForward() const{
	
	XTBBrowserStateList::iterator it=m_historyPos;
	it++;
	return it!=m_history.end();
}

void XTBBrowser::goBack(){
	// make sure we can go back.
	if(!canGoBack())
		XTBThrowException(L"XTBInvalidCallException", NULL);
	
	// retreat the history.
	XTBBrowserStateList::iterator it=m_historyPos;
	assert(it!=m_history.end());
	assert(it!=m_history.begin());
	it--;
	assert(it!=m_history.end());
	
	// perform the navigation.
	moveToHistoryPos(it);
	
	updateSearchBar();
}

void XTBBrowser::goForward(){
	// make sure we can go forward.
	if(!canGoForward())
		XTBThrowException(L"XTBInvalidCallException", NULL);
	
	// advance the history.
	XTBBrowserStateList::iterator it=m_historyPos;
	assert(it!=m_history.end());
	it++;
	assert(it!=m_history.end());
	
	// perform the navigation.
	moveToHistoryPos(it);
	
	updateSearchBar();
}

void XTBBrowser::openLink(const std::wstring & url){
	navigate(url);
}

void XTBBrowser::openLinkInNewTab(const std::wstring &url){
	if(m_delegate)
		m_delegate->openLinkInNewTab(url, this);
}

bool XTBBrowser::canOpenInNewTab() const{
	if(m_delegate)
		return m_delegate->canOpenInNewTab(const_cast<XTBBrowser *>(this));
	return false;
}

#pragma mark - System Call

void XTBBrowser::invalidSystemCall(){
	twWnd *parent=getRootWindow();
	twMsgBox(parent, XTBLocalizedString(L"XTBBrowserInvalidSystemCall"), twMBB_ok, XTBLocalizedString(L"XTBError"), true);
	return;
}

void XTBBrowser::invokeSystemCall(const XTBBrowserState &url){
	if(url.url.articleName==L"page"){
		if(url.fragmentId==L"first"){
			goFirstPage();
		}else if(url.fragmentId==L"last"){
			goLastPage();
		}else if(url.fragmentId==L"previous"){
			goPreviousPage();
		}else if(url.fragmentId==L"next"){
			goNextPage();
		}else{
			int pageNum=TWUtils::intValueFromString(url.fragmentId);
			if(pageNum<1 || pageNum>pagesCount()){
				invalidSystemCall();
			}else{
				setCurrentPage(pageNum);
			}
		}
	}else if(url.url.articleName==L"history"){
		if(url.fragmentId==L"back"){
			goBack();
		}else if(url.fragmentId==L"forward"){
			goForward();
		}else{
			invalidSystemCall();
		}
	}else if(url.url.articleName==L"contents"){
		if(url.fragmentId==L"show"){
			showContents();
		}else{
			invalidSystemCall();
		}
	}else if(url.url.articleName==L"bookmark"){
		if(url.fragmentId==L"register"){
			setPageBookmarked(true, currentActualState());
		}else if(url.fragmentId==L"unregister"){
			setPageBookmarked(false, currentActualState());
		}else if(url.fragmentId==L"toggle"){
			doBookmark();
		}else{
			invalidSystemCall();
		}
	}else if(url.url.articleName==L"footnote"){
		if(!showFootnote(url.fragmentId)){
			invalidSystemCall();
		}
	}else if(url.url.articleName==L"version"){
		twMsgBox(NULL, XTBLongVersionInformation(),
				 twMBB_ok, XTBLocalizedString(L"XTBVersionInformation"), true);
	}
}

bool XTBBrowser::showFootnote(const std::wstring &linkId){
	// find the footnote link.
	TWHTMLElement *node=m_viewer->htmlElementForId(linkId);
	if(!node){
		return false;
	}
	
	TWHTMLElement *footnote=findFootnoteBody(node);
	
	if(!footnote){
		return false;
	}
	
	{
		XTBFootnoteView footnoteView(footnote);
		
		footnoteView.showModal(NULL);
		
		// make sure footnote view is destroyed here.
	}
	// return the footnote body.
	
	node->addChild(footnote);
	return true;
}

TWHTMLElement *XTBBrowser::findFootnoteBody(TWHTMLElement *parent){
	TWNode *child=parent->firstChild();
	for(;child;child=child->nextSibling()){
		TWHTMLElement *elm=static_cast<TWHTMLElement *>(child);
		if(elm->type()==TWHTMLElementTypeDiv)
			return elm;
	}
	return NULL;
}

#pragma mark - Paging

bool XTBBrowser::isPaged() const{
	if(m_currentState.url.type()==XTBURLTypeDocument){
		
		return dynamic_cast<XTBArticle *>(m_shownDocument)!=NULL;
	}
	return false;
}

int XTBBrowser::pagesCount() const{
	
	assert(isPaged());
	
	if(isSinglePageMode()){
		return 1;
	}
	
	XTBArticle *article=dynamic_cast<XTBArticle *>(m_shownDocument);
	assert(article);
	
	return article->pagesCount();
}
int XTBBrowser::currentPage() const{
	assert(isPaged());
	return m_currentState.page;
}
void XTBBrowser::setCurrentPage(int page){
	
	assert(isPaged());	
	XTBBrowserState state=currentActualState();
	state.page=page;
	displayURL(state);
}

bool XTBBrowser::canGoFirstPage() const{
	if(!isPaged())
		return false;
	return currentPage()>1;
}
bool XTBBrowser::canGoPreviousPage() const{
	if(!isPaged())
		return false;
	return currentPage()>1;
}
bool XTBBrowser::canGoNextPage() const{
	if(!isPaged())
		return false;
	
	return currentPage()<pagesCount();
}
bool XTBBrowser::canGoLastPage() const{
	if(!isPaged())
		return false;
	return currentPage()<pagesCount();
}

void XTBBrowser::goFirstPage(){
	assert(canGoFirstPage());
	setCurrentPage(1);
}
void XTBBrowser::goPreviousPage(){
	assert(canGoPreviousPage());
	setCurrentPage(currentPage()-1);
}
void XTBBrowser::goNextPage(){
	assert(canGoNextPage());
	setCurrentPage(currentPage()+1);
}
void XTBBrowser::goLastPage(){
	assert(canGoLastPage());
	setCurrentPage(pagesCount());
}

#pragma mark - Bookmark

bool XTBBrowser::canBookmark() const{
	return true;
}

void XTBBrowser::doBookmark(){
	setPageBookmarked(!isPageBookmarked(m_currentState), m_currentState);
}

bool XTBBrowser::isPageBookmarked(const XTBBrowserState& state) const{
	XTBUserDefaults *def=XTBUserDefaults::standardUserDefaults();
	TPLDictionary *dic;
	dic=def->dictionaryForKey(XTBUserDefaultsKeyBookmarks);
	
	TPLAutoReleasePtr<TPLString> key=state.tplUrlString();
	
	return dic->objectForKey(key->UTF8String())!=NULL;
}

void XTBBrowser::setPageBookmarked(bool bookmarked, const XTBBrowserState& state) const{
	XTBUserDefaults *def=XTBUserDefaults::standardUserDefaults();
	TPLDictionary *dic;
	dic=def->dictionaryForKey(XTBUserDefaultsKeyBookmarks);
	
	TPLAutoReleasePtr<TPLString> key=state.tplUrlString();
	
	// no change?
	if((dic->objectForKey(key->UTF8String())!=NULL)==bookmarked){
		return;
	}
	
	if(!bookmarked){
		dic->removeObjectForKey(key->UTF8String());
	}else{
		std::string displayTitleUTF8=twW2M(displayTitle());
		TPLAutoReleasePtr<TPLString> title=new TPLString(displayTitleUTF8.c_str());
		dic->setObject(&(*title), key->UTF8String());
	}
	
	def->synchronize();
	
	XTBNotificationCenter *center=XTBNotificationCenter::defaultNotificationCenter();
	center->sendNotification(XTBBookmarkModifiedNotification);
	
	assert(isPageBookmarked(state)==bookmarked);
}

#pragma mark - Contents

bool XTBBrowser::canShowContents() const{
	if(m_currentState.url.type()==XTBURLTypeDocument){
		return dynamic_cast<XTBArticle *>(m_shownDocument)!=NULL;
	}
	return false;
}

void XTBBrowser::showContents(){
	XTBArticle *article=dynamic_cast<XTBArticle *>(m_shownDocument);
	
	if(!article){
		XTBThrowException(L"XTBInvalidCallException", NULL);
	}
	
	XTBContentsChooser chooser
	(article, currentIndex());
	if(chooser.showModal(NULL)==twDR_ok){
		XTBArticleIndexItem idx=article->indexAt(chooser.lastSelectedIndex());
		
		std::wstring fragmentId=idx.fragmentId;
		
		int page=idx.page;
		
		if(page!=currentPage()){
			setCurrentPage(page);
		}
		
		twRect rt=m_viewer->rectForId(fragmentId);
		
		m_viewer->setContentLocationAnimated(twPoint(0, rt.y));
		
	}
}

int XTBBrowser::currentIndex() const{
	int lastIndex=-1;
	XTBArticle *article=dynamic_cast<XTBArticle *>(m_shownDocument);
	
	if(!article){
		XTBThrowException(L"XTBInvalidCallException", NULL);
	}
	
	int indexCount=article->indicesCount();
	int curPage=currentPage();
	int contLoc=m_viewer->getContentLocation().y;
	int maxContLoc=contLoc+m_viewer->getClientRect().h;
	//contLoc.y+=m_viewer->getClientRect().h;
	for(int i=0;i<indexCount;i++){
		XTBArticleIndexItem idx=article->indexAt(i);
		if(idx.page==curPage){
			twRect rt=m_viewer->rectForId(idx.fragmentId);
			if(rt.y<maxContLoc)
			lastIndex=i;
			if(rt.y>=contLoc){
				break;
			}else{
				//break;
			}
		}else if(idx.page>curPage){
			lastIndex=i-1;
			break;
		}
	}
	
	return lastIndex;
}

#pragma mark - User Interface


bool XTBBrowser::clientHitTest(const twPoint&) const{
	return true;
}
void XTBBrowser::clientPaint(const twPaintStruct& p){
	twDC *dc=p.dc;
	const twFont *fnt=getFont();
	twRect rt(twPoint(0, 0), getClientRect().size());
	
	// render the search bar background, which is a navigation bar.
	
	dc->bitBlt(getSearchBarLeftImage(), twPoint(0, 0),
			   twRect(0, 0, 110, 24));
	
	if(m_drag==XTBBrowserDragModeBack && m_hover){
		dc->bitBlt(getSearchBarBackImage(), twPoint(0, 0),
				   twRect(0, 0, 95, 24));
	}
	if(m_drag==XTBBrowserDragModeForward && m_hover){
		dc->bitBlt(getSearchBarForwardImage(), twPoint(0, 0),
				   twRect(0, 0, 95, 24));
	}
	
	for(int x=110;x<rt.w-39;x+=52){
		dc->bitBlt(getSearchBarLeftImage(), twPoint(x, 0),
				   twRect(110, 0, std::min(52, rt.w-39-x), 24));
	}
	dc->bitBlt(getSearchBarRightImage(), twPoint(rt.w-39, 0),
			   twRect(0, 0, 39, 24));
	
	// fill the page bar's background.
	
	rt=rectForPageBar();
	for(int x=0;x<rt.w;x+=24){
		dc->bitBlt(getPageBarImage(), twPoint(rt.x+x, rt.y),
				   twRect(288, 0, std::min(24, rt.w-x), 24));
	}
	/*
	if(rectForFirstPageButton() && p.paintRect){
		if(canGoFirstPage()){
			if(m_drag==XTBBrowserDragModeFirstPage && m_hover){
				dc->bitBlt(getPageBarImage(), rectForFirstPageButton().loc(),
						   twRect(48+144, 0, 24, 24));
			}else{
				dc->bitBlt(getPageBarImage(), rectForFirstPageButton().loc(),
						   twRect(48, 0, 24, 24));
			}
		}else{
			dc->bitBlt(getPageBarImage(), rectForFirstPageButton().loc(),
					   twRect(48, 24, 24, 24));
		}
	}
	*/
	if(rectForPreviousPageButton() && p.paintRect){
		if(!isSinglePageMode()){
			if(canGoPreviousPage()){
				if(m_drag==XTBBrowserDragModePreviousPage && m_hover){
					dc->bitBlt(getPageBarImage(), rectForPreviousPageButton().loc(),
							   twRect(0+144, 0, 24, 24));
				}else{
					dc->bitBlt(getPageBarImage(), rectForPreviousPageButton().loc(),
							   twRect(0, 0, 24, 24));
				}
			}else{
				dc->bitBlt(getPageBarImage(), rectForPreviousPageButton().loc(),
						   twRect(0, 24, 24, 24));
			}
		}
	}
	
	if(rectForNextPageButton() && p.paintRect){
		if(!isSinglePageMode()){
			if(canGoNextPage()){
				if(m_drag==XTBBrowserDragModeNextPage && m_hover){
					dc->bitBlt(getPageBarImage(), rectForNextPageButton().loc(),
							   twRect(24+144, 0, 24, 24));
				}else{
					dc->bitBlt(getPageBarImage(), rectForNextPageButton().loc(),
							   twRect(24, 0, 24, 24));
				}
			}else{
				dc->bitBlt(getPageBarImage(), rectForNextPageButton().loc(),
						   twRect(24, 24, 24, 24));
			}
		}
	}
	/*
	if(rectForLastPageButton() && p.paintRect){
		if(canGoLastPage()){
			if(m_drag==XTBBrowserDragModeLastPage && m_hover){
				dc->bitBlt(getPageBarImage(), rectForLastPageButton().loc(),
						   twRect(72+144, 0, 24, 24));
			}else{
				dc->bitBlt(getPageBarImage(), rectForLastPageButton().loc(),
						   twRect(72, 0, 24, 24));
			}
		}else{
			dc->bitBlt(getPageBarImage(), rectForLastPageButton().loc(),
					   twRect(72, 24, 24, 24));
		}
	}
	*/
	if(rectForBookmarkButton() && p.paintRect){
		if(isPageBookmarked(m_currentState)){
			if(m_drag==XTBBrowserDragModeBookmark && m_hover){
				dc->bitBlt(getPageBarImage(), rectForBookmarkButton().loc(),
						   twRect(96+144, 24, 24, 24));
			}else{
				dc->bitBlt(getPageBarImage(), rectForBookmarkButton().loc(),
						   twRect(96, 24, 24, 24));
			}
		}else{
			if(m_drag==XTBBrowserDragModeBookmark && m_hover){
				dc->bitBlt(getPageBarImage(), rectForBookmarkButton().loc(),
						   twRect(96+144, 0, 24, 24));
			}else{
				dc->bitBlt(getPageBarImage(), rectForBookmarkButton().loc(),
						   twRect(96, 0, 24, 24));
			}
		}
	}
	
	if(rectForContentsButton() && p.paintRect){
		if(canShowContents()){
			if(m_drag==XTBBrowserDragModeContents && m_hover){
				dc->bitBlt(getPageBarImage(), rectForContentsButton().loc(),
						   twRect(120+144, 0, 24, 24));
			}else{
				dc->bitBlt(getPageBarImage(), rectForContentsButton().loc(),
						   twRect(120, 0, 24, 24));
			}
		}else{
			dc->bitBlt(getPageBarImage(), rectForContentsButton().loc(),
					   twRect(120, 24, 24, 24));
		}
	}
	
	if(rectForPageNumberBar() && p.paintRect){
		if(isPaged() && !isSinglePageMode()){
			std::wstring curPageStr=XTBFormatStd(L"%d", (int)currentPage());
			std::wstring pageCntStr=XTBFormatStd(L"%d", (int)(pagesCount()));
			std::wstring str=XTBFormat(XTBLocalizedString(L"XTBBrowserPageBar"),
														  curPageStr.c_str(),
														  pageCntStr.c_str(),
														  NULL);
			twSize sz=fnt->measure(str);
			twRect r=rectForPageNumberBar();
			twPoint pt;
			pt.x=r.x+(r.w-sz.w)/2;
			pt.y=r.y+(r.h-sz.h)/2;
			
			fnt->render(dc, twRGB(230, 230, 230), 
						twPoint(pt.x, pt.y+1), 
						str);
			fnt->render(dc, twRGB(0, 0, 0), 
						twPoint(pt.x, pt.y), 
						str);
		}
	}
	
	
}

void XTBBrowser::setRect(const twRect& rt){
	bool softkeyed=m_softKeyboard?(m_softKeyboard->getStyle().visible):false;
	twWnd::setRect(rt);
	m_searchBar->setRect(twRect(106, 5, rt.w-121, 14));
	
	twRect contentRect=twRect(0, 24, rt.w, rt.h-48);
	if(softkeyed){
		contentRect.h-=74;
	}
	
	m_viewer->setRect(contentRect);
	m_searchView->setRect(contentRect);
	m_startPageView->setRect(contentRect);
	
	if(softkeyed){
		m_softKeyboard->setRect(twRect(contentRect.x,
									   contentRect.y+contentRect.h,
									   contentRect.w,
									   74));
	}
}

#pragma mark - Soft Keyboard

void XTBBrowser::setSoftKeyboardVisible(bool b){
	if(softKeyboardVisible()==b)
		return;
	
	if(b)
		m_softKeyboard->show();
	else {
		m_softKeyboard->hide();
	}
	
	setRect(getRect());
}

bool XTBBrowser::softKeyboardVisible() const{
	if(!m_softKeyboard)
		return false;
	return m_softKeyboard->getStyle().visible;
}

#pragma mark - Interraction

void XTBBrowser::openLink(){
	TWHTMLElement *element=m_viewer->getLastTouchedElement();
	
	if(!element)
		return;
	
	std::wstring href=element->getAttribute(L"href");
	
	openLink(href);
}


void XTBBrowser::openLinkInNewTab(){
	TWHTMLElement *element=m_viewer->getLastTouchedElement();
	
	if(!element)
		return;
	
	std::wstring href=element->getAttribute(L"href");
	
	openLinkInNewTab(href);
}

void XTBBrowser::openLinkMenu(){
	TWHTMLElement *element=m_viewer->getLastTouchedElement();
	twRect rt=m_viewer->getLastTouchedRect();
	rt+=m_viewer->getWndRect().loc();
	rt+=m_viewer->getClientRect().loc();
	rt=rt.inflate(2);
	rt.y--; rt.h+=2;
	
	if(!element)
		return;
	
	std::wstring href=element->getAttribute(L"href");
	XTBBrowserState hrefState=stateForURLString(href);
	
	if(hrefState.url.type()==XTBURLTypeSearch){
		
		XTBMenu menu;
		menu.setTargetRect(rt);
		menu.addMenuItem(new XTBLabelMenuItem(href));
		menu.addMenuItem(new XTBSeparatorMenuItem());
		menu.addMenuItem(new XTBButtonMenuItem(XTBLocalizedString(L"XTBBrowserLinkSearch")));
		if(canOpenInNewTab())
			menu.addMenuItem(new XTBButtonMenuItem(XTBLocalizedString(L"XTBBrowserLinkSearchInNewTab")));
		menu.showModal(NULL);
		
		switch(menu.getLastSelection()){
			case 2:
				openLink(href);
				break;
			case 3:
				openLinkInNewTab(href);
				break;
		}
	}else if(hrefState.url.type()==XTBURLTypeSystemCall){
		openLink(href);
	}else if(hrefState.url==m_currentState.url){
		
		XTBArticle *article=dynamic_cast<XTBArticle *>(m_shownDocument);
		if(!article){
			goto Unreachable;
		}
		
		int page=article->pageForFragmentId(hrefState.fragmentId);
		
		XTBMenu menu;
		menu.setTargetRect(rt);
		menu.addMenuItem(new XTBLabelMenuItem(href));
		menu.addMenuItem(new XTBSeparatorMenuItem());
		if(page!=currentPage()){
			std::wstring pageStr=XTBFormatStd(L"%d", page);
			std::wstring s;
			s=XTBFormat(XTBLocalizedString(L"XTBBrowserLinkJumpPage"), pageStr.c_str(), NULL);
			menu.addMenuItem(new XTBButtonMenuItem(s));
		}else if(m_viewer->rectForId(hrefState.fragmentId).h!=0){
			menu.addMenuItem(new XTBButtonMenuItem(XTBLocalizedString(L"XTBBrowserLinkJump")));
		}else{
			goto Unreachable;
		}
		if(canOpenInNewTab())
			menu.addMenuItem(new XTBButtonMenuItem(XTBLocalizedString(L"XTBBrowserLinkOpenInNewTab")));
		menu.showModal(NULL);
		
		switch(menu.getLastSelection()){
			case 2:
				openLink(href);
				break;
			case 3:
				openLinkInNewTab(href);
				break;
		}
		
	}else if(hrefState.url.isReachable()){
		
		XTBMenu menu;
		menu.setTargetRect(rt);
		menu.addMenuItem(new XTBLabelMenuItem(href));
		menu.addMenuItem(new XTBSeparatorMenuItem());
		menu.addMenuItem(new XTBButtonMenuItem(XTBLocalizedString(L"XTBBrowserLinkOpen")));
		if(canOpenInNewTab())
			menu.addMenuItem(new XTBButtonMenuItem(XTBLocalizedString(L"XTBBrowserLinkOpenInNewTab")));
		menu.showModal(NULL);
		
		switch(menu.getLastSelection()){
			case 2:
				openLink(href);
				break;
			case 3:
				openLinkInNewTab(href);
				break;
		}
		
	}else{
		
	Unreachable:
		
		XTBMenu menu;
		menu.setTargetRect(rt);
		menu.addMenuItem(new XTBLabelMenuItem(href));
		menu.addMenuItem(new XTBSeparatorMenuItem());
		menu.addMenuItem(new XTBLabelMenuItem(XTBLocalizedString(L"XTBBrowserLinkNoArticle")));
		menu.showModal(NULL);
		
		switch(menu.getLastSelection()){
				
		}
		
	}
}

std::wstring XTBBrowser::formatSelectedText(const std::wstring &str)const{
	std::wstring outstr;
	outstr.reserve(str.size());
	for(size_t i=0;i<str.size();i++){
		if(str[i]==L'\n')
			continue;
		if(str[i]==L':')
			continue;
		outstr+=str[i];
	}
	return TWUtils::trimString(outstr);
}

void XTBBrowser::openSelectionMenu(){
	std::wstring str=formatSelectedText(m_viewer->getLastSelectedText());
	if(str.empty()){
		// no selection...
		return;
	}
	
	bool valid=true;
	if(str.size()>128)
		valid=false;
	
	twRect rt=m_viewer->getLastTouchedRect();
	rt+=m_viewer->getWndRect().loc();
	rt+=m_viewer->getClientRect().loc();
	rt=rt.inflate(1);
	rt.y--; rt.h+=2;
	
	XTBMenu menu;
	menu.setTargetRect(rt);
	
	XTBButtonMenuItem *item;
	
	item=new XTBButtonMenuItem(XTBLocalizedString(L"XTBBrowserLinkSearch"));
	item->setEnable(valid);
	menu.addMenuItem(item);
	
	if(canOpenInNewTab()){
		item=new XTBButtonMenuItem(XTBLocalizedString(L"XTBBrowserLinkSearchInNewTab"));
		item->setEnable(valid);
		menu.addMenuItem(item);
	}
	menu.showModal(NULL);
	
	switch(menu.getLastSelection()){
		case 0:
			openLink(str);
			break;
		case 1:
			openLinkInNewTab(str);
			break;
	}

}

void XTBBrowser::command(int id){
	switch(id){
		case XTBBrowserViewerId:
			{
				switch(m_viewer->getLastEventType()){
					case TWViewerEventTypeLinkTouched:
					case TWViewerEventTypeLinkHolded:
						if(XTBUserDefaults::standardUserDefaults()->numberForKey(XTBUserDefaultsKeySingleTouchLink, "false")->boolValue()){
							// single touch opens link, hold shows menu.
							if(m_viewer->getHolded())
								openLinkMenu();
							else
								openLink();
						}else{
							// always shows menu.
							openLinkMenu();
						}
						break;
					case TWViewerEventTypeTextSelected:
						openSelectionMenu();
						break;
				}
				
			}
			break;
		case XTBBrowserSearchBarId:
			{
				const std::wstring& queryText=m_searchBar->getTextField()->getTitle();
				if(queryText.find(L':')==std::wstring::npos){
					// there is no L':'; going to search.
					// update the search result for the incremental search.
					navigate(m_searchBar->getTextField()->getTitle());
				}
			}
			break;
		case XTBBrowserSearchViewId:
		{
			std::wstring url=m_searchView->lastSelectedTitle();
			if(url.empty())
				sendCmdToParent();
			else
				navigate(url);
		}
			break;
		case XTBBrowserStartPageViewId:
			{
				std::wstring url=m_startPageView->getLastSelectedUrl();
				navigate(url);
			}
			break;
		case XTBBrowserSoftKeyboardId:
			{
				m_searchBar->getTextField()->insertString
				(m_softKeyboard->lastKey());
				m_searchBar->getTextField()->focus();
				command(XTBBrowserSearchBarId);
			}
			break;
	}
}
twRect XTBBrowser::rectForBackButton() const{
	return twRect(5, 0, 42, 24);
}
twRect XTBBrowser::rectForForwardButton() const{
	return twRect(48, 0, 42, 24);
}
twRect XTBBrowser::rectForInvalidaingNavigationButtons() const{
	return twRect(0, 0, 95, 24);
}
twRect XTBBrowser::rectForPageBar() const{
	twRect rt(twPoint(0, 0), getClientRect().size());
	return twRect(rt.x, rt.y+rt.h-24, rt.w, 24);
}
twRect XTBBrowser::rectForFirstPageButton() const{
	return twRect::emptyRect();
}
twRect XTBBrowser::rectForPreviousPageButton() const{
	twRect pageBar=rectForPageBar();
	return twRect(pageBar.x, pageBar.y, 24, 24);
}
twRect XTBBrowser::rectForNextPageButton() const{
	twRect pageBar=rectForPageBar();
	return twRect(pageBar.x+pageBar.w-24, pageBar.y, 24, 24);
}
twRect XTBBrowser::rectForLastPageButton() const{
	return twRect::emptyRect();
}
twRect XTBBrowser::rectForBookmarkButton() const{
	twRect pageBar=rectForPageBar();
	twRect pageNumberBar=rectForPageNumberBar();
	return twRect(pageNumberBar.x-24, pageBar.y, 24, 24);
}
twRect XTBBrowser::rectForContentsButton() const{
	twRect pageBar=rectForPageBar();
	twRect pageNumberBar=rectForPageNumberBar();
	return twRect(pageNumberBar.x+pageNumberBar.w, pageBar.y, 24, 24);
}
twRect XTBBrowser::rectForPageNumberBar() const{
	twRect pageBar=rectForPageBar();
	const int height=16;
	const int width=32;
	return twRect(pageBar.x+(pageBar.w-width)/2,
				  pageBar.y+(pageBar.h-height)/2,
				  width, height);
}

void XTBBrowser::clientMouseDown(const twPoint& pt, twMouseButton mb){
	if(mb!=twMB_left)
		return;
	m_hover=false;
	
	if((rectForBackButton()&&pt) && canGoBack()){
		m_drag=XTBBrowserDragModeBack;
		m_hover=true;
		invalidateClientRect(rectForInvalidaingNavigationButtons());
	}
	
	if((rectForForwardButton()&&pt) && canGoForward()){
		m_drag=XTBBrowserDragModeForward;
		m_hover=true;
		invalidateClientRect(rectForInvalidaingNavigationButtons());
	}
	
	if((rectForFirstPageButton()&&pt) && canGoFirstPage()){
		m_drag=XTBBrowserDragModeFirstPage;
		m_hover=true;
		invalidateClientRect(rectForFirstPageButton());
	}
	
	if((rectForPreviousPageButton()&&pt) && canGoPreviousPage()){
		m_drag=XTBBrowserDragModePreviousPage;
		m_hover=true;
		invalidateClientRect(rectForPreviousPageButton());
	}
	
	if((rectForNextPageButton()&&pt) && canGoNextPage()){
		m_drag=XTBBrowserDragModeNextPage;
		m_hover=true;
		invalidateClientRect(rectForNextPageButton());
	}
	
	if((rectForLastPageButton()&&pt) && canGoLastPage()){
		m_drag=XTBBrowserDragModeLastPage;
		m_hover=true;
		invalidateClientRect(rectForLastPageButton());
	}
	
	if((rectForBookmarkButton()&&pt) && canBookmark()){
		m_drag=XTBBrowserDragModeBookmark;
		m_hover=true;
		invalidateClientRect(rectForBookmarkButton());
	}
	
	if((rectForContentsButton()&&pt) && canShowContents()){
		m_drag=XTBBrowserDragModeContents;
		m_hover=true;
		invalidateClientRect(rectForContentsButton());
	}
	
	
	if(rectForPageBar() && pt && !m_hover){
		setSoftKeyboardVisible(!softKeyboardVisible());
	}
	
	twSetCapture(this);
}

void XTBBrowser::clientMouseMove(const twPoint& pt){
	if(m_drag==XTBBrowserDragModeBack){
		bool newHover=rectForBackButton()&&pt;
		if(newHover!=m_hover){
			m_hover=newHover;
			invalidateClientRect(rectForInvalidaingNavigationButtons());
		}
	}
	if(m_drag==XTBBrowserDragModeForward){
		bool newHover=rectForForwardButton()&&pt;
		if(newHover!=m_hover){
			m_hover=newHover;
			invalidateClientRect(rectForInvalidaingNavigationButtons());
		}
	}
	
	if(m_drag==XTBBrowserDragModeFirstPage){
		bool newHover=rectForFirstPageButton()&&pt;
		if(newHover!=m_hover){
			m_hover=newHover;
			invalidateClientRect(rectForFirstPageButton());
		}
	}
	if(m_drag==XTBBrowserDragModePreviousPage){
		bool newHover=rectForPreviousPageButton()&&pt;
		if(newHover!=m_hover){
			m_hover=newHover;
			invalidateClientRect(rectForPreviousPageButton());
		}
	}
	if(m_drag==XTBBrowserDragModeNextPage){
		bool newHover=rectForNextPageButton()&&pt;
		if(newHover!=m_hover){
			m_hover=newHover;
			invalidateClientRect(rectForNextPageButton());
		}
	}
	if(m_drag==XTBBrowserDragModeLastPage){
		bool newHover=rectForLastPageButton()&&pt;
		if(newHover!=m_hover){
			m_hover=newHover;
			invalidateClientRect(rectForLastPageButton());
		}
	}
	if(m_drag==XTBBrowserDragModeBookmark){
		bool newHover=rectForBookmarkButton()&&pt;
		if(newHover!=m_hover){
			m_hover=newHover;
			invalidateClientRect(rectForBookmarkButton());
		}
	}
	if(m_drag==XTBBrowserDragModeContents){
		bool newHover=rectForContentsButton()&&pt;
		if(newHover!=m_hover){
			m_hover=newHover;
			invalidateClientRect(rectForContentsButton());
		}
	}
}

void XTBBrowser::clientMouseUp(const twPoint& pt, twMouseButton mb){
	if(mb!=twMB_left)
		return;
	if(m_drag==XTBBrowserDragModeBack){
		m_drag=XTBBrowserDragModeNone;
		if(m_hover){
			if(canGoBack())
				goBack();
			else
				twMsgBox(getRootWindow(), XTBLocalizedString(L"XTBBrowserCannotGoBack"),
						 twMBB_ok, XTBLocalizedString(L"XTBError"), true);
			invalidateClientRect(rectForInvalidaingNavigationButtons());
		}
	}
	if(m_drag==XTBBrowserDragModeForward){
		m_drag=XTBBrowserDragModeNone;
		if(m_hover){
			if(canGoForward())
				goForward();
			else
				twMsgBox(getRootWindow(), XTBLocalizedString(L"XTBBrowserCannotGoForward"),
						 twMBB_ok, XTBLocalizedString(L"XTBError"), true);
			invalidateClientRect(rectForInvalidaingNavigationButtons());
		}
	}
	
	if(m_drag==XTBBrowserDragModeFirstPage){
		m_drag=XTBBrowserDragModeNone;
		if(m_hover){
			goFirstPage();
			invalidateClientRect(rectForFirstPageButton());
		}
	}
	if(m_drag==XTBBrowserDragModePreviousPage){
		m_drag=XTBBrowserDragModeNone;
		if(m_hover){
			goPreviousPage();
			invalidateClientRect(rectForPreviousPageButton());
		}
	}
	if(m_drag==XTBBrowserDragModeNextPage){
		m_drag=XTBBrowserDragModeNone;
		if(m_hover){
			goNextPage();
			invalidateClientRect(rectForNextPageButton());
		}
	}
	if(m_drag==XTBBrowserDragModeLastPage){
		m_drag=XTBBrowserDragModeNone;
		if(m_hover){
			goLastPage();
			invalidateClientRect(rectForLastPageButton());
		}
	}
	if(m_drag==XTBBrowserDragModeBookmark){
		m_drag=XTBBrowserDragModeNone;
		if(m_hover){
			doBookmark();
			invalidateClientRect(rectForBookmarkButton());
		}
	}
	if(m_drag==XTBBrowserDragModeContents){
		m_drag=XTBBrowserDragModeNone;
		if(m_hover){
			showContents();
			invalidateClientRect(rectForContentsButton());
		}
	}
	twReleaseCapture();
}

void XTBBrowser::clientKeyDown(const std::wstring& name){
	if(name==L"Return"){
		navigate(m_searchBar->getTextField()->getTitle());
	}else if(name==L"BackSpace" || name==L"F1"){
		if(!m_searchBar->getTextField()->getStatus().focused){
			m_searchBar->getTextField()->focus();
			m_searchBar->getTextField()->selectLast();
			m_searchBar->clientKeyDown(name);
		}
	}else if(name==L"Up" || name==L"KeyPad8"){
		if(m_searchView->isVisible()){
			// while searching, focus the list view and enable keyboard mode.
			if(!m_searchView->listView()->getStatus().focused){
				m_searchView->listView()->focus();
				m_searchView->clientKeyDown(name);
			}
		}else if(m_startPageView->isVisible()){
		}else if(m_viewer->isVisible()){
			// while browsing, scroll the browser.
			twPoint pt=m_viewer->getContentLocation();
			pt.y-=30;
			if(pt.y<0)
				pt.y=0;
			m_viewer->setContentLocation(pt);
		}
	}else if(name==L"Down" || name==L"KeyPad2"){
		if(m_searchView->isVisible()){
			// while searching, focus the list view and enable keyboard mode.
			if(!m_searchView->listView()->getStatus().focused){
				m_searchView->listView()->focus();
				m_searchView->clientKeyDown(name);
			}
		}else if(m_startPageView->isVisible()){
		}else if(m_viewer->isVisible()){
			// while browsing, scroll the browser.
			twPoint pt=m_viewer->getContentLocation();
			twPoint maxs=m_viewer->getMaxContentLocation();
			pt.y+=30;
			if(pt.y>maxs.y)
				pt.y=maxs.y;
			m_viewer->setContentLocation(pt);
		}
	}else if(name==L"Left" || name==L"KeyPad4"){
		if(m_searchBar->getTextField()->getTitle().empty()){
			if(m_startPageView->isVisible()){
				// in start page, left arrow give focus to left list view.
				if(!m_startPageView->bookmarList()->getStatus().focused){
					m_startPageView->bookmarList()->focus();
					m_startPageView->clientKeyDown(name);
				}
			}
		}else{
			// otherwise, give focus to search bar, and let user move caret.
			if(!m_searchBar->getTextField()->getStatus().focused){
				m_searchBar->getTextField()->focus();
				m_searchBar->getTextField()->selectLast();
				m_searchBar->clientKeyDown(name);
			}
		}
	}else if(name==L"Right" || name==L"KeyPad6"){
		if(m_searchBar->getTextField()->getTitle().empty()){
			if(m_startPageView->isVisible()){
				// in start page, right arrow give focus to right list view.
				if(!m_startPageView->historyList()->getStatus().focused){
					m_startPageView->historyList()->focus();
					m_startPageView->clientKeyDown(name);
				}
			}
		}else{
			// otherwise, give focus to search bar, and let user move caret.
			if(!m_searchBar->getTextField()->getStatus().focused){
				m_searchBar->getTextField()->focus();
				m_searchBar->getTextField()->selectLast();
				m_searchBar->clientKeyDown(name);
			}
		}
	}else if(name==L"PageUp" || name==L"KeyPad9"){
		if(m_viewer->isVisible()){
			twPoint pt=m_viewer->getContentLocation();
			pt.y-=m_viewer->getClientRect().h;
			m_viewer->setContentLocationAnimated(pt);
		}
	}else if(name==L"PageDown" || name==L"KeyPad3"){
		if(m_viewer->isVisible()){
			twPoint pt=m_viewer->getContentLocation();
			pt.y+=m_viewer->getClientRect().h;
			m_viewer->setContentLocationAnimated(pt);
		}
	}else if(name==L"KeyPad1"){
		if(!m_searchBar->getTextField()->getStatus().focused){
			m_searchBar->getTextField()->focus();
			m_searchBar->clientKeyDown(name);
		}
	}else if(name.size()==1){
		if(!m_searchBar->getTextField()->getStatus().focused){
			m_searchBar->getTextField()->focus();
			m_searchBar->getTextField()->selectLast();
			m_searchBar->clientKeyDown(name);
		}
	}
}

void XTBBrowser::clientKeyPress(wchar_t c){
	if(!c)
		return;
	if(!m_searchBar->getTextField()->getStatus().focused){
		m_searchBar->getTextField()->focus();
		m_searchBar->getTextField()->selectLast();
		m_searchBar->clientKeyPress(c);
	}
	
}


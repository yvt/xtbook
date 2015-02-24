/*
 *  XTBBrowser.h
 *  XTBook
 *
 *  Created by Nexhawks on 2/21/11.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#pragma once
#include <tcw/twWnd.h>
#include <list>
#include <vector>
#include "XTBURL.h"
#include "TPList/TPLString.h"

#define XTBBROWSER_HISTORY_VECTOR	1

class TWViewer;
class XTBSearchBar;
class TWHTMLBodyElement;
class TWHTMLElement;

enum{
	XTBBrowserViewerId=1,
	XTBBrowserSearchBarId,
	XTBBrowserSearchViewId,
	XTBBrowserStartPageViewId,
	XTBBrowserSoftKeyboardId
};

class XTBArticle;
class XTBDocument;
class XTBSearchView;
class XTBStartPageView;

enum{
	XTBBrowserStatePageUnknown=0
};

/** The information for browsing history. */
struct XTBBrowserState{
	XTBURL url;
	std::wstring fragmentId;
	int page;
	twPoint contentLocation;
	
	std::wstring urlString() const;
	
	/** returns the url in TPLString 
	 * @return your TPLString */
	TPLString *tplUrlString() const;
};

struct XTBBrowserLoadPageResult;
struct XTBSearchViewQuery;
#if XTBBROWSER_HISTORY_VECTOR
typedef std::vector<XTBBrowserState> XTBBrowserStateList;
#else
typedef std::list<XTBBrowserState> XTBBrowserStateList;
#endif

enum XTBBrowserDragMode{
	XTBBrowserDragModeNone=0,
	XTBBrowserDragModeBack,
	XTBBrowserDragModeForward,
	XTBBrowserDragModeFirstPage,
	XTBBrowserDragModePreviousPage,
	XTBBrowserDragModeNextPage,
	XTBBrowserDragModeLastPage,
	XTBBrowserDragModeBookmark,
	XTBBrowserDragModeContents
};

class XTBBrowser;
class XTBSoftKeyboard;

class XTBBrowserDelegate{
public:
	XTBBrowserDelegate(){}
	virtual ~XTBBrowserDelegate(){}
	virtual void openLinkInNewTab(const std::wstring&, XTBBrowser *){}
	virtual bool canOpenInNewTab(XTBBrowser *){return false;}
};

/** Window that contains an address bar, navigation bar, and so on.
 * It manages events from its TWViewer. */
class XTBBrowser : public twWnd{
private:
	
	static twDC *m_searchBarBackImage;
	static twDC *m_searchBarForwardImage;
	static twDC *m_searchBarLeftImage;
	static twDC *m_searchBarRightImage;
	static twDC *m_pageBarImage;
	
	static twDC *getSearchBarBackImage();
	static twDC *getSearchBarForwardImage();
	static twDC *getSearchBarLeftImage();
	static twDC *getSearchBarRightImage();
	static twDC *getPageBarImage();
	
	XTBBrowserDelegate *m_delegate;
	
	XTBSearchBar *m_searchBar;
	TWViewer *m_viewer;
	XTBSearchView *m_searchView;
	XTBStartPageView *m_startPageView;
	XTBSoftKeyboard *m_softKeyboard;
	TWHTMLBodyElement *m_rootElement;
	
	XTBBrowserDragMode m_drag;
	bool m_hover;
	
	/** currently shown document or NULL if search or main page is shown. */
	XTBDocument *m_shownDocument;
	
	/** all histories. m_history.begin() is the first history. */
	XTBBrowserStateList m_history;
	
	/** history that represents the currently actually shown document. */
	XTBBrowserState m_currentState;
	
	/** iterator to history that represents the currently shown document. */
	XTBBrowserStateList::iterator m_historyPos;
	
	twRect rectForBackButton() const;
	twRect rectForForwardButton() const;
	twRect rectForInvalidaingNavigationButtons() const;
	twRect rectForPageBar() const;
	twRect rectForFirstPageButton() const;
	twRect rectForPreviousPageButton() const;
	twRect rectForNextPageButton() const;
	twRect rectForLastPageButton() const;
	twRect rectForBookmarkButton() const;
	twRect rectForContentsButton() const;
	twRect rectForPageNumberBar() const;
	
	/** internal function. */
	void loadPage(XTBBrowserState *, XTBBrowserLoadPageResult *);
	
	/** display the url overwriting the current history, which means, user
	 * cannot get back to the previous displayed page. This is much like 
	 * window.location.replace() in JS.
	 * @return true is there was no error and move to the new page, 
	 *         otherwise, false. */
	bool displayURL(XTBBrowserState);
	
	/** returns XTBBrowserState corresponding to the url.
	 * @return XTBBrowserState with url and fragmentID but without page nor contentLocation. */
	XTBBrowserState stateForURLString(const std::wstring&) const;
	
	XTBBrowserState initialHistory() const;
	
	/** gets the XTBDocument that corresponds to the url of the state. */
	XTBDocument *documentForBrowserState(const XTBBrowserState&) const;
	
	int pageForFragmentId(const std::wstring&, const XTBArticle *) const;
	
	/** comparing with the current state, checks whether we need to reload to
	 * transfer to the state. */
	bool isStateNeedsReloading(const XTBBrowserState&) const;
	
	/** comparing with the current state, checks whether we need to advanceHistory(). */
	bool isStateNeedsAdvancingHistory(const XTBBrowserState&) const;
	
	/** updates the current history to match the current actual state. */
	void updateHistory();
	
	/** get the current actual state. */
	XTBBrowserState currentActualState() const;
	
	/** forward the current history, and destroy the appropriate histories.
	 * the new history will be copied from the current actual state.
	 * remember to updateHistory before advanceHistory. */
	void advanceHistory();
	
	/** move to the specified history item.
	 * @return true if succeseded. */
	bool moveToHistoryPos(XTBBrowserStateList::iterator);
	
	/** show and hide appropriate views to the url type. */
	void setUrlType(XTBURLType);
	
	/** match search bar to current state. */
	void updateSearchBar();
	
	XTBSearchViewQuery queryForState(const XTBBrowserState&) const;
	
	bool isPageBookmarked(const XTBBrowserState&) const;
	void setPageBookmarked(bool, const XTBBrowserState&) const;
	
	void openLink(const std::wstring&);
	void openLinkInNewTab(const std::wstring&);
	bool canOpenInNewTab() const;
	
	void openLink();
	void openLinkInNewTab();
	void openLinkMenu();
	void openSelectionMenu();
	
	int currentIndex() const;
	
	void invalidSystemCall();
	void invokeSystemCall(const XTBBrowserState&);
	bool showFootnote(const std::wstring& linkId);
	TWHTMLElement *findFootnoteBody(TWHTMLElement *);
	
	std::wstring formatSelectedText(const std::wstring& str) const;
	
	bool isSinglePageMode() const;
	
public:
	XTBBrowser();
	virtual ~XTBBrowser();
	virtual void clientPaint(const twPaintStruct&);
	virtual bool clientHitTest(const twPoint&) const;
	virtual void setRect(const twRect&);
	
	virtual void command(int);
	
	virtual void clientMouseDown(const twPoint&, twMouseButton);
	virtual void clientMouseMove(const twPoint&);
	virtual void clientMouseUp(const twPoint&, twMouseButton);
	
	virtual void clientKeyDown(const std::wstring&);
	virtual void clientKeyPress(wchar_t);
	
	/** visit the url. history is correctly handled. */
	void navigate(const std::wstring& url);
	
	bool canGoBack() const;
	bool canGoForward() const;
	
	void goBack();
	void goForward();
	
	/** true if the shown page has a paging feature. 
	 * for example, home doesn't have any pages. */
	bool isPaged() const;
	
	int pagesCount() const;
	int currentPage() const;
	void setCurrentPage(int);
	
	bool canGoFirstPage() const;
	bool canGoPreviousPage() const;
	bool canGoNextPage() const;
	bool canGoLastPage() const;
	bool canBookmark() const;
	bool canShowContents() const;
	
	void goFirstPage();
	void goPreviousPage();
	void goNextPage();
	void goLastPage();
	void doBookmark();
	void showContents();
	
	void setSoftKeyboardVisible(bool);
	bool softKeyboardVisible() const;
	
	std::wstring displayTitle()const;
	
	void setDelegate(XTBBrowserDelegate *d){
		m_delegate=d;
	}
	XTBBrowserDelegate *delegate(){
		return m_delegate;
	}
};

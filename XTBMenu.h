//
//  XTBMenu.h
//  XTBook
//
//  Created by Kawada Tomoaki on 5/18/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include <vector>
#include "XTBOverlay.h"

class XTBMenuItem;

enum{
	XTBMenuNoIndex=-1
};

class XTBMenu: public XTBOverlay{
private:
	std::vector<XTBMenuItem *> m_items;
	int m_selection;
	twRect m_selectionRect;
	bool m_hover;
	bool m_drag;
	
	bool m_keyboardEnabled;
	int m_keyboardSelection;
	
	bool m_hasSelectable;
	
	void disableKeyboard();
	void enableKeyboard();
	
protected:
	twRect m_targetRect;
	
	twPoint getMenuItemOffset() const;

	virtual twPoint getStartupPos(const twSize&,
								  const twRect& desktop);
public:
	XTBMenu();
	virtual ~XTBMenu();
	
	void addMenuItem(XTBMenuItem *);
	void removeAllMenuItems();
	void setTargetRect(const twRect& rt){m_targetRect=rt;}
	void setTargetPos(const twPoint& pt){m_targetRect=twRect(pt.x, pt.y, 0, 0);}
	
	virtual void clientPaint(const twPaintStruct&);
	
	virtual void clientMouseDown(const twPoint&, twMouseButton);
	virtual void clientMouseMove(const twPoint&);
	virtual void clientMouseUp(const twPoint&, twMouseButton);
	
	virtual void clientKeyDown(const std::wstring&);
	
	virtual void backgroundTouched();
	
	int getLastSelection(){return m_selection;}
};

//
//  XTBFootnoteView.h
//  XTBook
//
//  Created by Kawada Tomoaki on 8/3/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "XTBDialogSheet.h"

class TWHTMLElement;
class TWHTMLBodyElement;
class TWViewer;

class XTBFootnoteView: public XTBDialogSheet{
	TWHTMLElement *m_footnoteBody;
	TWHTMLBodyElement *m_footnoteView;
	TWViewer *m_viewer;
	
	twRect rectForViewer() const;
protected:
	
	virtual twSize startupSize() const;
	
public:
	XTBFootnoteView(TWHTMLElement *footnoteBody);
	virtual ~XTBFootnoteView();
	
	virtual void backgroundTouched();
	
	virtual void command(int);
	
};

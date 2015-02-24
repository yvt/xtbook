/*
 *  XTBViewerTest.h
 *  XTBook
 *
 *  Created by Nexhawks on 2/20/11.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#pragma once
#include <tcw/twDialog.h>
#include "XTBViewer.h"
#include <tcw/twButton.h>


class XTBViewerTest : public twDialog {
protected:
	XTBViewer m_viewer;
	twButton m_button;
public:
	XTBViewerTest();
	virtual void command(int);
};


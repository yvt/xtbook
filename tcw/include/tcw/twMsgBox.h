/*
 *  twMsgBox.h
 *  
 *
 *  Created by Nexhawks on 10/01/30.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#pragma once

#include <tcw/twDialog.h>
#include <tcw/twButton.h>
#include <tcw/twLabel.h>

enum twMsgBoxBtns{
	twMBB_ok=0,
	twMBB_okCancel,
	twMBB_yesNo,
	twMBB_yesNoCancel,
	twMBB_retryCancel,
	twMBB_abortIgnoreRetry
};

class twMsgDlg : public twDialog{
protected:
	twButton b1, b2, b3;
	twLabel lbl;
public:
	twMsgDlg(std::wstring, twMsgBoxBtns);
	virtual ~twMsgDlg();
	
	virtual void command(int);
};

twDlgResult twMsgBox(twWnd *, std::wstring, twMsgBoxBtns,
					 std::wstring, bool dim=false);

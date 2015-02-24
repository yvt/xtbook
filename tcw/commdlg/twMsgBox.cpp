/*
 *  twMsgBox.cpp
 *  
 *
 *  Created by Nexhawks on 10/01/30.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#include <tcw/twMsgBox.h>
#include <tcw/twApp.h>

using namespace std;

twMsgDlg::twMsgDlg(std::wstring str, twMsgBoxBtns b){
	int btns;
	if(b==twMBB_ok){
		btns=1;
		b1.setTitle(L"OK");
		b1.setId((int)twDR_ok);
	}else if(b==twMBB_okCancel){
		btns=2;
		b1.setTitle(L"OK");
		b2.setTitle(L"Cancel");
		b1.setId((int)twDR_ok);
		b2.setId((int)twDR_cancel);
	}else if(b==twMBB_yesNo){
		btns=2;
		b1.setTitle(L"Yes");
		b2.setTitle(L"No");
		b1.setId((int)twDR_yes);
		b2.setId((int)twDR_no);
	}else if(b==twMBB_yesNoCancel){
		btns=3;
		b1.setTitle(L"Yes");
		b2.setTitle(L"No");
		b3.setTitle(L"Cancel");
		b1.setId((int)twDR_yes);
		b2.setId((int)twDR_no);
		b3.setId((int)twDR_cancel);
	}else if(b==twMBB_retryCancel){
		btns=2;
		b1.setTitle(L"Retry");
		b2.setTitle(L"Cancel");
		b1.setId((int)twDR_retry);
		b2.setId((int)twDR_cancel);
	}else if(b==twMBB_abortIgnoreRetry){
		btns=3;
		b1.setTitle(L"Abort");
		b2.setTitle(L"Ignore");
		b3.setTitle(L"Retry");
		b1.setId((int)twDR_abort);
		b2.setId((int)twDR_ignore);
		b3.setId((int)twDR_retry);
	}

	int bW, bH;
	const int xSpace=10;
	bH=(getFont()->measure(L"A").h*3)/2;
	bW=bH*4;
	
	int sW, sX, dW, sY;
	sW=bW+(bW+xSpace)*(btns-1);
	dW=sW;
	
	twSize sz;
	std::wstring strWrapped=getFont()->wrapString(str, tw_app->getScrSize().w-xSpace*2-8);
	sz=getFont()->measure(strWrapped);
	dW=max(dW, sz.w)+xSpace*2;
	
	sX=(dW-sW)/2;
	sY=sz.h+xSpace*2;
	
	b1.setParent(this);
	b2.setParent(this);
	b3.setParent(this);
	lbl.setParent(this);
	
	b1.setRect(twRect(sX, sY, bW, bH));
	b2.setRect(twRect(sX+(bW+xSpace), sY, bW, bH));
	b3.setRect(twRect(sX+(bW+xSpace)*2, sY, bW, bH));
	
	lbl.setRect(twRect(xSpace, xSpace, dW-xSpace*2, sz.h));
	lbl.setTitle(strWrapped);
	lbl.show();
	
	if(btns>=1)
		b1.show();
	if(btns>=2)
		b2.show();
	if(btns>=3)
		b3.show();
	
	setRect(twRect(0, 0, dW, sz.h+xSpace*3+bH)+getBorderSize());
	
	setDefault(&b1);
	
}

twMsgDlg::~twMsgDlg(){
	
}

void twMsgDlg::command(int id){
	endDialog((twDlgResult)id);
}

twDlgResult twMsgBox(twWnd *w, std::wstring s, twMsgBoxBtns b, std::wstring t, bool dim){
	twMsgDlg d(s, b);
	d.setTitle(t);
	d.setNeedsDimming(dim);
	return d.showModal(w);
}


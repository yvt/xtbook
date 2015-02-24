/*
 *  twDialog.h
 *  
 *
 *  Created by Nexhawks on 10/01/30.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#pragma once

#include <tcw/twTopWnd.h>
#include <tcw/twInvocation.h>
#include <tcw/twDimmer.h>

enum twDlgResult{
	twDR_invalid=0,
	twDR_none,
	twDR_ok,
	twDR_cancel,
	twDR_yes,
	twDR_no,
	twDR_abort,
	twDR_ignore,
	twDR_retry
};

enum twDlgMode{
	twDM_modal=0,
	twDM_handler
};


class twDialog : public twTopWnd{
protected:
	twDlgResult m_res;
	twDimmer *m_dimmer;
	twDlgMode m_mode;
	twInvocation *m_handler;
	twWnd *m_oldDesktop;
	twDimmerStyle m_dimmerStyle;
	bool m_needsDimming;
	
	virtual twPoint getStartupPos(const twSize&,
								  const twRect& desktop);
public:
	twDialog();
	virtual ~twDialog();
	
	virtual twDlgResult showModal(twWnd *);
	virtual void showWithCompletionHandler(twInvocation *handler);
	
	virtual void endDialog();
	virtual void endDialog(twDlgResult);
	twDlgResult getResult();
	void setNeedsDimming(bool value){
		m_needsDimming=value;
	}
	void setDimmerStyle(twDimmerStyle value){
		m_dimmerStyle=value;
	}
	
	virtual void backgroundTouched();
};

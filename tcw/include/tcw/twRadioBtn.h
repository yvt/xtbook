/*
 *  twRadioBtn.h
 *  
 *
 *  Created by Nexhawks on 10/01/19.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#include <tcw/twWnd.h>

class twRadioBtn : public twWnd{
protected:
	bool m_press, m_hover, m_keyPress;
	bool m_on;
	
	virtual void runCmd();
	
public:
	twRadioBtn();
	virtual ~twRadioBtn();
	
	virtual void clientKeyDown(const std::wstring&);
	virtual void clientKeyUp(const std::wstring&);
	virtual void clientMouseDown(const twPoint&, twMouseButton);
	virtual void clientMouseMove(const twPoint&);
	virtual void clientMouseUp(const twPoint&, twMouseButton);
	virtual void clientMouseEnter(const twPoint&);
	virtual void clientMouseLeave(const twPoint&);
	virtual void clientPaint(const twPaintStruct&);
	virtual bool clientHitTest(const twPoint&) const;
	
	virtual bool getCheckState() const;
	virtual void setCheckState(bool);
};

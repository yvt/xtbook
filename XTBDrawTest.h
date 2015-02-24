//
//  XTBDrawTest.h
//  XTBook
//
//  Created by Nexhawks on 4/25/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include <tcw/twDialog.h>

class XTBDrawTest: public twDialog{
protected:
	twDC *m_offscreen;
	bool m_drag;
	twPoint m_oldMousePos;
public:
	XTBDrawTest();
	~XTBDrawTest();
	
	
	virtual void backgroundTouched();
	
	virtual void clientMouseDown(const twPoint&, twMouseButton);
	virtual void clientMouseMove(const twPoint&);
	virtual void clientMouseUp(const twPoint&, twMouseButton);
	
	virtual void clientPaint(const twPaintStruct&);
};

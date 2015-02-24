/*
 *  XTBTextField.h
 *  XTBook
 *
 *  Created by Nexhawks on 2/13/11.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#pragma once

#include <tcw/twWnd.h>
#include <tcw/twTimer.h>

enum XTBTextFieldInputMode{
	XTBTextFieldInputModeDirect=0,
	XTBTextFieldInputModeHiragana
};


enum XTBTextFieldInputState{
	XTBTextFieldInputStateNone=0,
	XTBTextFieldInputStateInput,
	XTBTextFieldInputStateConvert
};


class XTBTextField: public twWnd{
	friend class XTBTextFieldBlankTimer;
protected:
	int m_scroll;
	int m_cursorIndex;
	int m_markIndex;
	bool m_drag;
	bool m_blanking;
	twTimer *m_blankTimer;
	XTBTextFieldInputMode m_inputMode;
	XTBTextFieldInputState m_inputState;
	int m_inputBegin;
	int m_inputLength;
	std::wstring m_preconvertInputString;
	
	void blank();
public:
	XTBTextField();
	virtual ~XTBTextField();
	
	virtual void setInputMode(XTBTextFieldInputMode);
	XTBTextFieldInputMode getInputMode() const{return m_inputMode;}
	
	virtual void setRect(const twRect&);
	virtual void clientPaint(const twPaintStruct&);
	virtual bool clientHitTest(const twPoint&) const;
	
	virtual void clientKeyDown(const std::wstring&);
	virtual void clientKeyPress(wchar_t);
	
	virtual void clientMouseDown(const twPoint&, twMouseButton);
	virtual void clientMouseMove(const twPoint&);
	virtual void clientMouseUp(const twPoint&, twMouseButton);
	
	virtual void clientEnter();
	virtual void clientLeave();
	
	virtual int positionForCharIndex(int) const;
	virtual int charIndexForPosition(int) const;
	virtual void scrollToPosition(int);
	virtual void scrollToCharIndex(int);
	virtual void scrollToCursor();
	virtual void insertString(const std::wstring&);
	virtual void backSpace();
	virtual void moveLeft();
	virtual void moveRight();
	virtual void selectAll();
	virtual void selectLast();
	virtual void removeAllContents();
	
	virtual void beginInput();
	virtual void acceptInput();
	virtual void setInputString(const std::wstring&);
	virtual std::wstring getInputString() const;
	virtual void doConvert();
	virtual void reverseConvert();
	virtual void inputStringModified();
	
	virtual void resetBlank();
	
};

/*
 *  XTBTextField.cpp
 *  XTBook
 *
 *  Created by Nexhawks on 2/13/11.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#include "XTBTextField.h"
#include <tcw/twBaseSkin.h>
#include <assert.h>
#include <tcw/twEvent.h>
#include "XTBRomajiTable.h"

class XTBTextFieldBlankTimer : public twTimer{
protected:
	XTBTextField *textField;
	virtual void fire(){
		textField->blank();
	}
public:
	XTBTextFieldBlankTimer(XTBTextField *field){
		textField=field;
	}
	virtual ~XTBTextFieldBlankTimer(){
	}
};

XTBTextField::XTBTextField(){
	m_drag=false;
	m_cursorIndex=0;
	m_markIndex=0;
	m_scroll=0;
	m_inputState=XTBTextFieldInputStateNone;
	m_inputMode=XTBTextFieldInputModeDirect;
	m_blankTimer=new XTBTextFieldBlankTimer(this);
	m_blankTimer->setInterval(500);
}
XTBTextField:: ~XTBTextField(){
	delete m_blankTimer;
}


void XTBTextField::setRect(const twRect& rt){
	
	twWnd::setRect(rt);
}
void XTBTextField::clientPaint(const twPaintStruct& p){
	twDC *dc=p.dc;
	
	dc->fillRect(tw_curSkin->getWndColor(), twRect(twPoint(0, 0), p.bound));
	
	int markStart, markEnd;
	if(m_cursorIndex<m_markIndex){
		markStart=m_cursorIndex;
		markEnd=m_markIndex;
	}else{
		markEnd=m_cursorIndex;
		markStart=m_markIndex;
	}
	
	twRect rt=getClientRect();
	const std::wstring& title=getTitle();
	std::wstring str;
	const twFont *font=getFont();
	int lastPos=-m_scroll;
	
	twColor selectColor=tw_curSkin->getSelectionColor();
	twColor selectTextColor=tw_curSkin->getSelectionTextColor();
	twColor textColor=tw_curSkin->getWndTextColor();
	twColor inputLineColor=twRGB(192, 192, 192);
	bool focused=getStatus().focused;
	
	
	
	for(int i=0;i<title.size();i++){
		str=title[i];
		
		twSize siz=font->measure(str);
		
		if(m_inputState!=XTBTextFieldInputStateNone){
			if(i>=m_inputBegin && i<m_inputBegin+m_inputLength)
				dc->drawLine(inputLineColor, twPoint(lastPos, rt.h-1), twPoint(lastPos+siz.w, rt.h-1));
		}
		if(i>=markStart && i<markEnd && focused){
			dc->fillRect(selectColor, twRect(lastPos, 0, siz.w, rt.h));
			font->render(dc, selectTextColor, twPoint(lastPos, 1), str);
		}else{
			font->render(dc, textColor, twPoint(lastPos, 1), str);
		}
		
		lastPos+=siz.w;
	}
	
	if(markStart==markEnd && focused && m_blanking){
		lastPos=positionForCharIndex(markStart);
		lastPos-=m_scroll;
		dc->fillRect(textColor, twRect(lastPos, 0, 1, rt.h));
	}
}
bool XTBTextField::clientHitTest(const twPoint&) const{
	return true;
}

void XTBTextField::clientMouseDown(const twPoint& pt, twMouseButton button){
	if(button==twMB_left){
		acceptInput();
		m_cursorIndex=charIndexForPosition(pt.x+m_scroll);
		m_markIndex=m_cursorIndex;
		m_drag=true;
		resetBlank();
		scrollToCursor();
		invalidate();
	}
	twSetCapture(this);
}
void XTBTextField::clientMouseMove(const twPoint& pt){
	if(m_drag){
		int oldCursorIndex=m_cursorIndex;
		m_cursorIndex=charIndexForPosition(pt.x+m_scroll);
		if(m_cursorIndex!=oldCursorIndex)
			invalidate();
		scrollToCursor();
	}
}
void XTBTextField::clientMouseUp(const twPoint& pt, twMouseButton button){
	if(button==twMB_left){
		m_drag=false;
	}
	twReleaseCapture();
}

void XTBTextField::clientKeyDown(const std::wstring& keyName){
	if(keyName.size()==1){
		if(m_inputState==XTBTextFieldInputStateConvert){
			acceptInput();
		}
		if(keyName==L" "){
			if(m_inputState==XTBTextFieldInputStateInput){
				doConvert();
				sendCmdToParent();
			}else{
				insertString(keyName);
				inputStringModified();
				sendCmdToParent();
			}	
		}else{
			beginInput();
			
			wchar_t ch=keyName[0];
			
			SDLMod modState=SDL_GetModState();
			if(modState&KMOD_CAPS){
#ifdef _WIN32_WCE
				switch(ch){
					case L'q':
						ch=L'1';
						break;
					case L'w':
						ch=L'2';
						break;
					case L'e':
						ch=L'3';
						break;
					case L'r':
						ch=L'4';
						break;
					case L't':
						ch=L'5';
						break;
					case L'y':
						ch=L'6';
						break;
					case L'u':
						ch=L'7';
						break;
					case L'i':
						ch=L'8';
						break;
					case L'o':
						ch=L'9';
						break;
					case L'p':
						ch=L'0';
						break;
					case L'-':
						ch=L' ';
						break;
				}
#endif
			}
			insertString(std::wstring(&ch, 1));
			inputStringModified();
			sendCmdToParent();
		}
	}else if(keyName==L"BackSpace"){
		backSpace();
	}else if(keyName==L"Right" || keyName==L"KeyPad6"){
		moveRight();
	}else if(keyName==L"Left" || keyName==L"KeyPad4"){
		moveLeft();
	}else if(keyName==L"Return"){
		acceptInput();
	}
}
void XTBTextField::clientKeyPress(wchar_t keyChar){
	//printf("0x%x %lc\n", (int)keyChar, keyChar);
}

int XTBTextField::positionForCharIndex(int index) const{
	const std::wstring& title=getTitle();
	if(index<0)
		index=0;
	if(index>title.size())
		index=title.size();
	const twFont *font=getFont();
	std::wstring str=title.substr(0, index);
	twSize size;
	size=font->measure(str);
	return size.w;
}
int XTBTextField::charIndexForPosition(int position) const{
	const std::wstring& title=getTitle();
	std::wstring str;
	const twFont *font=getFont();
	int lastPos=0;
	for(int i=0;i<title.size();i++){
		str=title[i];
		
		twSize siz=font->measure(str);
		if(position<lastPos+siz.w/2)
			return i;
		
		lastPos+=siz.w;
	}
	return title.size();
}
void XTBTextField::scrollToCursor(){
	scrollToCharIndex(m_cursorIndex);
}
void XTBTextField::scrollToPosition(int pos){
	const twRect &rt=getClientRect();
	if(pos<m_scroll){
		m_scroll=pos;
		invalidate();
	}
	if(pos>m_scroll+rt.w-2){
		m_scroll=pos-rt.w+2;
		invalidate();
	}
	
	int textWidth=positionForCharIndex(getTitle().size());
	if(textWidth+2>=rt.w){
		// may need scroll.
		if(m_scroll>textWidth-rt.w+2){
			m_scroll=textWidth-rt.w+2;
			invalidate();
		}
	}else{
		if(m_scroll!=0){
			m_scroll=0;
			invalidate();
		}
	}
	
}
void XTBTextField::scrollToCharIndex(int index){
	int position;
	position=positionForCharIndex(index);
	scrollToPosition(position);
}
void XTBTextField::insertString(const std::wstring& str){
	int markStart, markEnd;
	const std::wstring& title=getTitle();
	if(m_cursorIndex<m_markIndex){
		markStart=m_cursorIndex;
		markEnd=m_markIndex;
	}else{
		markEnd=m_cursorIndex;
		markStart=m_markIndex;
	}
	assert(markStart>=0);
	assert(markStart<=title.size());
	assert(markEnd>=0);
	assert(markEnd<=title.size());
	
	if(m_inputState!=XTBTextFieldInputStateNone){
		m_inputLength+=str.size()-(markEnd-markStart);
	}
	
	std::wstring newTitle;
	newTitle=title.substr(0, markStart)+str+title.substr(markEnd);
	m_cursorIndex=markStart+str.size();
	m_markIndex=m_cursorIndex;
	setTitle(newTitle);
	scrollToCursor();
	resetBlank();
}

void XTBTextField::backSpace(){
	if(m_cursorIndex!=m_markIndex){
		insertString(L"");
		sendCmdToParent();
	}else if(m_inputState==XTBTextFieldInputStateInput){
		if(m_markIndex>m_inputBegin){
			m_markIndex--;
			insertString(L"");
			//m_inputLength--;
			if(m_inputLength==0)
				m_inputState=XTBTextFieldInputStateNone;
			sendCmdToParent();
		}
	}else if(m_inputState==XTBTextFieldInputStateConvert){
		reverseConvert();
		backSpace();
		return;
	}else{
		if(m_markIndex>0){
			m_markIndex--;
			insertString(L"");
			sendCmdToParent();
		}
	}
	resetBlank();
	scrollToCursor();
}

void XTBTextField::moveLeft(){
	if(m_cursorIndex!=m_markIndex){
		if(m_cursorIndex>m_markIndex){
			m_cursorIndex=m_markIndex;
		}else{
			m_markIndex=m_cursorIndex;
		}
		invalidate();
	}else if(m_inputState!=XTBTextFieldInputStateNone){
		if(m_markIndex>m_inputBegin){
			m_markIndex--;
			m_cursorIndex=m_markIndex;
			invalidate();
		}
	}else{
		if(m_markIndex>0){
			m_markIndex--;
			m_cursorIndex=m_markIndex;
			invalidate();
		}
	}
	resetBlank();
	scrollToCursor();
}	

void XTBTextField::moveRight(){
	if(m_cursorIndex!=m_markIndex){
		if(m_cursorIndex<m_markIndex){
			m_cursorIndex=m_markIndex;
		}else{
			m_markIndex=m_cursorIndex;
		}
		invalidate();
	}else if(m_inputState!=XTBTextFieldInputStateNone){
		if(m_markIndex<m_inputBegin+m_inputLength){
			m_markIndex++;
			m_cursorIndex=m_markIndex;
			invalidate();
		}
	}else{
		const std::wstring& title=getTitle();
		if(m_markIndex<title.size()){
			m_markIndex++;
			m_cursorIndex=m_markIndex;
			invalidate();
		}
	}
	resetBlank();
	scrollToCursor();
}	

void XTBTextField::clientEnter(){
	m_blankTimer->addToEvent(tw_event);
	m_blanking=true;
}
void XTBTextField::clientLeave(){
	acceptInput();
	m_blankTimer->invalidate();
}


void XTBTextField::blank(){
	m_blanking=!m_blanking;
	invalidate();
}

void XTBTextField::resetBlank(){
	m_blanking=true;
	if(m_blankTimer->isValid()){
		m_blankTimer->invalidate();
		m_blankTimer->addToEvent(tw_event);
	}
	invalidate();
}

void XTBTextField::setInputMode(XTBTextFieldInputMode newInputMode){
	acceptInput();
	m_inputMode=newInputMode;
}

void XTBTextField::acceptInput(){
	if(m_inputState!=XTBTextFieldInputStateNone){
		m_inputState=XTBTextFieldInputStateNone;
		m_cursorIndex=m_inputBegin+m_inputLength;
		m_markIndex=m_cursorIndex;
	}
	resetBlank();
}

void XTBTextField::beginInput(){
	if(m_inputMode==XTBTextFieldInputModeDirect)
		return;
	if(m_inputState!=XTBTextFieldInputStateInput){
		insertString(L"");
		m_inputBegin=m_cursorIndex;
		m_inputLength=0;
		m_inputState=XTBTextFieldInputStateInput;
	}
}

void XTBTextField::setInputString(const std::wstring& str){
	assert(m_inputState!=XTBTextFieldInputStateNone);
	if(m_inputState!=XTBTextFieldInputStateNone){
		const std::wstring& title=getTitle();
		setTitle(title.substr(0, m_inputBegin)+str+title.substr(m_inputBegin+m_inputLength));
		//m_inputBegin=m_cursorIndex;
		m_inputLength=str.size();
		if(str.size()==0){
			m_inputState=XTBTextFieldInputStateNone;
		}
		invalidate();
	}
}

std::wstring XTBTextField::getInputString() const{
	if(m_inputState==XTBTextFieldInputStateNone)
		return L"";
	else
		return getTitle().substr(m_inputBegin, m_inputLength);
}

void XTBTextField::doConvert(){
	if(m_inputState!=XTBTextFieldInputStateInput)
		return;
	
	if(m_inputMode==XTBTextFieldInputModeHiragana) // there is't any "conversion"
		return;
	
	std::wstring inputString=getInputString();
	m_preconvertInputString=inputString;
	
	m_inputState=XTBTextFieldInputStateConvert;
	invalidate();
}
void XTBTextField::reverseConvert(){
	if(m_inputState!=XTBTextFieldInputStateConvert)
		return;
	setInputString(m_preconvertInputString);
	m_markIndex=m_inputBegin+m_inputLength;
	m_cursorIndex=m_markIndex;
	m_inputState=XTBTextFieldInputStateInput;
	invalidate();
}
void XTBTextField::inputStringModified(){
	if(m_inputState!=XTBTextFieldInputStateInput)
		return;
	
	if(m_inputMode==XTBTextFieldInputModeHiragana){
	
		std::wstring inputString=getInputString();
		int inputPos=m_cursorIndex-m_inputBegin;
		
		assert(inputPos>=0);
		assert(inputPos<=inputString.size());
		
		XTBRomajiTable *romaji=XTBRomajiTable::defaultRomajiTable();
		
		std::wstring newInputString;
		newInputString=romaji->convert(inputString.substr(0, inputPos))+inputString.substr(inputPos);
		inputPos+=newInputString.size()-inputString.size();
		
		m_cursorIndex=inputPos+m_inputBegin;
		m_markIndex=inputPos+m_inputBegin;
		
			
		setInputString(newInputString);
		scrollToCursor();
		
		invalidate();
			
	}
}
void XTBTextField::selectAll(){
	acceptInput();
	m_markIndex=0;
	m_cursorIndex=getTitle().size();
	scrollToCursor();
	resetBlank();
	invalidate();
}
void XTBTextField::selectLast(){
	acceptInput();
	m_markIndex=getTitle().size();
	m_cursorIndex=getTitle().size();
	scrollToCursor();
	resetBlank();
	invalidate();
}
void XTBTextField::removeAllContents(){
	acceptInput();
	setTitle(L"");
	m_markIndex=0;
	m_cursorIndex=0;
	scrollToCursor();
	resetBlank();
	invalidate();
}

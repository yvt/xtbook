//
//  XTBSoftKeyboard.cpp
//  XTBook
//
//  Created by 河田 智明 on 5/28/12.
//  Copyright (c) 2012 Nexhawks. All rights reserved.
//

#include "XTBSoftKeyboard.h"

XTBSoftKeyboard::XTBSoftKeyboard(){
#define AddKey(...) m_keys.push_back(SoftKey(__VA_ARGS__))
	AddKey(L"й", L"Й", 3*0+0, 3, 0, 1);
	AddKey(L"ц", L"Ц", 3*1+0, 3, 0, 1);
	AddKey(L"у", L"У", 3*2+0, 3, 0, 1);
	AddKey(L"к", L"К", 3*3+0, 3, 0, 1);
	AddKey(L"е", L"Е", 3*4+0, 3, 0, 1);
	AddKey(L"н", L"Н", 3*5+0, 3, 0, 1);
	AddKey(L"г", L"Г", 3*6+0, 3, 0, 1);
	AddKey(L"ш", L"Ш", 3*7+0, 3, 0, 1);
	AddKey(L"щ", L"Щ", 3*8+0, 3, 0, 1);
	AddKey(L"з", L"З", 3*9+0, 3, 0, 1);
	AddKey(L"х", L"Х", 3*10+0, 3, 0, 1);
	AddKey(L"ъ", L"Ъ", 3*11+0, 3, 0, 1);
	AddKey(L"ф", L"Ф", 3*0+1, 3, 1, 1);
	AddKey(L"ы", L"Ы", 3*1+1, 3, 1, 1);
	AddKey(L"в", L"В", 3*2+1, 3, 1, 1);
	AddKey(L"а", L"А", 3*3+1, 3, 1, 1);
	AddKey(L"п", L"П", 3*4+1, 3, 1, 1);
	AddKey(L"р", L"Р", 3*5+1, 3, 1, 1);
	AddKey(L"о", L"О", 3*6+1, 3, 1, 1);
	AddKey(L"л", L"Л", 3*7+1, 3, 1, 1);
	AddKey(L"д", L"Д", 3*8+1, 3, 1, 1);
	AddKey(L"ж", L"Ж", 3*9+1, 3, 1, 1);
	AddKey(L"э", L"Э", 3*10+1, 3, 1, 1);
	AddKey(L"ё", L"Ё", 3*11+1, 3, 1, 1);
	AddKey(L"я", L"Я", 3*0+2, 3, 2, 1);
	AddKey(L"ч", L"Ч", 3*1+2, 3, 2, 1);
	AddKey(L"с", L"С", 3*2+2, 3, 2, 1);
	AddKey(L"м", L"М", 3*3+2, 3, 2, 1);
	AddKey(L"и", L"И", 3*4+2, 3, 2, 1);
	AddKey(L"т", L"Т", 3*5+2, 3, 2, 1);
	AddKey(L"ь", L"Ь", 3*6+2, 3, 2, 1);
	AddKey(L"б", L"Б", 3*7+2, 3, 2, 1);
	AddKey(L"ю", L"Ю", 3*8+2, 3, 2, 1);
	
	m_cols=3*12+1;
	m_rows=3;
	m_height=72;
	
	m_shift.setTitle(L"A/a");
	
	layout();
}

void XTBSoftKeyboard::layout(){
	twRect rt=getClientRect();
	rt.h=m_height;
	rt.x+=4;
	rt.w-=8;
	rt.y++;
	
	for(size_t i=0;i<m_keys.size();i++){
		SoftKey& key=m_keys[i];
		if(!key.button){
			key.button=new twButton();
			key.button->setTitle(key.text);
			key.button->setParent(this);
			key.button->setId(i+1);
			
		}
		
		int x1, y1, x2, y2;
		x1=key.col; x2=x1+key.colspan;
		y1=key.row; y2=y1+key.rowspan;
		
		x1=(x1*rt.w)/m_cols;
		x2=(x2*rt.w)/m_cols;
		y1=(y1*rt.h)/m_rows;
		y2=(y2*rt.h)/m_rows;
		
		twRect r(x1, y1, x2-x1, y2-y1);
		r.x+=rt.x; r.y+=rt.y;
		r=r.inflate(-2);
		
		key.button->setRect(r);
		key.button->show();
		
	}
	
	m_shift.setParent(this);
	m_shift.setRect(twRect(rt.w-50, rt.h-22, 48, 20));
	m_shift.show();
	
}

XTBSoftKeyboard::~XTBSoftKeyboard(){
	for(size_t i=0;i<m_keys.size();i++){
		delete m_keys[i].button;
	}
}

void XTBSoftKeyboard::setRect(const twRect &rt){
	twWnd::setRect(rt);
	layout();
}

void XTBSoftKeyboard::command(int id){
	if(id==0) return;
	
	SoftKey& key=m_keys[id-1];
	m_lastKey=key.text;
	if(m_shift.getCheckState()){
		m_lastKey=key.shiftedText;
		m_shift.setCheckState(false);
	}
	
	sendCmdToParent();
}

void XTBSoftKeyboard::clientPaint(const twPaintStruct & ps){
	twDC *dc=ps.dc;
	twRect rt=ps.boundRect;
	dc->fillRect(0xc0c0c0, rt);
	dc->drawLine(0xa0a0a0, twPoint(rt.x,rt.y),
				 twPoint(rt.x+rt.w,rt.y));
}


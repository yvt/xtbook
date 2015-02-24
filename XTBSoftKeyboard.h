//
//  XTBSoftKeyboard.h
//  XTBook
//
//  Created by 河田 智明 on 5/28/12.
//  Copyright (c) 2012 Nexhawks. All rights reserved.
//

#pragma once

#include <tcw/twWnd.h>
#include <vector>
#include <tcw/twButton.h>
#include <tcw/twCheckBox.h>

class XTBSoftKeyboard: public twWnd{
	struct SoftKey{
		int col, row;
		int colspan, rowspan;
		std::wstring text, shiftedText;
		twButton *button;
		
		SoftKey(){}
		SoftKey(const std::wstring& _text,
				const std::wstring& _stext,
				int _col, int _colspan,
				int _row, int _rowspan):
		col(_col), row(_row),
		colspan(_colspan), rowspan(_rowspan),
		text(_text), shiftedText(_stext),
		button(NULL){
			
		}
	};
	
	twCheckBox m_shift;
	std::vector<SoftKey> m_keys;
	int m_cols;
	int m_rows;
	int m_height;
	
	void layout();
	
	std::wstring m_lastKey;
	
public:
	XTBSoftKeyboard();
	virtual ~XTBSoftKeyboard();
	
	virtual void setRect(const twRect&);
	virtual void command(int);
	
	virtual void clientPaint(const twPaintStruct&);
	
	const std::wstring& lastKey() const{
		return m_lastKey;
	}
};


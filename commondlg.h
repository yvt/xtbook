//
//  commondlg.h
//  XTBook
//
//  Created by Nexhawks on 4/21/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#pragma once

#include <tcw/twInvocation.h>
#include <string>

void XTBInvokeWithProgressOverlay(twInvocation *);
void XTBSetProgressText(const std::wstring&);
void XTBSetProgressPercentage(float);

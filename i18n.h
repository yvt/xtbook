//
//  i18n.h
//  XTBook
//
//  Created by Nexhawks on 4/17/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#pragma once

#include <string>
#include <vector>
#include "XTBStringTable.h"

#define XTBDefaultLocale	L"com.nexhawks.XTBook.langs.en"

const std::wstring& XTBLocalizedString(const std::wstring&);

std::vector<XTBStringTableInfo> XTBScanStringTables();

void XTBSetLocale(const std::wstring&);
const std::wstring& XTBGetLocale();

std::wstring XTBShortStringForDate(time_t);

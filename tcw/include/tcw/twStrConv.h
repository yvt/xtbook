/*
 *  twStrConv.h
 *  
 *
 *  Created by Nexhawks on 10/01/13.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#pragma once

#include <wchar.h>
#include <string>

void twM2W(wchar_t *, const char *);
void twW2M(char *, const wchar_t *);

std::wstring twM2W(const std::string&);
std::string twW2M(const std::wstring&);

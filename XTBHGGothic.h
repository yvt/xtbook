/*
 *  XTBHGGothic.h
 *  XTBook
 *
 *  Created by Nexhawks on 2/11/11.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#pragma once
#define XTB_STRIP_XTBHGGOTHIC	1

#ifdef __cplusplus

#include "XTBFont1Bit.h"



#if XTB_STRIP_XTBHGGOTHIC
#else

class XTBHGGothic : public XTBFont1Bit{
public:
	XTBHGGothic();
};

#endif

#endif
/*
 *  XTBHGGothic.cpp
 *  XTBook
 *
 *  Created by Nexhawks on 2/11/11.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#include "XTBHGGothic.h"

#if XTB_STRIP_XTBHGGOTHIC
#else

extern "C"{
	extern unsigned char XTBHGGothic[];
	extern unsigned int XTBHGGothicIndexMap[];
	extern unsigned char XTBHGGothicWidthMap[];
	
};

XTBHGGothic::XTBHGGothic():XTBFont1Bit(::XTBHGGothic, ::XTBHGGothicIndexMap, ::XTBHGGothicWidthMap) {
	
}

#endif

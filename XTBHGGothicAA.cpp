/*
 *  XTBHGGothicAA.cpp
 *  XTBook
 *
 *  Created by Nexhawks on 2/11/11.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#include "XTBHGGothicAA.h"


extern "C"{
	extern unsigned char XTBHGGothicAA[];
	extern unsigned int XTBHGGothicAAIndexMap[];
	extern unsigned char XTBHGGothicAAWidthMap[];
	
};

XTBHGGothicAA::XTBHGGothicAA():XTBFont4Bit(::XTBHGGothicAA, ::XTBHGGothicAAIndexMap, ::XTBHGGothicAAWidthMap) {
	topMargin=1;
	actualLineHeight=11;
}


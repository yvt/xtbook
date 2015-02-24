//
//  XTBImageLoader.h
//  XTBook
//
//  Created by Kawada Tomoaki on 5/28/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#pragma once

#include <tcw/twDC.h>
#include "XTBStream.h"

class XTBImageLoader{
	XTBImageLoader(){}
public:
	static twDC *dcForStream(XTBStream *);
};


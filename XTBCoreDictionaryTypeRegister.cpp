//
//  XTBCoreDictionaryTypeRegister.cpp
//  XTBook
//
//  Created by Kawada Tomoaki on 5/23/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "XTBCoreDictionaryTypeRegister.h"
#include "XTBDictionaryType.h"
#include "XTBSampleDictionaryType.h"
#include "XTBWikiplexus.h"
#include "XTBImageComplex.h"

void XTBRegisterCoreDictionaryTypes(){
	XTBDictionaryType::registerDictionaryType(new XTBSampleDictionaryType());
	XTBDictionaryType::registerDictionaryType(new XTBWikiplexus());
	XTBDictionaryType::registerDictionaryType(new XTBImageComplex());

}


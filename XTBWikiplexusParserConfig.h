//
//  XTBWikiplexusParserConfig.h
//  XTBook
//
//  Created by Kawada Tomoaki on 7/7/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#pragma once

#include "TWiki/TWParserConfig.h"

class XTBWikiplexusDictionary;

class XTBWikiplexusParserConfig: public TWParserConfig{
public:
	XTBWikiplexusParserConfig(XTBWikiplexusDictionary *dic);
	virtual ~XTBWikiplexusParserConfig();
};
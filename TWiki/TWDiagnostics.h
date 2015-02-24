//
//  TWDiagnostics.h
//  XTBook
//
//  Created by Kawada Tomoaki on 7/27/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#pragma once

class TWNode;

class TWDiagnostics{
private:
	TWDiagnostics(){}
	~TWDiagnostics(){}
	
	static void dumpNodeMemoryUsageIndented(TWNode *, int);
public:
	static size_t totalMemoryUsageForNode(TWNode *);
	static size_t memoryUsageForNode(TWNode *);
	static void dumpNodeMemoryUsage(TWNode *);
};


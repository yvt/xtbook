//
//  TWDiagnostics.cpp
//  XTBook
//
//  Created by Kawada Tomoaki on 7/27/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "TWDiagnostics.h"
#include "TWUtils.h"
#include "TWNode.h"
#include <typeinfo>
#include "TWHTMLText.h"
#include "TWRenderText.h"

void TWDiagnostics::dumpNodeMemoryUsageIndented(TWNode *node, int indent){
	std::string s;
	for(int i=0;i<indent*2;i++)
		s+=' ';
	s+=TWUtils::w2m(node->name());
	if(node->name().empty())
		s+="(anon)";
	
	s.resize(20, ' ');
	
	const std::type_info& ti=typeid(*node);
	s+=ti.name();
	
	s.resize(55, ' ');
	
	char buf[256];
	sprintf(buf, "%d", (int)memoryUsageForNode(node));
	s+=buf;
	
	s.resize(60, ' ');
	
	sprintf(buf, "%d", (int)totalMemoryUsageForNode(node));
	s+=buf;
	
	puts(s.c_str());
	for(TWNode *child=node->firstChild();
		child;child=child->nextSibling()){
		dumpNodeMemoryUsageIndented(child, indent+1);
	}
}

size_t TWDiagnostics::memoryUsageForNode(TWNode *node){
	size_t s=sizeof(*node);

	if(TWHTMLElement*obj=dynamic_cast<TWHTMLElement *>(node)){
		s=sizeof(*obj);
	}
	
	if(TWRenderObject*obj=dynamic_cast<TWRenderObject *>(node)){
		s=sizeof(*obj);
	}

	s+=node->name().capacity()*sizeof(wchar_t)+1;

	if(TWHTMLElement*obj=dynamic_cast<TWHTMLElement *>(node)){
		s+=obj->styleAttr().capacity()*sizeof(wchar_t);
	}
	
	if(TWHTMLText *txt=dynamic_cast<TWHTMLText *>(node)){
		s+=txt->innerText().size()*sizeof(wchar_t);
	}
	
	if(TWRenderText *txt=dynamic_cast<TWRenderText *>(node)){
		s+=txt->text().size()*sizeof(wchar_t);
		s+=txt->segments()*6;
	}
	
	return s;
}
size_t TWDiagnostics::totalMemoryUsageForNode(TWNode *node){
	size_t s=memoryUsageForNode(node);
	for(TWNode *child=node->firstChild();
		child;child=child->nextSibling()){
		s+=totalMemoryUsageForNode(child);
	}
	return s;
}

void TWDiagnostics::dumpNodeMemoryUsage(TWNode *node){
	dumpNodeMemoryUsageIndented(node, 0);
}
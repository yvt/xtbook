//
//  TWRandomSelectionExtension.cpp
//  XTBook
//
//  Created by Kawada Tomoaki on 8/1/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "TWRandomSelectionExtension.h"
#include "TWPreprocessorFrame.h"
#include "TWPreprocessorNode.h"
#include "TWStringSplitter.h"
#include "TWNumber.h"
#include "TWParser.h"

#include <stdlib.h>

struct SelectionOption{
	double weight;
	size_t startPos;
	size_t length;
};

std::wstring TWRandomSelectionExtension::runHook
(TWParser *parser,
 TWPreprocessorFrame *frame,
 const std::wstring& attr,
 const std::wstring& contents){
	// scan options.
	std::vector<SelectionOption> options;
	size_t pos=0, nextPos, pos2, pos3;
	double totalWeight=0.;
	
	while(pos<contents.size()){
		nextPos=contents.find(L"<option", pos);
		if(nextPos==std::wstring::npos){
			// no more options...
			break;
		}
		pos=nextPos+7;
		
		nextPos=contents.find(L'>', pos);
		if(nextPos==std::wstring::npos){
			// not closed.
			// no more options.
			break;
		}
		
		SelectionOption opt;
		
		std::wstring attr=contents.substr(pos, nextPos-pos);
		
		pos2=attr.find(L"weight=");
		if(pos2==std::wstring::npos ||
		   (attr[pos2+7]!=L'"' &&
		   attr[pos2+7]!=L'\'')){
			// no weight.
			opt.weight=1.;
		}else{
			pos2+=7;
			wchar_t openChar=attr[pos2];
			pos2++;
			pos3=attr.find(openChar, pos2);
			if(pos3==std::wstring::npos){
				// not closed.
				break;
			}
			attr=attr.substr(pos2, pos3-pos2);
			
			TWNumber num(attr);
			
			opt.weight=num.floatValue();
		}
		
		pos=nextPos+1;
		
		// now, find the contents.
		
		opt.startPos=pos;
		
		nextPos=contents.find(L"</option", pos);
		if(nextPos==std::wstring::npos){
			nextPos=contents.size();
		}
		
		opt.length=nextPos-pos;
		totalWeight+=opt.weight;
		
		options.push_back(opt);
		pos=nextPos;
	}
	
	double r=randomFloat(totalWeight);
	
	for(size_t i=0;i<options.size();i++){
		r-=options[i].weight;
		if(r<=0.){
			SelectionOption& o=options[i];
			return parser->recursiveTagParse
			(contents.substr(o.startPos, o.length), frame);
		}
	}
	
	return parser->recursiveTagParse
	(contents, frame);
}

double TWRandomSelectionExtension::randomFloat(double v) const{
	unsigned long r=rand();
	return (double)r/(double)RAND_MAX*v;
}

//
//  TWTitlePartsParserFunction.cpp
//  XTBook
//
//  Created by 河田 智明 on 9/23/11.
//  Copyright (c) 2011 Nexhawks. All rights reserved.
//

#include "TWTitlePartsParserFunction.h"
#include "TWUtils.h"

std::wstring TWTitlePartsParserFunction::execute(const std::wstring& arg0, 
												 const std::vector<std::wstring>& args, TWPreprocessorFrame *){
	if(args.empty())
		return TWUtils::displayTitleFor(arg0);
	
	std::vector<std::wstring> strs=TWUtils::split(arg0, L"/");
	std::wstring outString;
	
	int segmentsCount=TWUtils::intValueFromString(args[0]);
	int startSegment;
	if(segmentsCount==0)
		segmentsCount=25;
	if(args.size()>=2)
		startSegment=TWUtils::intValueFromString(args[1]);
	else
		startSegment=1;
	if(startSegment==0)
		startSegment=1;
	else if(startSegment<0)
		startSegment=strs.size()+1+startSegment;
	
	startSegment--;
	
	if(segmentsCount<0)
		segmentsCount=strs.size()-startSegment+segmentsCount;
	
	int endSegment=startSegment+segmentsCount;
	
	if(startSegment<0)
		startSegment=0;
	if(endSegment>(int)strs.size())
		endSegment=(int)strs.size();
	
	if(startSegment>=endSegment)
		return std::wstring();
	
	outString=strs[startSegment];
	for(int i=startSegment+1;i<endSegment;i++){
		outString+=L'/';
		outString+=strs[i];
	}
	
	return TWUtils::displayTitleFor(outString);
	
}

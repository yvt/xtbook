/*
 * OSXSpecific.m
 *  XTBook
 *
 *  Created by Nexhawks on 12/22/10.
 *  Copyright 2011 Nexhawks. All rights reserved.
 */

#import "OSXSpecific.h"
using namespace std;

void showErrorAlert(const char *str){
	NSAlert *alert=[NSAlert alertWithMessageText:@"Error while initializing XTBook."
								   defaultButton:@"Exit" 
								 alternateButton:nil 
									 otherButton:nil 
					   informativeTextWithFormat:@"%@", [NSString stringWithUTF8String:str]];
	[alert runModal];
}

void showOpenPanel(const char *ext, char *fnOut){
	NSOpenPanel *panel;
	NSAutoreleasePool *pool=[[NSAutoreleasePool alloc] init];
	panel=[NSOpenPanel openPanel];
	[panel setAllowedFileTypes:[NSArray arrayWithObject:[NSString stringWithUTF8String:ext]]];
	[panel setAllowsMultipleSelection:NO];
	[panel setCanChooseDirectories:NO];
	[panel setCanChooseFiles:YES];
	if([panel runModal]==NSFileHandlingPanelOKButton){
		NSURL *url=[panel URL];
		NSString *path=[url path];
		strcpy(fnOut, [path UTF8String]);
	}else{
		fnOut[0]=0;
	}
	[pool drain];
}
/*
 
  // DEPRECATED.
std::vector<std::string> pathsInBundleResourceDirectory(const char *dirName, const char *fileType){
	NSArray *paths;
	NSAutoreleasePool *pool=[[NSAutoreleasePool alloc] init];
	paths=[[NSBundle mainBundle] pathsForResourcesOfType:[NSString stringWithUTF8String:fileType] 
											 inDirectory:[NSString stringWithUTF8String:dirName]];
	vector<string>  ret;
	for(NSString *path in paths){
		std::string str;
		str=[path UTF8String];
		ret.push_back(str);
	}
	[pool drain];
	return ret;
}
*/

std::string appDirOSX(){
	NSAutoreleasePool *pool=[[NSAutoreleasePool alloc] init];
	NSString *str;
	str=[[NSBundle mainBundle] resourcePath];
	
	string ret=[str UTF8String];
	
	[pool drain];
	
	return ret;
}


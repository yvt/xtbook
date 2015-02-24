/*
 *  OSXSpecific.h
 *  XTBook
 *
 *  Created by Nexhawks on 12/22/10.
 *  Copyright 2011 Nexhawks. All rights reserved.
 */

#ifndef _OSXSPECIFIC_H
#define _OSXSPECIFIC_H

#ifdef __OBJC__
#import <Cocoa/Cocoa.h>
#endif

#include <vector>
#include <string>

#ifdef __cplusplus
extern "C"{
#endif
	
	/** display the message box that says the application failed to launch. */
	void showErrorAlert(const char *str);
	
	/** show the file chooser. */
	void showOpenPanel(const char *ext, char *fnOut);
	
	// DEPRECATED.
	//std::vector<std::string> pathsInBundleResourceDirectory(const char *dirName, const char *fileType);
	
	/** get the resource directory of the application bundle. */
	std::string appDirOSX();;
	
#ifdef __cplusplus
};
#endif

#endif

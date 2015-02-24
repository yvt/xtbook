//
//  TWViewerImageManager.h
//  XTBook
//
//  Created by Kawada Tomoaki on 7/29/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#pragma once

#include <map>
#include "XTBImageArticle.h"
#include <string>

struct TWViewerImage{
	std::wstring url;
	XTBImageArticle *article;
	int usageCount;
};
typedef std::map<std::wstring, TWViewerImage> TWViewerImageMap;
class TWViewerImageManager{
	std::map<std::wstring, TWViewerImage> m_images;
	void evict(TWViewerImageMap::iterator);
public:
	TWViewerImageManager();
	~TWViewerImageManager();
	
	XTBImageArticle *imageArticleForUrl(const std::wstring&);
	
	void resetUsageCount();
	
	void evictNotUsedImages();
};

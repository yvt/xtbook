//
//  TWViewerImageManager.cpp
//  XTBook
//
//  Created by Kawada Tomoakion 7/29/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "TWViewerImageManager.h"
#include "XTBDictionary.h"
#include <tcw/twStrConv.h>

TWViewerImageManager::TWViewerImageManager(){
	
}

TWViewerImageManager::~TWViewerImageManager(){
	for(TWViewerImageMap::iterator it=m_images.begin();
		it!=m_images.end();it++){
		delete it->second.article;
	}
}

void TWViewerImageManager::evict(TWViewerImageMap::iterator it){
	TWViewerImage& image=it->second;
	delete image.article;
	m_images.erase(it);
}

XTBImageArticle *TWViewerImageManager::imageArticleForUrl(const std::wstring &url){
	TWViewerImageMap::iterator it;
	
	// url without scheme isn't valid.
	if(url.find(L':')==std::wstring::npos)
		return NULL;
	
	it=m_images.find(url);
	//XTBLog("Requesting image article for \"%s\"...", twW2M(url).c_str());
	
	if(it==m_images.end()){
		TWViewerImage image;
		image.url=url;
		image.usageCount=1;
		
		XTBDictionary *dic;
		std::wstring scheme=url.substr(0, url.find(L':'));
		std::wstring name=url.substr(url.find(L':')+1);
		
		// receive dictionary.
		try{
			dic=XTBDictionary::dictionaryWithName(scheme);
		}catch(...){
			return NULL;
		}
		
		// make sure article is available.
		if(!dic->hasDocumentForName(name))
			return NULL;
		
		// receive article.
		XTBDocument *doc;
		doc=dic->documentForName(name);
		
		image.article=dynamic_cast<XTBImageArticle *>(doc);
		
		if(!image.article){
			// not image.
			delete doc;
			return NULL;
		}
		
		m_images[url]=image;
		
		//XTBLog("done.");
		return image.article;
	}else{
		TWViewerImage& image=it->second;
		image.usageCount++;
		return image.article;
	}
}

void TWViewerImageManager::resetUsageCount(){
	for(TWViewerImageMap::iterator it=m_images.begin();
		it!=m_images.end();it++){
		it->second.usageCount=0;
	}
}

void TWViewerImageManager::evictNotUsedImages(){
	std::vector<TWViewerImageMap::iterator> notUsedIters;
	for(TWViewerImageMap::iterator it=m_images.begin();
		it!=m_images.end();it++){
		if(it->second.usageCount==0)
			notUsedIters.push_back(it);
	}
	for(std::vector<TWViewerImageMap::iterator>::iterator it
		=notUsedIters.begin();it!=notUsedIters.end();it++){
		evict(*it);
	}
}

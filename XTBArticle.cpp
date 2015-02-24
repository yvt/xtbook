//
//  XTBArticle.cpp
//  XTBook
//
//  Created by Kawada Tomoaki on 5/22/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "XTBArticle.h"
#include "TWiki/TWUtils.h"
#include "XTBException.h"

XTBArticle::XTBArticle(){
	
}

XTBArticle::~XTBArticle(){
	
}

int XTBArticle::indicesCount() const{
	return 0;
}

XTBArticleIndexItem XTBArticle::indexAt(int) const{
	XTBThrowException(L"XTBNotImplementedException", NULL);
}

int XTBArticle::pageForFragmentId(std::wstring const &) const{
	return 1;
}

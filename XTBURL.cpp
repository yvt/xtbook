//
//  XTBURL.cpp
//  XTBook
//
//  Created by Kawada Tomoaki on 5/27/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "XTBURL.h"
#include "XTBDictionary.h"
#include <assert.h>

XTBURL::XTBURL(){
	
}

XTBURL::XTBURL(const std::wstring& str){
	std::wstring::size_type pos;
	if((pos=str.find(XTBURLSeparatorChar))==std::wstring::npos){
		// scheme=L""
		articleName=str;
	}else{
		scheme=str.substr(0, pos);
		articleName=str.substr(pos+1);
	}
}



bool XTBURL::isReachable() const{
	switch(type()){
		case XTBURLTypeSearch:
		case XTBURLTypeHome:
			return true;
		case XTBURLTypeDocument:
			try{
				XTBDictionary *dic=XTBDictionary::dictionaryWithName(scheme);
				return dic->hasDocumentForName(articleName);
			}catch(...){
				return false;
			}
		case XTBURLTypeSystemCall:
			return true;
	}
}

XTBDocument *XTBURL::document() const{
	assert(type()==XTBURLTypeDocument);
	
	XTBDictionary *dic=XTBDictionary::dictionaryWithName(scheme);
	return dic->documentForName(articleName);
}

XTBURLType XTBURL::type() const{
	if(scheme.empty()){
		if(articleName.empty())
			return XTBURLTypeHome;
		else
			return XTBURLTypeSearch;
	}else if(scheme==L"xtbook"){
		return XTBURLTypeSystemCall;
	}else{
		return XTBURLTypeDocument;
	}
}

bool XTBURL::operator ==(const XTBURL& url) const{
	return scheme==url.scheme && articleName==url.articleName;
}

bool XTBURL::operator !=(const XTBURL& url) const{
	return scheme!=url.scheme || articleName!=url.articleName;
}

std::wstring XTBURL::urlString() const{
	if(scheme.empty())
		return articleName;
	else
		return scheme+XTBURLSeparatorChar+articleName;
}


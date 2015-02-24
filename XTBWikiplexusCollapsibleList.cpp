//
//  XTBWikiplexusCollapsibleList.cpp
//  XTBook
//
//  Created by 河田 智明 on 8/4/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "XTBWikiplexusCollapsibleList.h"


#include "XTBWikiplexusSidebar.h"
#include "TWiki/TWUtils.h"

#include "i18n.h"

static int g_listId=1;
static const std::wstring g_emptyString;

static const std::wstring& ArgOrEmpty
(const std::map<std::wstring, std::wstring>& mp,
 const std::wstring& key){
	std::map<std::wstring, std::wstring>::const_iterator it;
	it=mp.find(key);
	if(it==mp.end())
		return g_emptyString;
	else
		return it->second;
}

static const std::wstring& ArgOrEmpty
(const std::map<std::wstring, std::wstring>& mp,
 const std::wstring& key1,
 const std::wstring& key2){
	std::map<std::wstring, std::wstring>::const_iterator it;
	it=mp.find(key1);
	if(it==mp.end()){
		it=mp.find(key2);
		if(it==mp.end())
			return g_emptyString;
		else
			return it->second;
	}else
		return it->second;
}

std::wstring XTBWikiplexusCollapsibleList::execute
(const std::wstring &arg0, 
 const std::vector<std::wstring> &args, 
 TWPreprocessorFrame *){
	// create arg map.
	std::map<std::wstring, std::wstring> allArgs;
	for(size_t i=0;i<args.size();i++){
		const std::wstring& s=args[i];
		size_t pos=s.find(L'=');
		if(pos==std::wstring::npos){
			// skip.
			continue;
		}
		
		if(s.find_first_not_of(L" \n\t", pos+1)!=std::wstring::npos)
		allArgs[TWUtils::trimString(s.substr(0, pos))]=
		TWUtils::trimString(s.substr(pos+1));
	}
	
	std::wstring outHtml;
	struct ArgProxy{
		const std::map<std::wstring, std::wstring>& args;
		ArgProxy(const std::map<std::wstring, std::wstring>& a):
		args(a){}
		const std::wstring& operator [](const std::wstring& s) const{
			return ArgOrEmpty(args, s);
		}
		const std::wstring& operator()(const std::wstring& key1,
									   const std::wstring& key2) const{
			return ArgOrEmpty(args, key1, key2);
		}
	} arg(allArgs);
	
	bool expanded=!arg[L"expand"].empty();
	
	outHtml+=L"<div style=\"";
	if(arg[L"framestyle"].empty() && arg[L"framestyle"].empty()){
		outHtml+=L"border:none;padding:0;";
	}else{
		outHtml+=arg[L"frame_style"];
		outHtml+=arg[L"framestyle"];
	}
	outHtml+=L"\">";
	
	// use table to show [Display] in the right side.
	
	outHtml+=L"<table><tr>";
	
	outHtml+=L"<td style=\"font-size:105%; ";
	if(arg[L"title_style"].empty() && arg[L"titlestyle"].empty()){
		outHtml+=L"background:transparent; text-align:left;";
	}else{
		outHtml+=arg[L"title_style"];
		outHtml+=arg[L"titlestyle"];
	}
	outHtml+=L"\">";
	
	if(arg[L"title"].empty()){
		outHtml+=XTBLocalizedString(L"XTBWikiplexusList");
	}else{
		outHtml+=arg[L"title"];
	}
	outHtml+=L"</td>";
	
	std::wstring listId=L"colList"+TWUtils::stringWithIntValue(g_listId);
	g_listId++;
	
	if(expanded){
		outHtml+=L"</tr></table>";
	}else{
		outHtml+=L"<td style=\"text-align: right;\">";
		outHtml+=L"<a id=\""+listId+L"\" href=\"xtbook:footnote#";
		outHtml+=listId;
		outHtml+=L"\">";
		outHtml+=XTBLocalizedString(L"XTBWikiplexusShow");
		outHtml+=L"<div>";
	}
	
	// no support for list-style and so on in TWiki...
	
	outHtml+=L"<ul>";
	
	std::wstring argName=L"0";
	
	for(int i=1;i<=50;i++){
		if(i<10){
			argName[0]=L'0'+i;
		}else{
			if(i==10)
				argName+=L'0';
			argName[0]=L'0'+i/10;
			argName[1]=L'0'+i%10;
		}
		
		const std::wstring& item=arg[argName];
		if(item.empty())
			break;
		
		outHtml+=L"<li>";
		outHtml+=item;
		outHtml+=L"</li>";
	}
	
	outHtml+=L"</ul>";
	
	if(expanded){
		
	}else{
		outHtml+=L"</div></a></td></tr></table>";
	}
	
	outHtml+=L"</div>";
	
	return outHtml;
}
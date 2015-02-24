//
//  XTBWikiplexusSidebar.cpp
//  XTBook
//
//  Created by 河田 智明 on 8/4/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "XTBWikiplexusSidebar.h"
#include "TWiki/TWUtils.h"

// implemented this in c++:
// http://ja.wikipedia.org/w/index.php?title=Template:Infobox&action=edit

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

std::wstring XTBWikiplexusSidebar::execute
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
		(s.substr(pos+1));
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
	
	if(arg[L"wraplinks"]!=L"true")
		outHtml+=L"<table class=\"infobox nowraplinks ";
	else
		outHtml+=L"<table class=\"infobox ";
	outHtml+=TWUtils::trimString(arg[L"class"]);
	outHtml+=' ';
	outHtml+=TWUtils::trimString(arg[L"bodyclass"]);
	outHtml+=' ';
	outHtml+=L"\" cellspacing=\"5\"";
	outHtml+=L"style=\"width: 180px; float:right; clear:right; color:black; background:#f9f9f9; border:1px solid #aaa;";
	outHtml+=L"margin:0 0 1.0em 1.0em; padding:0.2em; border-spacing:0.4em 0; text-align:center; line-height:1.4em; font-size:88%;";
	outHtml+=TWUtils::trimString(arg[L"style"]);
	outHtml+=TWUtils::trimString(arg[L"bodystyle"]);
	outHtml+=L"\" cellpadding=\"";
	outHtml+=arg[L"cellpadding"];
	outHtml+=L"\">";
	
	{
		const std::wstring& title=arg[L"outertitle"];
		const std::wstring& style=arg[L"outertitlestyle"];
		const std::wstring& className=arg[L"outertitleclass"];
		outHtml+=L"<caption class=\"";
		outHtml+=TWUtils::trimString(className);
		outHtml+=L"\" style=\"padding-bottom:0.2em; font-size:125%; line-height:1.15em; font-weight:bold; ";
		outHtml+=TWUtils::trimString(style);
		outHtml+=L"\">";
		outHtml+=title;
		outHtml+=L"</caption>";
	}
	
	
	{
		const std::wstring& title=arg[L"topimage"];
		const std::wstring& style=arg[L"topimagestyle"];
		const std::wstring& className=arg[L"topimageclass"];
		const std::wstring& caption=arg[L"topcaption"];
		const std::wstring& captionStyle=arg[L"topcaptionstyle"];
		outHtml+=L"<tr><td class=\"";
		outHtml+=TWUtils::trimString(className);
		outHtml+=L"\" style=\"padding:0.4em 0; ";
		outHtml+=TWUtils::trimString(style);
		outHtml+=L"\">";
		outHtml+=title;
		if(!caption.empty()){
			outHtml+=L"<div style=\"padding-top:0.2em; font-size:85%; line-height:1.2em; ";
			outHtml+=TWUtils::trimString(captionStyle);
			outHtml+=L"\">";
			outHtml+=caption;
			outHtml+=L"</div>";
		}
		outHtml+=L"</td></tr>";
	}
	
	{
		const std::wstring& title=arg[L"pretitle"];
		const std::wstring& style=arg[L"pretitlestyle"];
		outHtml+=L"<tr><td style=\"padding-top: 0.4em; font-size:85%; line-height:1.2em;  ";
		outHtml+=TWUtils::trimString(style);
		outHtml+=L"\">";
		outHtml+=title;
		outHtml+=L"</td></tr>";
	}
	
	{
		const std::wstring& title=arg[L"title"];
		const std::wstring& style=arg[L"titlestyle"];
		const std::wstring& className=arg[L"titleclass"];
		outHtml+=L"<tr><th class=\"";
		outHtml+=TWUtils::trimString(className);
		outHtml+=L"\" style=\"padding:0.2em 0.4em 0.2em; font-size:145%; line-height:1.15em; font-weight:bold;  ";
		if(!arg[L"pretitle"].empty()){
			outHtml+=L"padding-top: 0;";
		}
		outHtml+=TWUtils::trimString(style);
		outHtml+=L"\">";
		outHtml+=title;
		outHtml+=L"</th></tr>";
	}
	
	{
		const std::wstring& title=arg[L"image"];
		const std::wstring& style=arg[L"imagestyle"];
		const std::wstring& className=arg[L"imageclass"];
		const std::wstring& caption=arg[L"caption"];
		const std::wstring& captionStyle=arg[L"captionstyle"];
		outHtml+=L"<tr><td class=\"";
		outHtml+=TWUtils::trimString(className);
		outHtml+=L"\" style=\"padding:0.2em 0 0.4em; ";
		outHtml+=TWUtils::trimString(style);
		outHtml+=L"\">";
		outHtml+=title;
		if(!caption.empty()){
			outHtml+=L"<div style=\"padding-top:0.2em; font-size:85%; line-height:1.2em;  ";
			outHtml+=TWUtils::trimString(captionStyle);
			outHtml+=L"\">";
			outHtml+=caption;
			outHtml+=L"</div>";
		}
		outHtml+=L"</td></tr>";
	}
	
	{
		const std::wstring& title=arg[L"above"];
		const std::wstring& style=arg[L"abovestyle"];
		outHtml+=L"<tr><td style=\"padding:0.3em 0.4em 0.3em; font-weight:bold; ";
		outHtml+=TWUtils::trimString(style);
		outHtml+=L"\">";
		outHtml+=title;
		outHtml+=L"</td></tr>";
	}
	
	// headings...
	
	{
		std::wstring commonHeadingStyle=TWUtils::trimString(arg[L"headingstyle"]);
		std::wstring commonContentStyle=TWUtils::trimString(arg[L"contentstyle"]);
		
		std::wstring headingStyleArgName=L"headingstyle0";
		std::wstring contentStyleArgName=L"contentstyle0";
		std::wstring headingArgName=L"heading0";
		std::wstring contentArgName=L"content0";
		
		for(int i=1;i<=35;i++){
			if(i<10){
				headingArgName[7]=L'0'+i;
				contentArgName[7]=L'0'+i;
				headingStyleArgName[12]=L'0'+i;
				contentStyleArgName[12]=L'0'+i;
			}else{
				if(i==10){
					headingArgName+='0';
					contentArgName+='0';
					headingStyleArgName+='0';
					contentStyleArgName+='0';
				}
				
				headingArgName[7]=L'0'+i/10;
				contentArgName[7]=L'0'+i/10;
				headingStyleArgName[12]=L'0'+i/10;
				contentStyleArgName[12]=L'0'+i/10;
				
				headingArgName[8]=L'0'+i%10;
				contentArgName[8]=L'0'+i%10;
				headingStyleArgName[13]=L'0'+i%10;
				contentStyleArgName[13]=L'0'+i%10;
			}
			
			const std::wstring& heading=arg[headingArgName];
			const std::wstring& content=arg[contentArgName];
			
			if(heading.empty() && content.empty())
				goto noMoreContents;
			
			const std::wstring& headingStyle=arg[headingStyleArgName];
			const std::wstring& contentStyle=arg[contentStyleArgName];
			
			if(!heading.empty()){
				outHtml+=L"<tr><td style=\"padding-top:0.2em; font-weight:bold; ";
				outHtml+=commonHeadingStyle;
				outHtml+=TWUtils::trimString(headingStyle);
				outHtml+=L"\">";
				outHtml+=heading;
				outHtml+=L"</td></tr>";
			}
			
			if(!content.empty()){
				outHtml+=L"<tr><td style=\"padding-bottom:0.2em; ";
				outHtml+=commonContentStyle;
				outHtml+=TWUtils::trimString(contentStyle);
				outHtml+=L"\">";
				outHtml+=content;
				outHtml+=L"</td></tr>";
			}
			
		}
		
	noMoreContents:;
	}
	
	{
		const std::wstring& title=arg[L"below"];
		const std::wstring& style=arg[L"belowstyle"];
		outHtml+=L"<tr><td style=\"padding:0.3em 0.4em 0.3em; font-weight:bold; ";
		outHtml+=TWUtils::trimString(style);
		outHtml+=L"\">";
		outHtml+=title;
		outHtml+=L"</td></tr>";
	}
	outHtml+=L"</table>";
	return outHtml;
}

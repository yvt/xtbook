//
//  XTBWikiplexusInfobox.cpp
//  XTBook
//
//  Created by Kawada Tomoaki on 8/4/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "XTBWikiplexusInfobox.h"
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

std::wstring XTBWikiplexusInfobox::execute
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
	
	if(arg[L"child"]!=L"yes"){
		
		outHtml=L"<table class=\"infobox ";
		outHtml+=TWUtils::trimString(arg[L"bodyclass"]);
		outHtml+=L"\" style=\"width:30%;text-align:left; float:right; font-size:88%; ";
		outHtml+=TWUtils::trimString(arg[L"bodystyle"]);
		outHtml+=L"\">";
		
		const std::wstring& argTitle=arg[L"title"];
		if(!argTitle.empty()){
			outHtml+=L"<caption class=\"";
			outHtml+=arg[L"titleclass"];
			outHtml+=L"\" style=\"font-size:125%; font-weight:bold; ";
			outHtml+=TWUtils::trimString(arg[L"titlestyle"]);
			outHtml+=L"\">";
			outHtml+=argTitle;
			outHtml+=L"</caption>";
		}
		
		const std::wstring& argAbove=arg[L"above"];
		if(!argAbove.empty()){
			outHtml+=htmlForHeaderRow
			(arg[L"aboveclass"], 
			 arg[L"abovestyle"], 
			 argAbove);
		}
		
	}else{
		
		const std::wstring& argTitle=arg[L"title"];
		if(!argTitle.empty()){
			outHtml+=L"'''";
			outHtml+=argTitle;
			outHtml+=L"'''";
		}
		
	}
	
	// sub-headers.
	{
		const std::wstring& subheader=arg(L"subheader", L"subheader1");
		const std::wstring& subheaderStyle=arg[L"subheaderstyle"];
		const std::wstring& subheaderClass=arg[L"subheaderclass"];
		const std::wstring& subheaderRowClass=arg(L"subheaderrowclass",
										   L"subheaderrowclass1");
		
		if(!subheader.empty()){
			outHtml+=htmlForDataRow
			(subheaderRowClass, 
			 subheaderClass, 
			 subheaderStyle, 
			 subheader);
		}
	}
	
	{
		const std::wstring& subheader=arg[L"subheader2"];
		const std::wstring& subheaderStyle=arg[L"subheaderstyle"];
		const std::wstring& subheaderClass=arg[L"subheaderclass2"];
		const std::wstring& subheaderRowClass=arg[L"subheaderrowclass2"];
		
		if(!subheader.empty()){
			outHtml+=htmlForDataRow
			(subheaderRowClass, 
			 subheaderClass, 
			 subheaderStyle, 
			 subheader);
		}
	}
	
	// images.
	{
		const std::wstring& image=arg(L"image", L"image1");
		const std::wstring& imageStyle=arg[L"imagestyle"];
		const std::wstring& imageClass=arg[L"imageclass"];
		const std::wstring& imageRowClass=arg[L"imagerrowclass1"];
		
		if(!image.empty()){
			const std::wstring& caption=arg(L"caption", L"caption1");
			const std::wstring& captionStyle=arg[L"captionstyle"];
			if(!caption.empty()){
				outHtml+=htmlForDataRow
				(imageRowClass, 
				 imageClass, 
				 imageStyle, 
				 image+L"<div style=\""+captionStyle+L"\">"
				 +caption+L"</div>");
			}else{
				outHtml+=htmlForDataRow
				(imageRowClass, 
				 imageClass, 
				 imageStyle, 
				 image);
			}
		}
	}
	
	{
		const std::wstring& image=arg[L"image2"];
		const std::wstring& imageStyle=arg[L"imagestyle"];
		const std::wstring& imageClass=arg[L"imageclass"];
		const std::wstring& imageRowClass=arg[L"imagerrowclass2"];
		
		if(!image.empty()){
			const std::wstring& caption=arg[L"caption2"];
			const std::wstring& captionStyle=arg[L"captionstyle"];
			if(!caption.empty()){
				outHtml+=htmlForDataRow
				(imageRowClass, 
				 imageClass, 
				 imageStyle, 
				 image+L"<div style=\""+captionStyle+L"\">"
				 +caption+L"</div>");
			}else{
				outHtml+=htmlForDataRow
				(imageRowClass, 
				 imageClass, 
				 imageStyle, 
				 image);
			}
		}
	}
	
	// headers.
	{
		const std::wstring& headerStyle=arg[L"headerstyle"]; 
		const std::wstring& labelStyle=arg[L"labelstyle"]; 
		const std::wstring& dataStyle=arg[L"datastyle"]; 
		
		std::wstring headerArgName=L"header1";
		std::wstring labelArgName=L"label1";
		std::wstring dataArgName=L"data1";
		std::wstring classArgName=L"class1";
		std::wstring rowClassArgName=L"rowclass1";
		
		for(int i=1;i<=9;i++){
			headerArgName[6]='0'+i;
			labelArgName[5]='0'+i;
			dataArgName[4]='0'+i;
			classArgName[5]='0'+i;
			rowClassArgName[8]='0'+i;
			
			const std::wstring& header=arg[headerArgName];
			const std::wstring& data=arg[dataArgName];
			
			if(header.empty() && data.empty())
				goto noMoreHeaders;
			
			const std::wstring& label=arg[labelArgName];
			
			if(!header.empty()){
				outHtml+=htmlForHeaderRow
				(arg[classArgName], 
				 headerStyle, 
				 header);
			}else if(label.empty()){
				outHtml+=htmlForDataRow
				(arg[rowClassArgName], 
				 arg[classArgName], 
				 dataStyle, 
				 data);
			}else{
				outHtml+=htmlForLabeledDataRow
				(arg[rowClassArgName], 
				 labelStyle, 
				 label, 
				 arg[classArgName], 
				 dataStyle, 
				 data);
			}
		}
		
		headerArgName+=L'0';
		labelArgName+=L'0';
		dataArgName+=L'0';
		classArgName+=L'0';
		rowClassArgName+=L'0';
		
		for(int i=10;i<=80;i++){
			headerArgName[6]='0'+i/10;
			labelArgName[5]='0'+i/10;
			dataArgName[4]='0'+i/10;
			classArgName[5]='0'+i/10;
			rowClassArgName[8]='0'+i/10;
			
			headerArgName[7]='0'+i%10;
			labelArgName[6]='0'+i%10;
			dataArgName[5]='0'+i%10;
			classArgName[6]='0'+i%10;
			rowClassArgName[9]='0'+i%10;
			
			const std::wstring& header=arg[headerArgName];
			const std::wstring& data=arg[dataArgName];
			
			if(header.empty() && data.empty())
				goto noMoreHeaders;
			
			const std::wstring& label=arg[labelArgName];
			
			if(!header.empty()){
				outHtml+=htmlForHeaderRow
				(arg[classArgName], 
				 headerStyle, 
				 header);
			}else if(label.empty()){
				outHtml+=htmlForDataRow
				(arg[rowClassArgName], 
				 arg[classArgName], 
				 dataStyle, 
				 data);
			}else{
				outHtml+=htmlForLabeledDataRow
				(arg[rowClassArgName], 
				 labelStyle, 
				 label, 
				 arg[classArgName], 
				 dataStyle, 
				 data);
			}
		}
		
	noMoreHeaders:;
	}
	
	// below.
	{
		const std::wstring& argBelow=arg[L"below"];
		const std::wstring& argBelowStyle=arg[L"belowstyle"];
		const std::wstring& argBelowClass=arg[L"belowclass"];
		
		outHtml+=L"<tr><td colspan=\"2\" class=\"";
		outHtml+=TWUtils::trimString(argBelowClass);
		outHtml+=L"\" style=\"text-align:center; ";
		outHtml+=TWUtils::trimString(argBelowStyle);
		outHtml+=L"\">";
		outHtml+=argBelow;
		outHtml+=L"</td></tr>";
		
	}
	
	// Tnavbar is disabled because not supported by XTBook.
	/*{
		const std::wstring& argName=arg(L"name", L"tnavbar");
		outHtml+=L"<tr><td style=\"text-align: right\">";
	}*/
	
	if(arg[L"child"]!=L"yes"){
		outHtml+=L"</table>";
	}
	return outHtml;
	
}

std::wstring XTBWikiplexusInfobox::htmlForHeaderRow
(const std::wstring &className, 
 const std::wstring &style, 
 const std::wstring &header) const{
	std::wstring outHtml;
	outHtml=L"<tr><th colspan=\"2\" class=\"";
	outHtml+=TWUtils::trimString(className);
	outHtml+=L"\" style=\"text-align:center; ";
	outHtml+=TWUtils::trimString(style);
	outHtml+=L"\">";
	outHtml+=header;
	outHtml+=L"</th></tr>";
	return outHtml;
}

std::wstring XTBWikiplexusInfobox::htmlForDataRow
(const std::wstring &rowClassName, 
 const std::wstring &className, 
 const std::wstring &style, 
 const std::wstring &data) const{
	std::wstring outHtml;
	outHtml=L"<tr class=\"";
	outHtml+=TWUtils::trimString(rowClassName);
	outHtml+=L"\">";
	outHtml+=L"<td colspan=\"2\" class=\"";
	outHtml+=TWUtils::trimString(className);
	outHtml+=L"\" style=\"text-align: center;";
	outHtml+=TWUtils::trimString(style);
	outHtml+=L"\">";
	outHtml+=data;
	outHtml+=L"</td></tr>";
	return outHtml;
}

std::wstring XTBWikiplexusInfobox::htmlForLabeledDataRow
(const std::wstring &rowClassName,
 const std::wstring &labelStyle,
 const std::wstring &label,
 const std::wstring &className,
 const std::wstring &style,
 const std::wstring &data) const{
	std::wstring outHtml;
	outHtml=L"<tr class=\"";
	outHtml+=TWUtils::trimString(rowClassName);
	outHtml+=L"\">";
	outHtml+=L"<th style=\"text-align:left; white-space:nowrap; ";
	outHtml+=TWUtils::trimString(labelStyle);
	outHtml+=L"\">";
	outHtml+=label;
	outHtml+=L"</th>";
	outHtml+=L"<td class=\"";
	outHtml+=TWUtils::trimString(className);
	outHtml+=L"\" style=\"text-align: center;";
	outHtml+=TWUtils::trimString(style);
	outHtml+=L"\">";
	outHtml+=data;
	outHtml+=L"</td></tr>";
	return outHtml;
}




//
//  XTBWikiplexusFamilyTree.cpp
//  XTBook
//
//  Created by 河田 智明 on 9/25/11.
//  Copyright (c) 2011 Nexhawks. All rights reserved.
//

#include "XTBWikiplexusFamilyTree.h"

#include "TWiki/TWUtils.h"

#include "i18n.h"

#include <assert.h>

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

static bool isCharTile(const std::wstring& c){
	if(c.empty())
		return true;
	if(c.size()!=1)
		return false;
	switch(c[0]){
		case L',':
		case L'.':
		case L'`':
		case L'\'':
		case L'^':
		case L'v':
		case L'(':
		case L')':
		case L'-':
		case L'!':
		case L'+':
		case L'F':
		case L'7':
		case L'L':
		case L'J':
		case L'A':
		case L'V':
		case L'C':
		case L'D':
		case L'~':
		case L':':
		case L'%':
		case L'*':
		case L'{':
		case L'}':
		case L'#':
		case L']':
		case L'[':
		case L'y':
		case L'h':
		case L'i':
		case L'j':
			return true;
	}
	return false;
}

#define TileSize L"16px"
#define TileSizeHalf L"8px"
#define TileColor L"#8f8f8f"
#define TileWidth L"1px"

struct TileStyle{
	const wchar_t *left, *top, *right, *bottom;
};

static TileStyle tileStyleForChar(const std::wstring& c){
	assert(isCharTile(c));
	assert(!c.empty());
	
	TileStyle ts={NULL,NULL,NULL,NULL};
	static const wchar_t *solidStyle=TileWidth L" solid " TileColor;
	static const wchar_t *dashedStyle=TileWidth L" dashed " TileColor;
	
	switch(c[0]){
		case L',':
			ts.right=solidStyle;
			ts.bottom=solidStyle;
			break;
		case L'.':
			ts.left=solidStyle;
			ts.bottom=solidStyle;
			break;
		case L'`':
			ts.right=solidStyle;
			ts.top=solidStyle;
			break;
		case L'\'':
			ts.left=solidStyle;
			ts.top=solidStyle;
			break;
		case L'^':
			ts.left=solidStyle;
			ts.right=solidStyle;
			ts.top=solidStyle;
			break;
		case L'v':
			ts.left=solidStyle;
			ts.right=solidStyle;
			ts.bottom=solidStyle;
			break;
		case L'(':
			ts.left=solidStyle;
			ts.top=solidStyle;
			ts.bottom=solidStyle;
			break;
		case L')':
			ts.right=solidStyle;
			ts.top=solidStyle;
			ts.bottom=solidStyle;
			break;
		case L'-':
			ts.right=solidStyle;
			ts.left=solidStyle;
			break;
		case L'!':
			ts.top=solidStyle;
			ts.bottom=solidStyle;
			break;
		case L'+':
			ts.top=solidStyle;
			ts.bottom=solidStyle;
			ts.left=solidStyle;
			ts.right=solidStyle;
			break;
		
		case L'F':
			ts.right=dashedStyle;
			ts.bottom=dashedStyle;
			break;
		case L'7':
			ts.left=dashedStyle;
			ts.bottom=dashedStyle;
			break;
		case L'L':
			ts.right=dashedStyle;
			ts.top=dashedStyle;
			break;
		case L'J':
			ts.left=dashedStyle;
			ts.top=dashedStyle;
			break;
		case L'A':
			ts.left=dashedStyle;
			ts.right=dashedStyle;
			ts.top=dashedStyle;
			break;
		case L'V':
			ts.left=dashedStyle;
			ts.right=dashedStyle;
			ts.bottom=dashedStyle;
			break;
		case L'C':
			ts.left=dashedStyle;
			ts.top=dashedStyle;
			ts.bottom=dashedStyle;
			break;
		case L'D':
			ts.right=dashedStyle;
			ts.top=dashedStyle;
			ts.bottom=dashedStyle;
			break;
		case L'~':
			ts.right=dashedStyle;
			ts.left=dashedStyle;
			break;
		case L':':
			ts.top=dashedStyle;
			ts.bottom=dashedStyle;
			break;
		case L'%':
			ts.top=dashedStyle;
			ts.bottom=dashedStyle;
			ts.left=dashedStyle;
			ts.right=dashedStyle;
			break;
			
		case L'*':
			ts.left=solidStyle;
			ts.right=solidStyle;
			ts.top=dashedStyle;
			ts.bottom=dashedStyle;
			break;
			
		case L'}':
			ts.top=dashedStyle;
			ts.bottom=dashedStyle;
			ts.right=solidStyle;
			break;
			
		case L'{':
			ts.left=solidStyle;
			ts.top=dashedStyle;
			ts.bottom=dashedStyle;
			break;
			
		case L'#':
			ts.left=dashedStyle;
			ts.right=dashedStyle;
			ts.top=solidStyle;
			ts.bottom=solidStyle;
			break;
			
		case L']':
			ts.top=solidStyle;
			ts.bottom=solidStyle;
			ts.right=dashedStyle;
			break;
			
		case L'[':
			ts.top=solidStyle;
			ts.bottom=solidStyle;
			ts.left=dashedStyle;
			break;
			
		case L'y':
			ts.left=dashedStyle;
			ts.right=dashedStyle;
			ts.bottom=solidStyle;
			break;
			
		case L'h':
			ts.left=dashedStyle;
			ts.right=dashedStyle;
			ts.top=solidStyle;
			break;
			
		case L'i':
			ts.left=solidStyle;
			ts.right=solidStyle;
			ts.top=dashedStyle;
			break;
			
		case L'j':
			ts.left=solidStyle;
			ts.right=solidStyle;
			ts.bottom=dashedStyle;
			break;
			
		default:
			assert(false);
			;
			
	}
	
	return ts;
}

static void renderUpperTile(std::wstring& out, const std::wstring& c){
	assert(isCharTile(c));
	
	if(c.empty()){
		out+=L"<td style=\"width:" TileSize L";height:" TileSize 
		L"\" colspan=\"2\" rowspan=\"2\"></td>";
		return;
	}
	
	TileStyle style=tileStyleForChar(c);
	
	
	if(style.top){
		out+=L"<td style=\"width: " TileSizeHalf L";height: " TileSizeHalf;
		out+=L";border-right:";
		out+=style.top;
		out+=L"\"></td>";
	}else{
		out+=L"<td></td>";
	}
	
	if(style.right){
		out+=L"<td style=\"width: " TileSizeHalf L";height: " TileSizeHalf;
		out+=L";border-bottom:";
		out+=style.right;
		out+=L"\"></td>";
	}else{
		out+=L"<td></td>";
	}
	
}

static void renderLowerTile(std::wstring& out, const std::wstring& c){
	assert(isCharTile(c));
	
	if(c.empty()){
		return;
	}
	
	TileStyle style=tileStyleForChar(c);
	
	
	if(style.left){
		out+=L"<td style=\"width: " TileSizeHalf L";height: " TileSizeHalf;
		out+=L";border-top:";
		out+=style.left;
		out+=L"\"></td>";
	}else{
		out+=L"<td></td>";
	}
	
	
	
	if(style.bottom){
		out+=L"<td style=\"width: " TileSizeHalf L";height: " TileSizeHalf;
		out+=L";border-left:";
		out+=style.bottom;
		out+=L"\"></td>";
	}else{
		out+=L"<td></td>";
	}
	
}

std::wstring XTBWikiplexusFamilyTree::execute
(const std::wstring &arg0, 
 const std::vector<std::wstring> &args, 
 TWPreprocessorFrame *){
	// create arg map.
	std::map<std::wstring, std::wstring> allArgs;
	std::vector<std::wstring> unnumberedArgs;
	for(size_t i=0;i<args.size();i++){
		const std::wstring& s=args[i];
		size_t pos=s.find(L'=');
		if(pos==std::wstring::npos){
			unnumberedArgs.push_back(TWUtils::trimString(s));
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
	
	// check box style.
	std::wstring boxStyle;
	static const std::wstring borderParamName=L"border";
	static const std::wstring boxStyleParamName=L"boxstyle";
	std::wstring borderParam=arg[borderParamName];
	int borderWidth=1;
	if(!borderParam.empty()){
		borderWidth=TWUtils::intValueFromString(borderParam);
	}
	if(borderWidth>0){
		boxStyle=L"border: ";
		boxStyle+=TWUtils::stringWithIntValue(borderWidth);
		boxStyle+=L"px solid black;";
	}
	boxStyle+=L"text-align:center;";
	boxStyle+=arg[boxStyleParamName];
	
	outHtml+=L"<tr>";
	
	// skip empty cells.
	size_t startIndex=0;
	while(startIndex<unnumberedArgs.size()){
		if(!unnumberedArgs[startIndex].empty())
			break;
		startIndex++;
	}
	if(startIndex>0){
		outHtml+=L"<td style=\"height:" TileSize 
		L"\" colspan=\"";
		outHtml+=TWUtils::stringWithIntValue(startIndex*2);
		outHtml+=L"\" rowspan=\"2\"></td>";
	}
	
	while(unnumberedArgs.size()>startIndex){
		if(!unnumberedArgs.back().empty())
			break;
		unnumberedArgs.pop_back();
	}
	
	// render upper.
	for(size_t i=startIndex;i<unnumberedArgs.size();i++){
		const std::wstring& item=unnumberedArgs[i];
		
		if(isCharTile(item)){
			// this is a tile.
			if(item.empty()){
				// empty cell. continuous?
				if(i>startIndex && unnumberedArgs[i-1].empty())
					continue;
				
				// bond empty cells.
				size_t last=i;
				for(last=i;last<unnumberedArgs.size() && unnumberedArgs[last].empty();last++);
				outHtml+=L"<td style=\"height:" TileSize 
				L"\" colspan=\"";
				assert((last-i)<1024);
				outHtml+=TWUtils::stringWithIntValue((int)(last-i)*2);
				outHtml+=L"\" rowspan=\"2\"></td>";
			}else{
				renderUpperTile(outHtml, item);
			}
		}else{
			// this is a box.
			outHtml+=L"<td style=\"";
			outHtml+=boxStyle;
			outHtml+=L";";
			
			// apply local box style.
			outHtml+=arg[L"boxstyle_"+item];
			outHtml+=L"\" colspan=\"6\" rowspan=\"2\">";
			outHtml+=arg[item];
			outHtml+=L"</td>";
		}
	}
	outHtml+=L"</tr>";
	
	// render lower.
	outHtml+=L"<tr>";
	for(size_t i=startIndex;i<unnumberedArgs.size();i++){
		const std::wstring& item=unnumberedArgs[i];
		
		if(isCharTile(item)){
			// this is a tile.
			renderLowerTile(outHtml, item);
		}else{
			// this is a box.
			// nothing to do now.
		}
	}
	outHtml+=L"</tr>";
	
	return outHtml;
}
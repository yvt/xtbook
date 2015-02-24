//
//  XTBWikiplexusParser.cpp
//  XTBook
//
//  Created by Kawada Tomoaki on 7/7/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//

#include "XTBWikiplexusParser.h"
#include "XTBWikiplexusDictionary.h"
#include "TWiki/TWUtils.h"
#include "commondlg.h"
#include "i18n.h"
#include <tcw/twStrConv.h>
#include "TWiki/TWPreprocessor.h"
#include "XTBWikiplexusFootnoteExtension.h"
#include "XTBWikiplexusGalleryExtension.h"
#include "XTBWikiplexusInfobox.h"
#include "XTBWikiplexusSidebar.h"
#include "XTBWikiplexusSourceExtension.h"
#include "XTBWikiplexusCollapsibleList.h"
#include "XTBWikiplexusIsoDate.h"
#include "TWiki/TWConstantParserVariable.h"
#include "XTBWikiplexusMathCodizeExtension.h"
#include "XTBWikiplexusFamilyTree.h"
#include "XTBUserDefaults.h"
#include "TPList/TPLNumber.h"

XTBWikiplexusParser::XTBWikiplexusParser(TWParserConfig *cfg, XTBWikiplexusDictionary *dic):
TWParser(cfg),
m_dic(dic){
	m_parserExtensions.insert(std::make_pair(L"youtube", (TWParserExtension *)NULL));
	m_parserExtensions.insert(std::make_pair(L"css", (TWParserExtension *)NULL));
	m_parserExtensions.insert(std::make_pair(L"math", new XTBWikiplexusMathCodizeExtension()));

	m_parserExtensions.insert(std::make_pair(L"ref", new XTBWikiplexusFootnoteExtension()));
	m_parserExtensions.insert(std::make_pair(L"gallery", new XTBWikiplexusGalleryExtension()));
	m_parserExtensions.insert(std::make_pair(L"source", new XTBWikiplexusSourceExtension()));
	m_parserFunctions.insert(std::make_pair(L"familytree", new XTBWikiplexusFamilyTree()));
	m_parserFunctions.insert(std::make_pair(L"infobox", new XTBWikiplexusInfobox()));
	m_parserFunctions.insert(std::make_pair(L"sidebar", new XTBWikiplexusSidebar()));
	m_parserFunctions.insert(std::make_pair(L"collapsible list", new XTBWikiplexusCollapsibleList()));
	m_parserFunctions.insert(std::make_pair(L"isodate", new XTBWikiplexusIsoDate()));
	m_parserFunctions.insert(std::make_pair(L"checkdate", new XTBWikiplexusIsoDate()));
	m_parserFunctions.insert(std::make_pair(L"dmc", (TWParserFunction *)NULL));
	m_parserFunctions.insert(std::make_pair(L"etyl",  (TWParserFunction *)NULL)); /// TODO:
	m_parserFunctions.insert(std::make_pair(L"dmca", (TWParserFunction *)NULL));
	m_parserFunctions.insert(std::make_pair(L"asbox/templatepage", (TWParserFunction *)NULL));
	m_parserFunctions.insert(std::make_pair(L"reflist", (TWParserFunction *)NULL));
	m_parserFunctions.insert(std::make_pair(L"navperiodictable", (TWParserFunction *)NULL));
	m_parserFunctions.insert(std::make_pair(L"nowrap begin", new TWConstantParserVariable(L"<span style=\"white-space:nowrap\">")));
	m_parserFunctions.insert(std::make_pair(L"nowrap end", new TWConstantParserVariable(L"</span >")));
	m_parserFunctions.insert(std::make_pair(L"wrap", new TWConstantParserVariable(L"</span>\x3000<span style=\"white-space:nowrap\">")));
	m_parserFunctions.insert(std::make_pair(L"w", new TWConstantParserVariable(L"</span>\x3000<span style=\"white-space:nowrap\">")));
	m_parserFunctions.insert(std::make_pair(L"\xff65wrap", new TWConstantParserVariable(L"</span>\xff65<span style=\"white-space:nowrap\">")));
	m_parserFunctions.insert(std::make_pair(L"\xff65w", new TWConstantParserVariable(L"</span>\xff65<span style=\"white-space:nowrap\">")));
	m_parserFunctions.insert(std::make_pair(L"\x30fbwrap", new TWConstantParserVariable(L"</span>\x30fb<span style=\"white-space:nowrap\">")));
	m_parserFunctions.insert(std::make_pair(L"\x30fbw", new TWConstantParserVariable(L"</span>\x30fb<span style=\"white-space:nowrap\">")));
	m_parserFunctions.insert(std::make_pair(L"\x2219wrap", new TWConstantParserVariable(L"</span>\x2219<span style=\"white-space:nowrap\">")));
	m_parserFunctions.insert(std::make_pair(L"\x2219w", new TWConstantParserVariable(L"</span>\x2219<span style=\"white-space:nowrap\">")));
	m_parserFunctions.insert(std::make_pair(L"\x30fb\x30fbwrap", new TWConstantParserVariable(L"</span>\x30fb\x30fb<span style=\"white-space:nowrap\">")));
	m_parserFunctions.insert(std::make_pair(L"\x30fb\x30fbw", new TWConstantParserVariable(L"</span>\x30fb\x30fb<span style=\"white-space:nowrap\">")));
	m_parserFunctions.insert(std::make_pair(L"-wrap", new TWConstantParserVariable(L"</span> - <span style=\"white-space:nowrap\">")));
	m_parserFunctions.insert(std::make_pair(L"-w", new TWConstantParserVariable(L"</span> - <span style=\"white-space:nowrap\">")));
	m_parserFunctions.insert(std::make_pair(L"!wrap", new TWConstantParserVariable(L"</span>&#124;<span style=\"white-space:nowrap\">")));
	m_parserFunctions.insert(std::make_pair(L"!w", new TWConstantParserVariable(L"</span>&#124;<span style=\"white-space:nowrap\">")));
	m_parserFunctions.insert(std::make_pair(L"\\wrap", new TWConstantParserVariable(L"</span>/<span style=\"white-space:nowrap\">")));
	m_parserFunctions.insert(std::make_pair(L"\\w", new TWConstantParserVariable(L"</span>/<span style=\"white-space:nowrap\">")));
	m_parserFunctions.insert(std::make_pair(L"&wrap", new TWConstantParserVariable(L"</span>\x3000<span style=\"white-space:nowrap\">")));
	m_parserFunctions.insert(std::make_pair(L"&w", new TWConstantParserVariable(L"</span>\x3000<span style=\"white-space:nowrap\">")));
}


XTBWikiplexusParser::~XTBWikiplexusParser(){
	
}

std::wstring XTBWikiplexusParser::fetchPage(const TWTitle& title) const{
	//XTBLog("fetching %s", twW2M(title.dbKey).c_str());
	
	XTBUserDefaults *def=XTBUserDefaults::standardUserDefaults();
	if(def->numberForKey(XTBUserDefaultsKeyIgnoreTemplates)->boolValue())
		throw false;
	
	return m_dic->templateWikiForName(title.dbKey);
}
std::wstring XTBWikiplexusParser::makeSelfLink(const std::wstring& displayText){
	return TWParser::makeSelfLink(displayText);
}
std::wstring XTBWikiplexusParser::doFragmentId(const std::wstring &url) const{
	std::wstring::size_type pos;
	pos=url.find(L'#');
	if(pos!=std::wstring::npos){
		return url.substr(0, pos)+L'#'+
		m_dic->fragmentIdForIndexText(url.substr(pos+1));
	}else{
		return url;
	}
}
std::wstring XTBWikiplexusParser::urlForInternalLink(const TWTitle &referenceTo) const{
	return m_dic->scheme()+L":"+doFragmentId(expandTitle(referenceTo));
}
std::wstring XTBWikiplexusParser::makeInternalLink(const TWTitle& referenceTo, const std::wstring& displayText){
	return L"<a href=\""+TWUtils::escapeSpecialChars(urlForInternalLink(referenceTo))+L"\">"+displayText+L"</a>";
}
std::wstring XTBWikiplexusParser::makeImage2(const TWTitle& referenceTo, std::map<std::wstring,std::wstring>& frameParam, std::map<std::wstring,std::wstring>& handlerParam){
	std::wstring outHtml;
	const int maxWidth=180;
	
	XTBSetProgressText(XTBFormat(XTBLocalizedString(L"XTBWikiplexusArticleReadingImage"),
								 referenceTo.dbKey.c_str(), NULL));
	
	int width=TWUtils::intValueFromString(handlerParam[L"width"]);
	std::wstring alt=frameParam[L"alt"];
	std::wstring title=frameParam[L"title"];
	std::wstring align=frameParam[L"align"];
	// ignore valign because TWiki doesn't support it.
	bool isThumbnail=frameParam.find(L"thumbnail")!=frameParam.end();
	bool isFramed=frameParam.find(L"framed")!=frameParam.end();
	bool isFrameless=frameParam.find(L"frameless")!=frameParam.end();
	bool isManualThumb=frameParam.find(L"manualthumb")!=frameParam.end();
	
	std::wstring prefix, postfix;
	
	if(align==L"center"){
		prefix=L"<div class=\"center\">";
		postfix=L"</div>";
		align.clear();
	}
	
	if(isThumbnail || isFramed || isFrameless){
		if(width==0 || width>maxWidth)
			width=maxWidth;
	}
	
	if(isThumbnail || isManualThumb || isFramed){
		return prefix+makeThumbLink2(referenceTo, frameParam, handlerParam, width, align)+postfix;
	}
	
	
	
	outHtml+=L"<img src=\""+TWUtils::escapeSpecialChars(urlForImage(referenceTo))+L"\" ";
	if(frameParam.find(L"border")!=frameParam.end())
		outHtml+=L"class=\"thumbborder\" ";
	if(width!=0)
		outHtml+=L"width=\""+TWUtils::stringWithIntValue(width)+L"\" ";
	outHtml+=L"/>";
	
	outHtml=linkImage(outHtml, frameParam, referenceTo);
	
	return prefix+outHtml+postfix;
}
std::wstring XTBWikiplexusParser::makeThumbLink2
(const TWTitle& referenceTo, 
 std::map<std::wstring,std::wstring>& frameParam, 
 std::map<std::wstring,std::wstring>& handlerParam,
 int width,
 std::wstring align){
	
	std::wstring alt=frameParam[L"alt"];
	std::wstring title=frameParam[L"title"];
	// ignore valign because TWiki doesn't support it.
	
	bool isManualThumb=frameParam.find(L"manualthumb")!=frameParam.end();
	bool isUpright=frameParam.find(L"upright")!=frameParam.end();
	
	if(align.empty())
		align=L"right";
	
	if(width==0)
		width=isUpright?130:180;
	
	std::wstring url;
	url=TWUtils::escapeSpecialChars(urlForImage(referenceTo));
	
	if(isManualThumb){
		TWTitle title=parseTitle(frameParam[L"manualthumb"]);
		url=TWUtils::escapeSpecialChars(urlForImage(title));
	}
	
	int outerWidth=width+4;
	
	std::wstring outHtml;
	outHtml=L"<div class=\"thumb t"+align+L"\" style=\"width: ";
	outHtml+=TWUtils::stringWithIntValue(outerWidth+8);
	assert(!TWUtils::trimString(TWUtils::stringWithIntValue(outerWidth+8)).empty());
	outHtml+=L"px\">";
	outHtml+=L"<div class=\"thumbinner\" style=\"width: ";
	outHtml+=TWUtils::stringWithIntValue(outerWidth);
	assert(!TWUtils::trimString(TWUtils::stringWithIntValue(outerWidth)).empty());
	outHtml+=L"px\">";
	
	std::wstring imgHtml;
	
	imgHtml+=L"<img src=\""+url+L"\"";
	imgHtml+=L" class=\"thumbimage\"";
	imgHtml+=L" width=\""+TWUtils::stringWithIntValue(width)+L"\"";
	imgHtml+=L" twiki:nomagnify=\"nomagnify\"";
	imgHtml+=L" />";
	
	outHtml+=linkImage(imgHtml, frameParam, referenceTo);
	
	outHtml+=L"<div class=\"thumbcaption\">";
	outHtml+=frameParam[L"caption"];
	outHtml+=L"</div></div></div>";
	return outHtml;
}
std::wstring XTBWikiplexusParser::makeError(const std::wstring& msg) const{
	//XTBLog("TWiki Error: %s", twW2M(msg).c_str());
	return std::wstring();
	return L"<strong class=\"error\" style=\"color: red;\">" + TWUtils::escapeSpecialChars(msg) + L"</strong>";
}

std::wstring XTBWikiplexusParser::urlForImage(const TWTitle &title) const{
	std::wstring ttl;
	const std::wstring& scheme=m_dic->schemeForImages();
	
	if(scheme.empty())
		return std::wstring();
	
	ttl=title.dbKey;
	
	return scheme+L':'+ttl;
}

std::wstring XTBWikiplexusParser::linkImage(const std::wstring &html,
											std::map<std::wstring, std::wstring> &frameParam,
											const TWTitle& referenceTo){
	if(frameParam.find(L"no-link")!=frameParam.end()){
		return html;
	}
	
	std::wstring href;
	if(frameParam.find(L"link-url")!=frameParam.end()){
		href=frameParam[L"link-url"];
	}else if(frameParam.find(L"link-title")!=frameParam.end()){
		href=urlForInternalLink(parseTitle(frameParam[L"link-title"]));
	}else{
		href=urlForImage(referenceTo);
	}
	
	std::wstring outHtml;
	outHtml=L"<a href=\"";
	outHtml+=TWUtils::escapeSpecialChars(href);
	outHtml+=L"\">";
	outHtml+=html;
	outHtml+=L"</a>";
	return outHtml;
	
}


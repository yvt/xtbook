/*
 *  TWParser.cpp
 *  XTBook
 *
 *  Created by Nexhawks on 2/23/11.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */
/*
 * This file is part of XTBook.
 *
 * XTBook is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * XTBook is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with XTBook.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "TWParser.h"
#include "TWPreprocessor.h"
#include "TWPreprocessorFrame.h"
#include "TWPreprocessorTemplateFrame.h"
#include "TWPreprocessorNode.h"
#include "TWUtils.h"
#include "TWStringSplitter.h"
#include "TWParserFunction.h"
#include "TWPreparsedParserFunction.h"
#include "TWPostparsedParserFunction.h"
#include "TWSimpleParserFunction.h"
#include "TWDateParserVariable.h"
#include "TWConstantParserVariable.h"
#include "TWExpressionParserFunction.h"
#include "TWIfParserFunction.h"
#include "TWIfEqParserFunction.h"
#include "TWIfExprParserFunction.h"
#include "TWIfErrorParserFunction.h"
#include "TWRel2AbsParserFunction.h"
#include "TWSwitchParserFunction.h"
#include "TWDefaultSortParserFunction.h"
#include "TWDisplayTitleParserFunction.h"
#include "TWFullPageNameVariable.h"
#include "TWPageNameVariable.h"
#include "TWBasePageNameVariable.h"
#include "TWSubPageNameVariable.h"
#include "TWIgnoreParserFunction.h"
#include "TWNamespaceAliasParserFunction.h"
#include "TWTransformParserFunction.h"
#include "TWFormatNumParserFunction.h"
#include "TWPassThroughParserFunction.h"
#include "TWPadLeftParserFunction.h"
#include "TWPadRightParserFunction.h"
#include "TWPluralParserFunction.h"
#include "TWGrammarParserFunction.h"
#include "TWNamespaceVariable.h"
#include "TWRandomSelectionExtension.h"
#include "TWTitlePartsParserFunction.h"
#include "TWParserConfig.h"
#include "BlockElements.h"
#include <utility>
#include <algorithm>
#include <assert.h>
#include <exception>

static inline wchar_t easytolower(wchar_t in){
	if(in<=L'Z' && in>=L'A')
		return in-(L'Z'-L'z');
	return in;
} 
static inline wchar_t easytoupper(wchar_t in){
	if(in<=L'z' && in>=L'a')
		return in-(L'z'-L'Z');
	return in;
} 

TWParser::TWParser(TWParserConfig *cfg):
m_config(cfg){
	m_preprocessor=new TWPreprocessor(this);
	try{
		// magic words.
		m_parserFunctions.insert(std::make_pair(L"currentday", new TWDateParserVariable("%e", false)));
		m_parserFunctions.insert(std::make_pair(L"currentday2", new TWDateParserVariable("%d", false)));
		m_parserFunctions.insert(std::make_pair(L"currentdayname", new TWDateParserVariable("%A", false)));
		m_parserFunctions.insert(std::make_pair(L"currentdow", new TWDateParserVariable("%w", false)));
		m_parserFunctions.insert(std::make_pair(L"currentmonth", new TWDateParserVariable("%m", false)));
		m_parserFunctions.insert(std::make_pair(L"currentmonthabbrev", new TWDateParserVariable("%b", false)));
		m_parserFunctions.insert(std::make_pair(L"currentmonthname", new TWDateParserVariable("%B", false)));
		m_parserFunctions.insert(std::make_pair(L"currenttime", new TWDateParserVariable("%H:%M", false)));
		m_parserFunctions.insert(std::make_pair(L"currenthour", new TWDateParserVariable("%H", false)));
		m_parserFunctions.insert(std::make_pair(L"currentweek", new TWDateParserVariable("%V", false)));
		m_parserFunctions.insert(std::make_pair(L"currentyear", new TWDateParserVariable("%Y", false)));
		m_parserFunctions.insert(std::make_pair(L"currenttimestamp", new TWDateParserVariable("%G%m%d%H%M%S", false)));
		m_parserFunctions.insert(std::make_pair(L"localday", new TWDateParserVariable("%e", false)));
		m_parserFunctions.insert(std::make_pair(L"localday2", new TWDateParserVariable("%d", true)));
		m_parserFunctions.insert(std::make_pair(L"localdayname", new TWDateParserVariable("%A", true)));
		m_parserFunctions.insert(std::make_pair(L"localdow", new TWDateParserVariable("%w", true)));
		m_parserFunctions.insert(std::make_pair(L"localmonth", new TWDateParserVariable("%m", true)));
		m_parserFunctions.insert(std::make_pair(L"localmonthabbrev", new TWDateParserVariable("%b", true)));
		m_parserFunctions.insert(std::make_pair(L"localmonthname", new TWDateParserVariable("%B", true)));
		m_parserFunctions.insert(std::make_pair(L"localtime", new TWDateParserVariable("%H:%M", true)));
		m_parserFunctions.insert(std::make_pair(L"localhour", new TWDateParserVariable("%H", true)));
		m_parserFunctions.insert(std::make_pair(L"localweek", new TWDateParserVariable("%V", true)));
		m_parserFunctions.insert(std::make_pair(L"localyear", new TWDateParserVariable("%Y", true)));
		m_parserFunctions.insert(std::make_pair(L"localtimestamp", new TWDateParserVariable("%G%m%e%H%M%S", true)));
		m_parserFunctions.insert(std::make_pair(L"currentversion", new TWConstantParserVariable(L"TWiki")));
		m_parserFunctions.insert(std::make_pair(L"sitename", new TWConstantParserVariable(m_config->siteName())));
		m_parserFunctions.insert(std::make_pair(L"displaytitle", new TWDisplayTitleParserFunction(this)));
		m_parserFunctions.insert(std::make_pair(L"defaultsort", new TWDefaultSortParserFunction(this)));
		m_parserFunctions.insert(std::make_pair(L"defaultsortkey", new TWDefaultSortParserFunction(this)));
		m_parserFunctions.insert(std::make_pair(L"defaultcategorysort", new TWDefaultSortParserFunction(this)));
		m_parserFunctions.insert(std::make_pair(L"fullpagename", new TWFullPageNameVariable(this)));
		m_parserFunctions.insert(std::make_pair(L"pagename", new TWPageNameVariable(this)));
		m_parserFunctions.insert(std::make_pair(L"basepagename", new TWBasePageNameVariable(this)));
		m_parserFunctions.insert(std::make_pair(L"subpagename", new TWSubPageNameVariable(this)));
		m_parserFunctions.insert(std::make_pair(L"fullpagenamee", new TWFullPageNameVariable(this)));
		m_parserFunctions.insert(std::make_pair(L"pagenamee", new TWPageNameVariable(this)));
		m_parserFunctions.insert(std::make_pair(L"basepagenamee", new TWBasePageNameVariable(this)));
		m_parserFunctions.insert(std::make_pair(L"subpagenamee", new TWSubPageNameVariable(this)));
		m_parserFunctions.insert(std::make_pair(L"ns", new TWNamespaceAliasParserFunction(this)));
		m_parserFunctions.insert(std::make_pair(L"namespace", new TWNamespaceVariable(this)));
		m_parserFunctions.insert(std::make_pair(L"namespacee", new TWNamespaceVariable(this)));
		m_parserFunctions.insert(std::make_pair(L"lc", new TWTransformParserFunction(easytolower, false)));
		m_parserFunctions.insert(std::make_pair(L"lcfirst", new TWTransformParserFunction(easytolower, true)));
		m_parserFunctions.insert(std::make_pair(L"uc", new TWTransformParserFunction(easytoupper, false)));
		m_parserFunctions.insert(std::make_pair(L"ucfirst", new TWTransformParserFunction(easytoupper, true)));
		m_parserFunctions.insert(std::make_pair(L"formatnum", new TWFormatNumParserFunction()));
		m_parserFunctions.insert(std::make_pair(L"padleft", new TWPadLeftParserFunction()));
		m_parserFunctions.insert(std::make_pair(L"padright", new TWPadRightParserFunction()));
		m_parserFunctions.insert(std::make_pair(L"plural", new TWPluralParserFunction()));
		m_parserFunctions.insert(std::make_pair(L"grammar", new TWGrammarParserFunction()));
		
		
		// ParserFunction extensions.
		m_parserFunctions.insert(std::make_pair(L"#expr", new TWExpressionParserFunction()));
		m_parserFunctions.insert(std::make_pair(L"#if", new TWIfParserFunction()));
		m_parserFunctions.insert(std::make_pair(L"#ifexist", new TWIfParserFunction())); // ha-ha-ha! who cares!?
		m_parserFunctions.insert(std::make_pair(L"#ifeq", new TWIfEqParserFunction()));
		m_parserFunctions.insert(std::make_pair(L"#ifexpr", new TWIfExprParserFunction()));
		m_parserFunctions.insert(std::make_pair(L"#iferror", new TWIfErrorParserFunction()));
		m_parserFunctions.insert(std::make_pair(L"#rel2abs", new TWRel2AbsParserFunction(this)));
		m_parserFunctions.insert(std::make_pair(L"#switch", new TWSwitchParserFunction()));
		m_parserFunctions.insert(std::make_pair(L"#titleparts", new TWTitlePartsParserFunction()));
		
		// not implemented stuff.
		m_parserFunctions.insert(std::make_pair(L"server", new TWConstantParserVariable(L"http://www.nexhawks.net")));
		m_parserFunctions.insert(std::make_pair(L"servername", new TWConstantParserVariable(L"www.nexhawks.net")));
		m_parserFunctions.insert(std::make_pair(L"dirmark", new TWConstantParserVariable(L"")));
		m_parserFunctions.insert(std::make_pair(L"directionmark", new TWConstantParserVariable(L"")));
		m_parserFunctions.insert(std::make_pair(L"scriptpath", new TWConstantParserVariable(L"/w")));
		m_parserFunctions.insert(std::make_pair(L"stylepath", new TWConstantParserVariable(L"")));
		m_parserFunctions.insert(std::make_pair(L"contentlanguage", new TWConstantParserVariable(L"?")));
		m_parserFunctions.insert(std::make_pair(L"contentlang", new TWConstantParserVariable(L"?")));
		m_parserFunctions.insert(std::make_pair(L"revisionid", new TWConstantParserVariable(L"?")));
		m_parserFunctions.insert(std::make_pair(L"revisionday", new TWConstantParserVariable(L"?")));
		m_parserFunctions.insert(std::make_pair(L"revisionday2", new TWConstantParserVariable(L"?")));
		m_parserFunctions.insert(std::make_pair(L"revisionmonth", new TWConstantParserVariable(L"?")));
		m_parserFunctions.insert(std::make_pair(L"revisionmonth1", new TWConstantParserVariable(L"?")));
		m_parserFunctions.insert(std::make_pair(L"revisionyear", new TWConstantParserVariable(L"?")));
		m_parserFunctions.insert(std::make_pair(L"revisiontimestamp", new TWConstantParserVariable(L"?")));
		m_parserFunctions.insert(std::make_pair(L"revisionuser", new TWConstantParserVariable(L"?")));
		m_parserFunctions.insert(std::make_pair(L"pagesize", new TWConstantParserVariable(L"?")));
		m_parserFunctions.insert(std::make_pair(L"protectionlevel", new TWConstantParserVariable(L"?")));
		m_parserFunctions.insert(std::make_pair(L"numberofpages", new TWConstantParserVariable(L"?")));
		m_parserFunctions.insert(std::make_pair(L"numberofarticles", new TWConstantParserVariable(L"?")));
		m_parserFunctions.insert(std::make_pair(L"numberoffiles", new TWConstantParserVariable(L"?")));
		m_parserFunctions.insert(std::make_pair(L"numberofedits", new TWConstantParserVariable(L"?")));
		m_parserFunctions.insert(std::make_pair(L"numberofviews", new TWConstantParserVariable(L"?")));
		m_parserFunctions.insert(std::make_pair(L"numberofusers", new TWConstantParserVariable(L"?")));
		m_parserFunctions.insert(std::make_pair(L"numberofadmins", new TWConstantParserVariable(L"?")));
		m_parserFunctions.insert(std::make_pair(L"numberofactiveusers", new TWConstantParserVariable(L"?")));
		m_parserFunctions.insert(std::make_pair(L"pagesincategory", new TWConstantParserVariable(L"?")));
		m_parserFunctions.insert(std::make_pair(L"pagesincat", new TWConstantParserVariable(L"?")));
		m_parserFunctions.insert(std::make_pair(L"talkpagename", new TWConstantParserVariable(L"?")));
		m_parserFunctions.insert(std::make_pair(L"talkspace", new TWConstantParserVariable(L"?")));
		m_parserFunctions.insert(std::make_pair(L"numberingroup", new TWConstantParserVariable(L"?")));
		m_parserFunctions.insert(std::make_pair(L"numingroup", new TWConstantParserVariable(L"?")));
		m_parserFunctions.insert(std::make_pair(L"pagesinns", new TWConstantParserVariable(L"?")));
		m_parserFunctions.insert(std::make_pair(L"pagesinnamespace", new TWConstantParserVariable(L"?")));
		m_parserFunctions.insert(std::make_pair(L"localurl", new TWConstantParserVariable(L"http://?")));
		m_parserFunctions.insert(std::make_pair(L"fullurl", new TWConstantParserVariable(L"http://?")));
		m_parserFunctions.insert(std::make_pair(L"filepath", new TWConstantParserVariable(L"http://?")));
		m_parserFunctions.insert(std::make_pair(L"urlencode", new TWConstantParserVariable(L"?")));
		m_parserFunctions.insert(std::make_pair(L"anchorencode", new TWConstantParserVariable(L"?")));
		m_parserFunctions.insert(std::make_pair(L"dateformat", new TWPassThroughParserFunction()));
		m_parserFunctions.insert(std::make_pair(L"formatdate", new TWPassThroughParserFunction()));
		
		// double-underscore modifier. (ignored)
		m_underscoreFunctions.insert(std::make_pair(L"notoc", new TWIgnoreParserFunction()));
		m_underscoreFunctions.insert(std::make_pair(L"forcetoc", new TWIgnoreParserFunction()));
		m_underscoreFunctions.insert(std::make_pair(L"toc", new TWIgnoreParserFunction()));
		m_underscoreFunctions.insert(std::make_pair(L"noeditsection", new TWIgnoreParserFunction()));
		m_underscoreFunctions.insert(std::make_pair(L"newsectionlink", new TWIgnoreParserFunction()));
		m_underscoreFunctions.insert(std::make_pair(L"nonewsectionlink", new TWIgnoreParserFunction()));
		m_underscoreFunctions.insert(std::make_pair(L"nogallery", new TWIgnoreParserFunction()));
		m_underscoreFunctions.insert(std::make_pair(L"hiddencat", new TWIgnoreParserFunction()));
		m_underscoreFunctions.insert(std::make_pair(L"nocontentconvert", new TWIgnoreParserFunction()));
		m_underscoreFunctions.insert(std::make_pair(L"nocc", new TWIgnoreParserFunction()));
		m_underscoreFunctions.insert(std::make_pair(L"notitleconvert", new TWIgnoreParserFunction()));
		m_underscoreFunctions.insert(std::make_pair(L"notc", new TWIgnoreParserFunction()));
		m_underscoreFunctions.insert(std::make_pair(L"start", new TWIgnoreParserFunction()));
		m_underscoreFunctions.insert(std::make_pair(L"end", new TWIgnoreParserFunction()));
		m_underscoreFunctions.insert(std::make_pair(L"index", new TWIgnoreParserFunction()));
		m_underscoreFunctions.insert(std::make_pair(L"noindex", new TWIgnoreParserFunction()));
		m_underscoreFunctions.insert(std::make_pair(L"staticredirect", new TWIgnoreParserFunction()));
		
		m_parserExtensions.insert(std::make_pair(L"choose", new TWRandomSelectionExtension()));
		
	}catch(...){
		// who cares?
		puts("TWParser: exception while initializing parser functions. ignored.");
	}
	
	m_imageParams[L"left"]=L"horizAlign";
	m_imageParams[L"right"]=L"horizAlign";
	m_imageParams[L"center"]=L"horizAlign";
	m_imageParams[L"none"]=L"horizAlign";
	m_imageParams[L"baseline"]=L"vertAlign";
	m_imageParams[L"sub"]=L"vertAlign";
	m_imageParams[L"super"]=L"vertAlign";
	m_imageParams[L"top"]=L"vertAlign";
	m_imageParams[L"text-top"]=L"vertAlign";
	m_imageParams[L"middle"]=L"vertAlign";
	m_imageParams[L"bottom"]=L"vertAlign";
	m_imageParams[L"text-bottom"]=L"vertAlign";
	m_imageParams[L"thumbnail"]=L"frame";
	m_imageParams[L"thumb"]=L"frame";
	m_imageParams[L"manualthumb"]=L"frame";
	m_imageParams[L"framed"]=L"frame";
	m_imageParams[L"frameless"]=L"frame";
	m_imageParams[L"upright"]=L"frame";
	m_imageParams[L"border"]=L"frame";
	m_imageParams[L"link"]=L"frame";
	m_imageParams[L"alt"]=L"frame";
	
}

TWParser::~TWParser(){
	for(std::multimap<std::wstring, TWParserFunction *>::iterator it=m_parserFunctions.begin();
		it!=m_parserFunctions.end();it++){
		delete it->second;
	}
	for(std::multimap<std::wstring, TWParserFunction *>::iterator it=m_underscoreFunctions.begin();
		it!=m_underscoreFunctions.end();it++){
		delete it->second;
	}
	
	clearTemplateCache();
	delete m_preprocessor;
}



std::wstring TWParser::braceSubstitution(TWPreprocessorNode *node, TWPreprocessorFrame *frame){
	TWPreprocessorNodeTemplate bits=node->splitTemplate();
	std::wstring contents=frame->expand(node, TWPreprocessorFlagsNoArgs|TWPreprocessorFlagsNoTemplates);
	std::wstring title=frame->expand(bits.titleNode, flagsForPreprocessing(frame));
	std::wstring arg0;
	std::wstring::size_type pos;
	
	// TODO: support for subst:
	TWUtils::matchStartAndRemove(title, L"SUBST:");
	TWUtils::matchStartAndRemove(title, L"SAFESUBST:");
	
	// TODO: support for msg, msgnw
	TWUtils::matchStartAndRemove(title, L"MSG:");
	TWUtils::matchStartAndRemove(title, L"MSGNW:");
	
	if((pos=title.find(L':'))!=std::wstring::npos){
		arg0=title.substr(pos+1);
		title=title.substr(0, pos);
	}
	
	title=TWUtils::trimString(title);
	arg0=TWUtils::trimString(arg0);
	
	// sometimes "{{Template:template name}}" exists...
	if(title==L"Template" && !arg0.empty()){
		title.swap(arg0);
		arg0.clear();
	}
	
	int flag=flagsForPreprocessing(frame);
	
	std::wstring funcName=config()->resolveMagicWordAlias(title);
	std::multimap<std::wstring, TWParserFunction *>::iterator it;
	TWUtils::toLowerInplace(funcName);
	if((it=m_parserFunctions.find(funcName))!=m_parserFunctions.end()){
		TWParserFunction *func=it->second;
		if(!func){
			return std::wstring();
		}
		assert(!(func->shouldPreParseArgument()&&func->shouldPostParseResult()));
		if(func->shouldPreParseArgument()){
			assert(!func->shouldPostParseResult());
			TWPreparsedParserFunction *f=static_cast<TWPreparsedParserFunction *>(func);
			std::vector<std::wstring> args;
			for(std::vector<TWPreprocessorNode *>::iterator it2=bits.partNodes.begin();
				it2!=bits.partNodes.end();it2++){
				args.push_back(frame->expand(*it2, flag));
			}
			
			try {
				return TWUtils::trimString(f->execute(arg0, args, frame));
			}catch(const std::wstring& msg){
				return makeError(msg);
			}catch(const std::exception& ex){
				return makeError(TWUtils::m2w(ex.what()));
			}
			
		}else if(func->shouldPostParseResult()){
			TWPostparsedParserFunction *f=static_cast<TWPostparsedParserFunction *>(func);
			TWPreprocessorNode *newNode;
			try{
				newNode=f->execute(arg0, bits.partNodes, frame);
			}catch(const std::wstring& msg){
				return makeError(msg);
			}catch(const std::exception& ex){
				return makeError(TWUtils::m2w(ex.what()));
			}
			if(!newNode)
				return std::wstring();
			return TWUtils::trimString(frame->expand(newNode, flag));
		}else{
			TWSimpleParserFunction *f=static_cast<TWSimpleParserFunction *>(func);
			try{
				return TWUtils::trimString(f->execute(arg0, frame));
			}catch(const std::wstring& msg){
				return makeError(msg);
			}catch(const std::exception& ex){
				return makeError(TWUtils::m2w(ex.what()));
			}
		}
	}
	
	// if this is a parser function, not found.
	if(title[0]==L'#'){
		return makeError(L"unrecognized parser function \""+title+L"\".");
	}
	
	// this seems to be a template.
	TWPreprocessorNode *templateNode;
	TWTitle parsedTitle;
	
	
	
	parsedTitle.ns=TWStandardNamespaceTemplate;
	parsedTitle.dbKey=TWUtils::dbKeyFor(title);
	//try {
		templateNode=getTemplateDom(parsedTitle);
	/*}catch(const std::wstring& msg){
		return makeError(msg);
	}catch(const std::exception& ex){
		return makeError(TWUtils::m2w(ex.what()));
	}*/
	
	assert(templateNode!=NULL);
	
	TWPreprocessorTemplateFrame *childFrame=frame->newChild(bits.partNodes, title);
	std::wstring text;
	text=childFrame->expand(templateNode, flagsForPreprocessing(childFrame)); 
	delete childFrame;
	
	// don't delte templateNode because it is cached.
	
	//text=L"<!-- BEGIN "+title+L" -->"+text
	//+L"<!-- END "+title+L" -->";
	return (text);
}

std::wstring TWParser::argSubstitution(TWPreprocessorNode *piece, TWPreprocessorFrame *frame){
	TWPreprocessorNodeTemplate bits=piece->splitTemplate();
	std::wstring title=frame->expand(bits.titleNode, flagsForPreprocessing(frame));
	std::wstring text;
	
	text=frame->argumentForName(TWUtils::trimString(title));
	
	if(text.empty()){
		if(bits.partNodes.size()>0){
			// use supplied default.
			text=frame->expand(bits.partNodes[0], 0);
		}
	}
	
	return text;
}

std::wstring TWParser::extensionSubstitution(TWPreprocessorNode *params, TWPreprocessorFrame *frame){
	TWPreprocessorNodeExt ext=params->splitExt();
	
	int flag=flagsForPreprocessing(frame);
	
	std::wstring name=frame->expand(ext.nameNode, flag);
	
	std::wstring attrText=L"";
	if(ext.attrNode)
		attrText=frame->expand(ext.attrNode, flag);
	
	std::wstring content=L"";
	if(ext.innerNode)
		content=frame->expand(ext.innerNode, flag);
	
	std::multimap<std::wstring, TWParserExtension *>::iterator it;
	
	it=m_parserExtensions.find(name);
	
	if(it!=m_parserExtensions.end()){
		
		TWParserExtension *ext=it->second;
		
		if(ext==NULL)
			return std::wstring();
		
		try{
			return ext->runHook(this, frame, attrText, content);
		}catch(const std::wstring& msg){
			return makeError(msg);
		}catch(const std::exception& ex){
			return makeError(TWUtils::m2w(ex.what()));
		}
		
	}else{
		// TODO: attributes?
		
		
		if(name==L"css"){
			return std::wstring();
		}
		
		if(content.empty()){
			return L"<"+name+attrText+L"/>";
		}else{
			std::wstring close=L"";
			if(ext.closeNode)
				close=frame->expand(ext.closeNode, flag);
			
			return L"<"+name+attrText+L">"+content+close;
		}
	}
	
}

void TWParser::clearState(){
	
}

int TWParser::flagsForPreprocessing(TWPreprocessorFrame *frame) const{
	int flag=0;
	if(frame->depth()>1)
		flag|=TWPreprocessorFlagsInclusion;
	return flag;
}
std::wstring TWParser::internalParse(const std::wstring& text, TWPreprocessorFrame *frame){
	std::wstring out;
	if(frame){
		
		TWPreprocessorNode *dom;
		dom=preprocessor()->nodeFromText(text, flagsForPreprocessing(frame));
		out=frame->expand(dom, flagsForPreprocessing(frame));
		delete dom;
	}else{
		// TODO: replaceVariables
	}
	
	out=doTableStuff(out);
	// TODO: make <hr />
	if(out.find(L"__")!=std::wstring::npos)
		out=doDoubleUnderscore(out, frame);
	//out=doHeadings(out); - no need
	if(out.find(L'\'')!=std::wstring::npos)
		out=doAllQuotes(out);
	if(out.find(L"[[")!=std::wstring::npos)
		out=replaceInternalLinks(out);
	if(out.find(L"[http://")!=std::wstring::npos)
		out=replaceExternalLinks(out, L"http");
	if(out.find(L"[https://")!=std::wstring::npos)
		out=replaceExternalLinks(out, L"https");
	if(out.find(L"[ftp://")!=std::wstring::npos)
		out=replaceExternalLinks(out, L"ftp");
	
	//out=doMagicLinks(out);
	
	return out;
}

std::wstring TWParser::parse(const std::wstring& text, const std::wstring& title, 
				   bool lineStart, bool doClearState){
	if(doClearState)
		clearState();
	m_externalLinkId=1;
	m_title=title;
	m_parsedTitle=parseTitle(m_title);
	m_displayTitle=TWUtils::displayTitleFor(title);
	m_defaultSort.clear();
	
	TWPreprocessorFrame frame(preprocessor());
	std::wstring out=internalParse(doNoWikis(text), &frame);
	
	out=doBlockLevels(out);
	out=removeEmptyTags(out);
	if(out.find(L"<pre>")!=std::wstring::npos)
		out=removeEmptyTags2(out);
	
	clearTemplateCache();
	
	return out;
}

#pragma mark - Some Stuffs (internal Parse)

std::wstring TWParser::doTableStuff(const std::wstring& text){
	std::list<bool> tdHistory;
	std::list<bool> trHistory;
	std::list<bool> hasOpenedTr;
	std::list<std::wstring> lastTagHistory;
	std::list<std::wstring> trAttributes;
	int indentLevel=0;
	
	std::wstring out;
	std::wstring::size_type pos=0, lineEndPos, nextPos;
	
#if 0 
	puts("\n\n\n\n\n\n\n\n\n\n\n\n");
	puts("***** doTableStuff Input *****");
	XTBDumpWString(text);
	puts("**** end ****");
#endif
	
	while(pos<text.size()){
		assert(tdHistory.size()==trHistory.size());
		assert(tdHistory.size()==hasOpenedTr.size());
		
		lineEndPos=text.find(L'\n', pos);
		if(lineEndPos==std::wstring::npos)
			lineEndPos=text.size();
		
		if(lineEndPos==pos){
			// really empty line. ignore it.
			out+=L'\n';
			pos=lineEndPos+1;
			continue;
		}
		
		wchar_t firstChar=text[pos];
		
		// find like this: ":::{|"
		nextPos=text.find_first_not_of(L':', pos);
		if(nextPos==std::wstring::npos)
			nextPos=text.size();
		
		if(/*text.substr(nextPos, 2)==L"{|"*/
		   nextPos<=text.size()-2 &&
		   text[nextPos]==L'{' &&
		   text[nextPos+1]==L'|'){
			// found!
			indentLevel=nextPos-pos;
			
			// add indents.
			for(int i=0;i<indentLevel;i++)
				out+=L"<dl><dd>";
			
			// write table and attributes.
			pos=nextPos+2;
			out+=L"<table ";
			out+=TWUtils::sanitizeAttribute(text.substr(pos, lineEndPos-pos));
			out+=L'>';
			
			// push.
			tdHistory.push_back(false);
			lastTagHistory.push_back(std::wstring());
			trHistory.push_back(false);
			trAttributes.push_back(std::wstring());
			hasOpenedTr.push_back(false);
			
			pos=lineEndPos+1;
			out+=L'\n';
			continue;
		}
		
		if(tdHistory.empty()){
			// nothing to do.
			out.append(text, pos, lineEndPos-pos+1);
			pos=lineEndPos+1;
			continue;
		}
		
		if(text.substr(pos, 2)==L"|}"){
			// we are ending a table.
			if(!hasOpenedTr.back())
				out+=L"<tr><td></td></tr>";
			
			if(tdHistory.back()){
				out+=L"</";
				out+=lastTagHistory.back();
				out+=L'>';
			}
			
			if(trHistory.back())
				out+=L"</tr>";
			hasOpenedTr.pop_back();
			trHistory.pop_back();
			tdHistory.pop_back();
			trAttributes.pop_back();
			lastTagHistory.pop_back();
			out+=L"</table>";
			pos+=2;
			//lineEndPos=pos+2;
			out.append(text, pos, lineEndPos-pos);
			for(int i=0;i<indentLevel;i++)
				out+=L"</dd></dl>";
			out+=L'\n';
			pos=lineEndPos+1;
			continue;
		}
		
		// there is a case that | or ! is
		// not first char because of whitespaces.
		if(firstChar==L' ' || firstChar==L'\t'){
			std::wstring::size_type realFirstPos;
			realFirstPos=TWUtils::findInRangeFirstOf(text, L"|!", pos, lineEndPos-pos);
			if(realFirstPos!=std::wstring::npos &&
			   realFirstPos<lineEndPos){
				pos=realFirstPos;
				firstChar=text[realFirstPos];
			}
		}
		
		if(text.substr(pos, 2)==L"|-"){
			// now we have a table row. (original says so.)
			pos+=2;
			trAttributes.back()=text.substr(pos, lineEndPos-pos);
			hasOpenedTr.back()=true;
			
			// 1: here was fixed.
			
			if(tdHistory.back()){
				out+=L"</";
				out+=lastTagHistory.back();
				out+=L'>';
			}
			
			if(trHistory.back()){
				out+=L"</tr>";
			}
			
			trHistory.back()=false;
			tdHistory.back()=false;
			lastTagHistory.back()=std::wstring();
			
			out+=L'\n';
			pos=lineEndPos+1;
			continue;
		}
		
		if(firstChar==L'|' || firstChar==L'!'){
			if(firstChar==L'|' && text[pos+1]==L'+'){
				// caption?
				firstChar=L'+';
				pos++;
			}
			
			pos++;
			
			std::wstring line=text.substr(pos, lineEndPos-pos);
			
			if(firstChar==L'!'){
				// replace !! -> ||
				std::wstring::size_type pos2=0, nextPos2;
				while(pos2<line.size()){
					nextPos2=line.find(L"!!", pos2);
					if(nextPos2==std::wstring::npos){
						break;
					}
					line[nextPos2]=L'|';
					line[nextPos2+1]=L'|';
					pos2=nextPos2+2;
				}
			}
			
			TWStringSplitter<64> cells(line, L"||");
			for(int i=0;i<cells.elementCount();i++){
				std::wstring previous;
				wchar_t *cell=cells[i];
				
				if(firstChar!=L'+'){
					if(!trHistory.back()){
						previous=L"<tr ";
						previous+=TWUtils::sanitizeAttribute(trAttributes.back());
						previous+=L">\n";
					}
					trHistory.back()=true;
					trAttributes.back()=std::wstring();
					hasOpenedTr.back()=true;
				}
				
				if(tdHistory.back()){
					std::wstring s;
					s=L"</";
					s+=lastTagHistory.back();
					s+=L">";
					previous.insert(0, s);
				}
				
				if(firstChar==L'|'){
					lastTagHistory.back()=L"td";
				}else if(firstChar==L'!'){
					lastTagHistory.back()=L"th";
				}else if(firstChar==L'+'){
					lastTagHistory.back()=L"caption";
				}else{
					// this shouldn't happen?
					lastTagHistory.back()=std::wstring();
					assert(false);
				}
				
				out+=previous;
				
				wchar_t *cellSplit=wcschr(cell, L'|');
				if(cellSplit==NULL){
					out+=L'<';
					out+=lastTagHistory.back();
					out+=L'>';
					out+=cell;
				}else{
					// deal with bug 553.
					wchar_t *invalidLink=wcsstr(cell, L"[[");
					if(invalidLink && (invalidLink<cellSplit)){
						out+=L'<';
						out+=lastTagHistory.back();
						out+=L'>';
						out+=cell;
					}else{
						out+=L'<';
						out+=lastTagHistory.back();
						out+=L' ';
						out+=TWUtils::sanitizeAttribute(std::wstring(cell, cellSplit-cell));
						out+=L'>';
						out.append(cellSplit+1);
					}
				}
				
				tdHistory.back()=true;
			}
			
			out+=L'\n';
			pos=lineEndPos+1;
			
			continue;
		}
		
		out.append(text, pos, lineEndPos-pos+1);
		pos=lineEndPos+1;
		continue;
		
		
		
		
	}
	
	// closing open td, tr && table.
	while(!tdHistory.empty()){
		if(tdHistory.back()){
			out+=L"</td>\n";
		}
		if(trHistory.back()){
			out+=L"</tr>\n";
		}
		if(!hasOpenedTr.back()){
			out+=L"<tr><td></td></tr>\n";
		}
		tdHistory.pop_back();
		trHistory.pop_back();
		hasOpenedTr.pop_back();
		out+=L"</table>\n";
	}
#if 0
	puts("\n\n\n\n\n\n\n\n\n\n\n\n");
	puts("***** doTableStuff OUTPUT *****");
	XTBDumpWString(out);
	puts("**** end ****");
	puts("\n\n\n\n\n\n\n\n\n\n\n\n");
#endif
	return out;
}
std::wstring TWParser::doDoubleUnderscore(const std::wstring& text, TWPreprocessorFrame *frame){
	std::wstring out;
	std::wstring::size_type pos=0, nextPos;
	
	while(pos<text.size()){
		nextPos=text.find(L"__", pos);
		if(nextPos==std::wstring::npos){
			out.append(text, pos, text.size()-pos);
			break;
		}
		
		out.append(text, pos, nextPos-pos);
		
		pos=nextPos+2; // skipping opening "__"
		
		nextPos=text.find(L"__", pos);
		if(nextPos==std::wstring::npos || nextPos>pos+64){
			// closing "__" not found, or too far.
			out+=L"__";
			continue;
		}
		
		std::wstring innerText=text.substr(pos, nextPos-pos);
		std::wstring funcName=TWUtils::toLower(innerText);
		std::multimap<std::wstring, TWParserFunction *>::iterator it;
		
		it=m_underscoreFunctions.find(funcName);
		
		if(it==m_underscoreFunctions.end()){
			// function not found.
			out+=L"__";
			continue;
		}
		
		TWParserFunction *func=it->second;
		assert(!(func->shouldPreParseArgument()&&func->shouldPostParseResult()));
		if(func->shouldPreParseArgument()){
			assert(!func->shouldPostParseResult());
			TWPreparsedParserFunction *f=static_cast<TWPreparsedParserFunction *>(func);
			try {
				out+=f->execute(std::wstring(), std::vector<std::wstring>(), frame);
			}catch(const std::wstring& msg){
				out+=makeError(msg);
			}catch(const std::exception& ex){
				out+=makeError(TWUtils::m2w(ex.what()));
			}
			
		}else if(func->shouldPostParseResult()){
			TWPostparsedParserFunction *f=static_cast<TWPostparsedParserFunction *>(func);
			TWPreprocessorNode *newNode=NULL;
			try{
				newNode=f->execute(std::wstring(),std::vector<TWPreprocessorNode *>(), frame);
			}catch(const std::wstring& msg){
				out+=makeError(msg);
			}catch(const std::exception& ex){
				out+=makeError(TWUtils::m2w(ex.what()));
			}
			if(newNode)
				out+=frame->expand(newNode, 0);
		}else{
			TWSimpleParserFunction *f=static_cast<TWSimpleParserFunction *>(func);
			try{
				out+=f->execute(std::wstring(), frame);
			}catch(const std::wstring& msg){
				out+=makeError(msg);
			}catch(const std::exception& ex){
				out+=makeError(TWUtils::m2w(ex.what()));
			}
		}
		
		pos=nextPos+2;
		
	}
	
	return out;
}
std::wstring TWParser::doHeadings(const std::wstring& text){
	return text;
	// don't need to implement because this is done by preprocessor.
}
std::wstring TWParser::doAllQuotes(const std::wstring& text){
	std::wstring out;
	std::wstring::size_type pos;
	typedef enum{
		QuoteStateNone=0,
		QuoteStateB,
		QuoteStateI,
		QuoteStateBI,
		QuoteStateIB
	} QuoteState;
	QuoteState state=QuoteStateNone;
	
	pos=0;
	while(pos<text.size()){
		std::wstring::size_type newPos;
		newPos=text.find_first_of(L"'\n", pos);
		if(newPos==std::wstring::npos){
			out.append(text, pos, text.size()-pos);
			break;
		}
		
		out.append(text, pos, newPos-pos);
		
		if(text[newPos]==L'\n'){
			// new line. clear the state.
			switch(state){
				case QuoteStateNone:
					break;
				case QuoteStateB:
					out+=L"</b>";
					break;
				case QuoteStateI:
					out+=L"</i>";
					break;
				case QuoteStateBI:
					out+=L"</i></b>";
					break;
				case QuoteStateIB:
					out+=L"</b></i>";
					break;
			}
			state=QuoteStateNone;
			out+=L'\n';
			pos=newPos+1;
		}else{
			assert(text[newPos]==L'\'');
			pos=newPos;
			newPos=text.find_first_not_of(L"'", pos);
			if(newPos==std::wstring::npos)
				newPos=text.size();
			int count=newPos-pos;
			switch(count){
				case 1:
					out+=L'\'';
					break;
				case 2:
					switch(state){
						case QuoteStateNone:
							out+=L"<i>";
							state=QuoteStateI;
							break;
						case QuoteStateB:
							out+=L"<i>";
							state=QuoteStateBI;
							break;
						case QuoteStateI:
							out+=L"</i>";
							state=QuoteStateNone;
							break;
						case QuoteStateBI:
							out+=L"</i>";
							state=QuoteStateB;
							break;
						case QuoteStateIB:
							out+=L"</b></i><b>";
							state=QuoteStateB;
							break;
					}
					break;
				case 3:
					switch(state){
						case QuoteStateNone:
							out+=L"<b>";
							state=QuoteStateB;
							break;
						case QuoteStateB:
							out+=L"</b>";
							state=QuoteStateNone;
							break;
						case QuoteStateI:
							out+=L"<b>";
							state=QuoteStateIB;
							break;
						case QuoteStateBI:
							out+=L"</i></b><i>";
							state=QuoteStateI;
							break;
						case QuoteStateIB:
							out+=L"</b>";
							state=QuoteStateI;
							break;
					}
					break;
				case 5:
					switch(state){
						case QuoteStateNone:
							out+=L"<b><i>";
							state=QuoteStateBI;
							break;
						case QuoteStateB:
							out+=L"</b><i>";
							state=QuoteStateI;
							break;
						case QuoteStateI:
							out+=L"</i><b>";
							state=QuoteStateB;
							break;
						case QuoteStateBI:
							out+=L"</i></b>";
							state=QuoteStateNone;
							break;
						case QuoteStateIB:
							out+=L"</b></i>";
							state=QuoteStateNone;
							break;
					}
					break;
			}
			pos=newPos;
			
		}
	}
	
	// done.
	switch(state){
		case QuoteStateNone:
			break;
		case QuoteStateB:
			out+=L"</b>";
			break;
		case QuoteStateI:
			out+=L"</i>";
			break;
		case QuoteStateBI:
			out+=L"</i></b>";
			break;
		case QuoteStateIB:
			out+=L"</b></i>";
			break;
	}
	
	return out;
	
}
std::wstring TWParser::replaceInternalLinks(std::wstring& text){
	std::wstring out;
	std::wstring::size_type pos=0;

	// I'm very sad to see Parser.php because it is very very badly implemented.
	while(pos<text.size()){
		std::wstring::size_type newPos=text.find(L"[[", pos), nextPos;
		if(newPos==std::wstring::npos){
			out.append(text, pos, text.size()-pos);
			break;
		}
		out.append(text, pos, newPos-pos);
		pos=newPos+2;
		newPos=TWUtils::findMatchingBracket(text, pos-1);
		if(newPos==std::wstring::npos){
			newPos=text.size();
			nextPos=newPos;
		}else{
			nextPos=newPos+2;
		}
		
		TWStringSplitter<2> splitter(text, L"|", pos, newPos-pos);
		std::wstring title=splitter[0];
		TWTitle parsedTitle=parseTitle(title);
		
		if(parsedTitle.isInterWiki()){
			// language link. ignoring.
			// TODO: handle language links.
			pos=nextPos;
			continue;
		}
		
		if(TWUtils::matchStart(title, L"category:")){
			// category. ignoring.
			pos=nextPos;
			continue;
		}
		
		if(TWUtils::matchStart(title, L"Category:")){
			// category. ignoring.
			pos=nextPos;
			continue;
		}
		
		if(parsedTitle.ns==TWStandardNamespaceFile){
			std::wstring imgOut;
			if(splitter.elementCount()==1)
				imgOut=makeImage(parsedTitle, title);	
			else
				imgOut=makeImage(parsedTitle, splitter[1]);
			out+=replaceInternalLinks(imgOut);
			pos=nextPos;
			continue;
		}
			
		if(parsedTitle.ns==TWStandardNamespaceCategory){
			// category specifier. ignoring.
			pos=nextPos;
			continue;
		}
		
		std::wstring displayText;
		if(splitter.elementCount()==1){
			displayText=splitter[0];
		}else{
			displayText=splitter[1];
			if(displayText.empty()){
				displayText=splitter[0];
				std::wstring::size_type ttlPos=displayText.find(L':');
				if(ttlPos!=std::wstring::npos)
					displayText=displayText.substr(ttlPos+1);
			}
		}
		
		if(parsedTitle==m_parsedTitle){
			out+=makeSelfLink(displayText);
			pos=nextPos;
			continue;
		}
		
		out+=makeInternalLink(parsedTitle, displayText);
		pos=nextPos;
		
	}
	return out;
}
std::wstring TWParser::replaceExternalLinks(const std::wstring& text,
											const std::wstring& scheme){
	std::wstring outStr;
	std::wstring::size_type pos=0, nextPos, pos2;
	std::wstring search=L"["+scheme+L":";
	while(pos<text.size()){
		nextPos=text.find(search, pos);
		if(nextPos==std::wstring::npos){
			outStr.append(text, pos, text.size()-pos);
			break;
		}
		
		outStr.append(text, pos, nextPos-pos);
		pos=nextPos+1;
		
		nextPos=text.find_first_of(L" ]", pos);
		if(nextPos==std::wstring::npos){
			// invalid link.
			outStr+=L'[';
			continue;
		}
		pos2=pos;
		
		std::wstring url=text.substr(pos, nextPos-pos);
		std::wstring title;
		if(text[nextPos]==L']'){
			// numbered link.
			title=L"[";
			title+=TWUtils::stringWithIntValue(m_externalLinkId);
			title+=L"]";
			m_externalLinkId++;
		}else{
			pos=nextPos+1;
			nextPos=text.find_first_of(L']', pos);
			if(nextPos==std::wstring::npos){
				// invalid link.
				pos=pos2;
				outStr+=L'[';
				continue;
			}
			
			title=text.substr(pos, nextPos-pos);
		}
		pos=nextPos+1;
		
		//title=TWUtils::escapeSpecialChars(title);
		
		url=TWUtils::escapeHTMLSpecialChars(url);
		
		outStr+=L"<a href=\""+url+L"\">"+title+L"</a>";
		
	}
	
	return outStr;
}
std::wstring TWParser::doMagicLinks(const std::wstring& text){
	// TODO: implement!
	return text;
	return makeError(L"magic link isn't implemented!")+text;
}

std::wstring TWParser::recursiveTagParse(const std::wstring &text, TWPreprocessorFrame *frame){
	return internalParse(text, frame);
}

#pragma mark - Some Stuffs (Parse)

struct TWParserBlockState{
	std::wstring lastSection;
	bool inBlock;
	bool inPre;
	bool dtOpen;
	TWParserBlockState():
	inBlock(false),
	inPre(false),
	dtOpen(false){}
};

struct TWParserTagMatch{
	bool openMatch, closeMatch;
	TWParserTagMatch():
	openMatch(false),
	closeMatch(false){}
};

static TWParserTagMatch tagMatch(const std::wstring& str, std::wstring::size_type pos,
								 std::wstring::size_type n){
	TWParserTagMatch match;
	n+=pos;
	while(pos<n){
		std::wstring::size_type nextPos;
		nextPos=TWUtils::findInRange(str, L"<", pos, n-pos);
		if(nextPos==std::wstring::npos)
			break;
		pos=nextPos+1;

		bool isClose=false;
		if(str[pos]==L'/'){
			isClose=true; pos++;
		}
		nextPos=TWUtils::findInRangeFirstOf(str, L" />", pos, n-pos);
		if(nextPos==std::wstring::npos)
			nextPos=n;
		
		std::wstring tagName=str.substr(pos, nextPos-pos);
		if(isBlockElement(tagName.c_str())){
		
			if(isClose)
				match.closeMatch=true;
			else
				match.openMatch=true;
		}else{
			assert(tagName!=L"div");
		}
		
		pos=nextPos;
		nextPos=TWUtils::findInRange(str, L">", pos, n-pos);
		if(nextPos==std::wstring::npos)
			break;
		pos=nextPos+1;
	}
	return match;
}

static bool containsSomething(const std::wstring& str, std::wstring::size_type pos,
							  std::wstring::size_type n){
	return str.find_first_not_of(L" ", pos, n)!=std::wstring::npos;
}

static inline wchar_t generatePrefix2(wchar_t in){
	if(in==L';')
		return L':';
	return in;
} 

std::wstring TWParser::closeParagraph(TWParserBlockState& state) const{
	state.inPre=false;
	if(!state.lastSection.empty()){
		std::wstring ret=L"</"+state.lastSection+L">\n";
		state.lastSection.clear();
		return ret;
	}
	return std::wstring();
}

const wchar_t *TWParser::openList(wchar_t c, TWParserBlockState& state) const{
	
	switch(c){
		case L'*':
			return L"<ul><li>";
		case L'#':
			return L"<ol><li>";
		case L':':
			
			return L"<dl><dd>";
		case L';':
			state.dtOpen=true;
			
			return L"<dl><dt>";
		default:
			printf("[TWiki] TWParser::openList: oops, invalid list char: '%lc!'\n", c);
			fflush(stdout);
			assert("invalid list char!"==0);
			return NULL;
	}
}

const wchar_t *TWParser::nextItem(wchar_t c, TWParserBlockState& state) const{
	switch(c){
		case L'*':
		case L'#':
			return L"</li><li>";
		case L':':
			
			if(state.dtOpen){
				state.dtOpen=false;
				return L"</dt><dd>";
			}else{
				return L"</dd><dd>";
			}
		case L';':
			
			if(state.dtOpen){
				return L"</dt><dt>";
			}else{
				state.dtOpen=true;
				return L"</dd><dt>";
			}
		default:
			assert("invalid list char!"==0);
			return NULL;
	}
}
const wchar_t *TWParser::closeList(wchar_t c, TWParserBlockState& state) const{
	switch(c){
		case L'*':
			return L"</li></ul>";
		case L'#':
			return L"</li></ol>";
		case L':':
		case L';':
			
			if(state.dtOpen){
				state.dtOpen=false;
				return L"</dt></dl>";
			}else{
				return L"</dd></dl>";
			}
		default:
			assert("invalid list char!"==0);
			return NULL;
	}
}

std::wstring TWParser::doBlockLevels(std::wstring const &text) {
	// I'm very sad to see how the original routine works.
	
	std::wstring out;
	TWParserBlockState state;
	std::wstring lastPrefix, prefix, prefix2;
	std::wstring paragraphStack;
	
	std::wstring::size_type pos=0;
	
	while(pos<text.size()){
		
		std::wstring::size_type nextPos=pos;
		while(true){
			nextPos=text.find_first_of((text[pos]==L' '||state.inPre)?L"\n":L"\n<", nextPos);
			if(nextPos==std::wstring::npos){
				nextPos=text.size();
				break;
			}
			if(text[nextPos]==L'<'){
				
				
				nextPos=text.find(L'>', nextPos);
				if(nextPos==std::wstring::npos){
					nextPos=text.size();
					break;
				}
			}else{
				break;
			}
		}
		
		std::wstring::size_type outPos=pos;
		bool preOpenMatch=TWUtils::findInRange(text, L"<pre", outPos, nextPos-outPos)!=std::wstring::npos;
		bool preCloseMatch=TWUtils::findInRange(text, L"</pre", outPos, nextPos-outPos)!=std::wstring::npos;
		// TODO: support for <pre /> 
		
		if(!state.inPre){
			std::wstring::size_type prefixEndPos=TWUtils::findInRangeFirstNotOf(text, L"*#:;", pos, nextPos-pos);
			if(prefixEndPos==std::wstring::npos)
				prefixEndPos=nextPos;
			assert(prefixEndPos<=nextPos);
			//printf("%d\n", (int)(prefixEndPos-pos));
			prefix=text.substr(pos, prefixEndPos-pos);
			assert(prefix.find_first_not_of(L"*#:;")==std::wstring::npos);
			
			// replace L';' -> L':'
			prefix2=prefix;
			std::transform(prefix2.begin(), prefix2.end(), prefix2.begin(), generatePrefix2);
			
			outPos+=prefix.size();
			
			state.inPre=preOpenMatch;
		}else{
			// in preformated text. no prefix.
			prefix.clear();
		}
		
		if(!prefix.empty() && lastPrefix==prefix2){
			// needn't open nor close.
			out+=nextItem(prefix[prefix.size()-1], state);
			paragraphStack.clear();
			if(prefix[prefix.size()-1]==L';'){
				std::wstring::size_type colonPos;
				colonPos=TWUtils::findColonNoLinks(text, outPos, nextPos-outPos);
				if(colonPos!=std::wstring::npos){
					assert(colonPos<nextPos);
					// in this pattern: "; alice : bob"
					out.append(text, outPos, colonPos-outPos);
					out+=nextItem(L':', state);
					outPos=colonPos+1;
				}
			}
		}else if(!prefix.empty() || !lastPrefix.empty()){
			// need to open or close.
			std::wstring::size_type commonPrefixLength;
			commonPrefixLength=TWUtils::commonCharCount(prefix2, lastPrefix);
			paragraphStack.clear();
			
			//printf("%ls, %ls, %d\n", prefix2.c_str(), lastPrefix.c_str(), (int)commonPrefixLength);
			
			// close the non-shared prefixes.
			while(lastPrefix.size()>commonPrefixLength){
				out+=closeList(lastPrefix[lastPrefix.size()-1], state);
				lastPrefix.resize(lastPrefix.size()-1);
			}
			
			// continue the appropriate prefix, if needed.
			if(prefix.size()<=commonPrefixLength && commonPrefixLength>0){
				out+=nextItem(prefix[commonPrefixLength-1], state);
			}
			
			// open prefixes.
			while(prefix.size()>commonPrefixLength){
				wchar_t c=prefix[commonPrefixLength];
				out+=closeParagraph(state);
				out+=openList(c, state);
				if(c==L';'){
					std::wstring::size_type colonPos;
					colonPos=TWUtils::findColonNoLinks(text, outPos, nextPos-outPos);
					if(colonPos!=std::wstring::npos){
						assert(colonPos<nextPos);
						// this code again...
						out.append(text, outPos, colonPos-outPos);
						out+=nextItem(L':', state);
						outPos=colonPos+1;
					}
				}
				commonPrefixLength++;
			}
			lastPrefix=prefix2;
		}
		
		// If we have no prefixes, it's paragraph-time!
		if(prefix.empty()){
			TWParserTagMatch match;
			match=tagMatch(text, outPos, nextPos-outPos);
			if(match.openMatch || match.closeMatch){
				// block tag exists.
				paragraphStack.clear();
				out+=closeParagraph(state);
				
				if(preOpenMatch && !preCloseMatch){
					 // <pre> found, and no </pre> found.
					state.inPre=true;
				}
				
				if(match.closeMatch){
					state.inBlock=false;
				}else{
					state.inBlock=true;
				}
			}else if(!state.inBlock && !state.inPre){
				// currently, text is in <p> or <pre> ...
				bool nonEmpty=containsSomething(text, outPos, nextPos-outPos);
				if(text[outPos]==L' ' && (state.lastSection==L"pre" || 
										  nonEmpty)){
					// pre
					if(state.lastSection!=L"pre"){
						paragraphStack.clear();
						out+=closeParagraph(state);
						out+=L"<pre>";
						state.lastSection=L"pre";
					}
					outPos++;
				}else{
					// paragraph
					if(!nonEmpty){
						if(!paragraphStack.empty()){
							out+=paragraphStack;
							out+=L"<br />";
							paragraphStack.clear();
							state.lastSection=L"p";
						}else{
							if(state.lastSection!=L"p"){
								out+=closeParagraph(state);
								state.lastSection.clear();
								paragraphStack=L"<p>";
							}else{
								paragraphStack=L"</p><p>";
							}
						}
					}else{
						if(!paragraphStack.empty()){
							out+=paragraphStack;
							paragraphStack.clear();
							state.lastSection=L"p";
						}else if(state.lastSection!=L"p"){
							out+=closeParagraph(state);
							out+=L"<p>";
							state.lastSection=L"p";
						}
					}
				}
			}
		}
		
		// according to bug 785
		if(preCloseMatch && state.inPre){
			state.inPre=false;
		}
		
		if(paragraphStack.empty()){
			out.append(text, outPos, nextPos-outPos);
			out+=L"\n";
		}
		
		pos=nextPos+1;
		
	}
	
	while(!prefix.empty()){
		out+=closeList(prefix[prefix.size()-1], state);
		prefix.resize(prefix.size()-1);
	}
	
	if(!state.lastSection.empty()){
		out+=L"</"; out+=state.lastSection; out+=L">";
	}
	
	return out;
	
}

std::wstring TWParser::doNoWikis(const std::wstring &text){
	std::wstring out;
	std::wstring::size_type pos=0, newPos;
	while(pos<text.size()){
		newPos=text.find(L"<nowiki", pos);
		if(newPos==std::wstring::npos){
			out.append(text, pos, text.size()-pos);
			break;
		}
		out.append(text, pos, newPos-pos);
		pos=newPos;
		newPos=text.find(L'>', pos);
		if(newPos==std::wstring::npos){
			// no closed.
			out+=L'<';
			pos++;
			continue;
		}
		pos=newPos+1;
		
		// find close.
		newPos=text.find(L"</nowiki", pos);
		if(newPos==std::wstring::npos){
			// not closed.
			out+=TWUtils::escapeSpecialChars(text.substr(pos));
			break;
		}
		
		out+=TWUtils::escapeSpecialChars(text.substr(pos, newPos-pos));
		
		pos=newPos+1;
		newPos=text.find(L'>', pos);
		if(newPos==std::wstring::npos){
			break;
		}
		pos=newPos+1;
	}
	return out;
}

std::wstring TWParser::removeEmptyTags(const std::wstring& text){
	std::wstring out;
	std::wstring::size_type pos=0;
	while(pos<text.size()){
		std::wstring::size_type newPos;
		newPos=text.find(L"<p>", pos);
		if(newPos==std::wstring::npos){
			out.append(text, pos, text.size()-pos);
			break;
		}
		out.append(text, pos, newPos-pos);
		
		pos=newPos+3;
		
		newPos=text.find(L"</p>", pos);
		if(newPos==std::wstring::npos){
			// this shouldn't happen --- no </p> found!
			pos-=3;
			out.append(text, pos, text.size()-pos);
			out+=L"</p>";
			break;
		}
		
		if(TWUtils::findInRangeFirstNotOf(text, L" \n\r\t", pos, newPos-pos)==std::wstring::npos){
			// no content -- remove.
			newPos+=4;
		}else{
			// has content.
			newPos+=4;
			pos-=3;
			out.append(text, pos, newPos-pos);
		}
		
		pos=newPos;
	}
	
	return out;
}

std::wstring TWParser::removeEmptyTags2(const std::wstring& text){
	std::wstring out;
	std::wstring::size_type pos=0;
	while(pos<text.size()){
		std::wstring::size_type newPos;
		newPos=text.find(L"<pre>", pos);
		if(newPos==std::wstring::npos){
			out.append(text, pos, text.size()-pos);
			break;
		}
		out.append(text, pos, newPos-pos);
		
		pos=newPos+5;
		
		newPos=text.find(L"</pre>", pos);
		if(newPos==std::wstring::npos){
			// this shouldn't happen --- no </pre> found!
			pos-=5;
			out.append(text, pos, text.size()-pos);
			out+=L"</pre>";
			break;
		}
		
		if(TWUtils::findInRangeFirstNotOf(text, L" \n\r\t", pos, newPos-pos)==std::wstring::npos){
			// no content -- remove.
			newPos+=6;
		}else{
			// has content.
			newPos+=6;
			pos-=5;
			out.append(text, pos, newPos-pos);
		}
		
		pos=newPos;
	}
	
	return out;
}


std::wstring TWParser::makeImage(const TWTitle& file, const std::wstring& param){
	static const std::wstring imageParamDelimiter=L"|";
	std::vector<std::wstring> parts=TWUtils::split(param, imageParamDelimiter);
	std::map<std::wstring, std::map<std::wstring,std::wstring> >
	params;
	std::wstring caption;
	for(size_t i=0;i<parts.size();i++){
		std::wstring part=TWUtils::trimString(parts[i]);
		bool validated=false;
		if(part.rfind(L"px")==part.size()-2){
			if(part.find(L"x")<part.size()-2){
				// there is height.
				size_t pos=part.find(L"x");
				params[L"handler"][L"width"]=part.substr(0, pos);
				params[L"handler"][L"height"]=part.substr(pos+1, part.size()-pos-3);
			}else{
				params[L"handler"][L"width"]=part.substr(0, part.size()-2);
			}
			continue;
		}
		
		size_t equalPos=part.find(L'=');
		std::wstring name, value;
		if(equalPos!=std::wstring::npos){
			name=part.substr(0, equalPos);
			value=part.substr(equalPos+1);
		}else{
			name=part;
		}
		TWUtils::toLowerInplace(name);
		
		std::map<std::wstring, std::wstring>::iterator it;
		
		if((it=m_imageParams.find(name))!=m_imageParams.end()){
			if(name==L"manualthumb" || name==L"alt"){
				validated=true;
			}else if(name==L"link"){
				if(value.empty()){
					name=L"no-link";
				}else if(TWUtils::matchStart(value, L"http:")
						 || TWUtils::matchStart(value, L"ftp:")){
					name=L"link-url";
				}else{
					name=L"link-title";
				}
				validated=true;
			}else if(name==L"thumb"){
				validated=true;
				name=L"thumbnail";
			}else{
				if(value.empty()){
					validated=true;
				}else{
					validated=TWUtils::isNumeric(value);
				}
			}
			
			if(validated){
				params[it->second][name]=value;
			}
			
		}
		
		if(!validated){
			if(!caption.empty())
				caption+=L'|';
			caption+=part;
		}
		
	}
	
	if(params.find(L"horizAlign")!=params.end()){
		params[L"frame"][L"align"]=(params.find(L"horizAlign")->second).begin()->first;
	}
	
	if(params.find(L"vertAlign")!=params.end()){
		params[L"frame"][L"valign"]=(params.find(L"vertAlign")->second).begin()->first;
	}
	
	bool imageIsFramed=false;
	
	if(params[L"frame"].find(L"frame")!=params[L"frame"].end()){
		imageIsFramed=true;
	}else if(params[L"frame"].find(L"framed")!=params[L"frame"].end()){
		imageIsFramed=true;
	}else if(params[L"frame"].find(L"thumbnail")!=params[L"frame"].end()){
		imageIsFramed=true;
	}else if(params[L"frame"].find(L"manualthumb")!=params[L"frame"].end()){
		imageIsFramed=true;
	}
	
	params[L"frame"][L"caption"]=caption;
	
	if(imageIsFramed){
		if(caption.empty() && params[L"frame"].find(L"alt")==params[L"frame"].end()){
			params[L"frame"][L"alt"]=expandTitle(file);
		}
	}else{
		if(params[L"frame"].find(L"alt")==params[L"frame"].end()){
			if(caption.empty()){
				params[L"frame"][L"alt"]=expandTitle(file);
			}else{
				params[L"frame"][L"alt"]=caption;
			}
		}
		params[L"frame"][L"title"]=caption;
	}
	
	return makeImage2(file, params[L"frame"], params[L"handler"]);
}

#pragma mark - Utils

TWTitle TWParser::parseTitle(const std::wstring& str) const{
	TWTitle ttl;
	std::wstring::size_type pos=str.find(L':');
	if(pos!=std::wstring::npos){
		std::wstring prefix=str.substr(0, pos);
		int ns=config()->namespaceWithName(config()->resolveNamespaceAlias(prefix));
		if(ns!=TWStandardNamespaceInvalid){
			ttl.dbKey=str.substr(pos+1);
			ttl.ns=ns;
		}else if(doesLanguageExists(prefix)){
			ttl.dbKey=str.substr(pos+1);
			ttl.interWiki=prefix;
			ttl.ns=TWStandardNamespaceInvalid;
		}else{
			ttl.dbKey=str;
			ttl.ns=TWStandardNamespaceMain;
		}
	}else{
		ttl.dbKey=str;
		ttl.ns=TWStandardNamespaceMain;
	}
	ttl.dbKey=TWUtils::dbKeyFor(ttl.dbKey);
	return ttl;
}

std::wstring TWParser::expandTitle(const TWTitle& ttl) const{
	std::wstring out;
	if(ttl.isInterWiki())
		out=ttl.interWiki+L":";
	if(ttl.ns!=TWStandardNamespaceMain)
		out+=config()->nameForStandardNamespace(ttl.ns)+L":";
	out+=ttl.dbKey;
	return out;
}

void XTBDumpWString(const std::wstring&);

TWPreprocessorNode *TWParser::getTemplateDom(const TWTitle& ttl){
	std::map<std::wstring, TWPreprocessorNode *>::iterator it;
	it=m_cachedTemplates.find(ttl.dbKey);
	if(it!=m_cachedTemplates.end()){
		return it->second;
	}
	
	
	std::wstring txt=fetchPage(ttl);
	TWPreprocessorNode *node;
	
	
	node=preprocessor()->nodeFromText(txt, TWPreprocessorFlagsInclusion);
	
	m_cachedTemplates[ttl.dbKey]=node;
	
#if 0
	//if(txt.find(L"infoboxCountryName")!=std::wstring::npos){
	printf("!!! HEY TAKE A LOOK AT THIS: THIS IS ...s contents:\n");
	
	XTBDumpWString(ttl.dbKey);
	XTBDumpWString(node->toString());
	
	puts("!!! DONE.");
	//}
#endif
	
	return node;
}

void TWParser::clearTemplateCache(){
	std::map<std::wstring, TWPreprocessorNode *>::iterator it;
	for(it=m_cachedTemplates.begin();it!=m_cachedTemplates.end();it++){
		delete it->second;
	}
	m_cachedTemplates.clear();
}

bool TWParser::doesParserExtensionExists(const std::wstring & tag) const{
	if(m_parserExtensions.find(tag)==m_parserExtensions.end())
		return false;
	return true;
}

#pragma mark - Customize

std::wstring TWParser::fetchPage(const TWTitle&) const{
	throw std::wstring(L"fetchPage is not implemented!");
}

bool TWParser::doesLanguageExists(const std::wstring& name) const{
	if(name.size()<=3)
		return true;
	if(name[2]==L'-' && name.size()<=6)
		return true;
	if(name.find(L'-')<=3)
		return true;
	if(name==L"simple")
		return true;
	return false;
}

std::wstring TWParser::makeSelfLink(const std::wstring& displayText){
	return L"<b>"+displayText+L"</b>";
}

std::wstring TWParser::makeInternalLink(const TWTitle& referenceTo, const std::wstring& displayText){
	return L"<a href='twiki:"+TWUtils::escapeHTMLSpecialChars(expandTitle(referenceTo))+L"'>"+displayText+L"</a>";
}



std::wstring TWParser::makeImage2(const TWTitle& file, std::map<std::wstring,std::wstring>& frameParam, std::map<std::wstring,std::wstring>& handlerParam){
	return L"<img src='twiki:"+TWUtils::escapeHTMLSpecialChars(expandTitle(file))+L"' />";
}

std::wstring TWParser::makeError(std::wstring const & errMsg) const{
	return L"<span style='color: gray; font-weight: bold;'>"
	+errMsg+L"</span> ";
}


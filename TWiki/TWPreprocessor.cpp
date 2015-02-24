/*
 *  TWPreprocessor.cpp
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

#include "TWPreprocessor.h"
#include <stack>
#include "TWPreprocessorNode.h"
#include <set>
#include "TWPreprocessorAccum.h"
#include "TWPreprocessorStack.h"
#include "TWPreprocessorText.h"
#include "TWPreprocessorAttribute.h"
#include "TWParser.h"
#include <assert.h>
#include "TWUtils.h"
#include <memory>

#define TW_NOISY_DEBUG 0

enum TWPreprocessorFound{
	TWPreprocessorFoundNone=0,
	TWPreprocessorFoundAngle,
	TWPreprocessorFoundLineStart,
	TWPreprocessorFoundLineEnd,
	TWPreprocessorFoundOpen,
	TWPreprocessorFoundClose,
	TWPreprocessorFoundPipe,
	TWPreprocessorFoundEquals
};

static const std::wstring g_empty;
static const std::wstring g_newLine=L"\n";
static const std::wstring g_equal=L"=";
static const std::wstring g_templateRuleName=L"template";
static const std::wstring g_tplargRuleName=L"tplarg";
static const std::wstring g_openBracket=L"[";
static const std::wstring g_closeBracket=L"]";
static const std::wstring g_openBrace=L"{";
static const std::wstring g_closeBrace=L"}";
static const std::wstring g_openAngle=L"<";
static const std::wstring g_includeonlyTagName=L"includeonly";
static const std::wstring g_closedIncludeonlyTagName=L"/includeonly";
static const std::wstring g_noincludeTagName=L"noinclude";
static const std::wstring g_closedNoincludeTagName=L"/noinclude";
static const std::wstring g_onlyincludeTagName=L"onlyinclude";
static const std::wstring g_closedOnlyincludeTagName=L"/onlyinclude";
static const std::wstring g_defaultSearchBase=L"[{<\n";
static const std::wstring g_ignoreTagName=L"ignore";
static const std::wstring g_extTagName=L"ext";
static const std::wstring g_nameTagName=L"name";
static const std::wstring g_valueTagName=L"value";
static const std::wstring g_attrTagName=L"attr";
static const std::wstring g_innerTagName=L"inner";
static const std::wstring g_closeTagName=L"close";
static const std::wstring g_possibleHTagName=L"possible-h";
static const std::wstring g_levelTagName=L"level";
static const std::wstring g_iTagName=L"i";
static const std::wstring g_hTagName=L"h";
static const std::wstring g_indexTagName=L"index";
static const std::wstring g_titleTagName=L"title";
static const std::wstring g_partTagName=L"part";
static const std::wstring g_equalsTagName=L"equals";
static const std::wstring g_lineStartAttrName=L"lineStart";

TWPreprocessor::TWPreprocessor(TWParser *parser):
m_parser(parser){
	{
		TWPreprocessorRule rule;
		rule.start=g_openBrace;
		rule.end=g_closeBrace;
		rule.names[2]=g_templateRuleName;
		rule.names[3]=g_tplargRuleName;
		rule.minCount=2;
		rule.maxCount=3;
		m_rules[rule.start]=rule;
	}
	{
		TWPreprocessorRule rule;
		rule.start=g_openBracket;
		rule.end=g_closeBracket;
		rule.names[2]=g_empty;
		rule.minCount=2;
		rule.maxCount=2;
		m_rules[rule.start]=rule;
	}
}

static wchar_t easytolower(wchar_t in){
	if(in<=L'Z' && in>=L'A')
		return in-(L'Z'-L'z');
	return in;
} 

TWPreprocessorNode *TWPreprocessor::nodeFromText(const std::wstring& text, int flags){
	bool forInclusion=flags&TWPreprocessorFlagsInclusion;
	std::set<std::wstring> xmlishElements=m_parser->stripList();
	std::set<std::wstring> ignoredTags;
	std::set<std::wstring> ignoredElements;
	
	bool enableOnlyInclude=false;
	if(forInclusion){
		ignoredTags.insert(g_includeonlyTagName);
		ignoredTags.insert(g_closedIncludeonlyTagName);
		ignoredElements.insert(g_noincludeTagName);
		if(text.find(L"<onlyinclude>")!=std::wstring::npos){
			enableOnlyInclude=true;
		}
		xmlishElements.insert(g_noincludeTagName);
	}else{
		ignoredTags.insert(g_noincludeTagName);
		ignoredTags.insert(g_closedNoincludeTagName);
		ignoredTags.insert(g_onlyincludeTagName);
		ignoredTags.insert(g_closedOnlyincludeTagName);
		ignoredElements.insert(g_includeonlyTagName);
		xmlishElements.insert(g_includeonlyTagName);
	}
	
	/*
	if(text.find(L"{#if:{{{wraplinks|}}}|{{#ifeq:{{{wraplinks|}")!=std::wstring::npos){
		// TODO: printf("");
	}*/
	
	std::wstring searchBase=g_defaultSearchBase;
	std::wstring::size_type i=0;
	bool findEquals=false;
	//bool findPipe=false;
	
#if TW_NOISY_DEBUG
	puts("!!!! PPROCESSOR BEGIN");
#endif

#define findPipe	(accumStack.getFlags().findPipe)
#define inHeading	(accumStack.getFlags().inHeading)
	int headingIndex=1;
	//bool inHeading=false;
	bool noMoreGT=false;
	bool findOnlyInclude=enableOnlyInclude;
	bool fakeLineStart=true;
	TWPreprocessorStack accumStack(&m_stackArena);
	std::wstring search;
	search.reserve(searchBase.size()*2);
	
	while(true){
#if TW_NOISY_DEBUG
		printf("[%d]", accumStack.count());
#endif
		if(findOnlyInclude){
			std::wstring::size_type startPos=text.find(L"<onlyinclude>", i);
			if(startPos==std::wstring::npos){
				// IGNORE:
				//accumStack.accum()->addNodeWithText(g_ignoreTagName, text.substr(i));
				break;
			}
			std::wstring::size_type tagEndPos=startPos+13;
			// IGNORE:
			//accumStack.accum()->addNodeWithText(g_ignoreTagName, text.substr(i, tagEndPos-i));
			i=tagEndPos;
			findOnlyInclude=false;
		}
		
		TWPreprocessorFound found=TWPreprocessorFoundNone;
		TWPreprocessorRule *rule=NULL;
		
		wchar_t curChar=0;
		if(fakeLineStart){
			found=TWPreprocessorFoundLineStart;
		}else{
			// reduces std::wstring::~wstring() call.
			search.resize(searchBase.size());
			for(size_t j=0;j<4;j++)
				search[j]=searchBase[j];
			
			std::wstring currentClosing;
			if(accumStack.top()){
				currentClosing=accumStack.top()->close;
				search+=currentClosing;
			}
			if(findPipe)
				search+=L"|";
			if(findEquals)
				search+=L"=";
			
			
			std::wstring::size_type literalLength;
			literalLength=text.find_first_of(search, i);
			if(literalLength==std::wstring::npos)
				literalLength=text.size();
			literalLength-=i;
			if(literalLength>0){
				accumStack.accum()->addLiteral(text.substr(i, literalLength));
				i+=literalLength;
			}
			
			if(i>=text.size()){
				if(currentClosing==L"\n"){
					found=TWPreprocessorFoundLineEnd;
				}else{
					break;
				}
			}else{
				curChar=text[i];
				if(curChar==L'|'){
					found=TWPreprocessorFoundPipe;
				}else if(curChar==L'='){
					found=TWPreprocessorFoundEquals;
				}else if(curChar==L'<'){
					found=TWPreprocessorFoundAngle;
				}else if(curChar==L'\n'){
					if(inHeading)
						found=TWPreprocessorFoundLineEnd;
					else
						found=TWPreprocessorFoundLineStart;
				}else if(std::wstring(&curChar, 1)==currentClosing){
					found=TWPreprocessorFoundClose;
				}else if(m_rules.find(std::wstring(&curChar, 1))!=m_rules.end()){
					found=TWPreprocessorFoundOpen;
					rule=&(m_rules[std::wstring(&curChar, 1)]);
				}
			}
		}
		
		if(found==TWPreprocessorFoundAngle){
			if(enableOnlyInclude && text.substr(i, 14)==L"</onlyinclude>"){
				findOnlyInclude=true;
				continue;
			}
			if(text.substr(i, 4)==L"<!--"){
				std::wstring::size_type endPos=text.find(L"-->", i+4);
				if(endPos==std::wstring::npos){
					// IGNORE:
					//accumStack.accum()->addNodeWithText(L"comment", text.substr(i));
					i=text.size();
				}else{
					std::wstring::size_type whitespaceStart;
					std::wstring::size_type whitespaceEnd;
					whitespaceStart=text.find_last_not_of(L' ', i);
					whitespaceEnd=text.find_first_not_of(L' ', endPos+3);
					if(whitespaceStart==std::wstring::npos)
						whitespaceStart=0;
					else
						whitespaceStart++;
					if(whitespaceEnd==std::wstring::npos)
						whitespaceEnd=text.size()-1;
					else
						whitespaceEnd--;
					
					int startPos;
					
					startPos=i;
					
					if(whitespaceStart>0){
						if(text.substr(whitespaceStart-1, 1)==L"\n"){
							startPos=whitespaceStart;
							endPos=whitespaceEnd+1;
							
							int length=i-whitespaceStart;
							if(length>0){
								if(TWPreprocessorText *text=dynamic_cast<TWPreprocessorText *>(accumStack.accum()->lastChild())){
									text->setValue(text->value().substr(0, text->value().size()-length));
								}
							}
							fakeLineStart=true;
						}else{
							endPos+=2;
						}
					}else{
						endPos+=2;
					}
					
					if(accumStack.top()){
						TWPreprocessorPart *part=accumStack.currentPart();
						if(part->commentEnd==whitespaceStart-1){
							part->commentEnd=whitespaceEnd;
						}else{
							part->visualEnd=whitespaceStart;
							part->commentEnd=endPos;
						}
					}
					i=endPos+1;
					// IGNORE:
					//accumStack.accum()->addNodeWithText(L"comment", text.substr(startPos, endPos-startPos+1));
					
				}
				
				continue;
				
			}
		
			std::wstring::size_type nameEndPos=text.find_first_of(L" />", i+1);
			if(nameEndPos==std::wstring::npos){
				accumStack.accum()->addLiteral(g_openAngle);
				i++;
				continue;
			}
			
			std::wstring name=text.substr(i+1, nameEndPos-i-1);
			std::wstring lowerName=TWUtils::toLower(name);
			//std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), easytolower);
			std::wstring::size_type attrStart=i+name.size()+1;
			std::wstring::size_type tagEndPos;
			if(noMoreGT)
				tagEndPos=std::wstring::npos;
			else
				tagEndPos=text.find(L'>', attrStart);
			if(tagEndPos==std::wstring::npos){
				noMoreGT=true;
				i++;
				continue;
			}
			
			if(name.empty()){
				// ERROR.
				accumStack.accum()->addLiteral(g_openAngle);
				i++;
				continue;
			}
			
			std::wstring fltName=lowerName;
			TWUtils::matchStartAndRemove(fltName, L"/");
			if(!m_parser->doesParserExtensionExists(fltName)){
				// such a extension doesn't exist.
				accumStack.accum()->addLiteral(g_openAngle);
				i++;
				continue;
			}
			
			if(ignoredTags.find(lowerName)!=ignoredTags.end()){
				// IGNORE:
				/*accumStack.accum()->addNodeWithText(L"ignore", text.substr(i, tagEndPos-i+1));*/
				i=1+tagEndPos;
				continue;
			}
			
			//std::wstring::size_type tagStartPos=i;
			std::wstring::size_type attrEnd;
			std::wstring inner, close;
			if(text[tagEndPos-1]==L'/'){
				attrEnd=tagEndPos-1;
				i=tagEndPos+1;
			}else{
				attrEnd=tagEndPos;
				/* optimizing:
				 close=L"/"+name; */
				
				// search for the close tag.
				std::wstring::size_type closePos=tagEndPos+1;//=text.find(close, tagEndPos+1);
				int tagDepth=0;
				while(closePos<text.size()){
					std::wstring::size_type nextPos, lastPos;
					nextPos=text.find(L'<', closePos);
					if(nextPos==std::wstring::npos){
						// tag not found.
						closePos=std::wstring::npos;
						break;
					}
					
					lastPos=nextPos;
					closePos=nextPos+1;
					
					// get tag name.
					if(text[closePos]==L'/'){
						nextPos=text.find_first_of(L" />", closePos+1);
					}else{
						nextPos=text.find_first_of(L" />", closePos);
					}
					if(nextPos==std::wstring::npos){
						// this tag is incomplete.
						closePos=std::wstring::npos;
						break;
					}
					
					std::wstring curName=text.substr(closePos, nextPos-closePos);
					if(curName==name){
						// open.
						tagDepth++;
					/* opptimizing
					 }else if(curName==close){*/
					 }else if(curName[0]==L'/' && curName.size()==name.size()+1 && 
							  !wcscmp(curName.c_str()+1, name.c_str())){	
					
						// close
						tagDepth--;
						if(tagDepth<0){
							// I got it.
							closePos=lastPos;
							break;
						}
					}
					
					closePos=text.find(L'>', nextPos);
					if(closePos!=std::wstring::npos){
						if(text[closePos-1]==L'/'){
							// oops, instant close.
							if(curName==name){
								tagDepth--;
							}
						}
					}
				}
				if(closePos>=text.size())
					closePos=std::wstring::npos;
				if(closePos==std::wstring::npos){
					inner=text.substr(tagEndPos+1);
					i=text.size();
					close=std::wstring();
				}else{
					inner=text.substr(tagEndPos+1, closePos-tagEndPos-1);
					std::wstring::size_type closeClosePos=text.find(L'>', closePos);
					if(closeClosePos==std::wstring::npos){
						i=text.size();
					}else{
						i=closeClosePos+1;
					}
					close=text.substr(closePos, i-closePos);
				}
			}
			
			if(ignoredElements.find(lowerName)!=ignoredElements.end()){
				// IGNORE:
				/*accumStack.accum()->addNodeWithText(L"ignore", text.substr(tagStartPos, i-tagStartPos));*/
				continue;
			}
			
			std::wstring attr;
			if(attrEnd<=attrStart){
				//attr=L"";
			}else{
				attr=text.substr(attrStart, attrEnd-attrStart);
			}
#if TW_NOISY_DEBUG
			printf("!!! OPENEING TAG <%ls>\n", name.c_str());
#endif
			std::auto_ptr<TWPreprocessorNode> extNode(new TWPreprocessorNode(g_extTagName));
			extNode->addChild(TWPreprocessorNode::nodeWithText(g_nameTagName, name));
			
#if 1
			
			// CASE 1: Don't preprocess contents
			// seems to be the original behavior.
			
			extNode->addChild(TWPreprocessorNode::nodeWithText(g_attrTagName, attr));
			if(inner.size()){
				extNode->addChild(TWPreprocessorNode::nodeWithText(g_innerTagName, inner));
			}
			
#else
			
			// CASE 2: Preprocess contents
			
			// ???: is this really work-around?
			// this work-arounds the problem that wiki inside a tag is not parsed.
			{
				TWPreprocessorNode *node=nodeFromText(attr, flags);
				node->setName(g_attrTagName);
				extNode->addChild(node);
			}
			
#if TW_NOISY_DEBUG
			printf("!!!  TAG CONTENT OF <%ls>\n", name.c_str());
#endif
			{
				TWPreprocessorNode *node=nodeFromText(inner, flags);
				node->setName(g_innerTagName);
				extNode->addChild(node);
			}
			
#endif
			
#if TW_NOISY_DEBUG
			printf("!! END OF </%ls>\n", name.c_str());
#endif
			
			if(close.size())
				extNode->addChild(TWPreprocessorNode::nodeWithText(g_closeTagName, close));
			
			accumStack.accum()->addChild(extNode.release());
			
		}else if(found==TWPreprocessorFoundLineStart){
			if(fakeLineStart){
				fakeLineStart=false;
			}else{
				accumStack.accum()->addLiteral(std::wstring(&curChar, 1));
				i++;
			}
			
			std::wstring::size_type count=text.find_first_not_of(L'=', i);
			if(count==std::wstring::npos)
				count=text.size()-i;
			else
				count-=i;
			
			if(count>6)
				count=6;
			
			if(count==1 && findEquals){
				// name/value separator?
			}else if(count>0){
				std::auto_ptr<TWPreprocessorStackElement> piece(new TWPreprocessorStackElement());
				piece->open=g_newLine;
				piece->close=g_newLine;
				piece->addPart(text.substr(i, count));
				piece->startPos=i;
				piece->count=count;
				accumStack.push(piece.release());
				// extract()
				findEquals=accumStack.getFlags().findEquals;
				i+=count;
			}
		}else if(found==TWPreprocessorFoundLineEnd){
			TWPreprocessorStackElement *piece=accumStack.top();
			assert(piece != NULL);
			assert(piece->open==L"\n");
			TWPreprocessorPart *part=accumStack.currentPart();
			
			std::wstring::size_type searchStart;
			searchStart=text.find_last_not_of(L" \t", i-1);
			if(searchStart==std::wstring::npos)
				searchStart=0;
			
			if(part->commentEnd==searchStart-1){
				searchStart=part->visualEnd;
				searchStart=text.find_last_not_of(L" \t", searchStart-1);
			}
			
			int count=piece->count;
			std::wstring::size_type equalsLength;
			equalsLength=text.find_last_not_of(L'=', searchStart-1);
			if(equalsLength==std::wstring::npos)
				equalsLength=0;
			equalsLength=searchStart-equalsLength;
			
			TWPreprocessorNode *element;
			
			if(equalsLength>0){
				if(i-equalsLength==piece->startPos){
					count=equalsLength;
					if(count<3)
						count=0;
					else
						count=(count-1)/2;
					if(count>6)
						count=6;
				}else if(equalsLength<count){
					count=equalsLength;
				}
				if(count>0){
					element=new TWPreprocessorAccum(g_possibleHTagName);
					element->addChild(new TWPreprocessorAttribute(g_levelTagName, count));
					element->addChild(new TWPreprocessorAttribute(g_iTagName, headingIndex++));
					static_cast<TWPreprocessorAccum *>(element)->addAccum(accumStack.accum());
				}else{
					element=accumStack.accum();
				}
			}else{
				element=accumStack.accum();
			}
			
			piece=accumStack.pop();
			
			if(element==piece->accum())
				accumStack.accum()->addAccum(piece->accum());
			else
				accumStack.accum()->addChild(element);
			
#if TW_NOISY_DEBUG
			printf("!!! ENDLINE %d \n", accumStack.count());
#endif
			
			delete piece;
			
			// extract()
			findEquals=accumStack.getFlags().findEquals;
			
		}else if(found==TWPreprocessorFoundOpen){
			std::wstring::size_type count;
#if TW_NOISY_DEBUG
			printf("!!! OPENEING %d --> %d \n", accumStack.count(), accumStack.count()+1);
			XTBDumpWString(text.substr(i, 40));
#endif
			count=text.find_first_not_of(curChar, i);
			if(count==std::wstring::npos)
				count=text.size();
			count-=i;
			assert(rule != NULL);
			
			if(count>=rule->minCount){
				std::auto_ptr<TWPreprocessorStackElement> piece(new TWPreprocessorStackElement());
				piece->open=std::wstring(&curChar, 1);
				piece->close=rule->end;
				piece->addPart(g_empty);
				piece->count=count;
				piece->lineStart=false;
				piece->startPos=i;
				if(i>0){
					if(text[i-1]==L'\n')
						piece->lineStart=true;
				}
				accumStack.push(piece.release());
				
				// extract()
				findEquals=accumStack.getFlags().findEquals;
			}else{
				accumStack.accum()->addLiteral(text.substr(i, count));
			}
			i+=count;
		}else if(found==TWPreprocessorFoundClose){
			TWPreprocessorStackElement *piece=accumStack.top();
			int maxCount=piece->count;
			std::wstring::size_type count;
#if TW_NOISY_DEBUG
			printf("!!! CLOSING %d <-- %d \n", accumStack.count()-1, accumStack.count());
			XTBDumpWString(text.substr(i, 40));
			puts("!!! HERE'S CLOSED WIKITEXT:");
			XTBDumpWString(text.substr(piece->startPos, i-piece->startPos+maxCount));
#endif
			count=text.find_first_not_of(curChar, i);
			if(count==std::wstring::npos)
				count=text.size();
			count-=i;
			
			if(count>maxCount)
				count=maxCount;
			
			rule=&(m_rules[piece->open]);
			// !: this isn't always true. (ex. {{{{aa}}bb}})
			/* assert(count>=piece->count); */
			
			int matchingCount=0;
			if(count>rule->maxCount){
				matchingCount=rule->maxCount;
			}else{
				matchingCount=count;
				while(matchingCount>0 && rule->names.find(matchingCount)==rule->names.end()){
					matchingCount--;
				}
			}
			
			if(matchingCount<=0){
				accumStack.accum()->addLiteral(text.substr(i, count));
				i+=count;
				continue;
			}
			
			std::wstring name=rule->names[matchingCount];
			TWPreprocessorAccum *element;
			
			if(name.size()==0){
				element=piece->breakSyntax(matchingCount);
				element->addLiteral(text.substr(i, count));
			}else {
				element=new TWPreprocessorAccum(name);
				if(maxCount==matchingCount && piece->lineStart){
					element->addChild(new TWPreprocessorAttribute(g_lineStartAttrName, 1));
				}
				
				TWPreprocessorAccum *titleNode=piece->parts[0]->accum;
				titleNode->setName(g_titleTagName);
				element->addChild(titleNode);
				
				int argIndex=1;
				for(int n=1;n<piece->parts.size();n++){
					TWPreprocessorPart *part=piece->parts[n];
					if(part->eqpos){
						assert(part->eqpos->name() == L"equals");
						
						bool inValue=false;
						
						std::auto_ptr<TWPreprocessorAccum>nameNode(new TWPreprocessorAccum(g_nameTagName));
						std::auto_ptr<TWPreprocessorAccum>valueNode(new TWPreprocessorAccum(g_valueTagName));
						
						/*std::vector<TWNode *> nodes=part->accum->children();
						
						for(int j=0;j<nodes.size();j++){
							if(nodes[j]==part->eqpos){
								inValue=true;
							}else if(!inValue){
								nameNode->addChild(nodes[j]);
							}else{
								valueNode->addChild(nodes[j]);
							}
						}*/
						
						TWNode *child=part->accum->firstChild();
						while(child){
							TWNode *next=child->nextSibling();
							if(child==part->eqpos)
								inValue=true;
							else if(!inValue)
								nameNode->addChild(child);
							else
								valueNode->addChild(child);
							child=next;
						}
						
						TWPreprocessorAccum *partNode;
						partNode=new TWPreprocessorAccum(g_partTagName);
						partNode->addChild(nameNode.release());
						partNode->addChild(part->eqpos->firstChild());
						partNode->addChild(valueNode.release());
						
						element->addChild(partNode);
						
					}else{
						
						std::auto_ptr<TWPreprocessorAccum> nameNode(new TWPreprocessorAccum(g_nameTagName));
						std::auto_ptr<TWPreprocessorAccum> valueNode(new TWPreprocessorAccum(g_valueTagName));
						
						nameNode->addChild(new TWPreprocessorAttribute(g_indexTagName, argIndex++));
						valueNode->addAccum(part->accum);
						
						TWPreprocessorAccum *partNode;
						partNode=new TWPreprocessorAccum(g_partTagName);
						partNode->addChild(nameNode.release());
						partNode->addChild(valueNode.release());
						
						element->addChild(partNode);
					}
					
				}
			}
			
			i+=matchingCount;
			
			piece=accumStack.pop();
			
			if(matchingCount<piece->count){
				piece->removeAllParts();
				piece->addPart(g_empty);
				piece->count-=matchingCount;
				
				int skippedBraces=0;
				TWPreprocessorAccum *enclosingAccum=accumStack.accum();
				while(piece->count){
					if(rule->names.find(piece->count)!=rule->names.end()){
						accumStack.push(piece);
						break;
					}
					piece->count--;
					skippedBraces++;
				}
				
				std::wstring ss;
				for(int i=skippedBraces;i>0;i--){
					ss+=piece->open.substr(0, 1);
				}
				enclosingAccum->addLiteral(ss);
				//enclosingAccum->addLiteral(text.substr(i-skippedBraces, skippedBraces));
			}
			
			// extract()
			findEquals=accumStack.getFlags().findEquals;
			
			if(name.size()==0){
				accumStack.accum()->addAccum(element);
				if(element->parent()==NULL)
					delete element;
			}else
				accumStack.accum()->addChild(element);
			
			if(accumStack.top()!=piece)
				delete piece;
			
		}else if(found==TWPreprocessorFoundPipe){
			findEquals=true;
			accumStack.addPart(g_empty);
			i++;
		}else if(found==TWPreprocessorFoundEquals){
			findEquals=false;
			accumStack.accum()->addNodeWithText(g_equalsTagName, g_equal);
			accumStack.currentPart()->markLastChildAsEqpos();
			i++;
		}

	}
	
	while(TWPreprocessorStackElement *piece=accumStack.pop()){
#if TW_NOISY_DEBUG
		printf("!!! Reamined %ls\n", piece->open.c_str());
#endif
		accumStack.rootAccum->addAccum(piece->breakSyntax());
		delete piece;
		
	}
	
	for(TWNode *node=accumStack.accum()->firstChild();node;node=node->nextSibling()){
		if(node->name()==g_possibleHTagName){
			node->setName(g_hTagName);
		}
	}
#if TW_NOISY_DEBUG
	puts("!!!! PPROCESSOR END");
#endif
	return accumStack.accum();
}

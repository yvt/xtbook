//
//  TPLPropertyListSerialization.cpp
//  XTBook
//
//  Created by Kawada Tomoaki on 5/21/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//
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

#include "TPLPropertyListSerialization.h"
#include "TPLArray.h"
#include "TPLDictionary.h"
#include "TPLString.h"
#include "TPLNumber.h"
#include "../TinyXML/tinyxml.h"
#include "TPLAutoReleasePtr.h"
#include "TPLException.h"

#include <string.h>
#include <assert.h>

#include <typeinfo>

TiXmlElement *TPLPropertyListSerialization::elementForObject(TPLObject *obj){
	assert(obj);
	if(TPLNumber *number=dynamic_cast<TPLNumber *>(obj)){
		TPLAutoReleasePtr<TPLString> stringValue=number->stringValue();
		if(stringValue->isEqualToUTF8String("true") ||
		   stringValue->isEqualToUTF8String("false")){
			return new TiXmlElement(stringValue->UTF8String());
		}
		
		const char *str=stringValue->UTF8String();
		TiXmlElement *element;
		if(strchr(str, '.')){
			element=new TiXmlElement("real");
		}else{
			element=new TiXmlElement("integer");
		}
		element->LinkEndChild(new TiXmlText(str));
		return element;
	}else if(TPLString *string=dynamic_cast<TPLString *>(obj)){
		const char *str=string->UTF8String();
		TiXmlElement *element;
		element=new TiXmlElement("string");
		element->LinkEndChild(new TiXmlText(str));
		return element;
	}else if(TPLArray *array=dynamic_cast<TPLArray *>(obj)){
		TiXmlElement *element;
		element=new TiXmlElement("array");
		try{
			TPLUInteger count=array->count();
			for(TPLUInteger i=0;i<count;i++){
				element->LinkEndChild(elementForObject(array->objectAtIndex(i)));
			}
		}catch(...){
			delete element;
			throw;
		}
		return element;
	}else if(TPLDictionary *dic=dynamic_cast<TPLDictionary *>(obj)){
		TiXmlElement *element;
		element=new TiXmlElement("dict");
		try{
			TPLAutoReleasePtr<TPLArray> keys=dic->allKeys();
			TPLUInteger count=keys->count();
			for(TPLUInteger i=0;i<count;i++){
				TPLString *key=static_cast<TPLString *>(keys->objectAtIndex(i));
				TiXmlElement *keyElement=new TiXmlElement("key");
				keyElement->LinkEndChild(new TiXmlText(key->UTF8String()));
				element->LinkEndChild(keyElement);
				
				TPLObject *obj=dic->objectForKey(key->UTF8String());
				element->LinkEndChild(elementForObject(obj));
			}
		}catch(...){
			delete element;
			throw;
		}
		return element;
	}else{
		throw TPLException((std::string("Type \"")+typeid(*obj).name()+"\" cannot be serialized to a property list. Only "
		"TPLNumber, TPLArray, TPLDictionary, and TPLString can be.").c_str());
	}
}

TPLObject *TPLPropertyListSerialization::objectForElement(TiXmlElement *element){
	assert(element);
	if(element->ValueStr()=="true"){
		return new TPLNumber(true);
	}else if(element->ValueStr()=="false"){
		return new TPLNumber(false);
	}else if(element->ValueStr()=="real" || element->ValueStr()=="integer"){
		return new TPLNumber(element->GetText());
	}else if(element->ValueStr()=="string"){
		return new TPLString(element->GetText());
	}else if(element->ValueStr()=="array"){
		TPLArray *array=new TPLArray();
		try{
			TiXmlElement *child=element->FirstChildElement();
			while(child){
				array->addObject(TPLAutoReleasePtr<TPLObject>(objectForElement(child)));
				child=child->NextSiblingElement();
			}
		}catch(...){
			array->release();
			throw;
		}
		return array;
	}else if(element->ValueStr()=="dict"){
		TPLDictionary *dic=new TPLDictionary();
		try{
			TiXmlElement *child=element->FirstChildElement();
			while(child){
				if(child->ValueStr()!="key"){
					throw TPLException("invalid format of <dict>. <key> was expected but not found.");
				}
				const char *keyName=child->GetText();
				
				child=child->NextSiblingElement();
				if(child==NULL){
					throw TPLException("invalid format of <dict>. no element was found following <key>.");
				}
				dic->setObject(TPLAutoReleasePtr<TPLObject>(objectForElement(child)), keyName);
				
				child=child->NextSiblingElement();
			}
		}catch(...){
			dic->release();
			throw;
		}
		return dic;
	}else{
		throw TPLException((std::string("element \"")+element->ValueStr()+"\" cannot be unserialized to a property list. Only "
		"real, true, false, real, string, array, and dict can be.").c_str());
	}
}

static TiXmlUnknown *dtdForPList(){
	TiXmlUnknown *unk=new TiXmlUnknown();
	unk->SetValue("!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\"");
	return unk;
}

void TPLPropertyListSerialization::writePropertyList(TPLObject *obj, FILE *toStream){
	TiXmlDocument doc;
	doc.InsertEndChild(TiXmlDeclaration("1.0", "UTF-8", ""));
	doc.LinkEndChild(dtdForPList());
	
	TiXmlElement *elm=new TiXmlElement("plist");
	try{
		elm->LinkEndChild(elementForObject(obj));
	}catch(...){
		delete elm;
		throw;
	}
	
	doc.LinkEndChild(elm);
	
	// save.
	doc.SaveFile(toStream);
	
}

TPLObject *TPLPropertyListSerialization::propertyListWithStream(FILE *f){
	TiXmlDocument doc;
	doc.LoadFile(f);
	
	TiXmlElement *rootElement=dynamic_cast<TiXmlElement *>(doc.FirstChildElement("plist"));
	if(!rootElement){
		throw TPLException("the input file isn't a valid property list file. <plist> not found.");
	}
	
	TiXmlElement *element=rootElement->FirstChildElement();
	
	return objectForElement(element);
}

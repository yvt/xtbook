/*
 *  TWHTMLNode.h
 *  XTBook
 *
 *  Created by Nexhawks on 2/24/11.
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

#pragma once
#include "TWElement.h"
#include "TWHTMLStyle.h"

#include <vector>

enum TWHTMLElementType{
	TWHTMLElementTypeUnknown=0,
	TWHTMLElementTypeA=1,
	TWHTMLElementTypeAbbr,
	TWHTMLElementTypeAccess,
	TWHTMLElementTypeAcronym,
	TWHTMLElementTypeAction,
	TWHTMLElementTypeAddress,
	TWHTMLElementTypeApplet,
	TWHTMLElementTypeArea,
	TWHTMLElementTypeArticle,
	TWHTMLElementTypeAside,
	TWHTMLElementTypeAudio,
	TWHTMLElementTypeB,
	TWHTMLElementTypeBase,
	TWHTMLElementTypeBasefont,
	TWHTMLElementTypeBdo,
	TWHTMLElementTypeBig,
	TWHTMLElementTypeBlockcode,
	TWHTMLElementTypeBlockquote,
	TWHTMLElementTypeBody,
	TWHTMLElementTypeBr,
	TWHTMLElementTypeButton,
	TWHTMLElementTypeCanvas,
	TWHTMLElementTypeCaption,
	TWHTMLElementTypeCenter,
	TWHTMLElementTypeCite,
	TWHTMLElementTypeCode,
	TWHTMLElementTypeCol,
	TWHTMLElementTypeColgroup,
	TWHTMLElementTypeCommand,
	TWHTMLElementTypeDatalist,
	TWHTMLElementTypeDd,
	TWHTMLElementTypeDel,
	TWHTMLElementTypeDelete,
	TWHTMLElementTypeDetails,
	TWHTMLElementTypeDfn,
	TWHTMLElementTypeDi,
	TWHTMLElementTypeDir,
	TWHTMLElementTypeDispatch,
	TWHTMLElementTypeDiv,
	TWHTMLElementTypeDl,
	TWHTMLElementTypeDt,
	TWHTMLElementTypeEm,
	TWHTMLElementTypeEmbed,
	TWHTMLElementTypeFieldset,
	TWHTMLElementTypeFigcaption,
	TWHTMLElementTypeFigure,
	TWHTMLElementTypeFont,
	TWHTMLElementTypeFooter,
	TWHTMLElementTypeForm,
	TWHTMLElementTypeFrame,
	TWHTMLElementTypeFrameset,
	TWHTMLElementTypeGroup,
	TWHTMLElementTypeH,
	TWHTMLElementTypeH1,
	TWHTMLElementTypeH2,
	TWHTMLElementTypeH3,
	TWHTMLElementTypeH4,
	TWHTMLElementTypeH5,
	TWHTMLElementTypeH6,
	TWHTMLElementTypeHead,
	TWHTMLElementTypeHeader,
	TWHTMLElementTypeHgroup,
	TWHTMLElementTypeHr,
	TWHTMLElementTypeHtml,
	TWHTMLElementTypeI,
	TWHTMLElementTypeIframe,
	TWHTMLElementTypeImg,
	TWHTMLElementTypeInput,
	TWHTMLElementTypeIns,
	TWHTMLElementTypeInsert,
	TWHTMLElementTypeIsindex,
	TWHTMLElementTypeKbd,
	TWHTMLElementTypeKeygen,
	TWHTMLElementTypeL,
	TWHTMLElementTypeLabel,
	TWHTMLElementTypeLegend,
	TWHTMLElementTypeLi,
	TWHTMLElementTypeLink,
	TWHTMLElementTypeListener,
	TWHTMLElementTypeListing,
	TWHTMLElementTypeLoad,
	TWHTMLElementTypeMap,
	TWHTMLElementTypeMark,
	TWHTMLElementTypeMenu,
	TWHTMLElementTypeMessage,
	TWHTMLElementTypeMeta,
	TWHTMLElementTypeMeter,
	TWHTMLElementTypeModel,
	TWHTMLElementTypeNav,
	TWHTMLElementTypeNoframes,
	TWHTMLElementTypeNoscript,
	TWHTMLElementTypeObject,
	TWHTMLElementTypeOl,
	TWHTMLElementTypeOptgroup,
	TWHTMLElementTypeOption,
	TWHTMLElementTypeOutput,
	TWHTMLElementTypeP,
	TWHTMLElementTypeParam,
	TWHTMLElementTypePlaintext,
	TWHTMLElementTypePre,
	TWHTMLElementTypeProgress,
	TWHTMLElementTypeQ,
	TWHTMLElementTypeRange,
	TWHTMLElementTypeRb,
	TWHTMLElementTypeRbc,
	TWHTMLElementTypeRebuild,
	TWHTMLElementTypeRecalculate,
	TWHTMLElementTypeRefresh,
	TWHTMLElementTypeRepeat,
	TWHTMLElementTypeReset,
	TWHTMLElementTypeRevalidate,
	TWHTMLElementTypeRp,
	TWHTMLElementTypeRt,
	TWHTMLElementTypeRtc,
	TWHTMLElementTypeRuby,
	TWHTMLElementTypeS,
	TWHTMLElementTypeSamp,
	TWHTMLElementTypeScript,
	TWHTMLElementTypeSecret,
	TWHTMLElementTypeSection,
	TWHTMLElementTypeSelect,
	TWHTMLElementTypeSelect1,
	TWHTMLElementTypeSend,
	TWHTMLElementTypeSeparator,
	TWHTMLElementTypeSetfocus,
	TWHTMLElementTypeSetindex,
	TWHTMLElementTypeSetvalue,
	TWHTMLElementTypeSmall,
	TWHTMLElementTypeSource,
	TWHTMLElementTypeSpan,
	TWHTMLElementTypeStandby,
	TWHTMLElementTypeStrike,
	TWHTMLElementTypeStrong,
	TWHTMLElementTypeStyle,
	TWHTMLElementTypeSub,
	TWHTMLElementTypeSubmit,
	TWHTMLElementTypeSummary,
	TWHTMLElementTypeSup,
	TWHTMLElementTypeSwitch,
	TWHTMLElementTypeTable,
	TWHTMLElementTypeTbody,
	TWHTMLElementTypeTd,
	TWHTMLElementTypeTextarea,
	TWHTMLElementTypeTfoot,
	TWHTMLElementTypeTh,
	TWHTMLElementTypeThead,
	TWHTMLElementTypeTime,
	TWHTMLElementTypeTitle,
	TWHTMLElementTypeTr,
	TWHTMLElementTypeTrack,
	TWHTMLElementTypeTrigger,
	TWHTMLElementTypeTt,
	TWHTMLElementTypeU,
	TWHTMLElementTypeUl,
	TWHTMLElementTypeUpload,
	TWHTMLElementTypeVar,
	TWHTMLElementTypeVideo,
	TWHTMLElementTypeWbr,
	TWHTMLElementTypeXmp,
	TWHTMLElementTypeText
};

class TWHTMLBodyElement;

class TWHTMLElement : public TWElement {
protected:
	TWHTMLElementType m_type;
	TWHTMLStyle m_style;
	std::wstring m_styleAttr;
	virtual void applyCustomStyle(){}
public:
	TWHTMLElement(const std::wstring&);
	
	TWHTMLElementType type() const{return m_type;}
	
	const TWHTMLStyle& style() const{return m_style;}
	std::vector<std::wstring> classNames() const;
	
	TWHTMLBodyElement *body() const;

	static TWHTMLElement *nodeWithName(const std::wstring&);
	void calcStyle();
	
	virtual std::wstring innerText() const;
	virtual std::wstring innerHTML() const;
	virtual std::wstring outerHTML() const;
	
	virtual void setAttribute(const std::wstring&, const std::wstring&);
	virtual std::wstring getAttribute(const std::wstring&) const;
	virtual bool hasAttribute(const std::wstring&) const;
	virtual void removeAttribute(const std::wstring&);
	
	void setStyleAttr(const std::wstring& s){m_styleAttr=s;}
	const std::wstring& styleAttr() const{return m_styleAttr;}
	
};

TWHTMLElementType HTMLElementTypeForName(const wchar_t *name);


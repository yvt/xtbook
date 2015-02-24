//
//  TWExpressionEvaluator.cpp
//  XTBook
//
//  Created by Kawada Tomoaki on 7/20/11.
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

#include "TWExpressionEvaluator.h"
#include "TWExpressionReader.h"
#include <assert.h>
#include <string>

TWExpressionStackFrame::TWExpressionStackFrame(TWExpressionEvaluator& evaluator):
m_evaluator(evaluator){
	
}

TWNumber TWExpressionStackFrame::parseLiteral(){
	TWNumber num;
	bool negate=false;
	while((reader().currentTokenType()==TWExpressionTokenTypeOperator)
		  && (reader().currentTokenLength()==1)
		  && (reader().currentTokenChar()==L'-' ||
			  reader().currentTokenChar()==L'+')){
		if(reader().currentTokenChar()==L'-')
			negate=!negate;
		reader().forwardPointer();
	}
	if(reader().currentTokenType()==TWExpressionTokenTypeOperator){
		if(reader().currentTokenLength()!=1 || 
		   reader().currentTokenChar()!=L'('){
			throw L"unexpected token found('(' expected): "
			+reader().currentToken();
		}
		
		assert(reader().currentToken()==L"(");
		
		reader().forwardPointer();
		
		// push stack.
		TWExpressionStackFrame childFrame(m_evaluator);
		
		// calc.
		num=childFrame.evaluate();
		
		// make sure there is closing parenthesis.
		if(reader().currentTokenLength()!=1 || 
		   reader().currentTokenChar()!=L')'){
			throw L"unexpected token found(')' expected): "
			+reader().currentToken();
		}
		
		assert(reader().currentToken()==L")");
		
		reader().forwardPointer();
	}else if(reader().currentTokenType()==TWExpressionTokenTypeNumber){
		// just number.
		num=reader().currentToken();
		reader().forwardPointer();
	}else if(reader().currentTokenType()==TWExpressionTokenTypeSymbol){
		std::wstring symbolName=reader().currentToken();
		if(symbolName==L"e"){
			num=TWNumberE;
		}else if(symbolName==L"pi"){
			num=TWNumberPi;
		}else{
			throw L"invalid symbol name: "+symbolName;
		}
		reader().forwardPointer();
	}else{
		throw std::wstring(L"expected token not found.");
	}
	if(negate)
		return -num;
	else
		return num;
}

TWNumber TWExpressionStackFrame::parseFunctions(){
	int opType=m_evaluator.currentUnaryOperatorType();
	if(opType==TWExpressionOperatorInvalid){
		// value or something.
		// do precedence 10...
		TWNumber value=parseLiteral();
		while(reader().currentTokenType()!=TWExpressionTokenTypeEOS){
			opType=m_evaluator.currentBinaryOperatorType();
			if(opType!=TWExpressionOperatorExponent){
				// less than precedence 10...
				break;
			}
			
			reader().forwardPointer();
			TWNumber nextValue=parseLiteral();
			
			value=m_evaluator.m_operators[opType].binaryCallback
			(value, nextValue);
		}
		return value;
	}else{
		// function.
		// function is precedence 9.
		reader().forwardPointer();
		TWNumber num=parseFunctions();
		if(m_evaluator.m_operators[opType].unaryCallback)
			return m_evaluator.m_operators[opType].unaryCallback(num);
		return num;
	}
}

void TWExpressionStackFrame::completeTopStack(){
	assert(m_stackPos>0);
	
	TWExpressionSubStackFrame& frame=m_stack[m_stackPos];
	
	TWExpressionOperator& op=m_evaluator.m_operators[frame.op];
	
	frame.num1=op.binaryCallback(frame.num1, frame.num2);
}

void TWExpressionStackFrame::popStack(){
	assert(m_stackPos>0);
	
	TWExpressionSubStackFrame& frame=m_stack[m_stackPos];
	TWExpressionSubStackFrame& parentFrame=m_stack[m_stackPos-1];
	
	completeTopStack();
	
	parentFrame.num2=frame.num1;
	
	m_stackPos--;
}

TWNumber TWExpressionStackFrame::evaluate(){
	m_stack[0].num2=parseFunctions();
	m_stack[0].precedence=-1;
	m_stackPos=0;
	
	while(reader().currentTokenType()!=TWExpressionTokenTypeEOS){
		if(reader().currentTokenChar()==L')')
			break;
		
		int opType=m_evaluator.currentBinaryOperatorType();
		
		if(opType==TWExpressionOperatorInvalid){
			throw L"unexpected binary token: "+reader().currentToken();
		}
		
		reader().forwardPointer();
		
		TWExpressionOperator& op=m_evaluator.m_operators[opType];
		
		while(m_stack[m_stackPos].precedence>op.precedence)
			popStack();
		
		if(op.precedence==m_stack[m_stackPos].precedence){
			// continued.
			completeTopStack();
			
			TWExpressionSubStackFrame& frame=m_stack[m_stackPos];
			
			// new op type.
			frame.op=opType;
			
			// next value.
			frame.num2=parseFunctions();
		}else{
			// add new stack.
			TWExpressionSubStackFrame& parentFrame=m_stack[m_stackPos];
			m_stackPos++;
			TWExpressionSubStackFrame& frame=m_stack[m_stackPos];
			
			frame.op=opType;
			frame.precedence=op.precedence;
			frame.num1=parentFrame.num2;
			frame.num2=parseFunctions();
			
		}
		
	}
	while(m_stackPos>0)
		popStack();
	
	return m_stack[0].num2;
}

TWExpressionReader& TWExpressionStackFrame::reader() const{
	return m_evaluator.reader();
}

static TWNumber operatorNegate(const TWNumber& n){
	return -n;
}

static TWNumber operatorNot(const TWNumber& n){
	return !n;
}

static TWNumber operatorAdd(const TWNumber& a, const TWNumber& b){
	return a+b;
}

static TWNumber operatorSub(const TWNumber& a, const TWNumber& b){
	return a-b;
}

static TWNumber operatorMul(const TWNumber& a, const TWNumber& b){
	return a*b;
}

static TWNumber operatorDiv(const TWNumber& a, const TWNumber& b){
	return a/b;
}

static TWNumber operatorMod(const TWNumber& a, const TWNumber& b){
	return a%b;
}

static TWNumber operatorEqual(const TWNumber& a, const TWNumber& b){
	return a==b;
}

static TWNumber operatorNotEqual(const TWNumber& a, const TWNumber& b){
	return a!=b;
}

static TWNumber operatorLessThan(const TWNumber& a, const TWNumber& b){
	return a<b;
}

static TWNumber operatorGreaterThan(const TWNumber& a, const TWNumber& b){
	return a>b;
}

static TWNumber operatorLEThan(const TWNumber& a, const TWNumber& b){
	return a<=b;
}

static TWNumber operatorGEThan(const TWNumber& a, const TWNumber& b){
	return a>=b;
}

static TWNumber operatorAnd(const TWNumber& a, const TWNumber& b){
	return a&&b;
}

static TWNumber operatorOr(const TWNumber& a, const TWNumber& b){
	return a||b;
}


TWExpressionEvaluator::TWExpressionEvaluator(TWExpressionReader& reader):
m_reader(reader){
	TWExpressionOperator op;
	
	op.precedence=10;
	op.argumentCount=1;
	op.unaryCallback=operatorNegate;
	m_operators[TWExpressionOperatorNegate]=op;
	
	op.precedence=10;
	op.argumentCount=2;
	op.binaryCallback=TWNumber::makeExponent;
	m_operators[TWExpressionOperatorExponent]=op;
	
	op.precedence=9;
	op.argumentCount=1;
	op.unaryCallback=TWNumber::exp;
	m_operators[TWExpressionOperatorFuncExp]=op;
	
	op.precedence=9;
	op.argumentCount=1;
	op.unaryCallback=TWNumber::ln;
	m_operators[TWExpressionOperatorFuncLn]=op;
	
	op.precedence=9;
	op.argumentCount=1;
	op.unaryCallback=TWNumber::abs;
	m_operators[TWExpressionOperatorFuncAbs]=op;
	
	op.precedence=9;
	op.argumentCount=1;
	op.unaryCallback=TWNumber::trunc;
	m_operators[TWExpressionOperatorFuncTrunc]=op;
	
	op.precedence=9;
	op.argumentCount=1;
	op.unaryCallback=TWNumber::floor;
	m_operators[TWExpressionOperatorFuncFloor]=op;
	
	op.precedence=9;
	op.argumentCount=1;
	op.unaryCallback=TWNumber::ceil;
	m_operators[TWExpressionOperatorFuncCeil]=op;
	
	op.precedence=9;
	op.argumentCount=1;
	op.unaryCallback=TWNumber::sin;
	m_operators[TWExpressionOperatorFuncSin]=op;
	
	op.precedence=9;
	op.argumentCount=1;
	op.unaryCallback=TWNumber::cos;
	m_operators[TWExpressionOperatorFuncCos]=op;
	
	op.precedence=9;
	op.argumentCount=1;
	op.unaryCallback=TWNumber::tan;
	m_operators[TWExpressionOperatorFuncTan]=op;
	
	op.precedence=9;
	op.argumentCount=1;
	op.unaryCallback=TWNumber::asin;
	m_operators[TWExpressionOperatorFuncAsin]=op;
	
	op.precedence=9;
	op.argumentCount=1;
	op.unaryCallback=TWNumber::acos;
	m_operators[TWExpressionOperatorFuncAcos]=op;
	
	op.precedence=9;
	op.argumentCount=1;
	op.unaryCallback=TWNumber::atan;
	m_operators[TWExpressionOperatorFuncAtan]=op;
	
	op.precedence=9;
	op.argumentCount=1;
	op.unaryCallback=operatorNot;
	m_operators[TWExpressionOperatorNot]=op;
	
	op.precedence=8;
	op.argumentCount=2;
	op.binaryCallback=TWNumber::pow;
	m_operators[TWExpressionOperatorPower]=op;
	
	op.precedence=7;
	op.argumentCount=2;
	op.binaryCallback=operatorMul;
	m_operators[TWExpressionOperatorMul]=op;
	
	op.precedence=7;
	op.argumentCount=2;
	op.binaryCallback=operatorDiv;
	m_operators[TWExpressionOperatorDiv]=op;
	
	op.precedence=7;
	op.argumentCount=2;
	op.binaryCallback=operatorMod;
	m_operators[TWExpressionOperatorMod]=op;
	
	op.precedence=6;
	op.argumentCount=2;
	op.binaryCallback=operatorAdd;
	m_operators[TWExpressionOperatorAdd]=op;
	
	op.precedence=6;
	op.argumentCount=2;
	op.binaryCallback=operatorSub;
	m_operators[TWExpressionOperatorSub]=op;
	
	op.precedence=5;
	op.argumentCount=2;
	op.binaryCallback=TWNumber::round;
	m_operators[TWExpressionOperatorRound]=op;
	
	op.precedence=4;
	op.argumentCount=2;
	op.binaryCallback=operatorEqual;
	m_operators[TWExpressionOperatorEqual]=op;
	
	op.precedence=4;
	op.argumentCount=2;
	op.binaryCallback=operatorNotEqual;
	m_operators[TWExpressionOperatorNotEqual]=op;
	
	op.precedence=4;
	op.argumentCount=2;
	op.binaryCallback=operatorLessThan;
	m_operators[TWExpressionOperatorLessThan]=op;
	
	op.precedence=4;
	op.argumentCount=2;
	op.binaryCallback=operatorGreaterThan;
	m_operators[TWExpressionOperatorGreaterThan]=op;
	
	op.precedence=4;
	op.argumentCount=2;
	op.binaryCallback=operatorLEThan;
	m_operators[TWExpressionOperatorLEThan]=op;
	
	op.precedence=4;
	op.argumentCount=2;
	op.binaryCallback=operatorGEThan;
	m_operators[TWExpressionOperatorGEThan]=op;
	
	op.precedence=3;
	op.argumentCount=2;
	op.binaryCallback=operatorAnd;
	m_operators[TWExpressionOperatorAnd]=op;
	
	op.precedence=2;
	op.argumentCount=2;
	op.binaryCallback=operatorOr;
	m_operators[TWExpressionOperatorOr]=op;
	
	op.precedence=10;
	op.argumentCount=1;
	op.unaryCallback=NULL;
	m_operators[TWExpressionOperatorPositive]=op;
	
}

TWExpressionEvaluator::~TWExpressionEvaluator(){}

int TWExpressionEvaluator::currentUnaryOperatorType(){
	if(m_reader.currentTokenType()!=TWExpressionTokenTypeOperator
	   && m_reader.currentTokenType()!=TWExpressionTokenTypeSymbol){
		return TWExpressionOperatorInvalid;
	}
	
	if(m_reader.currentTokenLength()==1){
		switch(m_reader.currentTokenChar()){
			case L'-':
				return TWExpressionOperatorNegate;
			case L'+':
				return TWExpressionOperatorPositive;
		}
	}else{
		std::wstring s=m_reader.currentToken();
		if(s==L"exp"){
			return TWExpressionOperatorFuncExp;
		}else if(s==L"ln"){
			return TWExpressionOperatorFuncLn;
		}else if(s==L"abs"){
			return TWExpressionOperatorFuncAbs;
		}else if(s==L"trunc"){
			return TWExpressionOperatorFuncTrunc;
		}else if(s==L"floor"){
			return TWExpressionOperatorFuncFloor;
		}else if(s==L"ceil"){
			return TWExpressionOperatorFuncCeil;
		}else if(s==L"sin"){
			return TWExpressionOperatorFuncSin;
		}else if(s==L"cos"){
			return TWExpressionOperatorFuncCos;
		}else if(s==L"tan"){
			return TWExpressionOperatorFuncTan;
		}else if(s==L"asin"){
			return TWExpressionOperatorFuncAsin;
		}else if(s==L"acos"){
			return TWExpressionOperatorFuncAcos;
		}else if(s==L"atan"){
			return TWExpressionOperatorFuncAtan;
		}else if(s==L"not"){
			return TWExpressionOperatorNot;
		}
	}
	return TWExpressionOperatorInvalid;
}


int TWExpressionEvaluator::currentBinaryOperatorType(){
	if(m_reader.currentTokenType()!=TWExpressionTokenTypeOperator
	   && m_reader.currentTokenType()!=TWExpressionTokenTypeSymbol){
		return TWExpressionOperatorInvalid;
	}
	
	if(m_reader.currentTokenLength()==1){
		switch(m_reader.currentTokenChar()){
			case L'e':
				return TWExpressionOperatorExponent;
			case L'^':
				return TWExpressionOperatorPower;
			case L'+':
				return TWExpressionOperatorAdd;
			case L'-':
				return TWExpressionOperatorSub;
			case L'*':
				return TWExpressionOperatorMul;
			case L'/':
				return TWExpressionOperatorDiv;
			case L'=':
				return TWExpressionOperatorEqual;
			case L'<':
				return TWExpressionOperatorLessThan;
			case L'>':
				return TWExpressionOperatorGreaterThan;
				
		}
	}else{
		std::wstring s=m_reader.currentToken();
		if(s==L"div"){
			return TWExpressionOperatorDiv;
		}else if(s==L"mod"){
			return TWExpressionOperatorMod;
		}else if(s==L"round"){
			return TWExpressionOperatorRound;
		}else if(s==L"<>" || s==L"!="){
			return TWExpressionOperatorNotEqual;
		}else if(s==L"<="){
			return TWExpressionOperatorLEThan;
		}else if(s==L">="){
			return TWExpressionOperatorGEThan;
		}else if(s==L"and"){
			return TWExpressionOperatorAnd;
		}else if(s==L"or"){
			return TWExpressionOperatorOr;
		}
	}
	return TWExpressionOperatorInvalid;
}

TWNumber TWExpressionEvaluator::evaluate(){
	TWExpressionStackFrame stack(*this);
	return stack.evaluate();
}


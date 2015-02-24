//
//  TWExpressionEvaluator.h
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

#pragma once

#include "TWNumber.h"

class TWExpressionReader;
class TWExpressionEvaluator;

enum{
	TWExpressionOperatorInvalid,
	TWExpressionOperatorNegate,
	TWExpressionOperatorExponent,
	TWExpressionOperatorFuncExp,
	TWExpressionOperatorFuncLn,
	TWExpressionOperatorFuncAbs,
	TWExpressionOperatorFuncTrunc,
	TWExpressionOperatorFuncFloor,
	TWExpressionOperatorFuncCeil,
	TWExpressionOperatorFuncSin,
	TWExpressionOperatorFuncCos,
	TWExpressionOperatorFuncTan,
	TWExpressionOperatorFuncAsin,
	TWExpressionOperatorFuncAcos,
	TWExpressionOperatorFuncAtan,
	TWExpressionOperatorNot,
	TWExpressionOperatorPower,
	TWExpressionOperatorMul,
	TWExpressionOperatorDiv,
	TWExpressionOperatorMod,
	TWExpressionOperatorAdd,
	TWExpressionOperatorSub,
	TWExpressionOperatorRound,
	TWExpressionOperatorEqual,
	TWExpressionOperatorNotEqual,
	TWExpressionOperatorLessThan,
	TWExpressionOperatorGreaterThan,
	TWExpressionOperatorLEThan,
	TWExpressionOperatorGEThan,
	TWExpressionOperatorAnd,
	TWExpressionOperatorOr,
	TWExpressionOperatorPositive,
	
	TWExpressionOperatorsCount
};

struct TWExpressionOperator{
	int precedence;
	int argumentCount;
	union{
		TWNumber (*unaryCallback)(const TWNumber&);
		TWNumber (*binaryCallback)(const TWNumber&, const TWNumber&);
	};
};

struct TWExpressionSubStackFrame{
	int op;
	int precedence;
	TWNumber num1, num2;
};

class TWExpressionStackFrame{
	TWExpressionEvaluator& m_evaluator;
	
	// depth of stack cannot be deeper than
	// number of precedences.
	TWExpressionSubStackFrame m_stack[16];
	int m_stackPos;
	
	TWNumber parseLiteral(); // -*{(EXPR)|NUMERIC|SYMBOL}
	TWNumber parseFunctions(); // {FUNCNAME func|lit{ e lit}*}
	
	void completeTopStack(); // num1 ~ num2 -> num1
	void popStack(); // complete & num1 -> parent.num2
	
public:
	
	TWExpressionStackFrame(TWExpressionEvaluator&);
	
	TWNumber evaluate();
	
	TWExpressionReader& reader() const;
};

class TWExpressionEvaluator{
	
	friend class TWExpressionStackFrame;
	
	TWExpressionReader& m_reader;

	TWExpressionOperator m_operators[TWExpressionOperatorsCount];
	int currentUnaryOperatorType();
	int currentBinaryOperatorType();
	
public:
	
	TWExpressionEvaluator(TWExpressionReader&);
	~TWExpressionEvaluator();
	
	TWNumber evaluate();
	
	TWExpressionReader& reader() const{
		return m_reader;
	}
	
};


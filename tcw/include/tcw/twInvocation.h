/*
 *  twInvocation.h
 *  
 *
 *  Created by Nexhawks on 2/12/11.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#pragma once
#include <tcw/twTypes.h>
#include <tcw/twObject.h>

/* 
 * each instance of this class represents one invocation.
 * it must be deleted after each used.
 */
class twInvocation{
protected:
	twObject *m_associatedObject;
public:
	volatile bool m_valid;
	twInvocation(){
		m_valid=true;
		m_associatedObject=NULL;
	}
	twInvocation(twObject *obj){
		m_valid=true;
		m_associatedObject=obj;
		if(m_associatedObject){
			m_associatedObject->associateToInvocation(this);
		}
	}
	virtual void invoke()=0;
	virtual ~twInvocation();
};

class twNoArgumentGlobalFunctionInvocation : public twInvocation{
public:
	typedef void(*FunctionType)();
protected:
	FunctionType m_targetFunction;
public:
	twNoArgumentGlobalFunctionInvocation(FunctionType targetFunction){
		m_targetFunction=targetFunction;
	}
	virtual void invoke(){
		if(m_valid)
			(*m_targetFunction)();
	}
};

template <class arg1Type>
class twOneArgumentGlobalFunctionInvocation : public twInvocation{
public:
	typedef void(*FunctionType)(arg1Type);
protected:
	FunctionType m_targetFunction;	
	arg1Type arg1Value;
public:
	twOneArgumentGlobalFunctionInvocation(FunctionType targetFunction, arg1Type arg1){
		m_targetFunction=targetFunction;
		arg1Value=arg1;
	}
	virtual void invoke(){
		if(m_valid)
		(*m_targetFunction)(arg1Value);
	}
};

template <class arg1Type, class arg2Type>
class twTwoArgumentGlobalFunctionInvocation : public twInvocation{
public:
	typedef void(*FunctionType)(arg1Type, arg2Type);
protected:
	FunctionType m_targetFunction;	
	arg1Type arg1Value;
	arg2Type arg2Value;
public:
	twTwoArgumentGlobalFunctionInvocation(FunctionType targetFunction, arg1Type arg1, arg2Type arg2){
		m_targetFunction=targetFunction;
		arg1Value=arg1;
		arg2Value=arg2;
	}
	virtual void invoke(){
		if(m_valid)
		(*m_targetFunction)(arg1Value, arg2Value);
	}
};

template <class arg1Type, class arg2Type, class arg3Type, void(*targetFunction)(arg1Type, arg2Type, arg3Type)>
class twThreeArgumentGlobalFunctionInvocation : public twInvocation{
protected:
	arg1Type arg1Value;
	arg2Type arg2Value;
	arg3Type arg3Value;
public:
	twThreeArgumentGlobalFunctionInvocation(arg1Type arg1, arg2Type arg2, arg3Type arg3){
		arg1Value=arg1;
		arg2Value=arg2;
		arg3Value=arg3;
	}
	virtual void invoke(){
		if(m_valid)
		targetFunction(arg1Value, arg2Value, arg3Value);
	}
};

template <class targetClass>
class twNoArgumentMemberFunctionInvocation : public twInvocation{
protected:
	targetClass *target;
	void(targetClass::*targetFunction)();
public:
	twNoArgumentMemberFunctionInvocation(targetClass *tgt,void(targetClass::*func)()):
	twInvocation(dynamic_cast<twObject *>(tgt)){
		target=tgt;
		targetFunction=func;
	}	
	
	virtual void invoke(){
		if(m_valid && target)
		(target->*targetFunction)();
	}
};

template <class targetClass, class arg1Type>
class twOneArgumentMemberFunctionInvocation : public twInvocation{
protected:
	targetClass *target;
	arg1Type arg1Value;
	void(targetClass::*targetFunction)(arg1Type);
public:
	twOneArgumentMemberFunctionInvocation(targetClass *tgt, void(targetClass::*func)(arg1Type),
										  arg1Type arg1):
	twInvocation(dynamic_cast<twObject *>(tgt)){
		target=tgt;
		targetFunction=func;
		arg1Value=arg1;
	}	
	virtual void invoke(){
		if(m_valid && target)
		(target->*targetFunction)(arg1Value);
	}
};

template <class targetClass, class arg1Type, class arg2Type>
class twTwoArgumentMemberFunctionInvocation : public twInvocation{
protected:
	targetClass *target;
	arg1Type arg1Value;
	arg2Type arg2Value;
	void(targetClass::*targetFunction)(arg1Type, arg2Type);
public:
	twTwoArgumentMemberFunctionInvocation(targetClass *tgt, void(targetClass::*func)(arg1Type, arg2Type),
										  arg1Type arg1, arg2Type arg2):
	twInvocation(dynamic_cast<twObject *>(tgt)){
		target=tgt;
		arg1Value=arg1;
		arg2Value=arg2;
		targetFunction=func;
	}	
	virtual void invoke(){
		if(m_valid && target)
		(target->*targetFunction)(arg1Value, arg2Value);
	}
};

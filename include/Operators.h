#pragma once
#ifndef OPERATORS_H
#define OPERATORS_H

#define makeBinaryOperatorClass(name,leftType,op,rightType,returnType) \
class Operator_##leftType##_##name##_##rightType : public BinaryOperatorFunc { \
public: \
	Operator_##leftType##_##name##_##rightType () {	\
		type = new SimpleType( #returnType ); \
	} \
	virtual ~Operator_##leftType##_##name##_##rightType () { \
		delete type;\
	} \
	virtual ScriptObject evaluate(ScriptObject left,ScriptObject right) { \
		return ScriptObject(Script##returnType ( \
			(reinterpret_cast< Script##leftType *>(left.getWrapped()))->getValue() op  \
			(reinterpret_cast< Script##rightType *>(right.getWrapped()))->getValue())); \
	} \
	virtual Type* getType() { \
		return type; \
	} \
private: \
	Type* type; \
}; \
run(getDScriptEngine()->addBinaryOperator( \
BinaryOperatorType(new SimpleType( #leftType ), #op ,new SimpleType( #rightType ) )\
,new Operator_##leftType##_##name##_##rightType ());,Operator_##leftType##_##name##_##rightType)

#define BEGIN_MANUAL_BINARY_OPERATOR(name,returnType) \
class Operator_##name : public BinaryOperatorFunc { \
public: \
	Operator_##name () { \
		type = returnType; \
	} \
	virtual ~Operator_##name () { \
		delete type; \
	} \
	virtual Type* getType() { \
		return type; \
	} \
	virtual ScriptObject evaluate(ScriptObject operatorParamLeft,ScriptObject operatorParamRight) \
	

#define BINARY_OPERATOR_PARAM_LEFT(scriptType) \
reinterpret_cast< Script##scriptType * >(operatorParamLeft.getWrapped())->getValue();

#define BINARY_OPERATOR_PARAM_RIGHT(scriptType) \
reinterpret_cast< Script##scriptType * >(operatorParamRight.getWrapped())->getValue();

#define END_MANUAL_BINARY_OPERATOR(name,returnType,leftType,op,rightType) \
private: \
	Type* type; \
}; \
run(getDScriptEngine()->addBinaryOperator( \
BinaryOperatorType( leftType , op ,rightType )\
,new Operator_##name ());,Operator_##name##_instanciate)

#define makeUnaryOperatorClass(name,op,rightType,returnType) \
class Operator_##name##_##rightType : public UnaryOperatorFunc { \
public: \
	Operator_##name##_##rightType () {	\
		type = new SimpleType( #returnType ); \
	} \
	virtual ~Operator_##name##_##rightType () { \
		delete type;\
	} \
	\
	virtual ScriptObject evaluate(ScriptObject right) { \
		return ScriptObject(Script##returnType ( op  \
			(reinterpret_cast< Script##rightType *>(right.getWrapped()))->getValue())); \
	} \
	virtual Type* getType() { \
		return type; \
	} \
	private: \
		Type* type; \
}; \
run(getDScriptEngine()->addUnaryOperator( \
UnaryOperatorType( #op ,new SimpleType( #rightType ) )\
,new Operator_##name##_##rightType ());,Operator_##leftType##_##name##_##rightType)

#include "ScriptType.h"
#include "ScriptEngine.h"
#include "Expression.h"
#include "ScriptObject.h"
#include <string>

namespace dscript {
	
	class BinarySetOperator : public Expression {
	public:
		BinarySetOperator(VarExpr* set, Expression* equalTo);
		virtual ~BinarySetOperator();
		virtual ScriptObject evaluate(Scope*);
		virtual bool verify();
		virtual Type* getType();
	private:
		VarExpr* set;
		Expression* equalTo;
	};
	class UnarySetOperator : public Expression {
	public:
		UnarySetOperator(VarExpr* set,std::string op);
		UnarySetOperator(std::string op,VarExpr* set);
		virtual ~UnarySetOperator();
		virtual ScriptObject evaluate(Scope*);
		virtual bool verify();
		virtual Type* getType();
	protected:
		virtual void initialize(VarExpr* set,std::string op,bool opFirst);
	private:
		VarExpr* set;
		std::string op;
		bool opFirst;
	};
	
	class BinaryOperatorType {
	public:
		BinaryOperatorType(Type* left,std::string op,Type* right);
		virtual ~BinaryOperatorType();
		
		virtual bool operator<(const BinaryOperatorType&) const;
		virtual bool operator==(const BinaryOperatorType&) const;
		
	private:
		Type* left;
		std::string op;
		Type* right;
		std::string name;
	};
	
	class BinaryOperatorFunc {
	public:
		BinaryOperatorFunc();
		virtual ~BinaryOperatorFunc();
		
		virtual ScriptObject evaluate(ScriptObject left,ScriptObject right)=0;
		virtual Type* getType()=0;
	};
	
	class BinaryOperator : public Expression {
	public:
		BinaryOperator(Expression* left,std::string op,Expression* right);
		virtual ~BinaryOperator();
		
		virtual ScriptObject evaluate(Scope* s);
		virtual Type* getType();
		virtual bool verify();
	private:
		Expression* left;
		std::string op;
		Expression* right;
		BinaryOperatorFunc* opObj;
	};
	
	class UnaryOperatorType {
	public:
		UnaryOperatorType(std::string op,Type* right);
		virtual ~UnaryOperatorType();
		
		virtual bool operator<(const UnaryOperatorType&) const;
		virtual bool operator==(const UnaryOperatorType&) const;
		
	private:
		std::string op;
		Type* right;
		std::string name;
	};
	
	class UnaryOperatorFunc {
	public:
		UnaryOperatorFunc();
		virtual ~UnaryOperatorFunc();
		
		virtual ScriptObject evaluate(ScriptObject right)=0;
		virtual Type* getType()=0;
	};
	
	class UnaryOperator : public Expression {
	public:
		UnaryOperator(std::string op,Expression* right);
		virtual ~UnaryOperator();
		
		virtual ScriptObject evaluate(Scope* s);
		virtual Type* getType();
		virtual bool verify();
	private:
		std::string op;
		Expression* right;
		UnaryOperatorFunc* opObj;
	};
}
#endif
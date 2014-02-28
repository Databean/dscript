#pragma once
#ifndef OPERATORS_H
#define OPERATORS_H

#define scriptBinaryOperator(name,leftType,rightType,fn) \
run(auto f = fn;\
	getDScriptEngine()->addBinaryOperator( \
BinaryOperatorType(new CPPType<leftType>(), name, new CPPType<rightType>()), \
std::unique_ptr<BinaryOperatorFunc>(new BinaryOperatorCPPFunc<decltype(f(leftType(),rightType())),leftType,rightType>(f)));)

#define scriptUnaryOperator(name,type,fn) \
run(auto f = fn; \
	getDScriptEngine()->addUnaryOperator( \
UnaryOperatorType(name, new CPPType<type>), std::unique_ptr<UnaryOperatorFunc>(new UnaryOperatorCPPFunc<decltype(f(type())),type>(f)));)

#include "ScriptType.h"
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
		virtual Type& getType();
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
		virtual Type& getType();
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
		virtual Type& getType()=0;
	};
	
	template<typename Ret, typename LeftSide, typename RightSide>
	class BinaryOperatorCPPFunc : public BinaryOperatorFunc {
	private:
		typedef Ret(*Fn)(LeftSide,RightSide);
		Fn fn;
		CPPType<Ret> type;
	public:
		BinaryOperatorCPPFunc(Fn fn) : fn(fn) {}
		~BinaryOperatorCPPFunc() {}
		
		virtual ScriptObject evaluate(ScriptObject left, ScriptObject right) {
			CPPObjectWrapper<LeftSide>* lhs = dynamic_cast<CPPObjectWrapper<LeftSide>*>(left.getWrapped());
			if(!lhs) {
				throw "left side of operator type incorrect";
			}
			CPPObjectWrapper<RightSide>* rhs = dynamic_cast<CPPObjectWrapper<RightSide>*>(right.getWrapped());
			if(!rhs) {
				throw "right side of operator type incorrect";
			}
			return ScriptObject(CPPObjectWrapper<Ret>(fn(lhs->getValue(),rhs->getValue())));
		}
		virtual Type& getType() {
			return type;
		}
	};
	
	class BinaryOperator : public Expression {
	public:
		BinaryOperator(Expression* left,std::string op,Expression* right);
		virtual ~BinaryOperator();
		
		virtual ScriptObject evaluate(Scope* s);
		virtual Type& getType();
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
		virtual Type& getType()=0;
	};
	
	template<typename Ret, typename ParamType>
	class UnaryOperatorCPPFunc : public UnaryOperatorFunc {
	private:
		typedef Ret(*Fn)(ParamType);
		Fn fn;
		CPPType<Ret> type;
	public:
		UnaryOperatorCPPFunc(Fn fn) : fn(fn) {}
		~UnaryOperatorCPPFunc() {}
		
		virtual ScriptObject evaluate(ScriptObject param) {
			CPPObjectWrapper<ParamType>* par = dynamic_cast<CPPObjectWrapper<ParamType>*>(param.getWrapped());
			if(!par) {
				throw "right side of operator type incorrect";
			}
			return ScriptObject(CPPObjectWrapper<Ret>(fn(par->getValue())));
		}
		virtual Type& getType() {
			return type;
		}
	};
		
	
	class UnaryOperator : public Expression {
	public:
		UnaryOperator(std::string op,Expression* right);
		virtual ~UnaryOperator();
		
		virtual ScriptObject evaluate(Scope* s);
		virtual Type& getType();
		virtual bool verify();
	private:
		std::string op;
		Expression* right;
		UnaryOperatorFunc* opObj;
	};
}
#endif
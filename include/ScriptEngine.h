#pragma once
#ifndef SCRIPT_ENGINE_H
#define SCRIPT_ENGINE_H

#include "Node.h"
#include "Function.h"
#include "Operators.h"

namespace dscript {
	
	class Variable;
	class BinaryOperatorFunc;
	class UnaryOperatorFunc;
	class Function;
	class Statement;
	
	class ScriptEngine : public ScopedNode {
	public:
		ScriptEngine();
		virtual ~ScriptEngine();
		virtual bool verify();
		virtual Scope* getScope();
		
		virtual void addVariable(Variable* v);
		virtual Variable* getVariable(std::string name);
		
		virtual void addBinaryOperator(BinaryOperatorType, std::unique_ptr<BinaryOperatorFunc>);
		virtual BinaryOperatorFunc* getBinaryOperator(BinaryOperatorType);
		
		virtual void addUnaryOperator(UnaryOperatorType, std::unique_ptr<UnaryOperatorFunc>);
		virtual UnaryOperatorFunc* getUnaryOperator(UnaryOperatorType);
		
		virtual Function* getFunction(FunctionPrototype);
		virtual void addFunction(FunctionPrototype, std::unique_ptr<Function> f);
		virtual void addFunction(const std::string& name, std::unique_ptr<Function> f);
		
	private:
		std::map<BinaryOperatorType,std::unique_ptr<BinaryOperatorFunc>> binaryOperators;
		std::map<UnaryOperatorType,std::unique_ptr<UnaryOperatorFunc>> unaryOperators;
		std::map<FunctionPrototype,std::unique_ptr<Function>> functions;
		Scope* scope;
	};
	
	class Program : public ScopedNode {
	public:
		Program(std::vector<Statement*>*);
		virtual ~Program();
		
		virtual bool verify();
		virtual void evaluate();
		
		virtual Function* getFunction(FunctionPrototype fp);
		
	private:
		std::map<FunctionPrototype,Function*> functions;
		std::vector<Statement*>* statements;
		Scope scope;
	};
	
	ScriptEngine* getDScriptEngine();
}

#endif
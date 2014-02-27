#pragma once
#ifndef SCRIPT_ENGINE_H
#define SCRIPT_ENGINE_H

namespace dscript {
	
	class ScriptEngine : public ScopedNode {
	public:
		ScriptEngine();
		virtual ~ScriptEngine();
		virtual bool verify();
		virtual Scope* getScope();
		
		virtual void addVariable(Variable* v);
		virtual Variable* getVariable(std::string name);
		
		virtual void addBinaryOperator(BinaryOperatorType,BinaryOperatorFunc*);
		virtual BinaryOperatorFunc* getBinaryOperator(BinaryOperatorType);
		
		virtual void addUnaryOperator(UnaryOperatorType,UnaryOperatorFunc*);
		virtual UnaryOperatorFunc* getUnaryOperator(UnaryOperatorType);
		
		virtual Function* getFunction(FunctionPrototype);
		virtual void addFunction(FunctionPrototype,Function* f);
		virtual void addFunction(const std::string& name, Function* f);
		
	private:
		std::map<BinaryOperatorType,BinaryOperatorFunc*> binaryOperators;
		std::map<UnaryOperatorType,UnaryOperatorFunc*> unaryOperators;
		std::map<FunctionPrototype,Function*> functions;
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
		Scope* scope;
	};
	
	ScriptEngine* getDScriptEngine();
}

#endif
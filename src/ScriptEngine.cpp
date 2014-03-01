#include "ScriptEngine.h"

#include <iostream>

#include "ScriptLib.h"
#include "Function.h"
#include "Declaration.h"

namespace dscript {
	ScriptEngine::ScriptEngine() : scope(nullptr) {
		std::cout << "initializing script engine" << std::endl;
		
		addPrintingLibrary(*this);
	}
	ScriptEngine::~ScriptEngine() {
		
	}
	bool ScriptEngine::verify() {
		return true;
	}
	Variable* ScriptEngine::getVariable(std::string name) {
		return scope->getVariable(name);
	}
	void ScriptEngine::addVariable(Variable* v) {
		ScopedNode::addVariable(v);
		scope->addVariable(v);
	}
	Scope* ScriptEngine::getScope() {
		return scope;
	}
	void ScriptEngine::addBinaryOperator(BinaryOperatorType type, std::unique_ptr<BinaryOperatorFunc> func) {
		binaryOperators[type] = std::move(func);
	}
	BinaryOperatorFunc* ScriptEngine::getBinaryOperator(BinaryOperatorType type) {
		return binaryOperators[type].get();
	}
	void ScriptEngine::addUnaryOperator(UnaryOperatorType type, std::unique_ptr<UnaryOperatorFunc> func) {
		unaryOperators[type] = std::move(func);
	}
	UnaryOperatorFunc* ScriptEngine::getUnaryOperator(UnaryOperatorType type) {
		return unaryOperators[type].get();
	}
	Function* ScriptEngine::getFunction(FunctionPrototype fp) {
		return functions[fp].get();
	}
	void ScriptEngine::addFunction(FunctionPrototype fp, std::unique_ptr<Function> f) {
		functions[fp] = std::move(f);
	}
	void ScriptEngine::addFunction(const std::string& name, std::unique_ptr<Function> f) {
		functions[FunctionPrototype(name, f->getType()->getParamTypes())] = std::move(f);
	}
	
	Program::Program(std::vector<Statement*>* statements) : scope(getDScriptEngine()->getScope()) {
		this->statements = statements;
		
		for(size_t i=0;i<statements->size();i++) {
			statements->at(i)->setParent(this);
			FunctionDeclaration* fd = dynamic_cast<FunctionDeclaration*>(statements->at(i));
			if(fd) {
				functions[fd->getPrototype()]=fd->getFunction();
				fd->getFunction()->setParentScope(&scope);
			}
		}
	}
	Program::~Program() {
		for(Statement* statement : *statements) {
			delete statement;
		}
		delete statements;
	}
	bool Program::verify() {
		bool ret = true;
		for(std::vector<Statement*>::iterator it = statements->begin(); it != statements->end(); it++) {
			ret &= (*it)->verify();
		}
		return ret;
	}
	void Program::evaluate() {
		for(std::vector<Statement*>::iterator it = statements->begin(); it !=statements->end(); it++) {
			(*it)->evaluate(&scope);
		}
	}
	Function* Program::getFunction(FunctionPrototype fp) {
		if(functions.find(fp)!=functions.end()) {
			return functions[fp];
		}
		return parent->getFunction(fp);
	}
	
	ScriptEngine* getDScriptEngine() {
		static ScriptEngine* DScriptEngine = new ScriptEngine();
		return DScriptEngine;
	}
}
#include "Debug.h"
#include "Include.h"

namespace dscript {
	ScriptEngine::ScriptEngine() {
		std::cout << "initializing script engine" << std::endl;
		
		scope = new Scope(NULL);
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
	void ScriptEngine::addBinaryOperator(BinaryOperatorType type,BinaryOperatorFunc* func) {
		binaryOperators[type] = func;
	}
	BinaryOperatorFunc* ScriptEngine::getBinaryOperator(BinaryOperatorType type) {
		return binaryOperators[type];
	}
	void ScriptEngine::addUnaryOperator(UnaryOperatorType type,UnaryOperatorFunc* func) {
		unaryOperators[type] = func;
	}
	UnaryOperatorFunc* ScriptEngine::getUnaryOperator(UnaryOperatorType type) {
		return unaryOperators[type];
	}
	Function* ScriptEngine::getFunction(FunctionPrototype fp) {
		return functions[fp];
	}
	void ScriptEngine::addFunction(FunctionPrototype fp,Function* f) {
		functions[fp]=f;
	}
	
	Program::Program(std::vector<Statement*>* statements) {
		this->statements = statements;
		this->scope = getDScriptEngine()->getScope();
		
		for(size_t i=0;i<statements->size();i++) {
			statements->at(i)->setParent(this);
			FunctionDeclaration* fd = dynamic_cast<FunctionDeclaration*>(statements->at(i));
			if(fd) {
				functions[fd->getPrototype()]=fd->getFunction();
				fd->getFunction()->setParentScope(scope);
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
			(*it)->evaluate(scope);
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
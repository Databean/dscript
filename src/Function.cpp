#include "Debug.h"
#include "Include.h"

namespace dscript {
	
	Function::Function() {
		
	}
	Function::~Function() {
		
	}
	
	ScriptFunction::ScriptFunction(Type* retType,std::vector<Declaration*>* parameters,Statement* run) {
		this->parameters = parameters;
		this->run = run;
		
		std::vector<Type*>* paramTypes = new std::vector<Type*>;
		for(std::vector<Declaration*>::iterator it = parameters->begin(); it != parameters->end(); it++) {
			paramTypes->push_back((*it)->getType());
		}
		type = new FunctionType(retType,paramTypes);
	}
	ScriptFunction::~ScriptFunction() {
		delete paramTypes;
	}
	void ScriptFunction::setParentScope(Scope* s) {
		parentscope = s;
	}
	ScriptObject ScriptFunction::call(std::vector<ScriptObject>& paramValues) {
		Scope* scope = new Scope(parentscope);
		
		for(std::vector<Declaration*>::iterator it = parameters->begin(); it != parameters->end(); it++) {
			(*it)->evaluate(scope);
		}
		
		for(size_t i=0;i<paramValues.size();i++) {
			(scope->getVariable(parameters->at(i)->getName()))->setValue(paramValues[i]);
		}
		
		Scope* s = new Scope(scope);
		ScriptObject ret = run->evaluate(scope);
		delete s;
		delete scope;
		return ret;
	}/*
	ScriptObject* ScriptFunction::clone() {
		return new ScriptFunction(type,parameters,run,parentscope);
	}
	*/
	FunctionType* ScriptFunction::getType() {
		return type;
	}
	
	FunctionPrototype::FunctionPrototype(std::string name,std::vector<Type*>* params) {
		this->params = params;
		repString = name;
		for(std::vector<Type*>::iterator it = params->begin(); it < params->end(); it++) {
			repString += " " + (*it)->getName();
		}
	}
	FunctionPrototype::~FunctionPrototype() {
		
	}
	std::string FunctionPrototype::getString() const {
		return repString;
	}
	bool FunctionPrototype::operator<(const FunctionPrototype& ft) const {
		return repString < ft.repString;
	}
	bool FunctionPrototype::operator==(const FunctionPrototype& ft) const {
		return repString == ft.repString;
	}
}
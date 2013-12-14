#include "Include.h"
#include "Debug.h"

namespace dscript {
	
	Declaration::Declaration(Type* type,std::string* name) {
		this->type = type;
		this->name = *name;
		delete name;
	}
	Declaration::~Declaration() {
		delete type;
	}
	std::string Declaration::getName() {
		return name;
	}
	Type* Declaration::getType() {
		return type;
	}
	
	VarDeclaration::VarDeclaration(Type* type,std::string* name,Expression* initialValue) : Declaration(type,name), initialValue(initialValue) {
		if(initialValue) {
			initialValue->setParent(this);
		}
	}
	/*
	VarDeclaration::VarDeclaration(std::string* name,Expression* initialValue) {
		this->name = *name;
		delete name;
		this->initialValue = initialValue;
		initialValue->setParent(this);
		this->type = initialValue->getType();
	}*/
	VarDeclaration::~VarDeclaration() {
		if(initialValue) { delete initialValue; }
	}
	bool VarDeclaration::verify() {
		Variable* me = new NamedVariable(name,type,type->defaultObject());
		if(initialValue) {
			if(!(initialValue->verify())) {
				std::cout << "variable initial value bad" << std::endl;
				delete me;
				return false;
			}
			if(!(initialValue->getType() == *(type))) {
				std::cout << "initial value does not match type" << std::endl;
				delete me;
				return false;
			}
		}
		if(varLocalExists(name)) {
			std::cout << "variable " << name << " already declared in this scope" << std::endl;
			delete me;
			return false;
		}
		addVariable(me);
		return true;
	}
	ScriptObject VarDeclaration::evaluate(Scope* scope) {
		Variable* me = new NamedVariable(name,type,type->defaultObject());
		scope->addVariable(me);
		if(initialValue) {
			me->setValue(initialValue->evaluate(scope));
		}
		return ScriptObject();
	}
	
	FunctionDeclaration::FunctionDeclaration(std::string* name,Type* retType,std::vector<Declaration*>* vd,Statement* run) {
		executionBlock = new StatementBlock(run);
		paramTypes = new std::vector<Type*>;
		for(std::vector<Declaration*>::iterator it = vd->begin(); it != vd->end(); it++) {
			paramTypes->push_back((*it)->getType());
		}
		
		std::vector<Statement*>* varsRun = new std::vector<Statement*>;
		varsRun->assign(vd->begin(),vd->end());
		varsRun->push_back(executionBlock);
		varBlock = new StatementBlock(varsRun);
		
		function = new ScriptFunction(retType,vd,executionBlock);
		this->name = *name;
		delete name;
		this->vd = vd;
		this->run = run;
		varBlock->setParent(this);
	}
	FunctionDeclaration::~FunctionDeclaration() {
		delete function;
		delete varBlock;
	}
	Type* FunctionDeclaration::getType() {
		return function->getType();
	}
	std::string FunctionDeclaration::getName() {
		return name;
	}
	ScriptFunction* FunctionDeclaration::getFunction() {
		return function;
	}
	bool FunctionDeclaration::verify() {
		return varBlock->verify();
	}
	ScriptObject FunctionDeclaration::evaluate(Scope* s) {
		return ScriptObject();
	}
	FunctionPrototype FunctionDeclaration::getPrototype() {
		return FunctionPrototype(this->name,paramTypes);
	}
	
	ReferenceVarDeclaration::ReferenceVarDeclaration(Type* t,std::string* name,Expression* initialValue) : Declaration(t,name) {
		if(initialValue) {
			this->initialValue = initialValue;
			initialValue->setParent(this);
		}
	}
	ReferenceVarDeclaration::~ReferenceVarDeclaration() {
		delete type;
		delete initialValue;
	}
	Type* ReferenceVarDeclaration::getType() {
		return type;
	}
	std::string ReferenceVarDeclaration::getName() {
		return name;
	}
	bool ReferenceVarDeclaration::verify() {
		if(initialValue) {
			if(!initialValue->verify()) {
				std::cout << "variable " << name << " initial value does not verify" << std::endl;
				return false;
			}
			if(!dynamic_cast<ReferenceType*>(&initialValue->getType())) {
				std::cout << "variable " << name << " initial value is not a reference" << std::endl;
			}
		}
		Variable* me = new ReferenceVariable(name,type,new DefaultReference(type));
		if(varLocalExists(name)) {
			std::cout << "variable " << name << " already declared in this scope" << std::endl;
			delete me;
			return false;
		}
		addVariable(me);
		return true;
	}
	ScriptObject ReferenceVarDeclaration::evaluate(Scope* scope) {
		Reference* r = NULL;
		if(initialValue) {
			r = dynamic_cast<Reference*>(initialValue->evaluate(scope).getWrapped());
		} else {
			r = new DefaultReference(type);
		}
		Variable* me = new ReferenceVariable(name,type,r);
		scope->addVariable(me);
		return ScriptObject();
	}
}
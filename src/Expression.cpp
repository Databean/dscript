#include "Include.h"
#include "Debug.h"

namespace dscript {
	Expression::Expression() {
		
	}
	Expression::~Expression() {
		
	}
	
	FunctionCall::FunctionCall(std::string* func,std::vector<Expression*>* params) {
		this->func = *func;
		delete func;
		this->params = params;
		
		for(std::vector<Expression*>::iterator it=params->begin();it!=params->end();it++) {
			(*it)->setParent(this);
		}
	}
	FunctionCall::~FunctionCall() {
		for(std::vector<Expression*>::iterator it=params->begin();it!=params->end();it++) {
			delete (*it);
		}
		if(params) { delete params; }
	}
	ScriptObject FunctionCall::evaluate(Scope* scope) {
		std::vector<ScriptObject> v;
		for(std::vector<Expression*>::iterator it=params->begin();it!=params->end();it++) {
			v.push_back((*it)->evaluate(scope));
		}
		ScriptObject ret = calling->call(v);
		/*
		for(std::vector<ScriptObject*>::iterator it=v.begin();it!=v.end();it++) {
			delete (*it);
		}*/
		return ret;
	}
	bool FunctionCall::verify() {
		if(!getParent()) {
			DEBUG("function call has no parent");
			return false;
		}
		for(std::vector<Expression*>::iterator it=params->begin();it!=params->end();it++) {
			if(!(*it)->verify()) {
				std::cout << "function param does not verify" << std::endl;
				return false;
			}
		}
		std::vector<Type*>* paramTypes = new std::vector<Type*>;
		for(std::vector<Expression*>::iterator it=params->begin();it!=params->end();it++) {
			paramTypes->push_back((*it)->getType());
		}
		FunctionPrototype fp(func,paramTypes);
		if(!(calling = getFunction(fp))) {
			std::cout << "could not resolve function \"" << func << "\"" << std::endl;
			delete paramTypes;
			return false;
		}
		delete paramTypes;
		return true;
	}
	Type* FunctionCall::getType() {
		return dynamic_cast<FunctionType*>(calling->getType())->getReturnType();
	}
	
	StringLiteralExpr::StringLiteralExpr(std::string* s) {
		this->s = *s;
		delete s;
	}
	StringLiteralExpr::~StringLiteralExpr() {
		
	}
	ScriptObject StringLiteralExpr::evaluate(Scope* scope) {
		return ScriptObject(ScriptString(s));
	}
	bool StringLiteralExpr::verify() {
		return true;
	}
	Type* StringLiteralExpr::getType() {
		return &type;
	}
	
	IntLiteralExpr::IntLiteralExpr(int i) {
		this->i = i;
	}
	IntLiteralExpr::~IntLiteralExpr() {
		
	}
	ScriptObject IntLiteralExpr::evaluate(Scope* scope) {
		return ScriptObject(ScriptInt(i));
	}
	bool IntLiteralExpr::verify() {
		return true;
	}
	Type* IntLiteralExpr::getType() {
		return &type;
	}
	
	RealLiteralExpr::RealLiteralExpr(float d) {
		this->d = d;
	}
	RealLiteralExpr::~RealLiteralExpr() {
		
	}
	ScriptObject RealLiteralExpr::evaluate(Scope* scope) {
		return ScriptObject(ScriptReal(d));
	}
	bool RealLiteralExpr::verify() {
		return true;
	}
	Type* RealLiteralExpr::getType() {
		return &type;
	}
	
	BoolLiteralExpr::BoolLiteralExpr(bool b) {
		this->b = b;
	}
	BoolLiteralExpr::~BoolLiteralExpr() {
		
	}
	ScriptObject BoolLiteralExpr::evaluate(Scope* scope) {
		return ScriptObject(ScriptBool(b));
	}
	bool BoolLiteralExpr::verify() {
		return true;
	}
	Type* BoolLiteralExpr::getType() {
		return &type;
	}
	
	ReferenceExpr::ReferenceExpr(VarExpr* ve) {
		this->ve = ve;
		ve->setParent(this);
	}
	ReferenceExpr::~ReferenceExpr() {
		delete ve;
		delete type;
	}
	bool ReferenceExpr::verify() {
		DEBUG("verifying reference expr");
		if(!(ve->verify())) {
			std::cout << "Reference Expr Var Expr does not verify" << std::endl;
			return false;
		}
		DEBUG("blah");
		type = new ReferenceType(ve->getType());
		return true;
	}
	Type* ReferenceExpr::getType() {
		return type;
	}
	ScriptObject ReferenceExpr::evaluate(Scope* s) {
		return ScriptObject(*(ve->getReference(s)));
	}
	
	VarExpr::VarExpr() {
		
	}
	VarExpr::~VarExpr() {
		
	}
	
	IdentVar::IdentVar(std::string* name) {
		this->name = *name;
		delete name;
	}
	IdentVar::~IdentVar() {
		
	}
	void IdentVar::setValue(ScriptObject s,Scope* scope) {
		Variable* v = scope->getVariable(name);
		if(v) {
			v->setValue(s);
		} else {
			std::cout << "wtf?" << std::endl;
		}
	}
	bool IdentVar::verify() {
		Variable* v = getVariable(name);
		if(!v) {
			std::cout << "could not resolve variable " << name << std::endl;
		}
		return v;
	}
	ScriptObject IdentVar::evaluate(Scope* scope) {
		Variable* v = scope->getVariable(name);
		return v->getValue();
	}
	std::string IdentVar::getName() {
		return name;
	}
	Type* IdentVar::getType() {
		Variable* v = getVariable(name);
		return v ? v->getType() : NULL;
	}
	Reference* IdentVar::getReference(Scope* scope) {
		return new VariableReference(scope->getVariable(name));
	}
	
	/*
	FunctionExpression::FunctionExpression(Type* returnType,std::vector<VarDeclaration*>* parameters,Statement* run) {
		this->parameters = parameters;
		this->run = run;
		std::vector<Statement*>* stmtList = new std::vector<Statement*>;
		
		
		std::vector<Type*>* paramTypes = new std::vector<Type*>;
		for(std::vector<VarDeclaration*>::iterator it = parameters->begin(); it != parameters->end(); it++) {
			paramTypes->push_back((*it)->getType());
			stmtList->push_back(*it);
		}
		stmtList->push_back(run);
		fakeBlock = new StatementBlock(stmtList);
		type = new FunctionType(returnType,paramTypes);
		fakeBlock->setParent(this);
		DEBUG("");
	}
	FunctionExpression::~FunctionExpression() {
		delete type;
		delete fakeBlock;
	}
	Type* FunctionExpression::getType() {
		return type;
	}
	bool FunctionExpression::verify() {
		DEBUG("verifying function expression");
		return fakeBlock->verify();
	}
	ScriptFunction* FunctionExpression::evaluate(Scope* scope) {
		return new ScriptFunction(type,parameters,run,scope);
	}*/
}
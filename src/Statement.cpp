#include "Include.h"
#include "Debug.h"

namespace dscript {
	Statement::Statement() {
		
	}
	Statement::~Statement() {
		
	}
	
	StatementBlock::StatementBlock(std::vector<Statement*>* statements) {
		this->statements = statements;
		for(std::vector<Statement*>::iterator i = statements->begin(); i < statements->end(); i++) {
			(*i)->setParent((ScopedNode*)this);
		}
	}
	StatementBlock::StatementBlock(Statement* s) {
		this->statements = new std::vector<Statement*>;
		statements->push_back(s);
		s->setParent((ScopedNode*)this);
	}
	StatementBlock::~StatementBlock() {
		for(std::vector<Statement*>::iterator i = statements->begin(); i < statements->end(); i++) {
			delete (*i);
		}
		delete statements;
	}
	ScriptObject StatementBlock::evaluate(Scope* scope) {
		Scope* s = new Scope(scope);
		for(std::vector<Statement*>::iterator i = statements->begin(); i < statements->end(); i++) {
			ScriptObject so = (*i)->evaluate(s);
			if(so.getWrapped() && !dynamic_cast<Expression*>(*i)) { 
				delete s;
				return so; 
			}
		}
		delete s;
		return ScriptObject();
	}
	bool StatementBlock::verify() {
		if(!ScopedNode::getParent()) {
			DEBUG("statement block lacks a parent");
		}
		bool ret = true;
		for(std::vector<Statement*>::iterator i = statements->begin(); i < statements->end(); i++) {
			ret = ret && (*i)->verify();
		}
		scope.clear();
		return ret;
	}
	void StatementBlock::setParent(Node* n) {
		ScopedNode::setParent(n);
	}
	
	WhileStatement::WhileStatement(Expression* condition,Statement* loop) {
		this->condition = condition;
		this->loop = loop;
		condition->setParent(this);
		loop->setParent(this);
	}
	WhileStatement::~WhileStatement() {
		if(condition) { delete condition; }
		if(loop) { delete loop; }
	}
	bool WhileStatement::verify() {
		if(!condition->verify()) {
			std::cout << "while condition invalid" << std::endl;
			return false;
		}
		Type* t = condition->getType();
		SimpleType* st = dynamic_cast<SimpleType*>(t);
		if(!st) {
			std::cout << "while condition type is not simple" << std::endl;
			return false;
		}
		if(st->getTypeName()!="Bool") {
			std::cout << "while condition type is not Bool" << std::endl;
			return false;
		}
		return loop->verify();
	}
	ScriptObject WhileStatement::evaluate(Scope* scope) {
		while(true) {
			ScriptObject so = condition->evaluate(scope);
			ScriptBool* sb = dynamic_cast<ScriptBool*>(so.getWrapped());
			if(!sb) { return ScriptObject(); }
			if(!sb->getValue()) { return ScriptObject(); }
			ScriptObject so2 = loop->evaluate(scope);
			if(so2.getWrapped()) { return so2; }
		}
		return ScriptObject();
	}
	IfStatement::IfStatement(Expression* condition,Statement* then,Statement* els) {
		condition->setParent(this);
		then->setParent(this);
		if(els) {
			els->setParent(this);
		}
		this->condition = condition;
		this->then = then;
		this->els = els;
	}
	IfStatement::~IfStatement() {
		delete condition;
		delete then;
		if(els) {
			delete els;
		}
	}
	bool IfStatement::verify() {
		if(!(condition->verify())) {
			std::cout << "if statement condition invalid" << std::endl;
			return false;
		}
		if(!(then->verify())) {
			std::cout << "if statement then statement invalid" << std::endl;
			return false;
		}
		if(els && !(els->verify())) {
			std::cout << "if statement else statement invalid" << std::endl;
			return false;
		}
		Type* t = condition->getType();
		SimpleType* st = dynamic_cast<SimpleType*>(t);
		if(!st) {
			std::cout << "conditional type is not simple type" << std::endl;
			return false;
		}
		if(st->getName()!="Bool") {
			std::cout << "conditional type is not Bool" << std::endl;
			return false;
		}
		return true;
	}
	ScriptObject IfStatement::evaluate(Scope* s) {
		ScriptObject so = condition->evaluate(s);
		ScriptBool* sb = dynamic_cast<ScriptBool*>(so.getWrapped());
		if(sb->getValue()) {
			return then->evaluate(s);
		} else if(els) {
			return els->evaluate(s);
		}
		return ScriptObject();
	}
	ReturnStatement::ReturnStatement(Expression* val) {
		this->val = val;
		val->setParent(this);
	}
	ReturnStatement::~ReturnStatement() {
		delete val;
	}
	bool ReturnStatement::verify() {
		if(!(val->verify())) {
			std::cout << "return expression not valid" << std::endl;
		}
		Node* parent = getParent();
		while(parent && !dynamic_cast<FunctionDeclaration*>(parent)) {
			parent = parent->getParent();
		}
		if(!parent) {
			std::cout << "return statement not in function" << std::endl;
			return false;
		}
		FunctionDeclaration* func = dynamic_cast<FunctionDeclaration*>(parent);
		FunctionType* ft = dynamic_cast<FunctionType*>(func->getType());
		if(!ft) {
			std::cout << "function type is not a FunctionType" << std::endl;
			return false;
		}
		if(!(ft->getReturnType()->equals(val->getType()))) {
			std::cout << "return type does not match function type" << std::endl;
			std::cout << "function is supposed to return " << ft->getReturnType()->getName() << std::endl;
			std::cout << "return expression returns something of type " << val->getType()->getName() << std::endl;
			return false;
		}
		return true;
	}
	ScriptObject ReturnStatement::evaluate(Scope* scope) {
		return val->evaluate(scope);
	}
}
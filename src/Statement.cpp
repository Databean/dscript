#include "Statement.h"
#include "Debug.h"

#include "Declaration.h"
#include "Expression.h"

namespace dscript {
	Statement::Statement() {
		
	}
	Statement::~Statement() {
		
	}
	
	StatementBlock::StatementBlock(std::vector<Statement*>* statements) {
		for(auto& statement : *statements) {
			statement->setParent((ScopedNode*)this);
			this->statements.emplace_back(statement);
		}
		delete statements;
	}
	StatementBlock::StatementBlock(Statement* s) {
		statements.emplace_back(s);
		s->setParent((ScopedNode*)this);
	}
	StatementBlock::~StatementBlock() {
		
	}
	ScriptObject StatementBlock::evaluate(Scope* scope) {
		Scope s(scope);
		for(auto& statement : statements) {
			ScriptObject so = statement->evaluate(&s);
			if(so && !dynamic_cast<Expression*>(statement.get())) { 
				return std::move(so); 
			}
		}
		return ScriptObject();
	}
	bool StatementBlock::verify() {
		if(!ScopedNode::getParent()) {
			DEBUG("statement block lacks a parent");
		}
		bool ret = true;
		for(auto& statement : statements) {
			ret = ret && statement->verify();
		}
		scope.clear();
		return ret;
	}
	void StatementBlock::setParent(Node* n) {
		ScopedNode::setParent(n);
	}
	
	WhileStatement::WhileStatement(Expression* condition,Statement* loop) : condition(condition), loop(loop) {
		condition->setParent(this);
		loop->setParent(this);
	}
	WhileStatement::~WhileStatement() {
		
	}
	bool WhileStatement::verify() {
		if(!condition->verify()) {
			std::cout << "while condition invalid" << std::endl;
			return false;
		}
		Type& t = condition->getType();
		CPPType<bool> comp;
		if(!(t == comp)) {
			std::cout << "while condition type is not bool" << std::endl;
			return false;
		}
		return loop->verify();
	}
	ScriptObject WhileStatement::evaluate(Scope* scope) {
		while(true) {
			ScriptObject so = condition->evaluate(scope);
			ScriptBool* sb = dynamic_cast<ScriptBool*>(so.get());
			if(!sb) { return ScriptObject(); }
			if(!sb->getValue()) { return ScriptObject(); }
			ScriptObject so2 = loop->evaluate(scope);
			if(so2) { return std::move(so2); }
		}
		return ScriptObject();
	}
	IfStatement::IfStatement(Expression* condition,Statement* then,Statement* els) : condition(condition), then(then), els(els) {
		condition->setParent(this);
		then->setParent(this);
		if(els) {
			els->setParent(this);
		}
	}
	IfStatement::~IfStatement() {
		
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
		Type& t = condition->getType();
		CPPType<bool> comp;
		if(!(t == comp)) {
			std::cout << "conditional type is not Bool" << std::endl;
			return false;
		}
		return true;
	}
	ScriptObject IfStatement::evaluate(Scope* s) {
		ScriptObject so = condition->evaluate(s);
		ScriptBool* sb = dynamic_cast<ScriptBool*>(so.get());
		if(sb->getValue()) {
			return then->evaluate(s);
		} else if(els) {
			return els->evaluate(s);
		}
		return ScriptObject();
	}
	ReturnStatement::ReturnStatement(Expression* val) : val(val) {
		val->setParent(this);
	}
	ReturnStatement::~ReturnStatement() {
		
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
		if(!(*ft->getReturnType() == *(&val->getType()))) {
			std::cout << "return type does not match function type" << std::endl;
			std::cout << "function is supposed to return " << ft->getReturnType()->getName() << std::endl;
			std::cout << "return expression returns something of type " << val->getType().getName() << std::endl;
			return false;
		}
		return true;
	}
	ScriptObject ReturnStatement::evaluate(Scope* scope) {
		return val->evaluate(scope);
	}
}
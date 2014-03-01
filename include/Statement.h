#ifndef STATEMENT_H
#define STATEMENT_H

#include <memory>

#include "Node.h"
#include "ScriptObject.h"

namespace dscript {
	
	class Expression;
	
	class Statement : public Node {
	public:
		Statement();
		virtual ~Statement();
		virtual ScriptObject evaluate(Scope* scope)=0;
	};
	class StatementBlock : public Statement, ScopedNode {
	public:
		StatementBlock(std::vector<Statement*>*);
		StatementBlock(Statement*);
		virtual ~StatementBlock();
		virtual bool verify();
		virtual ScriptObject evaluate(Scope* scope);
		virtual void setParent(Node* n);
	private:
		std::vector<std::unique_ptr<Statement>> statements;
	};
	class WhileStatement : public Statement {
	public:
		WhileStatement(Expression* condition,Statement* loop);
		virtual ~WhileStatement();
		virtual ScriptObject evaluate(Scope* scope);
		virtual bool verify();
	private:
		std::unique_ptr<Expression> condition;
		std::unique_ptr<Statement> loop;
	};
	class IfStatement : public Statement {
	public:
		IfStatement(Expression* condition,Statement* then,Statement* els=0);
		virtual ~IfStatement();
		virtual ScriptObject evaluate(Scope* scope);
		virtual bool verify();
	private:
		std::unique_ptr<Expression> condition;
		std::unique_ptr<Statement> then;
		std::unique_ptr<Statement> els;
	};
	class ReturnStatement : public Statement {
	public:
		ReturnStatement(Expression* val);
		virtual ~ReturnStatement();
		virtual ScriptObject evaluate(Scope* scope);
		virtual bool verify();
	private:
		std::unique_ptr<Expression> val;
	};
}

#endif
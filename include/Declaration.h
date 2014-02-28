#ifndef DECLARATION_H
#define DECLARATION_H

#include "Statement.h"
#include "ScriptObject.h"

namespace dscript {
	
	class Declaration : public Statement {
	public:
		Declaration(Type* t,std::string* name);
		virtual ~Declaration();
		virtual Type* getType();
		virtual std::string getName();
		virtual bool verify()=0;
		virtual ScriptObject evaluate(Scope* s)=0;
	protected:
		Type* type;
		std::string name;
	};
	
	class VarDeclaration : public Declaration {
	public:
		VarDeclaration(Type* t,std::string* name,Expression* initialValue=0);
		//VarDeclaration(std::string* name,Expression* initialValue);
		virtual ~VarDeclaration();
		virtual bool verify();
		virtual ScriptObject evaluate(Scope* scope);
	private:
		Expression* initialValue;
	};
	
	class FunctionDeclaration : public Statement {
	public:
		FunctionDeclaration(std::string* name,Type* retType,std::vector<Declaration*>* vd,Statement* run);
		virtual ~FunctionDeclaration();
		virtual Type* getType();
		virtual std::string getName();
		virtual ScriptFunction* getFunction();
		virtual bool verify();
		virtual ScriptObject evaluate(Scope* s);
		virtual FunctionPrototype getPrototype();
	private:
		std::string name;
		StatementBlock* varBlock;
		StatementBlock* executionBlock;
		ScriptFunction* function;
		Statement* run;
		std::vector<Declaration*>* vd;
		std::vector<Type*>* paramTypes;
	};
	
	class ReferenceVarDeclaration : public Declaration {
	public:
		ReferenceVarDeclaration(Type* t,std::string* name,Expression* initialValue=0);
		virtual ~ReferenceVarDeclaration();
		virtual Type* getType();
		virtual std::string getName();
		virtual bool verify();
		virtual ScriptObject evaluate(Scope* s);
	private:
		Expression* initialValue;
	};
}

#endif
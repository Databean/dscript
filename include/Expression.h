#ifndef EXPRESSION_H
#define EXPRESSION_H

namespace dscript {
	
	class Expression : public Statement {
	public:
		Expression();
		virtual ~Expression();
		virtual ScriptObject evaluate(Scope*)=0;
		virtual Type& getType()=0;
	};
	
	class StringLiteralExpr : public Expression {
	public:
		StringLiteralExpr(std::string* s);
		virtual ~StringLiteralExpr();
		virtual ScriptObject evaluate(Scope*);
		virtual bool verify();
		virtual Type& getType();
	private:
		std::string s;
		CPPType<std::string> type;
	};
	
	class IntLiteralExpr : public Expression {
	public:
		IntLiteralExpr(int i);
		virtual ~IntLiteralExpr();
		virtual ScriptObject evaluate(Scope*);
		virtual bool verify();
		virtual Type& getType();
	private:
		int i;
		CPPType<int> type;
	};
	
	class RealLiteralExpr : public Expression {
	public:
		RealLiteralExpr(float d);
		virtual ~RealLiteralExpr();
		virtual ScriptObject evaluate(Scope*);
		virtual bool verify();
		virtual Type& getType();
	private:
		float d;
		CPPType<float> type;
	};
	
	class BoolLiteralExpr : public Expression {
	public:
		BoolLiteralExpr(bool b);
		virtual ~BoolLiteralExpr();
		virtual ScriptObject evaluate(Scope*);
		virtual bool verify();
		virtual Type& getType();
	private:
		bool b;
		CPPType<bool> type;
	};
	
	class FunctionCall : public Expression {
	public:
		FunctionCall(std::string* func,std::vector<Expression*>* params);
		virtual ~FunctionCall();
		virtual ScriptObject evaluate(Scope*);
		virtual bool verify();
		virtual Type& getType();
	private:
		std::string func;
		std::vector<Expression*>* params;
		Function* calling;
	};
	
	class ReferenceExpr : public Expression {
	public:
		ReferenceExpr(VarExpr*);
		virtual ~ReferenceExpr();
		virtual bool verify();
		virtual Type& getType();
		virtual ScriptObject evaluate(Scope*);
	private:
		VarExpr* ve;
		Type* type;
	};
	
	class VarExpr : public Expression {
	public:
		VarExpr();
		virtual ~VarExpr();
		virtual void setValue(ScriptObject newVal,Scope*)=0;
		virtual Reference* getReference(Scope*)=0;
		
	};
	
	class IdentVar : public VarExpr {
	public:
		IdentVar(std::string* name);
		virtual ~IdentVar();
		virtual bool verify();
		virtual ScriptObject evaluate(Scope*);
		virtual void setValue(ScriptObject newVal,Scope* scope);
		virtual std::string getName();
		virtual Type& getType();
		virtual Reference* getReference(Scope*);
	private:
		std::string name;
	};
	
	/*
	class FunctionExpression : public Expression {
	public:
		FunctionExpression(Type* returnType,std::vector<VarDeclaration*>* parameters,Statement* run);
		virtual ~FunctionExpression();
		
		virtual ScriptFunction* evaluate(Scope* s);
		virtual bool verify();
		virtual Type& getType();
	private:
		Type* type;
		std::vector<VarDeclaration*>* parameters;
		StatementBlock* fakeBlock;
		Statement* run;
	};*/
}

#endif
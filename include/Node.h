#ifndef NODE_H
#define NODE_H

#include <map>

#include "Function.h"

namespace dscript {
	
	class Variable;
	
	class Scope {
	public:
		Scope(Scope* parent);
		virtual ~Scope();
		
		virtual Variable* getVariable(std::string name);
		virtual void addVariable(Variable* v);
	private:
		std::map<std::string,Variable*> vars;
		Scope* parent;
	};
	
	class Node {
	public:
		Node();
		virtual ~Node();
		
		virtual void setParent(Node* n);
		virtual Node* getParent();
		
		virtual bool verify()=0;
		
		virtual bool varExists(std::string name);
		virtual bool varLocalExists(std::string name);
		virtual Variable* getVariable(std::string name);
		virtual void addVariable(Variable* v);
		virtual Function* getFunction(FunctionPrototype);
		//virtual Node* clone();
	protected:
		Node* parent;
	};
	
	class ScopedNode : public Node {
	public:
		ScopedNode();
		virtual ~ScopedNode();
		
		virtual bool verify()=0;
		
		
		virtual bool varLocalExists(std::string name);
		virtual bool varExists(std::string name);
		virtual Variable* getVariable(std::string name);
		virtual void addVariable(Variable* v);
		virtual void clearScope();
	protected:
		std::map<std::string,Variable*> scope;
	};
	

}

#endif
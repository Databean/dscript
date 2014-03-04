#include "Node.h"
#include "Debug.h"

#include <map>

#include "Variable.h"
#include "Function.h"

using std::map;
using std::unique_ptr;

namespace dscript {
	
	Scope::Scope(Scope* parent) {
		this->parent = parent;
	}
	Scope::~Scope() {
		
	}
	Variable* Scope::getVariable(std::string name) {
		if(vars.find(name)==vars.end()) {
			if(parent) {
				return parent->getVariable(name);
			} else {
				return NULL;
			}
		} else {
			return vars[name].get();
		}
	}
	void Scope::addVariable(std::unique_ptr<Variable> v) {
		vars[v->getName()] = std::move(v);
	}
	
	Node::Node() {
		
	}
	Node::~Node() {
		
	}
	void Node::setParent(Node* parent) {
		this->parent = parent;
	}
	Node* Node::getParent() {
		return parent;
	}
	bool Node::varExists(std::string name) {
		return parent->varExists(name);
	}
	bool Node::varLocalExists(std::string name) {
		return parent->varLocalExists(name);
	}
	Variable* Node::getVariable(std::string name) {
		return parent->getVariable(name);
	}
	Function* Node::getFunction(FunctionPrototype fp) {
		return parent->getFunction(fp);
	}
	void Node::addVariable(std::unique_ptr<Variable> v) {
		parent->addVariable(std::move(v));
	}
	
	//------------------------//
	ScopedNode::ScopedNode() {
		
	}
	ScopedNode::~ScopedNode() {
		
	}
	bool ScopedNode::varLocalExists(std::string name) {
		return scope.find(name)!=scope.end();
	}
	bool ScopedNode::varExists(std::string name) {
		if(!parent) {
			return varLocalExists(name);
		}
		return varLocalExists(name) ? true : parent->varExists(name);
	}
	Variable* ScopedNode::getVariable(std::string name) {
		if(parent) {
			return varLocalExists(name) ? scope[name].get() : parent->getVariable(name);
		}
		return varLocalExists(name) ? scope[name].get() : NULL;
	}
	void ScopedNode::addVariable(std::unique_ptr<Variable> v) {
		scope[v->getName()] = std::move(v);
	}
	void ScopedNode::clearScope() {
		scope.clear();
	}
}
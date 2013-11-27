#include "Include.h"
#include "Debug.h"

namespace dscript {
	
	Scope::Scope(Scope* parent) {
		this->parent = parent;
	}
	Scope::~Scope() {
		for(std::map<std::string,Variable*>::iterator it=vars.begin();it!=vars.end();it++) {
			delete it->second;
		}
	}
	Variable* Scope::getVariable(std::string name) {
		if(vars.find(name)==vars.end()) {
			if(parent) {
				return parent->getVariable(name);
			} else {
				return NULL;
			}
		} else {
			return vars[name];
		}
	}
	void Scope::addVariable(Variable* v) {
		vars[v->getName()]=v;
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
	void Node::addVariable(Variable* v) {
		parent->addVariable(v);
	}
	
	//------------------------//
	ScopedNode::ScopedNode() {
		
	}
	ScopedNode::~ScopedNode() {
		for(std::map<std::string,Variable*>::iterator it = scope.begin(); it != scope.end(); it++) {
			if((*it).second) { delete (*it).second; }
		}
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
			return varLocalExists(name) ? scope[name] : parent->getVariable(name);
		}
		return varLocalExists(name) ? scope[name] : NULL;
	}
	void ScopedNode::addVariable(Variable* v) {
		scope[v->getName()]=v;
	}
	void ScopedNode::clearScope() {
		for(std::map<std::string,Variable*>::iterator it = scope.begin(); it != scope.end(); it++) {
			if(it->second) { delete it->second; }
		}
		scope.clear();
	}
}
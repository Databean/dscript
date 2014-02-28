#include "Variable.h"
#include "Debug.h"

#include "ScriptType.h"

namespace dscript {
	Variable::Variable(std::string name,Type* type) {
		this->name = name;
		this->type = type;
	};
	Variable::~Variable() {
		
	}
	Type* Variable::getType() {
		return type;
	}
	std::string Variable::getName() {
		return name;
	}
	
	NamedVariable::NamedVariable(std::string name,Type* type,ScriptObject initialValue) : Variable(name,type), value(initialValue) {
		
	}
	NamedVariable::~NamedVariable() {
		
	}
	ScriptObject NamedVariable::getValue() {
		return value;
	}
	void NamedVariable::setValue(ScriptObject newval) {
		value = newval;
	}
	
	ReferenceVariable::ReferenceVariable(std::string name,Type* typ,Reference* ref) : Variable(name,typ) {
		this->ref = ref;
	}
	ReferenceVariable::~ReferenceVariable() {
		delete ref;
	}
	ScriptObject ReferenceVariable::getValue() {
		return ref->getValue();
	}
	void ReferenceVariable::setValue(ScriptObject so) {
		Reference* r = dynamic_cast<Reference*>(so.getWrapped());
		if(r) {
			setReference(r);
		} else {
			ref->setValue(so);
		}
	}
	void ReferenceVariable::setReference(Reference* _ref) {
		delete ref;
		ref = _ref;
	}
	
	Reference::Reference() {
		
	}
	Reference::~Reference() {
		
	}
	
	VariableReference::VariableReference(Variable* ref) {
		this->ref = ref;
	}
	VariableReference::~VariableReference() {
		
	}
	Type* VariableReference::getType() const {
		return ref->getType();
	}
	ScriptObject VariableReference::getValue() const {
		return ref->getValue();
	}
	void VariableReference::setValue(ScriptObject so) {
		ref->setValue(so);
	}
	ObjectBase* VariableReference::clone() const {
		return new VariableReference(ref);
	}
	
	DefaultReference::DefaultReference(Type* type) {
		this->type = type;
		this->value = type->defaultObject();
	}
	DefaultReference::~DefaultReference() {
		
	}
	Type* DefaultReference::getType() const {
		return type;
	}
	ScriptObject DefaultReference::getValue() const {
		return value;
	}
	void DefaultReference::setValue(ScriptObject so) {
		this->value = so;
	}
	ObjectBase* DefaultReference::clone() const {
		DefaultReference* ret = new DefaultReference(type);
		ret->setValue(value);
		return ret;
	}
	
}
#ifndef VARIABLE_H
#define VARIABLE_H

#include "ScriptObject.h"

namespace dscript {
	
	class Type;
	class Reference;
	
	class Variable {
	public:
		Variable(std::string,Type*);
		virtual ~Variable();
		virtual Type* getType();
		virtual std::string getName();
		virtual ScriptObject getValue()=0;
		virtual void setValue(ScriptObject)=0;
	private:
		std::string name;
		Type* type;
	};
	
	class NamedVariable : public Variable {
	public:
		NamedVariable(std::string name,Type* type,ScriptObject initialValue=ScriptObject());
		virtual ~NamedVariable();
		ScriptObject getValue();
		void setValue(ScriptObject);
	private:
		Type* type;
		ScriptObject value;
	};
	
	class ReferenceVariable : public Variable {
	public:
		ReferenceVariable(std::string,Type*,Reference* initialValue=0);
		virtual ~ReferenceVariable();
		ScriptObject getValue();
		void setValue(ScriptObject);
		void setReference(Reference*);
	private:
		Reference* ref;
	};
	
	class Reference : public ObjectBase {
	public:
		Reference();
		virtual ~Reference();
		virtual ScriptObject getValue() const =0;
		virtual void setValue(ScriptObject val)=0;
		virtual Type* getType() const =0;
		virtual ObjectBase* clone() const =0;
	};
	
	class VariableReference : public Reference {
	public:
		VariableReference(Variable*);
		virtual ~VariableReference();
		virtual ScriptObject getValue() const;
		virtual void setValue(ScriptObject);
		virtual Type* getType() const;
		virtual ObjectBase* clone() const;
	private:
		Variable* ref;
	}; 
	
	class DefaultReference : public Reference {
	public:
		DefaultReference(Type* t);
		virtual ~DefaultReference();
		virtual ScriptObject getValue() const;
		virtual void setValue(ScriptObject);
		virtual Type* getType() const;
		virtual ObjectBase* clone() const;
	private:
		Type* type;
		ScriptObject value;
	};
}

#endif
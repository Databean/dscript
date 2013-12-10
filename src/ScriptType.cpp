#include "Include.h"
#include "Debug.h"

namespace dscript {
	Type::Type() {
		
	}
	Type::~Type() {
		
	}
	
	SimpleType::SimpleType(std::string typeName) {
		this->typeName = typeName;
	}
	SimpleType::SimpleType(std::string* typeName) {
		this->typeName = *typeName;
		delete typeName;
	}
	SimpleType::~SimpleType() {
		
	}
	std::string SimpleType::getTypeName() {
		return typeName;
	}
	std::string SimpleType::getName() {
		return typeName;
	}
	bool SimpleType::equals(Type* other) {
		SimpleType* test = dynamic_cast<SimpleType*>(other);
		if(test) {
			return test->getTypeName()==typeName;
		} else {
			return false;
		}
	}
	bool SimpleType::verify() {
		return getDScriptEngine()->getDefaultObject(this).getWrapped() != 0;
	}
	SimpleType* SimpleType::clone() {
		return new SimpleType(typeName);
	}
	
	template<> std::string CPPType<int>::name = "C++Int";
	template<> std::string CPPType<std::string>::name = "C++String";
	template<> std::string CPPType<float>::name = "C++Real";
	template<> std::string CPPType<bool>::name = "C++Bool";
	template<> std::string CPPType<void>::name = "C++Void";
	
	FunctionType::FunctionType(Type* returnType,std::vector<Type*>* paramTypes) {
		this->returnType = returnType;
		this->paramTypes = paramTypes;
	}
	FunctionType::~FunctionType() {
		if(returnType) { delete returnType; }
		if(paramTypes) {
			for(std::vector<Type*>::iterator i = paramTypes->begin(); i < paramTypes->end();i++) {
				if(*i) {
					delete *i;
				}
			}
			delete paramTypes;
		}
	}
	std::string FunctionType::getName() {
		return "function";
	}
	bool FunctionType::equals(Type* other) {
		FunctionType* test = dynamic_cast<FunctionType*>(other);
		if(test) {
			return true;
		} else {
			return false;
		}
	}
	bool FunctionType::verify() {
		if(!(returnType->verify())) {
			return false;
		}
		for(std::vector<Type*>::iterator i = paramTypes->begin(); i < paramTypes->end();i++) {
			if(!((*i)->verify())) {
				return false;
			}
		}
		return false;
	}
	FunctionType* FunctionType::clone() {
		std::vector<Type*>* newParamVec = new std::vector<Type*>;
		for(std::vector<Type*>::iterator i = paramTypes->begin(); i < paramTypes->end();i++) {
			newParamVec->push_back((*i)->clone());
		}
		return new FunctionType(returnType->clone(),newParamVec);
	}
	Type* FunctionType::getReturnType() {
		return returnType;
	}
	std::vector<Type*>* FunctionType::getParamTypes() {
		return paramTypes;
	}
	
	VoidType::VoidType() {
		
	}
	VoidType::~VoidType() {
		
	}
	bool VoidType::equals(Type* t) {
		return dynamic_cast<VoidType*>(t) == 0;
	}
	std::string VoidType::getName() {
		return "void";
	}
	bool VoidType::verify() {
		return true;
	}
	VoidType* VoidType::clone() {
		return new VoidType();
	}
	
	ReferenceType::ReferenceType(Type* wrapped) {
		this->wrapped = wrapped;
	}
	ReferenceType::~ReferenceType() {
		delete wrapped;
	}
	bool ReferenceType::equals(Type* other) {
		ReferenceType* rt = dynamic_cast<ReferenceType*>(other);
		if(!rt) {
			return wrapped->equals(other);
		}
		return wrapped->equals(rt->wrapped);
	}
	std::string ReferenceType::getName() {
		return wrapped->getName()+"&";
	}
	Type* ReferenceType::getWrapped() {
		return wrapped;
	}
	bool ReferenceType::verify() {
		return wrapped->verify();
	}
	ReferenceType* ReferenceType::clone() {
		return new ReferenceType(wrapped->clone());
	}
}
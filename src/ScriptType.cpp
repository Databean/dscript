#include "Include.h"
#include "Debug.h"

namespace dscript {
	Type::Type() {
		
	}
	Type::~Type() {
		
	}
	
	template<> std::string CPPType<int>::name = "Int";
	template<> ScriptObject CPPType<int>::defaultInstance = ScriptObject(CPPObjectWrapper<int>(0));
	
	template<> std::string CPPType<std::string>::name = "String";
	template<> ScriptObject CPPType<std::string>::defaultInstance = ScriptObject(CPPObjectWrapper<std::string>(""));
	
	template<> std::string CPPType<float>::name = "Real";
	template<> ScriptObject CPPType<float>::defaultInstance = ScriptObject(CPPObjectWrapper<float>(0.f));
	
	template<> std::string CPPType<bool>::name = "Bool";
	template<> ScriptObject CPPType<bool>::defaultInstance = ScriptObject(CPPObjectWrapper<bool>(false));
	
	template<> std::string CPPType<void>::name = "Void";
	template<> ScriptObject CPPType<void>::defaultInstance = ScriptObject();
	
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
	bool FunctionType::operator==(Type& other) {
		FunctionType* test = dynamic_cast<FunctionType*>(&other);
		if(test) {
			if(!(*returnType == *(test->getReturnType()))) {
				return false;
			}
			if(paramTypes->size() != test->getParamTypes()->size()) {
				return false;
			}
			for(auto it1 = paramTypes->begin(), it2 = test->getParamTypes()->begin();
				it1 != paramTypes->end() && it2 != test->getParamTypes()->end();
				it1++, it2++) {
				
				if(!(**it1 == **it2)) {
					return false;
				}
			}
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
	ScriptObject FunctionType::defaultObject() {
		//TODO: but I don't think this needs to exist yet
		return ScriptObject();
	}
	
	VoidType::VoidType() {
		
	}
	VoidType::~VoidType() {
		
	}
	bool VoidType::operator==(Type& t) {
		return dynamic_cast<VoidType*>(&t) == 0;
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
	ScriptObject VoidType::defaultObject() {
		return ScriptObject();
	}
	
	ReferenceType::ReferenceType(Type* wrapped) {
		this->wrapped = wrapped;
	}
	ReferenceType::~ReferenceType() {
		delete wrapped;
	}
	bool ReferenceType::operator==(Type& other) {
		ReferenceType* rt = dynamic_cast<ReferenceType*>(&other);
		if(rt) {
			return *wrapped == *rt->wrapped;
		} else {
			return *wrapped == other;
		}
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
	ScriptObject ReferenceType::defaultObject() {
		return ScriptObject(DefaultReference(wrapped));
	}
}
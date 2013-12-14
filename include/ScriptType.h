#ifndef SCRIPT_TYPE_H
#define SCRIPT_TYPE_H

namespace dscript {
	
	class Type {
	public:
		Type();
		virtual ~Type();
		virtual bool operator==(Type& other)=0;
		virtual std::string getName()=0;
		virtual Type* clone()=0;
		virtual bool verify()=0;
		virtual ScriptObject defaultObject()=0;
	};
	template<typename T>
	class CPPType : public Type {
	private:
		static std::string name;
		static ScriptObject defaultInstance;
	public:
		CPPType() {}
		virtual ~CPPType() {}
		virtual std::string getTypeName() {
			return name;
		}
		virtual std::string getName() {
			return name;
		}
		virtual bool operator==(Type& other) {
			return dynamic_cast<CPPType<T>*>(&other) != NULL;
		}
		virtual CPPType<T>* clone() {
			return new CPPType<T>();
		}
		virtual bool verify() {
			return name != "";
		}
		virtual ScriptObject defaultObject() {
			return defaultInstance;
		}
	};
		
	class FunctionType : public Type {
	public:
		FunctionType(Type* returnType,std::vector<Type*>* paramTypes);
		virtual ~FunctionType();
		virtual bool operator==(Type& other);
		virtual Type* getReturnType();
		virtual std::string getName();
		virtual std::vector<Type*>* getParamTypes();
		virtual FunctionType* clone();
		virtual bool verify();
		virtual ScriptObject defaultObject();
	private:
		Type* returnType;
		std::vector<Type*>* paramTypes;
	};
	class VoidType : public Type {
	public:
		VoidType();
		virtual ~VoidType();
		virtual bool operator==(Type& other);
		virtual std::string getName();
		virtual bool verify();
		virtual VoidType* clone();
		virtual ScriptObject defaultObject();
	};
	class StructType : public Type {
	public:
		StructType(std::vector<VarDeclaration*>*);
		virtual bool operator==(Type& other);
		virtual std::string getName();
		virtual StructType* clone();
		virtual bool verify();
		virtual ScriptObject defaultObject();
	private:
		
	};
	class ReferenceType : public Type {
	public:
		ReferenceType(Type* wrapped);
		virtual ~ReferenceType();
		virtual bool operator==(Type& other);
		virtual std::string getName();
		virtual Type* getWrapped();
		virtual bool verify();
		virtual ReferenceType* clone();
		virtual ScriptObject defaultObject();
	private:
		Type* wrapped;
	};
	
}
#endif
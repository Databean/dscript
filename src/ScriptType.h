#ifndef SCRIPT_TYPE_H
#define SCRIPT_TYPE_H

namespace dscript {
	
	class Type {
	public:
		Type();
		virtual ~Type();
		virtual bool equals(Type* other)=0;
		virtual std::string getName()=0;
		virtual Type* clone()=0;
		virtual bool verify()=0;
	};
	class SimpleType : public Type {
	public:
		SimpleType(std::string typeName);
		SimpleType(std::string* typeName);
		virtual ~SimpleType();
		virtual std::string getTypeName();
		virtual std::string getName();
		virtual bool equals(Type* other);
		virtual SimpleType* clone();
		virtual bool verify();
	private:
		std::string typeName;
	};
	class FunctionType : public Type {
	public:
		FunctionType(Type* returnType,std::vector<Type*>* paramTypes);
		virtual ~FunctionType();
		virtual bool equals(Type* other);
		virtual Type* getReturnType();
		virtual std::string getName();
		virtual std::vector<Type*>* getParamTypes();
		virtual FunctionType* clone();
		virtual bool verify();
	private:
		Type* returnType;
		std::vector<Type*>* paramTypes;
	};
	class VoidType : public Type {
	public:
		VoidType();
		virtual ~VoidType();
		virtual bool equals(Type* other);
		virtual std::string getName();
		virtual bool verify();
		virtual VoidType* clone();
	};
	class StructType : public Type {
	public:
		StructType(std::vector<VarDeclaration*>*);
		virtual bool equals(Type* other);
		virtual std::string getName();
		virtual StructType* clone();
		virtual bool verify();
	private:
		
	};
	class ReferenceType : public Type {
	public:
		ReferenceType(Type* wrapped);
		virtual ~ReferenceType();
		virtual bool equals(Type* other);
		virtual std::string getName();
		virtual Type* getWrapped();
		virtual bool verify();
		virtual ReferenceType* clone();
	private:
		Type* wrapped;
	};
	
}
#endif
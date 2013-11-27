#ifndef FUNCTION_H
#define FUNCTION_H

#define SCRIPT_BEGIN_FUNC(funcID, retType, argc, ...) \
class ScriptFunc_##funcID : public dscript::Function { \
private: \
	dscript::FunctionType* type; \
public: \
	ScriptFunc_##funcID () { \
		std::vector<dscript::Type*>* paramTypes = new std::vector<dscript::Type*>(); \
		dscript::Type* types[] = { __VA_ARGS__ } ; \
		for(int i=0;i<argc;i++) { \
			paramTypes->push_back(types[i]); \
		} \
		type = new dscript::FunctionType(retType,paramTypes); \
	} \
	~ScriptFunc_##funcID () { \
		delete type; \
	} \
	dscript::FunctionType* getType() { \
		return type; \
	} \
	ScriptObject call(std::vector<dscript::ScriptObject>& params)

#define SCRIPT_PARAM(scriptType,num) \
	reinterpret_cast< Script##scriptType * >(params[num].getWrapped())->getValue();

#define SCRIPT_END_FUNC(funcID,scriptName) \
}; \
run(Function* sc = new ScriptFunc_##funcID () ; getDScriptEngine()->addFunction(FunctionPrototype( scriptName , sc->getType()->getParamTypes()) , sc) ; , ScriptAddFunc_##funcID) 

#define RUN_FIRST(name,code) \
class AAA##name { \
	AAA##name () { code } \
	~AAA##name () { } \
}; \
AAA##name AAA##name##Instance;

namespace dscript {
		
	class Function {
	public:
		Function();
		virtual ~Function();
		virtual ScriptObject call(std::vector<ScriptObject>& values)=0;
		virtual FunctionType* getType()=0;
	};
	class ScriptFunction : public Function {
	public:
		ScriptFunction(Type* type,std::vector<Declaration*>* parameters,Statement* run);
		~ScriptFunction();
		
		virtual ScriptObject call(std::vector<ScriptObject>&);
		virtual void setParentScope(Scope* s);
		/*
		virtual ScriptObject* clone();
		*/
		virtual FunctionType* getType();
	private:
		FunctionType* type;
		Scope* parentscope;
		std::vector<Declaration*>* parameters;
		std::vector<Type*>* paramTypes;
		Statement* run;
	};
	
	class FunctionPrototype {
	public:
		FunctionPrototype(std::string name,std::vector<Type*>* params);
		virtual ~FunctionPrototype();
		
		virtual std::string getString() const;
		virtual bool operator<(const FunctionPrototype& ft) const;
		virtual bool operator==(const FunctionPrototype& ft) const;
		
	private:
		std::string repString;
		std::vector<Type*>* params;
	};
	
	/*
	template<class T>
	class CPPFunctionWrapper0 : public Function {
	public:
		CPPFunctionWrapper(const T& func, Type* type) : func(func), type(new FunctionType(type,new vector<Type*>)) {}
		virtual ~CPPFunctionWrapper() { delete type; }
		
		virtual ScriptObject* call(std::vector<ScriptObject*>* values) { 
			return func(values);
		}
		virtual FunctionType* getType() { return type; }
	private:
		T func;
		FunctionType* type;
	};*/
}

#endif
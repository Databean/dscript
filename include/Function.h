#ifndef FUNCTION_H
#define FUNCTION_H

#include "ScriptObject.h"
#include "ScriptType.h"

#include <utility>
#include <functional>
#include <memory>

#define RUN_FIRST(name,code) \
class AAA##name { \
	AAA##name () { code } \
	~AAA##name () { } \
}; \
AAA##name AAA##name##Instance;

#define scriptFunction(func) \
scriptFunctionName(#func, func)

// The evaluation order of function parameters is undefined, so the the type variable has to be created first
// since std::move(sc) might evaluate first and leave sc in an invalid state to give type information.

#define scriptFunctionName(name, func) \
run(auto sc = makeCPPFunction(func); \
auto type = FunctionPrototype( name, sc->getType()->getParamTypes()); \
getDScriptEngine()->addFunction(type, std::move(sc));)

namespace dscript {
	
	class Node;
	class Declaration;
	class Scope;
	class Statement;
	class Type;
	class FunctionType;
		
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
	
	// http://stackoverflow.com/a/10766422/1290655
	// implementation details, users never invoke these directly
	namespace detail
	{
		template <typename F, typename Tuple, bool Done, int Total, int... N>
		struct call_impl
		{
			static auto call(F f, Tuple && t) ->
			decltype(call_impl<F, Tuple, Total == 1 + sizeof...(N), Total, N..., sizeof...(N)>::call(f, std::forward<Tuple>(t)))
			{
				return call_impl<F, Tuple, Total == 1 + sizeof...(N), Total, N..., sizeof...(N)>::call(f, std::forward<Tuple>(t));
			}
		};
		
		template <typename F, typename Tuple, int Total, int... N>
		struct call_impl<F, Tuple, true, Total, N...>
		{
			static auto call(F f, Tuple && t) ->
			decltype(f(std::get<N>(std::forward<Tuple>(t))...))
			{
				return f(std::get<N>(std::forward<Tuple>(t))...);
			}
		};
		
		template <typename F, typename Tuple, bool Done, int Total, int... N>
		struct call_void_impl
		{
			static void call(F f, Tuple && t)
			{
				call_void_impl<F, Tuple, Total == 1 + sizeof...(N), Total, N..., sizeof...(N)>::call(f, std::forward<Tuple>(t));
			}
		};
		
		template <typename F, typename Tuple, int Total, int... N>
		struct call_void_impl<F, Tuple, true, Total, N...>
		{
			static void call(F f, Tuple && t)
			{
				f(std::get<N>(std::forward<Tuple>(t))...);
			}
		};
	}
	
	// user invokes this
	template <typename F, typename Tuple>
	auto callFn(F f, Tuple && t) ->
	decltype(detail::call_impl<F, Tuple, 0 == std::tuple_size<typename std::decay<Tuple>::type>::value, std::tuple_size<typename std::decay<Tuple>::type>::value>::call(f, std::forward<Tuple>(t)))
	{
		typedef typename std::decay<Tuple>::type ttype;
		return detail::call_impl<F, Tuple, 0 == std::tuple_size<ttype>::value, std::tuple_size<ttype>::value>::call(f, std::forward<Tuple>(t));
	}
	
	template <typename F, typename Tuple>
	void callVoidFn(F f, Tuple && t) {
		typedef typename std::decay<Tuple>::type ttype;
		detail::call_void_impl<F, Tuple, 0 == std::tuple_size<ttype>::value, std::tuple_size<ttype>::value>::call(f, std::forward<Tuple>(t));
	}
	
	//end stack overflow
	
	template<typename Iterator, typename FirstArg>
	std::tuple<FirstArg> tupleFromVector(Iterator begin, Iterator end) {
		if(begin == end) {
			throw "Not enough arguments to function.";
		}
		if(dynamic_cast<CPPObjectWrapper<FirstArg>*>(begin->getWrapped())) {
			FirstArg arg = reinterpret_cast<CPPObjectWrapper<FirstArg>*>(begin->getWrapped())->getValue();
			return std::make_tuple(arg);
		} else {
			throw "Invalid argument type";
		}
	}
	
	template<typename Iterator, typename FirstArg, typename SecondArg, typename... Args>
	std::tuple<FirstArg, SecondArg, Args...> tupleFromVector(Iterator begin, Iterator end) {
		if(begin == end) {
			throw "Not enough arguments to function.";
		}
		if(dynamic_cast<CPPObjectWrapper<FirstArg>*>(begin->getWrapped())) {
			FirstArg arg = reinterpret_cast<CPPObjectWrapper<FirstArg>*>(begin->getWrapped())->getValue();
			begin++;
			return std::tuple_cat(std::make_tuple(arg), tupleFromVector<Iterator, SecondArg, Args...>(begin,end));
		} else {
			throw "Invalid argument type";
		}
	}
	
	template<typename... Args>
	std::vector<Type*>* argsVector() {
		return new std::vector<Type*> {new CPPType<Args>()...};
	}
	
	template<typename ReturnType, typename... Args>
	class CPPFunction : public Function {
	private:
		typedef ReturnType(*Func)(Args...);
		Func fn;
		FunctionType type;
	public:
		CPPFunction(Func fn) : fn(fn), type(new CPPType<ReturnType>(), argsVector<Args...>()) {
			
		}
		~CPPFunction() {}
		
		FunctionType* getType() {
			return &type;
		}
		
		virtual ScriptObject call(std::vector<ScriptObject>& args) {
			return ScriptObject(CPPObjectWrapper<ReturnType>(callFn(fn, tupleFromVector<std::vector<ScriptObject>::iterator, Args...>(args.begin(), args.end()))));
		}
	};
	
	template<typename... Args>
	class CPPFunction<void, Args...> : public Function {
	private:
		typedef void(*Func)(Args...);
		Func fn;
		FunctionType type;
	public:
		CPPFunction(Func fn) : fn(fn), type(new VoidType(), argsVector<Args...>()) {}
		~CPPFunction() {}
		
		FunctionType* getType() {
			return &type;
		}
		
		virtual ScriptObject call(std::vector<ScriptObject>& args) {
			callVoidFn(fn, tupleFromVector<std::vector<ScriptObject>::iterator, Args...>(args.begin(), args.end()));
			return ScriptObject();
		}
	};
	
	template <typename T>
	struct identity
	{
		typedef T type;
	};
	
	
	template<typename ReturnType, typename... Args>
	std::unique_ptr<Function> makeCPPFunction(ReturnType(*fn)(Args...)) {
		return std::unique_ptr<Function>(new CPPFunction<ReturnType, Args...>(fn));
	}
	
	template<typename ReturnType, typename... Args>
	std::unique_ptr<Function> makeCPPFunction(std::function<ReturnType(Args...)> fn) {
		return std::unique_ptr<Function>(new CPPFunction<ReturnType, Args...>(fn));
	}
	
}

#endif
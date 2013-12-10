#include "Include.h"
#include "Debug.h"

namespace dscript {
	ObjectBase::ObjectBase() { }
	ObjectBase::~ObjectBase() { }
	
	
	//SCRIPT_TYPE_IMPL(int,Int)
	//SCRIPT_TYPE_IMPL(std::string,String)
	//SCRIPT_TYPE_IMPL(double,Real)
	//SCRIPT_TYPE_IMPL(bool,Bool)
	
	template<> Type* ScriptInt::type = new CPPType<int>;
	template<> Type* ScriptString::type = new CPPType<std::string>;
	template<> Type* ScriptReal::type = new CPPType<float>;
	template<> Type* ScriptBool::type = new CPPType<bool>;
}
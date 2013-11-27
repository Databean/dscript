#include "Include.h"
#include "Debug.h"

namespace dscript {
	ObjectBase::ObjectBase() { }
	ObjectBase::~ObjectBase() { }
	
	
	//SCRIPT_TYPE_IMPL(int,Int)
	//SCRIPT_TYPE_IMPL(std::string,String)
	//SCRIPT_TYPE_IMPL(double,Real)
	//SCRIPT_TYPE_IMPL(bool,Bool)
	
	template<> Type* ScriptInt::type = new SimpleType("Int");
	template<> Type* ScriptString::type = new SimpleType("String");
	template<> Type* ScriptReal::type = new SimpleType("Real");
	template<> Type* ScriptBool::type = new SimpleType("Bool");
}
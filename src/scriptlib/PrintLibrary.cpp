#include "Include.h"
#include "Debug.h"

namespace dscript {
	
	SCRIPT_BEGIN_FUNC(printInt,new VoidType(),1,new SimpleType("Int")) {
		int si = SCRIPT_PARAM(Int,0);
		std::cout << si;
		return ScriptObject();
	} SCRIPT_END_FUNC(printInt,"print")
	
	SCRIPT_BEGIN_FUNC(printIntLn,new VoidType(),1,new SimpleType("Int")) {
		int si = SCRIPT_PARAM(Int,0);
		std::cout << si << std::endl;
		return ScriptObject();
	} SCRIPT_END_FUNC(printIntLn,"println")
	
	SCRIPT_BEGIN_FUNC(printReal,new VoidType(),1,new SimpleType("Real")) {
		float sr = SCRIPT_PARAM(Real,0)
		std::cout << sr;
		return ScriptObject();
	} SCRIPT_END_FUNC(printReal,"print")
	
	SCRIPT_BEGIN_FUNC(printRealLn,new VoidType(),1,new SimpleType("Real")) {
		float sr = SCRIPT_PARAM(Real,0)
		std::cout << sr << std::endl;
		return ScriptObject();
	} SCRIPT_END_FUNC(printRealLn,"println")
	
	SCRIPT_BEGIN_FUNC(printString,new VoidType(),1,new SimpleType("String")) {
		std::string ss = SCRIPT_PARAM(String,0);
		std::cout << ss;
		return ScriptObject();
	} SCRIPT_END_FUNC(printString,"print")
	
	SCRIPT_BEGIN_FUNC(printStringLn,new VoidType(),1,new SimpleType("String")) {
		std::string ss = SCRIPT_PARAM(String,0);
		std::cout << ss << std::endl;
		return ScriptObject();
	} SCRIPT_END_FUNC(printStringLn,"println");
	
	SCRIPT_BEGIN_FUNC(printBool,new VoidType(),1,new SimpleType("Bool")) {
		bool sb = SCRIPT_PARAM(Bool,0);
		std::cout << ( sb ? "true" : "false");
		return ScriptObject();
	} SCRIPT_END_FUNC(printBool,"print")
	
	SCRIPT_BEGIN_FUNC(printBoolLn,new VoidType(),1,new SimpleType("Bool")) {
		bool sb = SCRIPT_PARAM(Bool,0);
		std::cout << ( sb ? "true" : "false");
		return ScriptObject();
	} SCRIPT_END_FUNC(printBoolLn,"println")
	
}
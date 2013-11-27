#include "Include.h"
#include "Debug.h"
#include <sstream>

namespace dscript {
	
	std::string intToString(int n) {
		std::stringstream result;
		result << n;
		return result.str();
	}
	
	SCRIPT_BEGIN_FUNC(intToString,new SimpleType("String"),1,new SimpleType("Int")) {
		int n = SCRIPT_PARAM(Int,0);
		return ScriptObject(ScriptString(intToString(n)));
	} SCRIPT_END_FUNC(intToString,"string");
	
	BEGIN_MANUAL_BINARY_OPERATOR(stringPlusInt,new SimpleType("String")) {
		std::string left = BINARY_OPERATOR_PARAM_LEFT(String);
		int right = BINARY_OPERATOR_PARAM_RIGHT(Int);
		return ScriptObject(ScriptString(left + intToString(right)));
	} END_MANUAL_BINARY_OPERATOR(stringPlusInt,new SimpleType("String"),new SimpleType("String"),"+",new SimpleType("Int"))
	
	BEGIN_MANUAL_BINARY_OPERATOR(intPlusString,new SimpleType("String")) {
		int left = BINARY_OPERATOR_PARAM_LEFT(Int);
		std::string right = BINARY_OPERATOR_PARAM_RIGHT(String);
		return ScriptObject(ScriptString(intToString(left) + right));
	} END_MANUAL_BINARY_OPERATOR(intPlusString,new SimpleType("String"),new SimpleType("Int"),"+",new SimpleType("String"))
	
	
	int stringToInt(const std::string& s) {
		int result = 0;
		std::istringstream ss( s );
		ss >> result;
		if (!ss) return 0;
		return result;
	}
	
	SCRIPT_BEGIN_FUNC(stringToInt,new SimpleType("Int"),1,new SimpleType("String")) {
		std::string n = SCRIPT_PARAM(String,0);
		return ScriptObject(ScriptInt(stringToInt(n)));
	} SCRIPT_END_FUNC(stringToInt,"int");
	
	std::string floatToString(float n) {
		std::ostringstream result;
		result << n;
		return result.str();
	}
	
	SCRIPT_BEGIN_FUNC(realToString,new SimpleType("String"),1,new SimpleType("Real")) {
		float n = SCRIPT_PARAM(Real,0);
		return ScriptObject(ScriptString(floatToString(n)));
	} SCRIPT_END_FUNC(realToString,"string")
	
	BEGIN_MANUAL_BINARY_OPERATOR(stringPlusReal,new SimpleType("String")) {
		std::string left = BINARY_OPERATOR_PARAM_LEFT(String);
		float right = BINARY_OPERATOR_PARAM_RIGHT(Real);
		return ScriptObject(ScriptString(left + floatToString(right)));
	} END_MANUAL_BINARY_OPERATOR(stringPlusReal,new SimpleType("String"),new SimpleType("String"),"+",new SimpleType("Real"))
	
	BEGIN_MANUAL_BINARY_OPERATOR(realPlusString,new SimpleType("String")) {
		float left = BINARY_OPERATOR_PARAM_LEFT(Real);
		std::string right = BINARY_OPERATOR_PARAM_RIGHT(String);
		return ScriptObject(ScriptString(floatToString(left) + right));
	} END_MANUAL_BINARY_OPERATOR(realPlusString,new SimpleType("String"),new SimpleType("Real"),"+",new SimpleType("String"))
	
	int stringToFloat(const std::string& s) {
		int result = 0;
		std::istringstream ss( s );
		ss >> result;
		if (!ss) return 0;
		return result;
	}

	SCRIPT_BEGIN_FUNC(stringToReal,new SimpleType("Real"),1,new SimpleType("String")) {
		std::string n = SCRIPT_PARAM(String,0);
		return ScriptObject(ScriptReal(stringToFloat(n)));
	} SCRIPT_END_FUNC(stringToInt,"real");
	
	SCRIPT_BEGIN_FUNC(stringLength,new SimpleType("Int"),1,new SimpleType("String")) {
		std::string ss = SCRIPT_PARAM(String,0);
		return ScriptObject(ScriptInt(ss.length()));
	} SCRIPT_END_FUNC(stringLength,"length")
	
	SCRIPT_BEGIN_FUNC(stringSize,new SimpleType("Int"),1,new SimpleType("String")) {
		std::string ss = SCRIPT_PARAM(String,0);
		return ScriptObject(ScriptInt(ss.length()));
	} SCRIPT_END_FUNC(stringSize,"size")
	
	SCRIPT_BEGIN_FUNC(stringSubstring,new SimpleType("String"),2,new SimpleType("String"),new SimpleType("Int")) {
		std::string p1 = SCRIPT_PARAM(String,0);
		int p2 = SCRIPT_PARAM(Int,1);
		return ScriptObject(ScriptString(p1.substr(p2)));
	} SCRIPT_END_FUNC(stringSubstring,"substring")
	
	SCRIPT_BEGIN_FUNC(stringSubstringLen,new SimpleType("String"),3,new SimpleType("String"),new SimpleType("Int"),new SimpleType("Int")) {
		std::string p0 = SCRIPT_PARAM(String,0);
		int p1 = SCRIPT_PARAM(Int,1);
		int p2 = SCRIPT_PARAM(Int,2);
		return ScriptObject(ScriptString(p0.substr(p1,p2)));
	} SCRIPT_END_FUNC(stringSubstringLen,"substring")
	
	SCRIPT_BEGIN_FUNC(stringFind,new SimpleType("Int"),2,new SimpleType("String"),new SimpleType("String")) {
		std::string p0 = SCRIPT_PARAM(String,0);
		std::string p1 = SCRIPT_PARAM(String,1);
		return ScriptObject(ScriptInt(p0.find(p1)));
	} SCRIPT_END_FUNC(stringFind,"find")
	
	SCRIPT_BEGIN_FUNC(stringRFind,new SimpleType("Int"),2,new SimpleType("String"),new SimpleType("String")) {
		std::string p0 = SCRIPT_PARAM(String,0);
		std::string p1 = SCRIPT_PARAM(String,1);
		return ScriptObject(ScriptInt(p0.rfind(p1)));
	} SCRIPT_END_FUNC(stringRFind,"rfind")
	
	SCRIPT_BEGIN_FUNC(stringCompare,new SimpleType("Int"),2,new SimpleType("String"),new SimpleType("String")) {
		std::string p0 = SCRIPT_PARAM(String,0);
		std::string p1 = SCRIPT_PARAM(String,1);
		return ScriptObject(ScriptInt(p0.compare(p1)));
	} SCRIPT_END_FUNC(stringCompare,"compare");
	
	SCRIPT_BEGIN_FUNC(stringAt,new SimpleType("String"),2,new SimpleType("String"),new SimpleType("Int")) {
		std::string p0 = SCRIPT_PARAM(String,0);
		int p1 = SCRIPT_PARAM(Int,1);
		return ScriptObject(ScriptString(std::string(1,p0.at(p1))));
	} SCRIPT_END_FUNC(stringAt,"at");
	
	makeBinaryOperatorClass(stringEquals,String,==,String,Bool);
	makeBinaryOperatorClass(stringAdd,String,+,String,String);
}
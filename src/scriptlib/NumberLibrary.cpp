#include "Include.h"
#include "Debug.h"

#include <algorithm>
#include <math.h>

namespace dscript {
	
	#define makeIntRealOperator(name,op) \
	makeBinaryOperatorClass(name,Int,op,Int,Int) \
	makeBinaryOperatorClass(name,Real,op,Int,Real) \
	makeBinaryOperatorClass(name,Int,op,Real,Real) \
	makeBinaryOperatorClass(name,Real,op,Real,Real)
	
	#define makeComparisonOperator(name,op) \
	makeBinaryOperatorClass(name,Int,op,Int,Bool) \
	makeBinaryOperatorClass(name,Real,op,Int,Bool) \
	makeBinaryOperatorClass(name,Int,op,Real,Bool) \
	makeBinaryOperatorClass(name,Real,op,Real,Bool)
	
	makeIntRealOperator(plus,+)
	makeIntRealOperator(minus,-)
	makeIntRealOperator(multiply,*)
	makeIntRealOperator(divide,/)
	makeComparisonOperator(greaterThan,>)
	makeComparisonOperator(lessThan,<)
	makeComparisonOperator(equality,==)
	makeComparisonOperator(greaterThanOrEqualTo,>=)
	makeComparisonOperator(lessThanOrEqualTo,<=)
	
	makeBinaryOperatorClass(modulo,Int,%,Int,Int)
	makeBinaryOperatorClass(leftShift,Int,<<,Int,Int)
	makeBinaryOperatorClass(rightShift,Int,>>,Int,Int)
	makeBinaryOperatorClass(bitwiseAnd,Int,&,Int,Int)
	makeBinaryOperatorClass(bitwiseOr,Int,|,Int,Int)
	makeBinaryOperatorClass(bitwiseXor,Int,^,Int,Int)
	makeBinaryOperatorClass(logicalAnd,Bool,&&,Bool,Bool)
	makeBinaryOperatorClass(logicalOr,Bool,||,Bool,Bool)
	
	makeUnaryOperatorClass(unaryMinus,-,Int,Int)
	makeUnaryOperatorClass(unaryMinus,-,Real,Real)
	makeUnaryOperatorClass(bitwiseNot,~,Int,Int)
	makeUnaryOperatorClass(booleanNot,!,Bool,Bool)
	
	BEGIN_MANUAL_BINARY_OPERATOR(intRaiseInt,new SimpleType("Int")) {
		int left = BINARY_OPERATOR_PARAM_LEFT(Int);
		int right = BINARY_OPERATOR_PARAM_RIGHT(Int);
		int result = 1;
		while(right>0) {
			while(right%2==0) {
				right /= 2;
				left *= left;
			}
			right--;
			result *= left;
		}
		return ScriptObject(ScriptInt(result));
	} END_MANUAL_BINARY_OPERATOR(intRaiseInt,new SimpleType("Int"),new SimpleType("Int"),"^",new SimpleType("Int"))
	
	BEGIN_MANUAL_BINARY_OPERATOR(realRaiseInt,new SimpleType("Real")) {
		float left = BINARY_OPERATOR_PARAM_LEFT(Real);
		float right = BINARY_OPERATOR_PARAM_RIGHT(Int);
		return ScriptObject(ScriptReal(pow(left,right)));
	} END_MANUAL_BINARY_OPERATOR(realRaiseInt,new SimpleType("Real"),new SimpleType("Real"),"^",new SimpleType("Int"))
	
	BEGIN_MANUAL_BINARY_OPERATOR(intRaiseReal,new SimpleType("Real")) {
		float left = BINARY_OPERATOR_PARAM_LEFT(Int);
		float right = BINARY_OPERATOR_PARAM_RIGHT(Real);
		return ScriptObject(ScriptReal(pow(left,right)));
	} END_MANUAL_BINARY_OPERATOR(intRaiseReal,new SimpleType("Real"),new SimpleType("Int"),"^",new SimpleType("Real"))
	
	BEGIN_MANUAL_BINARY_OPERATOR(realRaiseReal,new SimpleType("Real")) {
		float left = BINARY_OPERATOR_PARAM_LEFT(Real);
		float right = BINARY_OPERATOR_PARAM_RIGHT(Real);
		return ScriptObject(ScriptReal(pow(left,right)));
	} END_MANUAL_BINARY_OPERATOR(realRaiseReal,new SimpleType("Real"),new SimpleType("Real"),"^",new SimpleType("Real"))
	
	SCRIPT_BEGIN_FUNC(intIntMax,new SimpleType("Int"),2,new SimpleType("Int"),new SimpleType("Int")) {
		int a = SCRIPT_PARAM(Int,0);
		int b = SCRIPT_PARAM(Int,1);
		return ScriptObject(ScriptInt(std::max<int>(a,b)));
	} SCRIPT_END_FUNC(intIntMax,"max")
	
	SCRIPT_BEGIN_FUNC(intRealMax,new SimpleType("Real"),2,new SimpleType("Int"),new SimpleType("Real")) {
		float a = SCRIPT_PARAM(Int,0);
		float b = SCRIPT_PARAM(Real,1);
		return ScriptObject(ScriptReal(std::max<float>(a,b)));
	} SCRIPT_END_FUNC(intRealMax,"max")
	
	SCRIPT_BEGIN_FUNC(realIntMax,new SimpleType("Real"),2,new SimpleType("Real"),new SimpleType("Int")) {
		float a = SCRIPT_PARAM(Real,0);
		int b = SCRIPT_PARAM(Int,1);
		return ScriptObject(ScriptReal(std::max<float>(a,b)));
	} SCRIPT_END_FUNC(realIntMax,"max")
	
	SCRIPT_BEGIN_FUNC(realRealMax,new SimpleType("Real"),2,new SimpleType("Real"),new SimpleType("Real")) {
		float a = SCRIPT_PARAM(Real,0);
		float b = SCRIPT_PARAM(Real,1);
		return ScriptObject(ScriptReal(std::max<float>(a,b)));
	} SCRIPT_END_FUNC(realRealMax,"max")
	
	SCRIPT_BEGIN_FUNC(intIntMin,new SimpleType("Int"),2,new SimpleType("Int"),new SimpleType("Int")) {
		int a = SCRIPT_PARAM(Int,0);
		int b = SCRIPT_PARAM(Int,1);
		return ScriptObject(ScriptInt(std::min<int>(a,b)));
	} SCRIPT_END_FUNC(intIntMin,"min")
	
	SCRIPT_BEGIN_FUNC(intRealMin,new SimpleType("Real"),2,new SimpleType("Int"),new SimpleType("Real")) {
		float a = SCRIPT_PARAM(Int,0);
		float b = SCRIPT_PARAM(Real,1);
		return ScriptObject(ScriptReal(std::min<float>(a,b)));
	} SCRIPT_END_FUNC(intRealMin,"min")
	
	SCRIPT_BEGIN_FUNC(realIntMin,new SimpleType("Real"),2,new SimpleType("Real"),new SimpleType("Int")) {
		float a = SCRIPT_PARAM(Real,0);
		int b = SCRIPT_PARAM(Int,1);
		return ScriptObject(ScriptReal(std::min<float>(a,b)));
	} SCRIPT_END_FUNC(realIntMin,"min")
	
	SCRIPT_BEGIN_FUNC(realRealMin,new SimpleType("Real"),2,new SimpleType("Real"),new SimpleType("Real")) {
		float a = SCRIPT_PARAM(Real,0);
		float b = SCRIPT_PARAM(Real,1);
		return ScriptObject(ScriptReal(std::min<float>(a,b)));
	} SCRIPT_END_FUNC(realRealMin,"min")
	
	SCRIPT_BEGIN_FUNC(intToReal,new SimpleType("Real"),1,new SimpleType("Int")) {
		int a = SCRIPT_PARAM(Int,0);
		return ScriptObject(ScriptReal(a));
	} SCRIPT_END_FUNC(intToReal,"real");
	
	SCRIPT_BEGIN_FUNC(realToInt,new SimpleType("Int"),1,new SimpleType("Real")) {
		float a = SCRIPT_PARAM(Real,0);
		return ScriptObject(ScriptInt(a));
	} SCRIPT_END_FUNC(realToInt,"int")
	
	SCRIPT_BEGIN_FUNC(intToInt,new SimpleType("Int"),1,new SimpleType("Int")) {
		int a = SCRIPT_PARAM(Int,0);
		return ScriptObject(ScriptInt(a));
	} SCRIPT_END_FUNC(intToInt,"int")
	
	SCRIPT_BEGIN_FUNC(realToReal,new SimpleType("Real"),1,new SimpleType("Real")) {
		float a = SCRIPT_PARAM(Real,0);
		return ScriptObject(ScriptReal(a));
	} SCRIPT_END_FUNC(realToReal,"real")
	
	SCRIPT_BEGIN_FUNC(realSqrt,new SimpleType("Real"),1,new SimpleType("Real")) {
		float a = SCRIPT_PARAM(Real,0);
		return ScriptObject(ScriptReal(sqrt(a)));
	} SCRIPT_END_FUNC(realSqrt,"sqrt")
	
	SCRIPT_BEGIN_FUNC(intSqrt,new SimpleType("Real"),1,new SimpleType("Int")) {
		float a = SCRIPT_PARAM(Int,0);
		return ScriptObject(ScriptReal(sqrt(a)));
	} SCRIPT_END_FUNC(intSqrt,"sqrt");
	
}
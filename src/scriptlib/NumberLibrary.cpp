#include "Preprocessor.h"
#include "ScriptEngine.h"
#include "Function.h"
#include "Operators.h"
#include "Debug.h"

#include <algorithm>
#include <math.h>

namespace dscript {
	
	#define makeIntRealOperator(op) \
	scriptBinaryOperator(#op, int, int, [](int a, int b) -> int { return a op b; }); \
	scriptBinaryOperator(#op, float, int, [](float a, int b) -> float { return a op b; }); \
	scriptBinaryOperator(#op, int, float, [](int a, float b) -> float { return a op b; }); \
	scriptBinaryOperator(#op, float, float, [](float a, float b) -> float { return a op b; });
	
	#define makeComparisonOperator(op) \
	scriptBinaryOperator(#op, int, int, [](int a, int b) -> bool { return a op b; }); \
	scriptBinaryOperator(#op, float, int, [](float a, int b) -> bool { return a op b; }); \
	scriptBinaryOperator(#op, int, float, [](int a, float b) -> bool { return a op b; }); \
	scriptBinaryOperator(#op, float, float, [](float a, float b) -> bool { return a op b; });
	
	makeIntRealOperator(+)
	makeIntRealOperator(-)
	makeIntRealOperator(*)
	makeIntRealOperator(/)
	makeComparisonOperator(>)
	makeComparisonOperator(<)
	makeComparisonOperator(==)
	makeComparisonOperator(>=)
	makeComparisonOperator(<=)
	
	scriptBinaryOperator("^", float, float, [](float a, float b) -> float { return std::pow(a, b); });
	
	#define intOp(op) \
	scriptBinaryOperator(#op, int, int, [](int a, int b) -> int { return a % b; });
	
	intOp(%)
	intOp(&)
	intOp(|)
	intOp(^)
	intOp(<<)
	intOp(>>)
	intOp(&&)
	intOp(||)
	
	scriptUnaryOperator("-", int, [](int i) -> int { return -i; });
	scriptUnaryOperator("-", float, [](float f) -> float { return -f; });
	scriptUnaryOperator("~", int, [](int i) -> int { return ~i; });
	scriptUnaryOperator("!", bool, [](bool b) -> bool { return !b; });
	
	int maxIntInt(int a, int b) {
		return std::max<int>(a, b);
	}
	scriptFunctionName("max", maxIntInt);
	
	int maxIntFloat(int a, float b) {
		return std::max<float>(a, b);
	}
	scriptFunctionName("max", maxIntFloat);
	
	int maxFloatInt(float a, int b) {
		return std::max<float>(a, b);
	}
	scriptFunctionName("max", maxFloatInt);
	
	int maxFloatFloat(float a, float b) {
		return std::max<float>(a, b);
	}
	scriptFunctionName("max", maxFloatFloat);
	
	int minIntInt(int a, int b) {
		return std::min<int>(a, b);
	}
	scriptFunctionName("min", minIntInt);
	
	int minIntFloat(int a, float b) {
		return std::min<float>(a, b);
	}
	scriptFunctionName("min", minIntFloat);
	
	int minFloatInt(float a, int b) {
		return std::min<float>(a, b);
	}
	scriptFunctionName("min", minFloatInt);
	
	int minFloatFloat(float a, float b) {
		return std::min<float>(a, b);
	}
	scriptFunctionName("min", minFloatFloat);
	
	float makeReal(int i) {
		return i;
	}
	scriptFunctionName("real", makeReal);
	
	int makeInt(float f) {
		return f;
	}
	scriptFunctionName("int", makeInt);
	
	float sqrtReal(float f) {
		return std::sqrt(f);
	}
	scriptFunctionName("sqrt", sqrtReal);
	
	float sqrtInt(int i) {
		return std::sqrt(i);
	}
}
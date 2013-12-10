#include "Include.h"
#include "Debug.h"
#include <sstream>

namespace dscript {
	
	std::string intToString(int n) {
		std::stringstream result;
		result << n;
		return result.str();
	}
	scriptFunction(intToString);
	
	scriptBinaryOperator("+", std::string, int, [](std::string s, int i) -> std::string { return s + intToString(i); });
	scriptBinaryOperator("+", int, std::string, [](int i, std::string s) -> std::string { return intToString(i) + s; });
	
	int stringToInt(const std::string s) {
		int result = 0;
		std::istringstream ss( s );
		ss >> result;
		if (!ss) return 0;
		return result;
	}
	scriptFunction(stringToInt);
	
	std::string realToString(float n) {
		std::ostringstream result;
		result << n;
		return result.str();
	}
	scriptFunction(realToString);
	
	scriptBinaryOperator("+", std::string, float, [](std::string s, float f) -> std::string { return s + realToString(f); });
	scriptBinaryOperator("+", float, std::string, [](float f, std::string s) -> std::string { return realToString(f) + s; });
	
	float stringToReal(std::string s) {
		int result = 0;
		std::istringstream ss( s );
		ss >> result;
		if (!ss) return 0;
		return result;
	}
	scriptFunction(stringToReal);
	
	int stringLength(std::string str) {
		return str.length();
	}
	scriptFunction(stringLength)
	
	int stringSize(std::string str) {
		return str.length();
	}
	scriptFunction(stringSize);
	
	std::string substring(std::string str, int index) {
		return str.substr(index);
	}
	scriptFunction(substring);
	
	std::string substringLen(std::string str, int index, int len) {
		return str.substr(index, len);
	}
	scriptFunctionName("substring", substringLen);
	
	int find(std::string haystack, std::string needle) {
		return haystack.find(needle);
	}
	scriptFunction(find);
	
	int rfind(std::string haystack, std::string needle) {
		return haystack.rfind(needle);
	}
	scriptFunction(rfind);
	
	int compare(std::string a, std::string b) {
		return a.compare(b);
	}
	scriptFunction(compare);
	
	std::string at(std::string str, int index) {
		return std::string(1, str.at(index));
	}
	
	scriptBinaryOperator("==", std::string, std::string, [](std::string a, std::string b) -> bool { return a == b; });
	scriptBinaryOperator("+", std::string, std::string, [](std::string a, std::string b) -> std::string { return a + b; });
}
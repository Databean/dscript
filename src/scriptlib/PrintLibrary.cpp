#include "Include.h"
#include "Debug.h"

namespace dscript {
	
	
	void printInt(int i) {
		std::cout << i;
	}
	scriptFunctionName("print", printInt);
	
	void printlnInt(int i) {
		std::cout << i << std::endl;
	}
	scriptFunctionName("println", printlnInt);
	
	void printFloat(float f) {
		std::cout << f;
	}
	scriptFunctionName("print", printFloat);
	
	void printlnFloat(float f) {
		std::cout << f << std::endl;
	}
	scriptFunctionName("println", printlnFloat);
	
	void printString(std::string s) {
		std::cout << s;
	}
	scriptFunctionName("print", printString);
	
	void printlnString(std::string s) {
		std::cout << s << std::endl;
	}
	scriptFunctionName("println", printlnString);
	
	void printBool(bool b) {
		std::cout << (b ? "true" : "false");
	}
	scriptFunctionName("print", printBool);
	
	void printlnBool(bool b) {
		std::cout << (b ? "true" : "false") << std::endl;
	}
	scriptFunctionName("println", printlnBool);
	
}
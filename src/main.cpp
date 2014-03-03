#include "y.tab.h"
#include "scanner.h"
#include <sstream>
#include <fstream>
#include <iostream>

#include "ScriptContext.h"

int main(int argc,char** args) {
	std::string filename;
	if(argc > 1) {
		filename = args[1];
	} else {
		std::cout << ">>parsing test.ds, no filename given" << std::endl;
		filename = "test.ds";
	}
	
	std::fstream file(filename.c_str(), std::fstream::in);
	
	std::cout << ">>running parser, lexer" << std::endl;
	
	ScriptContext context(file);
	
	int success = yyparse(&context);
	
	if(success == 0) {
		context.program->setParent(getDScriptEngine());
		if(context.program->verify()) {
			context.program->evaluate();
		}
	} else {
		std::cout << "error " << success << std::endl;
	}
	std::cout << ">>done" <<std::endl;
}
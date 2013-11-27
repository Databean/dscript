#include "y.tab.h"
#include "scanner.h"
#include <sstream>
#include <fstream>
#include <iostream>

int yyparse();
int yylex();

int main(int argc,char** args) {
	std::string filename;
	if(argc > 1) {
		filename = args[1];
	} else {
		std::cout << ">>parsing test.ds, no filename given" << std::endl;
		filename = "test.ds";
	}
	
	std::cout << ">>this is the file" << std::endl;
	
	std::fstream file(filename.c_str(), std::fstream::in);
	std::stringbuf text;
	file >> &text;
	file.close();
	
	std::cout << text.str() << std::endl;
	
	std::cout << ">>running parser, lexer" << std::endl;
	
	chooseFile(text.str().c_str());
	
	yyparse();
	
	cleanBufferState();
	
	
	std::cout << ">>done" <<std::endl;
}
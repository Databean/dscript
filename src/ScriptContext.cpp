#include "ScriptContext.h"

#include "y.tab.h"
#include "scanner.h"

using std::istream;

ScriptContext::ScriptContext(istream& input) : scannerState(nullptr), input(input) {
	yylex_init(&scannerState);
	yyset_extra(this, scannerState);
}

ScriptContext::~ScriptContext() {
	yylex_destroy(scannerState);
}

void* ScriptContext::getScannerState() {
	return scannerState;
}

istream& ScriptContext::getInput() {
	return input;
}

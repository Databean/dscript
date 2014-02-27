#include "Include.h"
#include "Debug.h"

using std::cout;
using std::endl;

namespace dscript {
	
	void addPrintingLibrary(ScriptEngine& engine) {
		engine.addFunction("print", new CPPFunction<void, int>([](int i) { cout << i; }));
		engine.addFunction("println", new CPPFunction<void, int>([](int i) { cout << i << endl; }));
		engine.addFunction("print", new CPPFunction<void, float>([](float f) { cout << f; }));
		engine.addFunction("println", new CPPFunction<void, float>([](float f) { cout << f << endl; }));
		engine.addFunction("print", new CPPFunction<void, std::string>([](std::string f) { cout << f; }));
		engine.addFunction("println", new CPPFunction<void, std::string>([](std::string f) { cout << f << endl; }));
		engine.addFunction("print", new CPPFunction<void, bool>([](bool b) { cout << (b ? "true" : "false"); }));
		engine.addFunction("println", new CPPFunction<void, bool>([](bool b) { cout << (b ? "true" : "false") << endl; }));
	}
	
}
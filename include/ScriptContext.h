#ifndef SCRIPT_CONTEXT_H
#define SCRIPT_CONTEXT_H

#include <iostream>
#include <memory>

#include "ScriptEngine.h"

namespace dscript {

	struct ScriptContext {
	private:
		void* scannerState;
		std::istream& input;
		
	public:
		std::unique_ptr<Program> program;
		
		ScriptContext(std::istream& input);
		ScriptContext(ScriptContext&) = delete;
		~ScriptContext();
		ScriptContext& operator=(ScriptContext&) = delete;
		
		void* getScannerState();
		std::istream& getInput();
	};
}

#endif
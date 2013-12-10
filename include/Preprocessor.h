#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#define VARIABLE __LINE__
#define PASTER(x,y) x ## _ ## y
#define EVALUATOR(x,y)  PASTER(x,y)
#define NAME(fun) EVALUATOR(fun, VARIABLE)

#define VARIABLE2 __LINE__
#define PASTER2(x,y) x ## _ ## y ## _instance
#define EVALUATOR2(x,y)  PASTER2(x,y)
#define NAME2(fun) EVALUATOR2(fun, VARIABLE)

#define CONCAT_IMPL( x, y ) x##y
#define MACRO_CONCAT( x, y ) CONCAT_IMPL( x, y )

#define run_impl(code,name) \
	class name { \
	public: \
		name () { \
			code \
		} \
	}; \
	name MACRO_CONCAT(name, _instance);

#define run(code) \
	run_impl(code, NAME(MACRO_CONCAT(run_, __COUNTER__)))

#endif
#ifndef INCLUDE_H
#define INCLUDE_H

namespace dscript {
	//Node.h
	class Scope;
	class Node;
	class ScopedNode;
	//ScriptObject.h
	//class ScriptObject;
	class ObjectBase;
	//Variable.h
	class Variable;
	class NamedVariable;
	class ReferenceVariable;
	class Reference;
	class VariableReference;
	//ScriptEngine.h
	class ScriptEngine;
	class Program;
	//Declaration.h
	class Declaration;
	class VarDeclaration;
	class FunctionDeclaration;
	class ReferenceVarDeclaration;
	//Function.h
	class Function;
	class ScriptFunction;
	class FunctionPrototype;
	//Statement.h
	class Statement;
	class StatementBlock;
	class WhileStatement;
	class IfStatement;
	class ReturnStatement;
	//Expression.h
	class Expression;
	class StringLiteralExpr;
	class VarExpr;
	class IntLiteralExpr;
	class RealLiteralExpr;
	class BoolLiteralExpr;
	class ReferenceExpr;
	class FunctionCall;
	class IdentVar;
	//ScriptType.h
	class Type;
	class SimpleType;
	class FunctionType;
	class VoidType;
	class StructType;
	class ReferenceType;
	//Operators.h
	class BinarySetOperator;
	class UnarySetOperator;
	class BinaryOperatorType;
	class BinaryOperatorFunc;
	class BinaryOperator;
	class UnaryOperatorType;
	class UnaryOperatorFunc;
	class UnaryOperator;
}

//include commonly used std classes
#include <vector>
#include <iostream>
#include <map>
#include <string>
#include <typeinfo>
#include <memory>
//include script h files
#include "Preprocessor.h"
#include "Node.h"
#include "ScriptObject.h"
#include "Variable.h"
#include "ScriptType.h"
#include "Statement.h"
#include "Expression.h"
#include "Function.h"
#include "Declaration.h"
#include "Operators.h"
#include "ScriptLib.h"


#endif
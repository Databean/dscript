%{
#include "y.tab.h"
#include <iostream>
%}

%code requires {

#include "scanner.h"
#include <string>
#include <vector>
#include <iostream>

#include "Declaration.h"
#include "Statement.h"
#include "ScriptEngine.h"
#include "Expression.h"
#include "Variable.h"

using namespace dscript;
using namespace std;

} 

%union {
	std::string* string;
	int intLiteral;
	float realLiteral;
	bool boolLiteral;
	std::vector<Statement*>* statementList;
	Statement* statement;
	Declaration* varDeclaration;
	Expression* expression;
	std::vector<Expression*>* expressionList;
	Type* type;
	VarExpr* varExpr;
	StatementBlock* statementBlock;
	std::vector<Declaration*>* varDeclarationList;
	Program* program;
}

%token <string> T_StringLiteral T_Identifier
%token <intLiteral> T_IntLiteral
%token <realLiteral> T_RealLiteral
%token <boolLiteral> T_True T_False
%token T_IntType T_StringType T_RealType T_BoolType
%token T_Dimensions T_LeftShift T_RightShift
%token T_LEqual T_REqual T_Equal T_NEqual T_BooleanAnd T_BooleanOr T_BooleanXor
%token T_PlusEquals T_MinusEquals T_MultEquals T_DivideEquals T_BitwiseXorEquals
%token T_BitwiseAndEquals T_BitwiseOrEquals T_BooleanAndEquals T_BooleanOrEquals
%token T_LeftShiftEquals T_RightShiftEquals T_ModEquals T_BooleanXorEquals
%token T_MinusMinus T_PlusPlus
%token T_Arrow
%token T_While T_Function T_Void T_Return T_Auto
%token T_If T_Else T_Struct

/* Nonterminal Types */
%type <program> Program
%type <statementList> StatementList RootList
%type <statement> Statement RootItem
%type <varDeclaration> VarDeclaration ParamDecl
%type <varDeclarationList> ParamDeclList RParamDeclList
%type <expression> Expression;
%type <expressionList> ParamCallList RParamCallList
%type <type> Type;
%type <varExpr> VarExpr;

%left T_If
%left '=' T_PlusEquals T_MinusEquals T_MultEquals T_DivideEquals T_XorEquals T_BitwiseAndEquals T_BitwiseOrEquals T_BooleanAndEquals T_BooleanOrEquals T_BooleanXorEquals T_LeftShiftEquals T_RightShiftEquals T_ModEquals T_BitwiseXorEquals
%left T_BooleanXor
%left T_BooleanOr
%left T_BooleanAnd
%left '|'
%left '^'
%left '&'
%left T_Equal T_NEqual
%left '<' T_LEqual T_REqual '>'
%left T_LeftShift T_RightShift
%left '+' '-'
%left '*' '/' '%'
%right UnaryArith '!' '~' PrefixIncDec Reference
%left PostfixIncDec T_PlusPlus T_MinusMinus
%left T_Arrow
%left '(' ')'

//dangling else
%expect 1

%%

Program			:	RootList								{
															cout << $1->size() << endl;
															$$ = new Program($1);
															$$->setParent(getDScriptEngine());
															cout << ">>>verifying program" << std::endl;
															if($$->verify()) {
																std::cout << ">>>evaluating program" << std::endl;
																Scope* s = new Scope(getDScriptEngine()->getScope());
																$$->evaluate();
																delete s;
															}
															delete $$;
															}
				;

RootList		:	RootItem RootList						{$2 ->insert($2->begin(),$1); $$ = $2;}
				|											{$$ = new std::vector<Statement*>();}
				;

RootItem		:	Statement											{$$ = $1;}
				|	Type T_Identifier '(' ParamDeclList ')' Statement	{$$ = new FunctionDeclaration($2,$1,$4,$6);}
				;

StatementList	:	Statement StatementList					{$2 ->insert($2->begin(),$1); $$ = $2;}
				|											{$$ = new std::vector<Statement*>();}
				;

Statement		:	Expression ';'										{$$ = $1;}
				|	'{' StatementList '}'								{$$ = new StatementBlock($2); }
				|	T_While '(' Expression ')' Statement				{$$ = new WhileStatement($3,$5); }
				|	VarDeclaration 														{$$ = $1;}
				|	Type '&' T_Identifier '=' Expression ';' %prec Reference			{$$ = new ReferenceVarDeclaration($1,$3,$5);}
				|	T_Return Expression ';'								{$$ = new ReturnStatement($2); }
				|	T_If '(' Expression ')' Statement					{$$ = new IfStatement($3,$5); }
				|	T_If '(' Expression ')' Statement T_Else Statement	{$$ = new IfStatement($3,$5,$7); }
				;

VarDeclaration	:	Type T_Identifier ';'					{$$ = new VarDeclaration($1,$2);}
				|	Type T_Identifier '=' Expression ';'	{$$ = new VarDeclaration($1,$2,$4); }
				;

Expression		:	VarExpr '=' Expression					{$$ = new BinarySetOperator($1,$3);}

				|	Expression '+' Expression				{$$ = new BinaryOperator($1,"+",$3);}
				|	VarExpr T_PlusEquals Expression			{$$ = new BinarySetOperator($1,new BinaryOperator($1,"+",$3));}
				
				|	Expression '-' Expression				{$$ = new BinaryOperator($1,"-",$3);}
				|	VarExpr T_MinusEquals Expression		{$$ = new BinarySetOperator($1,new BinaryOperator($1,"-",$3));}
				
				|	Expression '*' Expression				{$$ = new BinaryOperator($1,"*",$3);}
				|	VarExpr T_MultEquals Expression			{$$ = new BinarySetOperator($1,new BinaryOperator($1,"*",$3));}
				
				|	Expression '/' Expression				{$$ = new BinaryOperator($1,"/",$3);}
				|	VarExpr T_DivideEquals Expression		{$$ = new BinarySetOperator($1,new BinaryOperator($1,"/",$3));}
				
				|	Expression '^' Expression				{$$ = new BinaryOperator($1,"^",$3);}
				|	VarExpr T_BitwiseXorEquals Expression	{$$ = new BinarySetOperator($1,new BinaryOperator($1,"^",$3));}		
				
				|	Expression '&' Expression				{$$ = new BinaryOperator($1,"&",$3);}
				|	VarExpr T_BitwiseAndEquals Expression	{$$ = new BinarySetOperator($1,new BinaryOperator($1,"&",$3));}
				
				|	Expression '|' Expression				{$$ = new BinaryOperator($1,"|",$3);}
				|	VarExpr T_BitwiseOrEquals Expression	{$$ = new BinarySetOperator($1,new BinaryOperator($1,"|",$3));}
				
				|	Expression T_LeftShift Expression		{$$ = new BinaryOperator($1,"<<",$3);}
				|	VarExpr T_LeftShiftEquals Expression	{$$ = new BinarySetOperator($1,new BinaryOperator($1,"<<",$3));}
				
				|	Expression T_RightShift Expression		{$$ = new BinaryOperator($1,">>",$3);}
				|	VarExpr T_RightShiftEquals Expression	{$$ = new BinarySetOperator($1,new BinaryOperator($1,">>",$3));}
				
				|	Expression '%' Expression				{$$ = new BinaryOperator($1,"%",$3);}
				|	VarExpr T_ModEquals Expression			{$$ = new BinarySetOperator($1,new BinaryOperator($1,"%",$3));}
				
				|	Expression T_BooleanAnd Expression		{$$ = new BinaryOperator($1,"&&",$3);}
				|	VarExpr T_BooleanAndEquals Expression	{$$ = new BinarySetOperator($1,new BinaryOperator($1,"&&",$3));}
				
				|	Expression T_BooleanOr Expression		{$$ = new BinaryOperator($1,"||",$3);}
				|	VarExpr T_BooleanOrEquals Expression	{$$ = new BinarySetOperator($1,new BinaryOperator($1,"||",$3));}
				
				|	Expression T_BooleanXor Expression		{$$ = new BinaryOperator($1,"^^",$3);}
				|	VarExpr T_BooleanXorEquals Expression	{$$ = new BinarySetOperator($1,new BinaryOperator($1,"^^",$3));}
				
				|	VarExpr T_PlusPlus %prec PostfixIncDec	{$$ = new UnarySetOperator($1,"++");}
				|	T_PlusPlus VarExpr %prec PrefixIncDec	{$$ = new UnarySetOperator("++",$2);}
				
				|	VarExpr T_MinusMinus %prec PostfixIncDec	{$$ = new UnarySetOperator($1,"--");}
				|	T_MinusMinus VarExpr %prec PrefixIncDec		{$$ = new UnarySetOperator("--",$2);}
				
				|	Expression '>' Expression				{$$ = new BinaryOperator($1,">",$3); }
				|	Expression '<' Expression				{$$ = new BinaryOperator($1,"<",$3); }
				|	Expression T_LEqual Expression			{$$ = new BinaryOperator($1,"<=",$3); }
				|	Expression T_REqual Expression			{$$ = new BinaryOperator($1,">=",$3); }
				|	Expression T_Equal Expression			{$$ = new BinaryOperator($1,"==",$3); }
				|	Expression T_NEqual Expression			{$$ = new BinaryOperator($1,"!=",$3); }
				|	'!' Expression							{$$ = new UnaryOperator("!",$2);}
				|	'+' Expression %prec UnaryArith			{$$ = new UnaryOperator("+",$2);}
				|	'-' Expression %prec UnaryArith			{$$ = new UnaryOperator("-",$2);}
				|	'~' Expression							{$$ = new UnaryOperator("~",$2);}
				|	'(' Expression ')'						{$$ = $2;}
				
				|	T_Identifier '(' ParamCallList ')'						{$$ = new FunctionCall($1,$3);}
				|	Expression T_Arrow T_Identifier '(' ParamCallList ')'	{$5->insert($5->begin(),$1); $$ = new FunctionCall($3,$5);}
				
				|	'&' VarExpr %prec Reference				{$$ = new ReferenceExpr($2); }
				
				|	VarExpr									{$$ = $1;}
				
				|	T_StringLiteral							{$$ = new StringLiteralExpr($1);}
				|	T_IntLiteral							{$$ = new IntLiteralExpr($1);}
				|	T_RealLiteral							{$$ = new RealLiteralExpr($1);}
				|	T_True									{$$ = new BoolLiteralExpr(true);}
				|	T_False									{$$ = new BoolLiteralExpr(false);}
				;

VarExpr			:	T_Identifier							{$$ = new IdentVar($1);}
				;

ParamCallList	:											{$$ = new std::vector<Expression*>();}
				|	RParamCallList							{$$ = $1;}
				;

RParamCallList	:	Expression								{($$ = new std::vector<Expression*>)->push_back($1);}
				|	RParamCallList ',' Expression			{($$ = $1)->push_back($3);}
				;

ParamDeclList	:											{$$ = new std::vector<Declaration*>();}
				|	RParamDeclList							{$$ = $1;}
				;

RParamDeclList	:	ParamDecl								{($$ = new std::vector<Declaration*>)->push_back($1);}
				|	RParamDeclList ',' ParamDecl			{($$ = $1)->push_back($3);}
				;

ParamDecl		:	Type T_Identifier						{$$ = new VarDeclaration($1,$2);}
				|	Type '&' T_Identifier					{$$ = new ReferenceVarDeclaration($1,$3);}
				;

Type			:	T_IntType								{$$ = new CPPType<int>();}
				|	T_RealType								{$$ = new CPPType<float>();}
				|	T_StringType							{$$ = new CPPType<std::string>();}
				|	T_BoolType								{$$ = new CPPType<bool>();}
				|	T_Void									{$$ = new VoidType();}
				;



%%


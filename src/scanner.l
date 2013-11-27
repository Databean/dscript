%{
#include "y.tab.h"
#include "scanner.h"
#include <string>
YY_BUFFER_STATE bufferState;
%}

%option noyywrap
%option yylineno

WHITESPACE		[ \t\r\n]+
INT				[0-9]+
REAL			(([0-9]+\.[0-9]*)|([0-9]*\.[0-9]+))(E(\+|\-)?[0-9]+)?
IDENTIFIER		[a-zA-Z][a-zA-Z0-9]*
STRING			\"[^\r\n\"]*\"
UNCLOSED_STR	\"[^\r\n\"]*
LINE_COMMENT	"//".*
C_COMMENT		((?:\/\*(?:[^*]|(?:\*+[^*\/]))*\*+\/)|(?:\/\/.*))

%%
{WHITESPACE}	{;}
{LINE_COMMENT}	{;}
{C_COMMENT}		{;}
{REAL}			{yylval.realLiteral = float(atof(yytext)); return T_RealLiteral;}
{INT}			{yylval.intLiteral = atoi(yytext); return T_IntLiteral;}
{STRING}		{yylval.string = new string(yytext,1,strlen(yytext)-2); return T_StringLiteral;}
Int				{return T_IntType;}
Real			{return T_RealType;}
String			{return T_StringType;}
Bool			{return T_BoolType;}
Void			{return T_Void;}
true			{return T_True;}
false			{return T_False;}
while			{return T_While;}
function		{return T_Function;}
return			{return T_Return;}
Auto			{return T_Auto;}
if				{return T_If;}
else			{return T_Else;}
struct			{return T_Struct;}

"[]"			{return T_Dimensions;}
">>"			{return T_RightShift;}
"<<"			{return T_LeftShift;}
"<="			{return T_LEqual; }
">="			{return T_REqual; }
"!="			{return T_NEqual; }
"=="			{return T_Equal; }
"&&"			{return T_BooleanAnd;}
"||"			{return T_BooleanOr;}
"^^"			{return T_BooleanXor;}
"->"			{return T_Arrow;}

"+="			{return T_PlusEquals;}
"-="			{return T_MinusEquals;}
"*="			{return T_MultEquals;}
"/="			{return T_DivideEquals;}
"^="			{return T_BitwiseXorEquals;}
"&="			{return T_BitwiseAndEquals;}
"|="			{return T_BitwiseOrEquals;}
">>="			{return T_RightShiftEquals;}
"<<="			{return T_LeftShiftEquals;}
"%="			{return T_ModEquals;}
"||="			{return T_BooleanOrEquals;}
"&&="			{return T_BooleanAndEquals;}
"^^="			{return T_BooleanXorEquals;}

"++"			{return T_PlusPlus;}
"--"			{return T_MinusMinus;}

{IDENTIFIER}	{yylval.string = new string(yytext); return T_Identifier;}
.				{return *yytext; }

%%

void yyerror(char* s) {
	fprintf (stderr, "%s\n", s);
}

void chooseFile(const char* text) {
	bufferState = yy_scan_string(text);
}

void cleanBufferState() {
	yy_delete_buffer(bufferState);
}
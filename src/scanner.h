#ifndef SCANNER_H
#define SCANNER_H

int yylex();

void yyerror(char* s);

void chooseFile ( const char *str );

void cleanBufferState();

#endif
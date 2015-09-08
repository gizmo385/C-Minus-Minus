/* calculator.l */

/* Declarations */

%{
#include "calculator.tab.h"
%}

%option noyywrap

NUM [0-9]
INT [a-z]
%%

 /* Rules */

 /* Ignores all white space except for newlines */
[[:space:]]{-}[\n]
\n		 {return yytext[0]; }

 /* operators */
"+"|"-"|"*"|"/"|"("|")"  { return yytext[0]; }

 /* Ints */
[0-9]+      { yylval = atoi(yytext); return INT; }  

 /* Floats */
{NUM}+\.{NUM}+   { yylval = atoi(yytext); return FLOAT; }

 /* Prints an error if a symbol is found that doesn't match another pattern */
. { fprintf(stderr, "Unknown symbol: %s\n ", yytext ); } 
 
%%

/* A main function that just calls yylex() to run the scanner */

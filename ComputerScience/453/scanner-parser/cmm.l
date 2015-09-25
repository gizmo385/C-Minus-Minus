%{
#include <stdio.h>
#include <stdlib.h>
#include "cmm.tab.h"

int mylineno = 1;
int mycolno = 1;
%}

%option nounput
%option noinput
%option noyywrap

%%

    /* EQUALITY OPERATORS */
"==" { mycolno += strlen(yytext); return EQ; }
"!=" { mycolno += strlen(yytext); return NEQ; }
"!" { mycolno += strlen(yytext); return NOT; }

    /* RELATIONAL OPERATORS */
">=" { mycolno += strlen(yytext); return GTE; }
"<=" { mycolno += strlen(yytext); return LTE; }
">" { mycolno += strlen(yytext); return GT; }
"<" { mycolno += strlen(yytext); return LT; }

    /* RELATIONAL OPERATORS */
"+" { mycolno += strlen(yytext); return ADD; }
"-" { mycolno += strlen(yytext); return MINUS; }
"*" { mycolno += strlen(yytext); return MUL; }
"/" { mycolno += strlen(yytext); return DIV; }

"=" { mycolno += strlen(yytext); return ASSIGN; }

"(" { mycolno += strlen(yytext); return LEFT_PAREN; }
")" { mycolno += strlen(yytext); return RIGHT_PAREN; }
"[" { mycolno += strlen(yytext); return LEFT_SQUARE_BRACKET; }
"]" { mycolno += strlen(yytext); return RIGHT_SQUARE_BRACKET; }
"{" { mycolno += strlen(yytext); return LEFT_CURLY_BRACKET; }
"}" { mycolno += strlen(yytext); return RIGHT_CURLY_BRACKET; }

";" { mycolno += strlen(yytext); return SEMICOLON; }
"," { mycolno += strlen(yytext); return COMMA; }

    /* Match string literals */
\"[^\"]*\" {mycolno += strlen(yytext); return STRINGCON; }
\'([[:print:]]{-}[\"\\]|\n\0)\' {mycolno += strlen(yytext); return CHARCON; }

    /* RESERVED KEYWORDS */
"for" {mycolno += strlen(yytext); return FOR; }
"while" {mycolno += strlen(yytext); return WHILE; }
"if" {mycolno += strlen(yytext); return IF; }
"return" {mycolno += strlen(yytext); return RETURN; }
"else" {mycolno += strlen(yytext); return ELSE; }
"char" {mycolno += strlen(yytext); return CHAR; }
"void" {mycolno += strlen(yytext); return VOID; }
"int" {mycolno += strlen(yytext); return INT; }
"extern" {mycolno += strlen(yytext); return EXTERN; }

    /* LITERALS */
[a-zA-Z][a-zA-Z0-9_]* { mycolno += strlen(yytext); return ID; }
[0-9]+ { mycolno += strlen(yytext); return INTCON; }

%%

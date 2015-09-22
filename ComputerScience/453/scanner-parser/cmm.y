%{
#include <stdio.h>
#include <stdlib.h>
#include "cmm.tab.h"
extern int mylineno;
extern int mycolno;
extern char *yytext;
%}

/* tokens */

%token WHILE_KEYWORD FOR_KEYWORD
%token INT_KEYWORD CHAR_KEYWORD VOID_KEYWORD
%token LEFT_PAREN RIGHT_PAREN
%token LEFT_SQUARE_BRACKET RIGHT_SQUARE_BRACKET
%token LEFT_CURLY_BRACKET RIGHT_CURLY_BRACKET
%token IF_KEYWORD ELSE_KEYWORD

%token SPACE

%%

Wspace  : SPACE
        |
        ;

%%

int main(int argc, char **argv){
   yyparse();
   return 0;
}

int yyerror() {
    fprintf(stderr, "%d:%d - Encountered error while parsing: \"%s\"\n", mylineno, mycolno, yytext);
    exit(1);
}

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

\n {
    mylineno += 1;
    mycolno = 1;
    printf("\n");
}
[[:space:]{-}[\n]]+ { return SPACE; }

[[:print:]{-}[<]]+ { mycolno += strlen(yytext); return TEXT; }

%%

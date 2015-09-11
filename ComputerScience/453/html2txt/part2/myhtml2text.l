%{
#include <stdio.h>
#include <stdlib.h>
#include "myhtml2txt.tab.h"

int yycolno = 1;
%}

%option nounput
%option noinput
%option noyywrap

yylineno = 1;


%%

[[:space:]]{-}[\n]+ { yycolno += strlen(yytext); return SPACE;}
[[:space:]]+ {
    yycolno = 0;
    yylineno += 1;
    return SPACE;
}

\<html\> { yycolno += strlen(yytext); return DOC_START;}
\<\/html\> { yycolno += strlen(yytext); return DOC_END;}

\<head\> { yycolno += strlen(yytext); return HEAD_START;}
\<\/head\> { yycolno += strlen(yytext); return HEAD_END;}

\<body\> { yycolno += strlen(yytext); return BODY_START;}
\<\/body\> { yycolno += strlen(yytext); return BODY_END;}

\<b\> { yycolno += strlen(yytext); return BF_START;}
\<\/b\> { yycolno += strlen(yytext); return BF_END;}

\<i\> { yycolno += strlen(yytext); return IT_START; }
\<\/i\> { yycolno += strlen(yytext); return IT_END; }

\<ul\> { yycolno += strlen(yytext); return UL_START; }
\<\/ul\> { yycolno += strlen(yytext); return UL_END; }

\<ol\> { yycolno += strlen(yytext); return OL_START; }
\<\/ol\> { yycolno += strlen(yytext); return OL_END; }

\<li\> { yycolno += strlen(yytext); return LI_START; }
\<\/li\> { yycolno += strlen(yytext); return LI_END; }

    /* Handling tags */
\<[^[:space:]>][:print:]{-}[>]*> { yycolno += strlen(yytext); return TAG;}

\n {
    yylineno += 1;
    yycolno = 0;
    printf("\n");
}

[:print:]{-}[<]+ { return TEXT; }

%%

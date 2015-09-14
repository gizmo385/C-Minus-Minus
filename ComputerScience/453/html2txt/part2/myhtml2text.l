%{
#include <stdio.h>
#include <stdlib.h>
#include "myhtml2txt.tab.h"

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

\<html\> { mycolno += strlen(yytext); return DOC_START;}
\<\/html\> { mycolno += strlen(yytext); return DOC_END;}

\<head\> { mycolno += strlen(yytext); return HEAD_START;}
\<\/head\> { mycolno += strlen(yytext); return HEAD_END;}

\<body\> { mycolno += strlen(yytext); return BODY_START;}
\<\/body\> { mycolno += strlen(yytext); return BODY_END;}

\<b\> { mycolno += strlen(yytext); return BF_START;}
\<\/b\> { mycolno += strlen(yytext); return BF_END;}

\<i\> { mycolno += strlen(yytext); return IT_START; }
\<\/i\> { mycolno += strlen(yytext); return IT_END; }

\<ul\> { mycolno += strlen(yytext); return UL_START; }
\<\/ul\> { mycolno += strlen(yytext); return UL_END; }

\<ol\> { mycolno += strlen(yytext); return OL_START; }
\<\/ol\> { mycolno += strlen(yytext); return OL_END; }

\<li\> { mycolno += strlen(yytext); return LI_START; }
\<\/li\> { mycolno += strlen(yytext); return LI_END; }

    /* HTML SPECIAL ENTITIES */
\&amp; { mycolno += strlen(yytext); yytext = "&"; return TEXT; }
\&gt; { mycolno += strlen(yytext); yytext = ">"; return TEXT; }
\&lt; { mycolno += strlen(yytext); yytext = "<"; return TEXT; }
\&quot; { mycolno += strlen(yytext); yytext = "\""; return TEXT; }

    /* Handling tags */
\<[^[:space:]>][[:print:]]{-}[>]*\> { mycolno += strlen(yytext); return TAG; }

[[:print:]{-}[<]]+ { mycolno += strlen(yytext); return TEXT; }

%%

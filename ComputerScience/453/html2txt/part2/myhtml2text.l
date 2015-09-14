%{
#include <stdio.h>
#include <stdlib.h>
#include "myhtml2txt.tab.h"

int mylineno = 1;
%}

%option nounput
%option noinput
%option noyywrap

%%

\n { mylineno += 1; printf("\n"); }
[[:space:]{-}[\n]]+ { return SPACE; }

\<html\> { return DOC_START;}
\<\/html\> { return DOC_END;}

\<head\> { return HEAD_START;}
\<\/head\> { return HEAD_END;}

\<body\> { return BODY_START;}
\<\/body\> { return BODY_END;}

\<b\> { return BF_START;}
\<\/b\> { return BF_END;}

\<i\> { return IT_START; }
\<\/i\> { return IT_END; }

\<ul\> { return UL_START; }
\<\/ul\> { return UL_END; }

\<ol\> { return OL_START; }
\<\/ol\> { return OL_END; }

\<li\> { return LI_START; }
\<\/li\> { return LI_END; }

    /* HTML SPECIAL ENTITIES */
\&amp; { yytext = "&"; return TEXT; }
\&gt; { yytext = ">"; return TEXT; }
\&lt; { yytext = "<"; return TEXT; }
\&quot; { yytext = "\""; return TEXT; }

    /* Handling tags */
\<[^[:space:]>][[:print:]]{-}[>]*\> { return TAG; }

[[:print:]{-}[<]]+ { return TEXT; }

%%

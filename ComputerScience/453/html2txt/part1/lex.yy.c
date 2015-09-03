%option nounput
%option noinput
%{
#include <stdio.h>
#include <stdlib.h>
%}
%%
    /* HTML SPECIAL ENTITIES */
\&amp; printf("&");
\&gt; printf(">");
\&lt; printf("<");
\&quot; printf("\"");

    /* Handling tags (This should be states) */
\<[^[:space:]>][[:print:]]{-}[>]*\> printf("");
%%
int main()
{
    yylex();
}

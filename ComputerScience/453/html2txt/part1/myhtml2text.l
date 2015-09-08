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

    /* Handling tags */
\<[^[:space:]>][[:print:]]{-}[>]*\>
%%
int main()
{
    yylex();
}

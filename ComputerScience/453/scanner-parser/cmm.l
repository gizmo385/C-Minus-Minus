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


%x comment
%%


    /* EQUALITY OPERATORS */
"=="                                { mycolno += strlen(yytext); return EQ; }
"!="                                { mycolno += strlen(yytext); return NEQ; }
"!"                                 { mycolno += strlen(yytext); return NOT; }

    /* RELATIONAL OPERATORS */
">="                                { mycolno += strlen(yytext); return GTE; }
"<="                                { mycolno += strlen(yytext); return LTE; }
">"                                 { mycolno += strlen(yytext); return GT; }
"<"                                 { mycolno += strlen(yytext); return LT; }

    /* RELATIONAL OPERATORS */
"+"                                 { mycolno += strlen(yytext); return ADD; }
"-"                                 { mycolno += strlen(yytext); return MINUS; }
"*"                                 { mycolno += strlen(yytext); return MUL; }
"/"                                 { mycolno += strlen(yytext); return DIV; }

"="                                 { mycolno += strlen(yytext); return ASSIGN; }

    /* PARENTHESIS TOKENS */
"("                                 { mycolno += strlen(yytext); return LEFT_PAREN; }
")"                                 { mycolno += strlen(yytext); return RIGHT_PAREN; }
"["                                 { mycolno += strlen(yytext); return LEFT_SQUARE_BRACKET; }
"]"                                 { mycolno += strlen(yytext); return RIGHT_SQUARE_BRACKET; }
"{"                                 { mycolno += strlen(yytext); return LEFT_CURLY_BRACKET; }
"}"                                 { mycolno += strlen(yytext); return RIGHT_CURLY_BRACKET; }

    /* Delimiters */
";"                                 { mycolno += strlen(yytext); return SEMICOLON; }
","                                 { mycolno += strlen(yytext); return COMMA; }

    /* LITERALS */
\"[^\"]*\"                          { mycolno += strlen(yytext); return STRINGCON; }
\'(\\n|\\0|[[:print:]]{-}[\\\"])\'  { mycolno += strlen(yytext); return CHARCON; }
[0-9]+                              { mycolno += strlen(yytext); return INTCON; }

    /* RESERVED KEYWORDS */
"for"                               { mycolno += strlen(yytext); return FOR; }
"while"                             { mycolno += strlen(yytext); return WHILE; }
"if"                                { mycolno += strlen(yytext); return IF; }
"return"                            { mycolno += strlen(yytext); return RETURN; }
"else"                              { mycolno += strlen(yytext); return ELSE; }
"char"                              { mycolno += strlen(yytext); return CHAR; }
"void"                              { mycolno += strlen(yytext); return VOID; }
"int"                               { mycolno += strlen(yytext); return INT; }
"extern"                            { mycolno += strlen(yytext); return EXTERN; }

    /* DISCARD COMMENTS */
"/*"                                { mycolno += strlen(yytext); BEGIN(comment); }
<comment>[^*\n]*                    { mycolno += strlen(yytext); }
<comment>"*"+[^*/\n]*               { mycolno += strlen(yytext); }
<comment>\n                         { mylineno += 1; mycolno = 1;}
<comment>"*"+"/"                    { mycolno += strlen(yytext); BEGIN(INITIAL); }

    /* IDENTIFIERS */
[a-zA-Z][a-zA-Z0-9_]*               { mycolno += strlen(yytext); return ID; }

    /* WHITESPACE */
[[:space:]]{-}[\n]+                 {mycolno += strlen(yytext);}
\n                                  { mylineno += 1; mycolno = 1; }

%%

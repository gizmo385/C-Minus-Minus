%{
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "utils.h"
#include "cmm.tab.h"

int mylineno = 1;
int mycolno = 1;
%}

%option nounput
%option noinput
%option noyywrap


%x comment
%%


    /* LITERALS */
\"[[:print:]]{-}[\n\"]*\"           { mycolno += strlen(yytext); return STRINGCON; }
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

    /* EQUALITY OPERATORS */
"=="                                { mycolno += strlen(yytext); return EQ; }
"!="                                { mycolno += strlen(yytext); return NEQ; }
"!"                                 { mycolno += strlen(yytext); return NOT; }

    /* LOGICAL OPERATORS */
"\&\&"                              { mycolno += strlen(yytext); return AND; }
"||"                                { mycolno += strlen(yytext); return OR; }

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

    /* IDENTIFIERS */
[a-zA-Z][a-zA-Z0-9\_]*               { mycolno += strlen(yytext); yylval.string = strdup(yytext); return ID; }

    /* WHITESPACE */
[[:space:]]{-}[\n]+                 { mycolno += strlen(yytext); }

    /* DISCARD COMMENTS */
"/*"                                { mycolno += strlen(yytext); BEGIN(comment); }
<comment><<EOF>>                    {
    fprintf(stderr, "Unexpected end of file while parsing comment on line %d.\n", mylineno);
    exit(1);
}
<comment>[^*\n]*                    { mycolno += strlen(yytext); }
<comment>"*"+[^*/\n]*               { mycolno += strlen(yytext); }
<comment>\n                         { mylineno += 1; mycolno = 1;}
<comment>"*"+"/"                    { mycolno += strlen(yytext); BEGIN(INITIAL); }

\/\/.+\n                            { mycolno = 0; mylineno += 1; }

    /* Newlines */
\n                                  { mylineno += 1; mycolno = 1; }

%%

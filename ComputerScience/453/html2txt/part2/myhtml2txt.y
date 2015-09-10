
%{
#include <stdio.h>
#include <stdlib.h>
extern int yylineno;
extern char *yytext;
%}


/* tokens */

%token INT FLOAT



%%

start :
	  ;

%%

int main(int argc, char **argv){
   yyparse();
   return 0;
}

int yyerror() {
   /* TODO: Fix this abyssmal error message */
   fprintf(stderr, "There was an error\n");
   return 1;
}

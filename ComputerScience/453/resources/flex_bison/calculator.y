
%{
# include <stdio.h>
# include <stdlib.h>
extern int yylineno;
extern char *yytext;
%}


/* tokens */

%token INT FLOAT



%%

start : 
	  | start line 
	  ;

line : '\n' 
	 |expr '\n'			{ printf("%d\n", $1); }
	 ;

expr : expr '+' term	{ $$ = $1 + $3; }
	 | expr '-' term	{ $$ = $1 - $3; }
	 | term				{ $$ = $1; }
     ;

factor : '(' expr ')'	{ $$ = $2; }
	   | '-' factor 	{ $$ = -$2; }
	   | INT 			{ $$ = $1; }
	   | FLOAT			{ $$ = $1; }
	   ;


term : term '*' factor	{ $$ = $1 * $3; }
	 | term '/' factor	{ $$ = $1 / $3; }
	 | factor			{ $$ = $1; }
	 ;
	 

	  


%%

main( int argc, char **argv){
   yyparse();
   return 0;
}  

yyerror(){
   /* Do not use this. This is a terrible error message, that doesn't have any useful information. */	
   fprintf(stderr, "There was an error\n"); 
}   

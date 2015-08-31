/* calculator.l */

/* Declarations */

NUM [0-9]

 /* Defines an inclusive start state called FLOAT */
%s FLOAT

%%

 /* Rules */

 /* Ignores all white space except for newlines */
[[:space:]]{-}[\n] 

 /* Two patterns that recognize operators */
[+\-*/()] { printf("op2: %s ", yytext ); }
 /* "+"|"-"|"*"|"/"|"("|")"  { printf("op: %s ", yytext ); } */

 /* A pattern that recognizes an x,y,or followed by at least one number
    and then either an a, b, or c */
[xyz][0-9]+[abc]   {printf("Surrounded number: %s ", yytext); }

 /* Ints */
-?[0-9]+      { printf("int: %s ", yytext ); }  


 /* Switches to FLOAT state */
floats {BEGIN(FLOAT); }

 /* Floats */
<FLOAT>-?{NUM}+\.{NUM}+   { printf("float: %s ", yytext ); }

 /* Switches back to initial state */
nofloat {BEGIN(INITIAL); }

 /* Prints an error if a symbol is found that doesn't match another pattern */
. { fprintf(stderr, "Unknown symbol: %s\n ", yytext ); } 
 
%%

/* A main function that just calls yylex() to run the scanner */

int main(int argc, char **argv){
	yylex();
	return 0;
}
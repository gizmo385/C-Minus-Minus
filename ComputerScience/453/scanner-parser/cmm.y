%{
#include <stdio.h>
#include <stdlib.h>
#include "cmm.tab.h"
extern int mylineno;
extern int mycolno;
extern char *yytext;
%}

    /* Language Tokens */
%token WHILE FOR
%token INT CHAR VOID
%token IF ELSE
%token EXTERN RETURN
%token COMMA SEMICOLON

%token LEFT_PAREN RIGHT_PAREN
%token LEFT_SQUARE_BRACKET RIGHT_SQUARE_BRACKET
%token LEFT_CURLY_BRACKET RIGHT_CURLY_BRACKET

    /* Operators */
%token EQ NEQ LTE LT GT GTE NOT
%token AND OR
%token ADD MINUS MUL DIV
%token ASSIGN

    /* Text tokens */
%token INTCON CHARCON STRINGCON
%token ID
%token TEXT SPACE

    /* If/Else Precedence fix */
%nonassoc WITHOUT_ELSE
%nonassoc ELSE WITH_ELSE

    /* Operator Precedence specification */
%left OR
%left AND
%left EQ NEQ equality_op
%left GTE LTE GT LT relop
%left ADD MINUS add_sub
%left MUL DIV mul_div
%right UMINUS '!'

%%

prog : decl prog
     | func prog
     | epsilon

decl : type var_decl_list SEMICOLON

var_decl : ID
         | ID LEFT_SQUARE_BRACKET INTCON RIGHT_SQUARE_BRACKET

var_decl_list : var_decl
              | var_decl_list COMMA var_decl
              | epsilon
              ;

type : CHAR
     | INT
     ;

param_types : VOID
            | param_types_list
            ;

non_void_param_type : type ID
                    | type ID LEFT_SQUARE_BRACKET RIGHT_SQUARE_BRACKET
                    ;

param_types_list : non_void_param_type
                 | param_types_list COMMA non_void_param_type
                 | epsilon
                 ;

<<<<<<< HEAD
func : type ID LEFT_PAREN param_types RIGHT_PAREN LEFT_CURLY_BRACKET var_decl_list stmt_list RIGHT_CURLY_BRACKET
     | VOID ID LEFT_PAREN param_types RIGHT_PAREN LEFT_CURLY_BRACKET var_decl_list  stmt_list RIGHT_CURLY_BRACKET
     | EXTERN type ID LEFT_PAREN param_types RIGHT_PAREN LEFT_CURLY_BRACKET var_decl_list stmt_list RIGHT_CURLY_BRACKET
     | EXTERN VOID ID LEFT_PAREN param_types RIGHT_PAREN LEFT_CURLY_BRACKET var_decl_list  stmt_list RIGHT_CURLY_BRACKET

=======
func : type ID LEFT_PAREN param_types RIGHT_PAREN LEFT_CURLY_BRACKET optional_var_decl_list stmt_list RIGHT_CURLY_BRACKET
     | VOID ID LEFT_PAREN param_types RIGHT_PAREN LEFT_CURLY_BRACKET optional_var_decl_list  stmt_list RIGHT_CURLY_BRACKET
>>>>>>> c5dbbae... 453 2: Add leading variable declarations in fn
     ;

optional_var_decl_list : type var_decl_list SEMICOLON optional_var_decl_list
                       | epsilon

stmt : IF LEFT_PAREN expr RIGHT_PAREN stmt %prec WITHOUT_ELSE
     | IF LEFT_PAREN expr RIGHT_PAREN stmt ELSE stmt
     | WHILE LEFT_PAREN expr RIGHT_PAREN stmt
     | FOR LEFT_PAREN optional_assign SEMICOLON optional_expr SEMICOLON optional_assign RIGHT_PAREN stmt
     | RETURN optional_expr SEMICOLON
     | assg SEMICOLON
     | ID LEFT_PAREN expr_list RIGHT_PAREN /* Function call */
     | LEFT_CURLY_BRACKET stmt_list RIGHT_CURLY_BRACKET
     | SEMICOLON
     ;

optional_assign: assg
               | epsilon
               ;

optional_expr : expr
              | epsilon
              ;

stmt_list : stmt stmt_list
          | epsilon
          ;

assg : ID ASSIGN expr
     | ID LEFT_SQUARE_BRACKET expr RIGHT_SQUARE_BRACKET ASSIGN expr
     ;

expr : MINUS expr %prec UMINUS
     | NOT expr %prec UMINUS
     | expr ADD expr %prec add_sub
     | expr MINUS expr %prec add_sub
     | expr MUL expr %prec mul_div
     | expr DIV expr %prec mul_div
     | expr EQ expr %prec equality_op
     | expr NEQ expr %prec equality_op
     | expr GTE expr %prec relop
     | expr LTE expr %prec relop
     | expr GT expr %prec relop
     | expr LT expr %prec relop
     | ID
     | ID LEFT_PAREN expr_list RIGHT_PAREN /* Function call with arguments */
     | LEFT_PAREN expr RIGHT_PAREN
     | INTCON
     | CHARCON
     | STRINGCON
     ;

expr_list : expr
          | expr_list COMMA expr
          | epsilon

epsilon:
       ;
%%

int main(int argc, char **argv){
    /*yydebug = 1;*/
    return yyparse();
}

int yyerror() {
    if(yytext[0] == 0) {
        fprintf(stderr, "%d:%d - Encountered error while parsing: \"%s\"\n", mylineno, mycolno, yytext);
    } else {
        fprintf(stderr, "Encountered unexpected EOF while parsing \"%s\" starting on line %d.\n",
                yytext, mylineno);
    }
}

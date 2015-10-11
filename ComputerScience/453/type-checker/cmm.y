%{
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "cmm.tab.h"
extern int mylineno;
extern int mycolno;
extern char *yytext;

bool foundError = false;
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
%right UMINUS NOT

%%

prog : decl prog
     | func prog
     | epsilon

decl : type var_decl_list SEMICOLON
     | type name_args_lists SEMICOLON
     | VOID name_args_lists SEMICOLON
     | EXTERN type name_args_lists SEMICOLON
     | EXTERN VOID name_args_lists SEMICOLON
     | error SEMICOLON {foundError = true}
     ;

func : type ID LEFT_PAREN param_types RIGHT_PAREN LEFT_CURLY_BRACKET optional_var_decl_list stmt_list RIGHT_CURLY_BRACKET
     | VOID ID LEFT_PAREN param_types RIGHT_PAREN LEFT_CURLY_BRACKET optional_var_decl_list  stmt_list RIGHT_CURLY_BRACKET
     | error RIGHT_CURLY_BRACKET {foundError = true}
     ;

stmt : IF LEFT_PAREN expr RIGHT_PAREN stmt %prec WITHOUT_ELSE
     | IF LEFT_PAREN expr RIGHT_PAREN stmt ELSE stmt
     | WHILE LEFT_PAREN expr RIGHT_PAREN stmt
     | FOR LEFT_PAREN optional_assign SEMICOLON optional_expr SEMICOLON optional_assign RIGHT_PAREN stmt
     | RETURN optional_expr SEMICOLON
     | assg SEMICOLON
     | ID LEFT_PAREN expr_list RIGHT_PAREN SEMICOLON/* Function call */
     | LEFT_CURLY_BRACKET stmt_list RIGHT_CURLY_BRACKET
     | SEMICOLON
     | error SEMICOLON {foundError = true}
     | error RIGHT_CURLY_BRACKET {foundError = true}
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
     | ID LEFT_SQUARE_BRACKET expr RIGHT_SQUARE_BRACKET /* Array access */
     | LEFT_PAREN expr RIGHT_PAREN
     | INTCON
     | CHARCON
     | STRINGCON
     | error {foundError = true}
     ;

name_args_lists : ID LEFT_PAREN param_types RIGHT_PAREN
                | name_args_lists COMMA ID LEFT_PAREN param_types RIGHT_PAREN
                ;

var_decl : ID
         | ID LEFT_SQUARE_BRACKET INTCON RIGHT_SQUARE_BRACKET
         ;

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

optional_var_decl_list : type var_decl_list SEMICOLON optional_var_decl_list
                       | epsilon

optional_assign: assg
               | error {foundError = true}
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

expr_list : expr
          | expr_list COMMA expr
          | epsilon

epsilon:
       ;

%%

int main(int argc, char **argv){
    /*yydebug = 1;*/
    yyparse();

    if(foundError) {
        return 1;
    } else {
        return 0;
    }
}

int yyerror() {
    if(yytext[0] == 0) {
        fprintf(stderr, "Encountered unexpected EOF while parsing \"%s\" starting on line %d.\n",
                yytext, mylineno);
    } else {
        fprintf(stderr, "%d:%d - Encountered error while parsing: \"%s\"\n", mylineno, mycolno,
                yytext);
    }

    return 1;
}

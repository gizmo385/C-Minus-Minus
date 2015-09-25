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
%token ADD SUB MUL DIV
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
%left ADD SUB add_sub
%left MUL DIV mul_div
%right UMINUS '!'

%%

prog : decl prog
     | func prog
     | epsilon

decl : type var_decl_list SEMICOLON

var_decl : ID
         | ID LEFT_SQUARE_BRACKET INTCON RIGHT_SQUARE_BRACKET

var_decl_list : type var_decl SEMICOLON var_decl_list
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

func : type ID LEFT_PAREN param_types RIGHT_PAREN LEFT_CURLY_BRACKET var_decl_list stmt_list RIGHT_CURLY_BRACKET
     | VOID ID LEFT_PAREN param_types RIGHT_PAREN LEFT_CURLY_BRACKET var_decl_list  stmt_list RIGHT_CURLY_BRACKET
     ;

stmt : IF LEFT_PAREN expr RIGHT_PAREN stmt %prec WITHOUT_ELSE
     | IF LEFT_PAREN expr RIGHT_PAREN stmt ELSE stmt
     | WHILE LEFT_PAREN expr RIGHT_PAREN stmt
     | FOR LEFT_PAREN optional_assign SEMICOLON optional_expr SEMICOLON optional_assign RIGHT_PAREN stmt
     | RETURN optional_expr SEMICOLON
     | assg SEMICOLON
     | ID LEFT_PAREN param_list RIGHT_PAREN /* Function call with arguments */
     | ID LEFT_PAREN RIGHT_PAREN /* Calling a thunk */
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

expr : '-' expr %prec UMINUS
     | '!' expr %prec UMINUS
     | expr ADD expr %prec add_sub
     | expr SUB expr %prec add_sub
     | expr MUL expr %prec mul_div
     | expr DIV expr %prec mul_div
     | expr relop expr
     | expr equality_op expr
     | ID
     | ID LEFT_PAREN param_list RIGHT_PAREN /* Function call with arguments */
     | ID LEFT_PAREN RIGHT_PAREN /* Calling a thunk */
     | LEFT_PAREN expr RIGHT_PAREN
     | INTCON
     | CHARCON
     | STRINGCON
     ;

param_list : ID
           | ID COMMA param_list

epsilon:
       ;
%%

int main(int argc, char **argv){
    yydebug = 1;
    return yyparse();
}

int yyerror() {
    fprintf(stderr, "%d:%d - Encountered error while parsing: \"%s\"\n", mylineno, mycolno, yytext);
}

%{
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "symtab.h"
#include "utils.h"
#include "ast.h"
#include "globals.h"
#include "cmm.tab.h"

extern char *yytext;

int mylineno;
int mycolno;
bool foundError = false;

Type currentFunctionReturnType;
Scope *scope;
Type baseDeclType;

int diffComp(void *a, void *b);

%}

%union {
    Expression *expression;
    Statement *statement;
    StatementList *statementList;
    List *expressionList;
    Type type;
    char *string;
}

%type<expression> expr optional_expr
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
%type<statementList> stmt_list
%type<statement> stmt assg optional_assign
=======
/*%type<statementList> stmt_list*/
/*%type<statement> stmt assg optional_assign*/
>>>>>>> dfa249c... 453 3: Actions for non-identifier expressions
=======
=======
%type<expressionList> expr_list
>>>>>>> 0f11d54... 453 3: Creating expression lists in parser
%type<statement> stmt assg optional_assign func
%type<type> type
>>>>>>> 741c33c... 453 3: Setting current function return type

/* Language Tokens */
%token WHILE FOR
%token INT CHAR VOID
%token IF ELSE
%token EXTERN RETURN
%token COMMA SEMICOLON

/* Brackets */
%token LEFT_PAREN RIGHT_PAREN
%token LEFT_SQUARE_BRACKET RIGHT_SQUARE_BRACKET
%token LEFT_CURLY_BRACKET RIGHT_CURLY_BRACKET

/* Operators */
%token EQ NEQ LTE LT GT GTE NOT
%token AND OR
%token ADD MINUS MUL DIV
%token ASSIGN

/* Text tokens */
%token <expression> INTCON
%token <expression> CHARCON
%token <expression> STRINGCON
<<<<<<< HEAD
%token ID
%type<string> ID
=======
%token <string> ID
>>>>>>> dfa249c... 453 3: Actions for non-identifier expressions
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
     | error SEMICOLON
     ;

func : type ID LEFT_PAREN param_types RIGHT_PAREN LEFT_CURLY_BRACKET optional_var_decl_list stmt_list RIGHT_CURLY_BRACKET
     { currentFunctionReturnType = $1; }
     | VOID ID LEFT_PAREN param_types RIGHT_PAREN LEFT_CURLY_BRACKET optional_var_decl_list  stmt_list RIGHT_CURLY_BRACKET
     { currentFunctionReturnType = VOID_TYPE; }
     | error RIGHT_CURLY_BRACKET
     ;

<<<<<<< HEAD
<<<<<<< HEAD
stmt : IF LEFT_PAREN expr RIGHT_PAREN stmt %prec WITHOUT_ELSE { $$ = newIfStatement($3, $5); }
     | IF LEFT_PAREN expr RIGHT_PAREN stmt ELSE stmt { $$ = newIfElseStatement($3, $5, $7); }
     | WHILE LEFT_PAREN expr RIGHT_PAREN stmt { $$ = newWhileStatement($3, $5); }
     | FOR LEFT_PAREN optional_assign SEMICOLON optional_expr SEMICOLON optional_assign RIGHT_PAREN stmt {
        $$ = newForStatement((AssignmentStatement *)$3, $5, (AssignmentStatement *)$7, $9);
     }
     | RETURN optional_expr SEMICOLON   { $$ = newReturnStatement($2); }
     | assg SEMICOLON                   { $$ = $1 }
     | ID LEFT_PAREN expr_list RIGHT_PAREN SEMICOLON { $$ = NULL; /* TODO */ }
     | LEFT_CURLY_BRACKET stmt_list RIGHT_CURLY_BRACKET {
        Statement *stmt = malloc(sizeof(Statement));
        stmt->stmt_list = $2;
        stmt->type = ST_LIST;

        $$ = stmt;
        }
     | SEMICOLON                        { $$ = NULL; }
     | error SEMICOLON                  { $$ = NULL; }
     | error RIGHT_CURLY_BRACKET        { $$ = NULL; }
=======
stmt : IF LEFT_PAREN expr RIGHT_PAREN stmt %prec WITHOUT_ELSE
     | IF LEFT_PAREN expr RIGHT_PAREN stmt ELSE stmt
     | WHILE LEFT_PAREN expr RIGHT_PAREN stmt
     | FOR LEFT_PAREN optional_assign SEMICOLON optional_expr SEMICOLON optional_assign RIGHT_PAREN stmt
     | RETURN optional_expr SEMICOLON
     | assg SEMICOLON
     | ID LEFT_PAREN expr_list RIGHT_PAREN SEMICOLON/* Function call */
=======
stmt : IF LEFT_PAREN expr RIGHT_PAREN stmt %prec WITHOUT_ELSE   { $$ = newIfStatement(scope, $3, $5); }
     | IF LEFT_PAREN expr RIGHT_PAREN stmt ELSE stmt            { $$ = newIfElseStatement(scope, $3, $5, $7); }
     | WHILE LEFT_PAREN expr RIGHT_PAREN stmt                   { $$ = newWhileStatement(scope, $3, $5); }
     | FOR LEFT_PAREN optional_assign SEMICOLON optional_expr SEMICOLON optional_assign RIGHT_PAREN stmt
     | RETURN optional_expr SEMICOLON                           { $$ = newReturnStatement(scope, $2); }
     | assg SEMICOLON                                           { $$ = $1; }
     | ID LEFT_PAREN expr_list RIGHT_PAREN SEMICOLON
>>>>>>> 33626bd... 453 3: Creating statement nodes in parser
     | LEFT_CURLY_BRACKET stmt_list RIGHT_CURLY_BRACKET
     | SEMICOLON
     | error SEMICOLON
     | error RIGHT_CURLY_BRACKET
>>>>>>> aeb3970... 453 3: foundError set in yyerror, not in action
     ;

<<<<<<< HEAD
<<<<<<< HEAD
expr : MINUS expr %prec UMINUS          { $$ = newUnaryExpression(NEG_OP, $2); }
     | NOT expr %prec UMINUS            { $$ = newUnaryExpression(NOT_OP, $2); }
     | expr ADD expr %prec add_sub      { $$ = newBinaryExpression(ADD_OP, $1, $3); }
     | expr MINUS expr %prec add_sub    { $$ = newBinaryExpression(SUB_OP, $1, $3); }
     | expr MUL expr %prec mul_div      { $$ = newBinaryExpression(MUL_OP, $1, $3); }
     | expr DIV expr %prec mul_div      { $$ = newBinaryExpression(DIV_OP, $1, $3); }
     | expr EQ expr %prec equality_op   { $$ = newBinaryExpression(EQ_OP, $1, $3); }
     | expr NEQ expr %prec equality_op  { $$ = newBinaryExpression(NEQ_OP, $1, $3); }
     | expr GTE expr %prec relop        { $$ = newBinaryExpression(GTE_OP, $1, $3); }
     | expr LTE expr %prec relop        { $$ = newBinaryExpression(LTE_OP, $1, $3); }
     | expr GT expr %prec relop         { $$ = newBinaryExpression(GT_OP, $1, $3); }
     | expr LT expr %prec relop         { $$ = newBinaryExpression(LT_OP, $1, $3); }
     | ID LEFT_PAREN expr_list RIGHT_PAREN  { $$ = NULL; /* TODO*/ }
     | ID LEFT_SQUARE_BRACKET expr RIGHT_SQUARE_BRACKET { $$ = NULL; /* TODO*/ }
     | ID {
        ScopeVariable *var = findScopeVariable(scope, yylval.string);

        if(var) {
            $$ = newConstExpression(var->type, var->value);
        } else {
            fprintf(stderr, "ERROR: Invalid Identifier: %s\n", yylval.string);
        }
     }
     | LEFT_PAREN expr RIGHT_PAREN      { $$ = $2; }
     | INTCON                           { $$ = newIntConstExpression(atoi(yytext)); }
     | CHARCON                          { $$ = newCharConstExpression(yytext[0]); }
     | STRINGCON                        { $$ = newCharArrayConstExpression(strdup(yytext)); }
     | error                            { $$ = NULL; }
=======
expr : MINUS expr %prec UMINUS
     | NOT expr %prec UMINUS
     | expr ADD expr %prec add_sub
     | expr MINUS expr %prec add_sub
     | expr AND expr
     | expr OR expr
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
<<<<<<< HEAD
     | error {foundError = true}
>>>>>>> 6fe734b... 453 3: Fix AND and OR parsing
=======
     | error
>>>>>>> aeb3970... 453 3: foundError set in yyerror, not in action
=======
expr : MINUS expr %prec UMINUS                          { $$ = newUnaryExpression(NEG_OP, $2); }
     | NOT expr %prec UMINUS                            { $$ = newUnaryExpression(NOT_OP, $2); }
     | expr ADD expr %prec add_sub                      { $$ = newBinaryExpression(ADD_OP, $1, $3); }
     | expr MINUS expr %prec add_sub                    { $$ = newBinaryExpression(SUB_OP, $1, $3); }
     | expr AND expr                                    { $$ = newBinaryExpression(AND_OP, $1, $3); }
     | expr OR expr                                     { $$ = newBinaryExpression(OR_OP, $1, $3); }
     | expr MUL expr %prec mul_div                      { $$ = newBinaryExpression(MUL_OP, $1, $3); }
     | expr DIV expr %prec mul_div                      { $$ = newBinaryExpression(DIV_OP, $1, $3); }
     | expr EQ expr %prec equality_op                   { $$ = newBinaryExpression(EQ_OP, $1, $3); }
     | expr NEQ expr %prec equality_op                  { $$ = newBinaryExpression(NEQ_OP, $1, $3); }
     | expr GTE expr %prec relop                        { $$ = newBinaryExpression(GTE_OP, $1, $3); }
     | expr LTE expr %prec relop                        { $$ = newBinaryExpression(LTE_OP, $1, $3); }
     | expr GT expr %prec relop                         { $$ = newBinaryExpression(GT_OP, $1, $3); }
     | expr LT expr %prec relop                         { $$ = newBinaryExpression(LT_OP, $1, $3); }
     | ID LEFT_PAREN expr_list RIGHT_PAREN              { $$ = newFunctionExpression(scope, $1, $3); }
     | ID LEFT_SQUARE_BRACKET expr RIGHT_SQUARE_BRACKET { $$ = newVariableExpression(scope, $1, $3); }
     | ID                                               { $$ = newVariableExpression(scope, $1, NULL); }
     | LEFT_PAREN expr RIGHT_PAREN                      { $$ = $2; }
     | INTCON                                           { $$ = newIntConstExpression(atoi(yytext)); }
     | CHARCON                                          { $$ = newCharConstExpression(yytext[0]); }
     | STRINGCON                                        { $$ = newCharArrayConstExpression(strdup(yytext)); }
     | error                                            { $$ = NULL; }
>>>>>>> dfa249c... 453 3: Actions for non-identifier expressions
     ;

name_args_lists : ID LEFT_PAREN param_types RIGHT_PAREN
                | name_args_lists COMMA ID LEFT_PAREN param_types RIGHT_PAREN
                ;

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
var_decl : ID { declareUndeclaredVar(scope, baseDeclType, yylval.string); }
         | ID LEFT_SQUARE_BRACKET INTCON RIGHT_SQUARE_BRACKET {
            if(baseDeclType == CHAR_TYPE) {
                declareUndeclaredVar(scope, CHAR_ARRAY_TYPE, yylval.string);
            } else if(baseDeclType == INT_TYPE) {
                declareUndeclaredVar(scope, INT_ARRAY_TYPE, yylval.string);
            }
            }
=======
var_decl : ID { declareUndeclaredVar(scope, baseDeclType, $1); }
=======
var_decl : ID {
            bool success = declareVar(scope, baseDeclType, $1);

            if(! success) {
                fprintf(stderr, "ERROR: On line %d, %s has already been declared!\n", mylineno, $1);
                foundError = true;
            }
        }
>>>>>>> d8c9c6b... 453 3: Declaring variables in scope
=======
var_decl : ID { declareVar(scope, baseDeclType, $1); }
>>>>>>> 623c239... 453 3: Call new declareVar constructor in parser
         | ID LEFT_SQUARE_BRACKET INTCON RIGHT_SQUARE_BRACKET {
            char *id = $1;
            if(baseDeclType == INT_TYPE) {
                declareVar(scope, INT_ARRAY_TYPE, id);
            } else if(baseDeclType == CHAR_TYPE) {
                declareVar(scope, CHAR_ARRAY_TYPE, id);
            } else {
                fprintf(stderr, "ERROR: Cannot determine type when declaring %s on line %d!\n", id, mylineno);
            }
        }
>>>>>>> 4132105... 453 3: Variable declarations in the symbol table
         ;

var_decl_list : var_decl
              | var_decl_list COMMA var_decl
              | epsilon
              ;

<<<<<<< HEAD
<<<<<<< HEAD
type : CHAR                             { baseDeclType = CHAR_TYPE; }
     | INT                              { baseDeclType = INT_TYPE; }
=======
type : CHAR { baseDeclType = CHAR_TYPE; }
     | INT { baseDeclType = INT_TYPE; }
>>>>>>> 4132105... 453 3: Variable declarations in the symbol table
=======
type : CHAR     { baseDeclType = CHAR_TYPE; $$ = CHAR_TYPE; }
     | INT      { baseDeclType = INT_TYPE;  $$ = INT_TYPE; }
>>>>>>> 741c33c... 453 3: Setting current function return type
     ;

param_types : VOID
            | non_void_param_type
            | param_types_list COMMA non_void_param_type
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

<<<<<<< HEAD
<<<<<<< HEAD
optional_assign: assg                   { $$ = $1; }
               | error                  { $$ = NULL; }
               | epsilon                { $$ = NULL; }
=======
optional_assign: assg
               | error
               | epsilon
>>>>>>> aeb3970... 453 3: foundError set in yyerror, not in action
=======
optional_assign: assg                                               { $$ = $1; }
               | error                                              { $$ = NULL; }
               | epsilon                                            { $$ = NULL; }
>>>>>>> 33626bd... 453 3: Creating statement nodes in parser
               ;

<<<<<<< HEAD
optional_expr : expr                    { $$ = $1; }
              | epsilon                 { $$ = NULL; }
=======
optional_expr : expr                                                { $$ = $1; }
              | epsilon                                             { $$ = NULL; }
>>>>>>> 0f11d54... 453 3: Creating expression lists in parser
              ;

stmt_list : stmt stmt_list              { $$ = newStatementList($1, $2); }
          | epsilon                     { $$ = NULL; }
          ;

<<<<<<< HEAD
assg : ID ASSIGN expr
     | ID LEFT_SQUARE_BRACKET expr RIGHT_SQUARE_BRACKET ASSIGN expr { $$ = NULL; /* TODO*/ }
=======
assg : ID ASSIGN expr                                               { $$ = newAssignmentStatement(scope, $1, NULL, $3); }
     | ID LEFT_SQUARE_BRACKET expr RIGHT_SQUARE_BRACKET ASSIGN expr { $$ = newAssignmentStatement(scope, $1, $3, $6); }
>>>>>>> 33626bd... 453 3: Creating statement nodes in parser
     ;

expr_list : optional_expr {
            if($1) {
                List *list = newList(diffComp);
                listInsert(list, $1);

                $$ = list;
            } else {
                $$ = NULL;
            }
        }
          | expr_list COMMA expr {
            if($1) {
                List *list = $1;
                listInsert(list, $3);
                $$ = list;
            } else {
                List *list = newList(diffComp);
                listInsert(list, $3);
                $$ = list;
            }
        }

epsilon:
       ;

%%
int diffComp(void *a, void *b) {
    return -1;
}

int main(int argc, char **argv){
#ifdef DEBUG
    setDebuggingLevel(E_ALL);
#endif
    scope = newScope(NULL);
    yyparse();

    if(foundError) {
        return 1;
    } else {
        return 0;
    }
}

int yyerror() {
<<<<<<< HEAD
    foundError = true;
=======
    foundError = 1;
>>>>>>> aeb3970... 453 3: foundError set in yyerror, not in action

    if(yytext[0] == 0) {
        fprintf(stderr, "Encountered unexpected EOF while parsing \"%s\" starting on line %d.\n",
                yytext, mylineno);
    } else {
        fprintf(stderr, "%d:%d - Encountered error while parsing: \"%s\"\n", mylineno, mycolno,
                yytext);
    }

    return 1;
}

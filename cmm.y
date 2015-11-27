%{
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "symtab.h"
#include "utils.h"
#include "ast.h"
#include "globals.h"
#include "codegen.h"
#include "cmm.tab.h"

extern char *yytext;

// Parse state information
int mylineno;
int mycolno;
bool foundError = false;

// Typing and scoping
bool funcTypeSet = false;
bool declaredExtern = false;
Type currentFunctionReturnType = VOID_TYPE;
Scope *globalScope;
Scope *scope;
Type baseDeclType;

// Root of the syntax tree
FunctionDeclaration *root;

// Helper functions
Vector *parametersVector(FunctionParameter *parameters);
bool addFunctionDeclarationToScope(Type type, char *identifier, FunctionParameter *parameters);
void resetFunctionType();
void markGlobals(Scope *scope);

%}

%union {
    Expression *expression;
    Statement *statement;
    Type type;
    FunctionParameter *parameter;
    FunctionDeclaration *functionDeclaration;
    VariableDeclaration *variableDeclaration;
    StructField *structField;
    StructDeclaration *structDeclaration;
    char *string;
}

%type<expression> expr optional_expr expr_list int_expr char_expr
%type<statement> stmt assg optional_assign stmt_list
%type<type> type
%type<functionDeclaration> func prog
%type<variableDeclaration> optional_var_decl_list var_decl var_decl_list
%type<structField> field_name field_name_list struct_field struct_field_list
%type<structDeclaration> struct_declaration
%type<parameter> param_types param_types_list non_void_param_type
%type<string> func_header

/* Language Tokens */
%token WHILE FOR
%token INT CHAR VOID
%token IF ELSE
%token EXTERN RETURN STRUCT
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
%token <expression> STRINGCON
%token <string> ID INTCON CHARCON

/* If/Else Precedence fix */
%nonassoc WITHOUT_ELSE
%nonassoc ELSE

/* Operator Precedence specification */
%left OR
%left AND
%left EQ NEQ equality_op
%left GTE LTE GT LT relop
%left ADD MINUS add_sub
%left MUL DIV mul_div
%right UMINUS NOT

%%

prog : decl prog { $$ = $2; root = $$; }
     | func prog { $1->next = $2; $$ = $1; root = $$; }
     | epsilon { $$ = NULL; root = $$; }

/**************************************************************************************************
 * DECLARATIONS
 *************************************************************************************************/
decl : type var_decl_list SEMICOLON
        {
            markGlobals(scope);
            globalScope = flattenScope(scope);
            scope = newScope(globalScope); // Clear out the scope the declaration was made in (ew)
            resetFunctionType();
        }
     | type name_args_lists SEMICOLON
        {
            scope = newScope(globalScope);
            resetFunctionType();
        }
     | void name_args_lists SEMICOLON
        {
            scope = newScope(globalScope);
            resetFunctionType();
        }
     | extern type name_args_lists SEMICOLON
        {
            scope = newScope(globalScope);
            declaredExtern = false;
            resetFunctionType();
        }
     | extern void name_args_lists SEMICOLON
        {
            scope = newScope(globalScope);
            declaredExtern = false;
            resetFunctionType();
        }
     | struct_declaration
     | error SEMICOLON
     ;

name_args_lists : ID LEFT_PAREN param_types RIGHT_PAREN
                    {
                        debug(E_DEBUG, "Declaring prototype for %s with type %s\n", $1, typeName(currentFunctionReturnType));
                        Vector *params = parametersVector($3);
                        declareFunction(globalScope, currentFunctionReturnType, $1, params, declaredExtern, true);
                        scope = newScope(globalScope);
                    }
                | name_args_lists COMMA ID LEFT_PAREN param_types RIGHT_PAREN
                    {
                        debug(E_DEBUG, "Declaring prototype for %s with type %s\n", $3, typeName(currentFunctionReturnType));
                        Vector *params = parametersVector($5);
                        declareFunction(globalScope, currentFunctionReturnType, $3, params, declaredExtern, true);
                        scope = newScope(globalScope);
                    }
                ;


/**************************************************************************************************
 * FUNCTION DECLARATIONS
 *************************************************************************************************/
extern : EXTERN { declaredExtern = true; }

func_header : type ID LEFT_PAREN param_types RIGHT_PAREN
                {
                    addFunctionDeclarationToScope($1, $2, $4);
                    $$ = $2;
                }
            | void ID LEFT_PAREN param_types RIGHT_PAREN
                {
                    addFunctionDeclarationToScope(VOID_TYPE, $2, $4);
                    $$ = $2;
                }

func : func_header LEFT_CURLY_BRACKET optional_var_decl_list stmt_list RIGHT_CURLY_BRACKET
        {
            char *identifier = $1;
            ScopeElement *elem = findScopeElement(globalScope, identifier);
            if(elem->elementType == SCOPE_FUNC) {
                ScopeFunction *func = elem->function;
                FunctionDeclaration *decl = newFunction(currentFunctionReturnType, identifier,
                                                        func->parameters, $3, $4);
                decl->functionScope = scope;
                scope = newScope(globalScope);

                $$ = decl;
            } else {
                $$ = NULL;
            }
            resetFunctionType();
        }
        ;

optional_var_decl_list : type var_decl_list SEMICOLON optional_var_decl_list
                        { baseDeclType = $1; $2->next = $4; $$ = $2; }
                       | epsilon                                { $$ = NULL; }


/**************************************************************************************************
 * STATEMENTS
 *************************************************************************************************/
stmt : IF LEFT_PAREN expr RIGHT_PAREN stmt %prec WITHOUT_ELSE   { $$ = newIfStatement(scope, $3, $5); }
     | IF LEFT_PAREN expr RIGHT_PAREN stmt ELSE stmt            { $$ = newIfElseStatement(scope, $3, $5, $7); }
     | WHILE LEFT_PAREN expr RIGHT_PAREN stmt                   { $$ = newWhileStatement(scope, $3, $5); }
     | FOR LEFT_PAREN optional_assign SEMICOLON optional_expr SEMICOLON optional_assign RIGHT_PAREN stmt
        { $$ = newForStatement(scope, $3, $5, $7, $9); }
     | RETURN optional_expr SEMICOLON                           { $$ = newReturnStatement(scope, $2); }
     | assg SEMICOLON                                           { $$ = $1; }
     | ID LEFT_PAREN expr_list RIGHT_PAREN SEMICOLON
        { $$ = newFunctionCallStatement(scope, newFunctionExpression(scope, $1, $3)); }
     | LEFT_CURLY_BRACKET stmt_list RIGHT_CURLY_BRACKET         { $$ = $2; }
     | SEMICOLON                                                { $$ = NULL; }
     | error SEMICOLON                                          { $$ = NULL; }
     | error RIGHT_CURLY_BRACKET                                { $$ = NULL; }
     ;

stmt_list : stmt stmt_list
            {
                if($1) {
                    $1->next = $2;
                    $$ = $1;
                } else {
                    $$ = NULL;
                }
            }
          | epsilon                                             { $$ = NULL; }
          ;

/**************************************************************************************************
 * EXPRESSIONS
 *************************************************************************************************/
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
     | int_expr                                         { $$ = $1; }
     | char_expr                                        { $$ = $1; }
     | STRINGCON                                        { $$ = newCharArrayConstExpression(strdup(yytext)); }
     | error                                            { $$ = NULL; }
     ;

optional_expr : expr                                    { $$ = $1; }
              | epsilon                                 { $$ = NULL; }
              ;

expr_list : optional_expr                               { $$ = $1; }
          | expr_list COMMA expr                        { $3->next = $1; $$ = $3; }

int_expr: INTCON                                        { $$ = newIntConstExpression(atoi(strdup(yytext))); }
        ;

char_expr: CHARCON                                      { $$ = newCharConstExpression(strdup(yytext)); }
         ;

/**************************************************************************************************
 * VARIABLE DECLARATIONS
 *************************************************************************************************/
var_decl : ID
            {
                declareVar(scope, baseDeclType, $1, false);
                $$ = newVariable(baseDeclType, $1);
            }
         | ID LEFT_SQUARE_BRACKET int_expr RIGHT_SQUARE_BRACKET
            {
                Type arrayType = baseDeclType == INT_TYPE ? INT_ARRAY_TYPE : CHAR_ARRAY_TYPE;
                ScopeElement *elem = declareVar(scope, arrayType, $1, false);
                $$ = newVariable(arrayType, $1);
                elem->variable->size = $3->constantExpression->value->integer_value;
                elem->variable->value = NULL;
            }
         ;

var_decl_list : var_decl                        { $$ = $1; }
              | var_decl_list COMMA var_decl    { $1->next = $3; $$ = $1; }
              | epsilon                         { $$ = NULL; }

/**************************************************************************************************
 * TYPES
 *************************************************************************************************/
void : VOID
        {
            if(!funcTypeSet) {
                currentFunctionReturnType = VOID_TYPE;
                funcTypeSet = true;
            }
        }
type : CHAR
        {
            if(!funcTypeSet) {
                currentFunctionReturnType = CHAR_TYPE;
                funcTypeSet = true;
            }
            baseDeclType = CHAR_TYPE; $$ = CHAR_TYPE;
        }
     | INT
        {
            if(!funcTypeSet) {
                currentFunctionReturnType = INT_TYPE;
                funcTypeSet = true;
            }
            baseDeclType = INT_TYPE; $$ = INT_TYPE;
        }
     ;

/**************************************************************************************************
 * STRUCT DECLARATIONS
 *************************************************************************************************/
field_name : ID { $$ = newStructField($1, baseDeclType); }
           | ID LEFT_SQUARE_BRACKET int_expr RIGHT_SQUARE_BRACKET
            {
                Type arrayType = baseDeclType == INT_TYPE ? INT_ARRAY_TYPE : CHAR_ARRAY_TYPE;
                StructField *field = newStructField($1, arrayType);
                field->size = $3->constantExpression->value->integer_value;

                $$ = field;
            }

field_name_list : field_name { $$ = $1; }
                | field_name_list COMMA field_name
                {
                    StructField *old = $1;
                    StructField *new = $3;
                    new->next = old;
                    $$ = new;
                }

struct_field : type field_name_list { $$ = $2 }

struct_field_list : struct_field SEMICOLON struct_field_list
                    {
                        StructField *old = $3;
                        StructField *new = $1;
                        new->next = old;

                        $$ = new;
                    }
                  | epsilon { $$ = NULL; }

struct_declaration : STRUCT ID LEFT_CURLY_BRACKET struct_field_list RIGHT_CURLY_BRACKET SEMICOLON
                    {
                        StructDeclaration *structure = newStructDeclaration($2, $4);
                        declareStruct(globalScope, $2, structure);
                        $$ = structure;
                    }

/**************************************************************************************************
 * FUNCTION PARAMETERS
 *************************************************************************************************/
param_types : void                                              { $$ = NULL; }
            | non_void_param_type                               { $$ = $1; }
            | param_types_list COMMA non_void_param_type        { $3->next = $1; $$ = $3; }
            ;

non_void_param_type : type ID
                        {
                            declareVar(scope, baseDeclType, $2, true);
                            $$ = newFunctionParameter(baseDeclType, $2);
                        }
                    | type ID LEFT_SQUARE_BRACKET RIGHT_SQUARE_BRACKET
                        {
                            if(baseDeclType == INT_TYPE) {
                                declareVar(scope, INT_ARRAY_TYPE, $2, true);
                                $$ = newFunctionParameter(INT_ARRAY_TYPE, $2);
                            } else if(baseDeclType == CHAR_TYPE) {
                                declareVar(scope, CHAR_ARRAY_TYPE, $2, true);
                                $$ = newFunctionParameter(CHAR_ARRAY_TYPE, $2);
                            } else {
                                fprintf(stderr, "Type error, on line %d: Arrays of type %s are not supported.\n",
                                        mylineno, typeName(baseDeclType));
                                foundError = true;
                            }
                        }
                    ;

param_types_list : non_void_param_type                          { $$ = $1; }
                 | param_types_list COMMA non_void_param_type
                    {
                        $3->next = $1;
                        $$ = $3;
                    }
                 | epsilon                                      { $$ = NULL; }
                 ;

/**************************************************************************************************
 * ASSIGNMENTS
 *************************************************************************************************/
assg : ID ASSIGN expr
        {
            $$ = newAssignmentStatement(scope, $1, NULL, $3);
        }
     | ID LEFT_SQUARE_BRACKET expr RIGHT_SQUARE_BRACKET ASSIGN expr
        {
            $$ = newAssignmentStatement(scope, $1, $3, $6);
        }
     ;

optional_assign: assg                                           { $$ = $1; }
               | error                                          { $$ = NULL; }
               | epsilon                                        { $$ = NULL; }
               ;

epsilon:
       ;

%%

void markGlobals(Scope *scope) {
    Vector *vars = scope->variables;
    for(int i = 0; i < vars->size; i++) {
        ScopeElement *elem = vectorGet(vars, i);
        if(elem->elementType == SCOPE_VAR) {
            debug(E_DEBUG, "Making global variable %s\n", elem->identifier);
            elem->variable->global = true;
        }
    }
}

Vector *parametersVector(FunctionParameter *parameters) {
    Vector *params = newVector(5);
    while(parameters) {
        vectorAdd(params, parameters);
        parameters = parameters->next;
    }
    return params;
}

bool addFunctionDeclarationToScope(Type type, char *identifier, FunctionParameter *parameters) {
    Vector *params = parametersVector(parameters);
    bool success = declareFunction(globalScope, type, identifier, params, declaredExtern, false);

    // Mark the function as implemented
    if(success) {
        ScopeElement *elem = findScopeElement(globalScope, identifier);
        ScopeFunction *func = elem->function;
        func->implemented = true;
    }

    return success;
}

void resetFunctionType() {
    funcTypeSet = false;
    currentFunctionReturnType = VOID_TYPE;
}

int main(int argc, char **argv) {
#ifdef DEBUG
    FILE *debugFile = NULL;
    debugFile = fopen("debug.log", "w");
    setDebugOutputStream(debugFile);
    setDebuggingLevel(E_ALL);
#endif
    globalScope = newScope(NULL);
    scope = newScope(globalScope);
    yyparse();

    if(foundError) {
        return 1;
    } else {
        // Generate code for the syntax tree
        debug(E_DEBUG, "-------------------BEGIN CODE GEN-------------------\n");
        generateCode(root);
    }

#ifdef DEBUG
    fclose(debugFile);
#endif

    return 0;

}

int yyerror() {
    foundError = true;

    if(yytext[0] == 0) {
        fprintf(stderr, "Encountered unexpected EOF while parsing \"%s\" starting on line %d.\n",
                yytext, mylineno);
    } else {
        fprintf(stderr, "%d:%d - Encountered error while parsing: \"%s\"\n", mylineno, mycolno,
                yytext);
    }

    return 1;
}

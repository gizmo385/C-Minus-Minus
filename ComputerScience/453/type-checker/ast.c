#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "typecheck.h"
#include "symtab.h"
#include "utils.h"
#include "globals.h"

Expression *newBinaryExpression(BinaryOperation op, Expression *left, Expression *right) {
    BinaryExpression *binaryExpr = malloc(sizeof(BinaryExpression));
    binaryExpr->operation = op;
    binaryExpr->leftOperand = left;
    binaryExpr->rightOperand = right;

    Expression *expr = malloc(sizeof(Expression));
    expr->binaryExpression = binaryExpr;
    expr->type = BINARY;

    debug(E_DEBUG, "Creating binary expression with left operand types %s and %s\n",
            expressionTypeName(left), expressionTypeName(right));


    return expr;
}

Expression *newUnaryExpression(UnaryOperation op, Expression *operand) {
    UnaryExpression *unaryExpr = malloc(sizeof(UnaryExpression));
    unaryExpr->operation = op;
    unaryExpr->operand = operand;

    Expression *expr = malloc(sizeof(Expression));
    expr->unaryExpression = unaryExpr;
    expr->type = UNARY;

    debug(E_DEBUG, "Creating unary expression with operand of type %s\n",
            expressionTypeName(operand));


    return expr;
}

Expression *newVariableExpression(Scope *scope, char *identifier, Expression *arrayIndex) {
    ScopeElement *elem = findScopeElement(scope, identifier);
    VariableExpression *variableExpression = malloc(sizeof(VariableExpression));
    Expression *expr = malloc(sizeof(Expression));

    if(elem) {
        if(elem->elementType == SCOPE_VAR) {
            // Create the variable expression
            ScopeVariable *var = elem->variable;
            Type varType = var->type;
            variableExpression->type = varType;
            variableExpression->identifier = identifier;
            variableExpression->arrayIndex = arrayIndex;

            // Wrap it
            expr->variableExpression = variableExpression;
            expr->type = VARIABLE;

            debug(E_DEBUG, "Creating variable expression for ID %s\n", identifier);

        } else {
            fprintf(stderr, "ERROR: On line %d, %s is a function!\n", mylineno, identifier);
            foundError = true;
        }
    } else {
        fprintf(stderr, "ERROR: Undeclared identifier \"%s\" on line %d.\n", identifier, mylineno);
        foundError = true;
    }

    return expr;
}

Expression *newFunctionExpression(Scope *scope, char *identifier, Expression *arguments) {
    ScopeElement *elem = findScopeElement(scope, identifier);
    FunctionExpression *functionExpression = malloc(sizeof(FunctionExpression));
    Expression *expr = malloc(sizeof(Expression));

    if(elem) {
        if(elem->elementType == SCOPE_FUNC) {
            // Create the function expression
            ScopeFunction *func = elem->function;
            functionExpression->identifier = identifier;
            functionExpression->returnType = func->returnType;
            functionExpression->arguments = arguments;

            // Wrap it
            expr->functionExpression = functionExpression;
            expr->type = FUNCTION;

            debug(E_DEBUG, "Creating function expression for ID %s\n", identifier);
        } else {
            fprintf(stderr, "ERROR: On line %d, %s is not a function!\n", mylineno, identifier);
            foundError = true;
        }
    } else {
        fprintf(stderr, "ERROR: Undeclared function \"%s\" on line %d.\n", identifier, mylineno);
        foundError = true;
    }

    return expr;
}

Expression *newConstExpression(Type type, Value value) {
    ConstExpression *constExpr = malloc(sizeof(ConstExpression));
    constExpr->type = type;
    constExpr->value = value;

    Expression *expr = malloc(sizeof(Expression));
    expr->constantExpression = constExpr;
    expr->type = CONST;

    return expr;
}

Expression *newIntConstExpression(int val) {
    Value value;
    value.integer_value = val;

    debug(E_DEBUG, "Creating int const with value %d\n", val);
    return newConstExpression(INT_TYPE, value);
}

Expression *newCharConstExpression(char val) {
    Value value;
    value.char_value = val;

    debug(E_DEBUG, "Creating char const with value %c\n", val);
    return newConstExpression(CHAR_TYPE, value);
}

Expression *newCharArrayConstExpression(char val[]) {
    Value value;
    value.char_array_value = val;

    debug(E_DEBUG, "Creating char array const with value %s\n", val);
    return newConstExpression(CHAR_ARRAY_TYPE, value);
}

Expression *newIntArrayConstExpression(int val[]) {
    Value value;
    value.int_array_value = val;

    debug(E_DEBUG, "Creating int array const with value %d\n", val);
    return newConstExpression(INT_ARRAY_TYPE, value);
}


/* Constructor functions for Statements */
Statement *newForStatement(Scope *scope, AssignmentStatement *initial, Expression *condition,
        AssignmentStatement *change, Statement *body) {
    ForStatement *forStatement = malloc(sizeof(ForStatement));
    forStatement->initial = initial;
    forStatement->condition = condition;
    forStatement->change = change;
    forStatement->body = body;

    Statement *stmt = malloc(sizeof(Statement));
    stmt->stmt_for = forStatement;
    stmt->type = ST_FOR;

    typeCheckStatement(scope, stmt);
    return stmt;
}

Statement *newWhileStatement(Scope *scope, Expression *condition, Statement *body) {
    WhileStatement *whileStatement = malloc(sizeof(WhileStatement));
    whileStatement->condition = condition;
    whileStatement->body = body;

    Statement *stmt = malloc(sizeof(Statement));
    stmt->stmt_while = whileStatement;
    stmt->type = ST_WHILE;

    typeCheckStatement(scope, stmt);
    return stmt;
}

Statement *newFunctionCallStatement(Scope *scope, Expression *functionCall) {
    FunctionCallStatement *callStatement = malloc(sizeof(FunctionCallStatement));
    callStatement->functionCall = functionCall;

    Statement *stmt = malloc(sizeof(Statement));
    stmt->stmt_func = callStatement;
    stmt->type = ST_FUNC;

    typeCheckStatement(scope, stmt);
    return stmt;
}

Statement *newIfStatement(Scope *scope, Expression *condition, Statement *body) {
    IfStatement *ifStatement = malloc(sizeof(IfStatement));
    ifStatement->condition = condition;
    ifStatement->satisfied = body;

    Statement *stmt = malloc(sizeof(Statement));
    stmt->stmt_if = ifStatement;
    stmt->type = ST_IF;

    typeCheckStatement(scope, stmt);
    return stmt;
}

Statement *newIfElseStatement(Scope *scope, Expression *condition, Statement *satisfied, Statement *unsatisfied) {
    IfElseStatement *ifElseStatement = malloc(sizeof(IfElseStatement));
    ifElseStatement->condition = condition;
    ifElseStatement->satisfied = satisfied;
    ifElseStatement->unsatisfied = unsatisfied;

    Statement *stmt = malloc(sizeof(Statement));
    stmt->stmt_if_else = ifElseStatement;
    stmt->type = ST_IF_ELSE;

    typeCheckStatement(scope, stmt);
    return stmt;
}

Statement *newReturnStatement(Scope *scope, Expression *returnValue) {
    ReturnStatement *returnStatement = malloc(sizeof(ReturnStatement));
    returnStatement->returnValue = returnValue;

    Statement *stmt = malloc(sizeof(Statement));
    stmt->stmt_return = returnStatement;
    stmt->type = ST_RETURN;

    typeCheckStatement(scope, stmt);
    return stmt;
}

Statement *newAssignmentStatement(Scope *scope, char *identifier, Expression *arrayIndex, Expression *expression) {
    AssignmentStatement *assign = malloc(sizeof(AssignmentStatement));
    assign->identifier = identifier;
    assign->arrayIndex = arrayIndex;
    assign->expression = expression;

    Statement *stmt = malloc(sizeof(Statement));
    stmt->stmt_assign = assign;
    stmt->type = ST_ASSIGN;

    typeCheckStatement(scope, stmt);
    return stmt;
}

/* Constructor functions for Declarations */
VariableDeclaration *newVariable(Type type, char *identifier) {
    VariableDeclaration *varDecl = malloc(sizeof(VariableDeclaration));
    varDecl->type = type;
    varDecl->identifier = identifier;

    return varDecl;
}

FunctionParameter *newFunctionParameter(Type type, char *identifier) {
    FunctionParameter *param = malloc(sizeof(FunctionParameter));
    param->type = type;
    param->identifier = identifier;

    return param;
}

FunctionDeclaration *newFunction(Type returnType, char *functionName, List *argumentNames,
        List *argumentTypes, VariableDeclaration *declarations, Statement *body) {
    FunctionParameter *parameters = NULL;
    if(argumentNames && argumentTypes) {
        ListNode *nameNode = argumentNames->head;
        ListNode *typeNode = argumentTypes->head;

        while(nameNode && typeNode) {
            if(nameNode->data && typeNode->data) {
                Type type = *((Type *) typeNode->data);
                char *name = typeNode->data;
                if(parameters) {
                    parameters->next = newFunctionParameter(type, name);
                } else {
                    parameters = newFunctionParameter(type, name);
                }
            }

            nameNode = nameNode->next;
            typeNode = typeNode->next;
        }
    }

    FunctionDeclaration *funcDecl = malloc(sizeof(FunctionDeclaration));
    funcDecl->returnType = returnType;
    funcDecl->functionName = functionName;
    funcDecl->parameters = parameters;
    funcDecl->declarations = declarations;
    funcDecl->body = body;

    return funcDecl;
}

/* Utility Functions */
char *expressionTypeName(Expression *expression) {
    char *name = "ERROR";
    if(expression) {
        ExpressionType type = expression->type;
        switch(type) {
            case CONST: name = "Constant"; break;
            case VARIABLE: name = "Variable"; break;
            case FUNCTION: name = "Function"; break;
            case UNARY: name = "Unary"; break;
            case BINARY: name = "Binary"; break;
        }
    }
    return name;
}

char *typeName(Type type) {
    char *name = "ERROR";
    switch(type) {
        case INT_TYPE: name = "INT"; break;
        case CHAR_TYPE: name = "CHAR"; break;
        case CHAR_ARRAY_TYPE: name = "CHAR_ARRAY"; break;
        case INT_ARRAY_TYPE: name = "INT_ARRAY"; break;
        case VOID_TYPE: name = "VOID"; break;
        case BOOL_TYPE: name = "BOOL"; break;
        default: break;
    }
    return name;
}

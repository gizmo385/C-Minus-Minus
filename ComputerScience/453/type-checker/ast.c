#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "typecheck.h"
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

    typeCheckExpression(expr);

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

    typeCheckExpression(expr);

    return expr;
}

Expression *newVariableExpression(Type type, char *identifier, Expression *arrayIndex) {
    VariableExpression *variableExpression = malloc(sizeof(VariableExpression));
    variableExpression->type = type;
    variableExpression->identifier = identifier;
    variableExpression->arrayIndex = arrayIndex;

    Expression *expr = malloc(sizeof(Expression));
    expr->variableExpression = variableExpression;
    expr->type = VARIABLE;

    debug(E_DEBUG, "Creating variable expression for ID %s\n", identifier);

    typeCheckExpression(expr);

    return expr;
}

Expression *newConstExpression(Type type, Value value) {
    ConstExpression *constExpr = malloc(sizeof(ConstExpression));
    constExpr->type = type;
    constExpr->value = value;

    Expression *expr = malloc(sizeof(Expression));
    expr->constantExpression = constExpr;
    expr->type = CONST;

    typeCheckExpression(expr);

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

    debug(E_DEBUG, "Creating int const with value %d\n", val);
    return newConstExpression(CHAR_TYPE, value);
}

Expression *newCharArrayConstExpression(char val[]) {
    Value value;
    value.char_array_value = val;

    debug(E_DEBUG, "Creating int const with value %d\n", val);
    return newConstExpression(CHAR_ARRAY_TYPE, value);
}

Expression *newIntArrayConstExpression(int val[]) {
    Value value;
    value.int_array_value = val;

    debug(E_DEBUG, "Creating int const with value %d\n", val);
    return newConstExpression(INT_ARRAY_TYPE, value);
}


/* Constructor functions for Statements */
Statement *newForStatement(AssignmentStatement *initial, Expression *condition,
        AssignmentStatement *change, Statement *body) {
    ForStatement *forStatement = malloc(sizeof(ForStatement));
    forStatement->initial = initial;
    forStatement->condition = condition;
    forStatement->change = change;
    forStatement->body = body;

    Statement *stmt = malloc(sizeof(Statement));
    stmt->stmt_for = forStatement;
    stmt->type = ST_FOR;

    return stmt;
}

Statement *newWhileStatement(Expression *condition, Statement *body) {
    WhileStatement *whileStatement = malloc(sizeof(WhileStatement));
    whileStatement->condition = condition;
    whileStatement->body = body;

    Statement *stmt = malloc(sizeof(Statement));
    stmt->stmt_while = whileStatement;
    stmt->type = ST_WHILE;

    return stmt;
}

Statement *newIfStatement(Expression *condition, Statement *body) {
    IfStatement *ifStatement = malloc(sizeof(IfStatement));
    ifStatement->condition = condition;
    ifStatement->satisfied = body;

    Statement *stmt = malloc(sizeof(Statement));
    stmt->stmt_if = ifStatement;
    stmt->type = ST_IF;

    return stmt;
}

Statement *newIfElseStatement(Expression *condition, Statement *satisfied, Statement *unsatisfied) {
    IfElseStatement *ifElseStatement = malloc(sizeof(IfElseStatement));
    ifElseStatement->condition = condition;
    ifElseStatement->satisfied = satisfied;
    ifElseStatement->unsatisfied = unsatisfied;

    Statement *stmt = malloc(sizeof(Statement));
    stmt->stmt_if_else = ifElseStatement;
    stmt->type = ST_IF_ELSE;

    return stmt;
}

Statement *newReturnStatement(Expression *returnValue) {
    ReturnStatement *returnStatement = malloc(sizeof(ReturnStatement));
    returnStatement->returnValue = returnValue;

    Statement *stmt = malloc(sizeof(Statement));
    stmt->stmt_return = returnStatement;
    stmt->type = ST_RETURN;

    return stmt;
}

Statement *newAssignmentStatement(char *identifier, Expression *expression) {
    AssignmentStatement *assign = malloc(sizeof(AssignmentStatement));
    assign->identifier = identifier;
    assign->expression = expression;

    Statement *stmt = malloc(sizeof(Statement));
    stmt->stmt_assign = assign;
    stmt->type = ST_ASSIGN;

    return stmt;
}

StatementList *newStatementList(Statement *statement, StatementList *rest) {
    StatementList *stmt_list = malloc(sizeof(StatementList));
    stmt_list->statement = statement;
    stmt_list->next = rest;

    return stmt_list;
}

/* Constructor functions for Declarations */
VariableDeclaration *newVariable(Type type, char *identifier) {
    VariableDeclaration *varDecl = malloc(sizeof(VariableDeclaration));
    varDecl->type = type;
    varDecl->identifier = identifier;

    return varDecl;
}

FunctionDeclaration *newFunction(Type type, char *identifier, FunctionParameter *parameters,
        Statement *body) {
    FunctionDeclaration *funcDecl = malloc(sizeof(FunctionDeclaration));
    funcDecl->returnType = type;
    funcDecl->functionName = identifier;
    funcDecl->parameters = parameters;
    funcDecl->body = body;

    return funcDecl;
}

/* Utility Functions */
char *expressionTypeName(Expression *expression) {
    if(expression) {
        ExpressionType type = expression->type;

        if(type == CONST) {
            return "Constant";
        } else if (type == VARIABLE) {
            return "VARIABLE";
        } else if(type == UNARY) {
            return "Unary";
        } else if(type == BINARY) {
            return "Binary";
        } else {
            return "Wat";
        }
    } else {
        return "NULL";
    }
}

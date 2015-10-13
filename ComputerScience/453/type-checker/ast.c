#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "list.h"
#include "utils.h"

Expression *newBinaryExpression(BinaryOperation op, Expression *left, Expression *right) {
    BinaryExpression *binaryExpr = malloc(sizeof(BinaryExpression));
    binaryExpr->operation = op;
    binaryExpr->leftOperand = left;
    binaryExpr->rightOperand = right;

    Expression *expr = malloc(sizeof(Expression));
    expr->binaryExpression = binaryExpr;
    expr->type = BINARY;

    debug(E_DEBUG, "Creating binary expression with left operand types %s and %s\n",
            expressionTypeName(left->type), expressionTypeName(right->type));

    return expr;
}

Expression *newUnaryExpression(UnaryOperation op, Expression *operand) {
    UnaryExpression *unaryExpr = malloc(sizeof(UnaryExpression));
    unaryExpr->operation = op;
    unaryExpr->operand = operand;

    Expression *expr = malloc(sizeof(Expression));
    expr->unaryExpression = unaryExpr;
    expr->type = UNARY;

    debug(E_DEBUG, "Creating unary expression with operand of type %d\n", operand->type);

    return expr;
}

Expression *newIntConstExpression(int val) {
    Value value;
    value.integer_value = val;

    ConstExpression *constExpr = malloc(sizeof(ConstExpression));
    constExpr->value = value;
    constExpr->type = INT_TYPE;

    Expression *expr = malloc(sizeof(Expression));
    expr->constantExpression = constExpr;
    expr->type = CONST;

    debug(E_DEBUG, "Creating int const with value %d\n", val);
    return expr;
}

Expression *newCharConstExpression(char val) {
    return NULL;
}

Expression *newCharArrayConstExpression(char val[]) {
    Value value;
    value.char_array_value = val;

    ConstExpression *constExpr = malloc(sizeof(ConstExpression));
    constExpr->value = value;
    constExpr->type = CHAR_ARRAY_TYPE;

    Expression *expr = malloc(sizeof(Expression));
    expr->constantExpression = constExpr;
    expr->type = CONST;

    debug(E_DEBUG, "Creating string const with value %s\n", val);
    return expr;
}

Expression *newIntArrayConstExpression(int val[]) {
    return NULL;
}


/* Constructor functions for Statements */
Statement *newForStatement(AssignmentStatement *initial, Expression *condition,
        AssignmentStatement *change, Statement *body) {
    ForStatement *stmt = malloc(sizeof(ForStatement));
    stmt->type = ST_FOR;
    stmt->initial = initial;
    stmt->condition = condition;
    stmt->change = change;
    stmt->body = body;

    return (Statement *) stmt;
}

Statement *newWhileStatement(Expression *condition, Statement *body) {
    WhileStatement *stmt = malloc(sizeof(WhileStatement));
    stmt->type = ST_WHILE;
    stmt->condition = condition;
    stmt->body = body;

    return (Statement *) stmt;
}

Statement *newIfStatement(Expression *condition, Statement *body) {
    IfStatement *stmt = malloc(sizeof(IfStatement));
    stmt->type = ST_IF;
    stmt->condition = condition;
    stmt->satisfied = body;

    return (Statement *) stmt;
}

Statement *newIfElseStatement(Expression *condition, Statement *satisfied, Statement *unsatisfied) {
    IfElseStatement *stmt = malloc(sizeof(IfElseStatement));
    stmt->type = ST_IF_ELSE;
    stmt->condition = condition;
    stmt->satisfied = satisfied;
    stmt->unsatisfied = unsatisfied;

    return (Statement *) stmt;
}

Statement *newReturnStatment(Expression *returnValue) {
    ReturnStatement *stmt = malloc(sizeof(ReturnStatement));
    stmt->type = ST_RETURN;
    stmt->returnValue = returnValue;

    return (Statement *) stmt;
}

Statement *newAssignmentStatement(char *identifier, Expression *expression) {
    AssignmentStatement *stmt = malloc(sizeof(AssignmentStatement));
    stmt->type = ST_ASSIGN;
    stmt->identifier = identifier;
    stmt->expression = expression;

    return (Statement *) stmt;
}

Statement *newStatementList(Statement *statement) {
    StatementList *stmt_list = malloc(sizeof(StatementList));
    stmt_list->statement = statement;
    stmt_list->type = ST_LIST;
    stmt_list->next = NULL;

    return (Statement *) stmt_list;
}


/* Constructor functions for Declarations */
VariableDeclaration *newVariable(Type type, char *identifier, Expression *expression) {
    VariableDeclaration *varDecl = malloc(sizeof(VariableDeclaration));
    varDecl->type = type;
    varDecl->identifier = identifier;
    varDecl->expression = expression;

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
char *expressionTypeName(ExpressionType type) {
    if(type == CONST) {
        return "Constant";
    } else if(type == UNARY) {
        return "Unary";
    } else if(type == BINARY) {
        return "Binary";
    } else {
        return "Wat";
    }
}

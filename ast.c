#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "typecheck.h"
#include "symtab.h"
#include "utils.h"
#include "globals.h"
#include "errors.h"
#include "vector.h"

static inline Statement *newStatement() {
    Statement *statement = calloc(1, sizeof(Statement));
    statement->code = newVector(25);

    return statement;
}

static Expression *newExpression() {
    Expression *expr = calloc(1, sizeof(Expression));
    expr->type = CONST;
    expr->inferredType = UNKNOWN;
    expr->code = newVector(25);

    return expr;
}

Expression *newBinaryExpression(BinaryOperation op, Expression *left, Expression *right) {
    BinaryExpression *binaryExpr = calloc(1, sizeof(BinaryExpression));
    binaryExpr->operation = op;
    binaryExpr->leftOperand = left;
    binaryExpr->rightOperand = right;

    Expression *expr = newExpression();
    expr->binaryExpression = binaryExpr;
    expr->type = BINARY;

    debug(E_INFO, "Creating binary expression with left operand types %s and %s\n",
            expressionTypeName(left), expressionTypeName(right));

    typeCheckExpression(expr);
    return expr;
}

Expression *newUnaryExpression(UnaryOperation op, Expression *operand) {
    UnaryExpression *unaryExpr = calloc(1, sizeof(UnaryExpression));
    unaryExpr->operation = op;
    unaryExpr->operand = operand;

    Expression *expr = newExpression();
    expr->unaryExpression = unaryExpr;
    expr->type = UNARY;

    debug(E_INFO, "Creating unary expression with operand of type %s\n",
            expressionTypeName(operand));

    typeCheckExpression(expr);
    return expr;
}

Expression *newVariableExpression(Scope *scope, char *identifier, Expression *arrayIndex) {
    ScopeElement *elem = findScopeElement(scope, identifier);
    VariableExpression *variableExpression = calloc(1, sizeof(VariableExpression));
    Expression *expr = newExpression();

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

            debug(E_INFO, "Creating variable expression for ID %s\n", identifier);

        } else {
            error(VAR_AS_FUNCTION, identifier);
        }
    } else {
        error(UNDECLARED_INDENTIFIER, identifier);
        foundError = true;
    }

    typeCheckExpression(expr);
    return expr;
}

Expression *newFunctionExpression(Scope *scope, char *identifier, Expression *arguments) {
    ScopeElement *elem = findScopeElement(scope, identifier);
    Expression *expr = NULL;

    if(elem) {
        if(elem->elementType == SCOPE_FUNC) {
            FunctionExpression *functionExpression = calloc(1, sizeof(FunctionExpression));
            expr = newExpression();

            // Create the function expression
            ScopeFunction *func = elem->function;
            functionExpression->identifier = identifier;
            functionExpression->returnType = func->returnType;
            functionExpression->arguments = arguments;

            // Wrap it
            expr->functionExpression = functionExpression;
            expr->type = FUNCTION;

            debug(E_INFO, "Creating function expression for ID %s\n", identifier);
        } else {
            error(VAR_AS_FUNCTION, identifier);
        }
    } else {
        error(CALL_UNDEF_FUNCTION, identifier);
    }

    typeCheckExpression(expr);
    return expr;
}

Expression *newConstExpression(Type type, Value *value) {
    ConstExpression *constExpr = calloc(1, sizeof(ConstExpression));
    constExpr->type = type;
    constExpr->value = value;

    Expression *expr = newExpression();
    expr->constantExpression = constExpr;
    expr->type = CONST;
    expr->inferredType = type;

    return expr;
}

Expression *newIntConstExpression(int val) {
    Value *value = calloc(1, sizeof(Value));
    value->integer_value = val;

    debug(E_INFO, "Creating int const with value %d\n", val);
    return newConstExpression(INT_TYPE, value);
}

Expression *newCharConstExpression(char *val) {
    Value *value = calloc(1, sizeof(Value));
    if(val[1] == '\\') {
        if(val[2] == 0) {
            value->char_value = '\0';
        } else if(val[2] == 'n') {
            value->char_value = '\n';
        }
    } else {
        value->char_value = val[1];
    }

    debug(E_INFO, "Creating char const with value %c\n", val);
    return newConstExpression(CHAR_TYPE, value);
}

Expression *newCharArrayConstExpression(char val[]) {
    Value *value = calloc(1, sizeof(Value));
    value->char_array_value = val;

    debug(E_INFO, "Creating char array const with value %s\n", val);
    return newConstExpression(CHAR_ARRAY_TYPE, value);
}

Expression *newIntArrayConstExpression(int val[]) {
    Value *value = calloc(1, sizeof(Value));
    value->int_array_value = val;

    debug(E_INFO, "Creating int array const with value %d\n", val);
    return newConstExpression(INT_ARRAY_TYPE, value);
}


/* Constructor functions for Statements */
Statement *newForStatement(Scope *scope, Statement *initial, Expression *condition,
        Statement *change, Statement *body) {
    ForStatement *forStatement = calloc(1, sizeof(ForStatement));
    forStatement->initial = initial;
    forStatement->condition = condition;
    forStatement->change = change;
    forStatement->body = body;

    Statement *stmt = newStatement();
    stmt->stmt_for = forStatement;
    stmt->type = ST_FOR;

    typeCheckStatement(scope, stmt);
    return stmt;
}

Statement *newWhileStatement(Scope *scope, Expression *condition, Statement *body) {
    WhileStatement *whileStatement = calloc(1, sizeof(WhileStatement));
    whileStatement->condition = condition;
    whileStatement->body = body;

    Statement *stmt = newStatement();
    stmt->stmt_while = whileStatement;
    stmt->type = ST_WHILE;

    typeCheckStatement(scope, stmt);
    return stmt;
}

Statement *newFunctionCallStatement(Scope *scope, Expression *functionCall) {
    FunctionCallStatement *callStatement = calloc(1, sizeof(FunctionCallStatement));
    callStatement->functionCall = functionCall;

    Statement *stmt = newStatement();
    stmt->stmt_func = callStatement;
    stmt->type = ST_FUNC;

    typeCheckStatement(scope, stmt);
    return stmt;
}

Statement *newIfStatement(Scope *scope, Expression *condition, Statement *body) {
    IfStatement *ifStatement = calloc(1, sizeof(IfStatement));
    ifStatement->condition = condition;
    ifStatement->satisfied = body;

    Statement *stmt = newStatement();
    stmt->stmt_if = ifStatement;
    stmt->type = ST_IF;

    typeCheckStatement(scope, stmt);
    return stmt;
}

Statement *newIfElseStatement(Scope *scope, Expression *condition, Statement *satisfied, Statement *unsatisfied) {
    IfElseStatement *ifElseStatement = calloc(1, sizeof(IfElseStatement));
    ifElseStatement->condition = condition;
    ifElseStatement->satisfied = satisfied;
    ifElseStatement->unsatisfied = unsatisfied;

    Statement *stmt = newStatement();
    stmt->stmt_if_else = ifElseStatement;
    stmt->type = ST_IF_ELSE;

    typeCheckStatement(scope, stmt);
    return stmt;
}

Statement *newReturnStatement(Scope *scope, Expression *returnValue) {
    ReturnStatement *returnStatement = calloc(1, sizeof(ReturnStatement));
    returnStatement->returnValue = returnValue;

    Statement *stmt = newStatement();
    stmt->stmt_return = returnStatement;
    stmt->type = ST_RETURN;

    typeCheckStatement(scope, stmt);
    return stmt;
}

Statement *newAssignmentStatement(Scope *scope, char *identifier, Expression *arrayIndex, Expression *expression) {
    AssignmentStatement *assign = calloc(1, sizeof(AssignmentStatement));
    assign->identifier = identifier;
    assign->arrayIndex = arrayIndex;
    assign->expression = expression;

    Statement *stmt = newStatement();
    stmt->stmt_assign = assign;
    stmt->type = ST_ASSIGN;

    typeCheckStatement(scope, stmt);
    return stmt;
}

/* Constructor functions for Declarations */
VariableDeclaration *newVariable(Type type, char *identifier) {
    VariableDeclaration *varDecl = calloc(1, sizeof(VariableDeclaration));
    varDecl->type = type;
    varDecl->identifier = identifier;

    return varDecl;
}

FunctionParameter *newFunctionParameter(Type type, char *identifier) {
    FunctionParameter *param = calloc(1, sizeof(FunctionParameter));

    if(param) {
        param->type = type;
        param->identifier = identifier;
    }

    return param;
}

FunctionDeclaration *newFunction(Type returnType, char *functionName, Vector *parameters,
        VariableDeclaration *declarations, Statement *body) {

    FunctionDeclaration *funcDecl = calloc(1, sizeof(FunctionDeclaration));
    funcDecl->returnType = returnType;
    funcDecl->functionName = functionName;
    funcDecl->parameters = parameters;
    funcDecl->declarations = declarations;
    funcDecl->body = body;
    funcDecl->code = newVector(25);

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

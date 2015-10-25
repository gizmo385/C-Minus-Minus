#ifndef AST_H
#define AST_H
#include <stdbool.h>

/* Defining types */
typedef enum {
    INT_TYPE,
    CHAR_TYPE,
    CHAR_ARRAY_TYPE,
    INT_ARRAY_TYPE,
    VOID_TYPE,
    BOOL_TYPE,
    ERROR_TYPE
} Type;

/* Various types of values */
typedef union {
    int integer_value;
    bool boolean_value;
    char char_value;
    char *char_array_value;
    int *int_array_value;
} Value;

#include "symtab.h"

/* Different kinds of operations */
typedef enum { NOT_OP, NEG_OP } UnaryOperation;
typedef enum { ADD_OP, SUB_OP, MUL_OP, DIV_OP, AND_OP, OR_OP, EQ_OP, NEQ_OP, LTE_OP, GTE_OP,
    GT_OP, LT_OP } BinaryOperation;

/* Different kinds of expressions */
typedef enum { CONST, VARIABLE, FUNCTION, UNARY, BINARY } ExpressionType;
typedef struct Expression Expression;
typedef struct {
    // Constant expressions just have a value and a type
    Type type;
    Value value;
} ConstExpression;

typedef struct {
    // A unary expression has a single operand and an operator
    Type type;
    Expression *operand;
    UnaryOperation operation;
} UnaryExpression;

typedef struct {
    // A binary expression has two operands and an operator
    Expression *leftOperand;
    Expression *rightOperand;
    BinaryOperation operation;
} BinaryExpression;

typedef struct {
    char *identifier;
    Type type;
    Expression *arrayIndex;
} VariableExpression;

typedef struct {
    char *identifier;
    Type returnType;
    Expression *arguments;
} FunctionExpression;

struct Expression {
    ExpressionType type;
    union {
        VariableExpression *variableExpression;
        FunctionExpression *functionExpression;
        ConstExpression *constantExpression;
        UnaryExpression *unaryExpression;
        BinaryExpression *binaryExpression;
    };
    Expression *next;
};

/* Different kinds of statements */
typedef struct Statement Statement;

typedef struct {
    char *identifier;
    Expression *arrayIndex;
    Expression *expression;
} AssignmentStatement;

typedef struct {
    AssignmentStatement *initial;
    Expression *condition;
    AssignmentStatement *change;
    Statement *body;
} ForStatement;

typedef struct {
    Expression *condition;
    Statement *body;
} WhileStatement;

typedef struct {
    Expression *condition;
    Statement *satisfied;
} IfStatement;

typedef struct {
    Expression *condition;
    Statement *satisfied;
    Statement *unsatisfied;
} IfElseStatement;

typedef struct {
    Expression *returnValue;
} ReturnStatement;

typedef struct {
    Expression *functionCall;
} FunctionCallStatement;

/* Statement list and statements */
typedef enum { ST_FOR, ST_WHILE, ST_IF, ST_IF_ELSE, ST_RETURN, ST_FUNC, ST_ASSIGN } StatementType;
struct Statement {
    StatementType type;
    union {
        ForStatement *stmt_for;
        WhileStatement *stmt_while;
        IfStatement *stmt_if;
        IfElseStatement *stmt_if_else;
        ReturnStatement *stmt_return;
        AssignmentStatement *stmt_assign;
        FunctionCallStatement *stmt_func;
    };
    Statement *next;
};

typedef union {} Declaration;

typedef struct VariableDeclaration {
    Type type;
    char *identifier;
    struct VariableDeclaration *next;
} VariableDeclaration;

typedef struct FunctionParameter {
    Type type;
    char *identifier;
    struct FunctionParameter *next;
} FunctionParameter;

typedef struct FunctionDeclaration {
    Type returnType;
    char *functionName;
    FunctionParameter *parameters;
    VariableDeclaration *declarations;
    Statement *body;
} FunctionDeclaration;

/* Constructor functions for Expressions */
extern Expression *newBinaryExpression(BinaryOperation op, Expression *left, Expression *right);
extern Expression *newUnaryExpression(UnaryOperation op, Expression *operand);
extern Expression *newVariableExpression(Scope *scope, char *identifier, Expression *arrayIndex);
extern Expression *newFunctionExpression(Scope *scope, char *identifier, Expression *arguments);
extern Expression *newConstExpression(Type type, Value value);
extern Expression *newIntConstExpression(int val);
extern Expression *newCharConstExpression(char val);
extern Expression *newCharArrayConstExpression(char val[]);
extern Expression *newIntArrayConstExpression(int val[]);

/* Constructor functions for Statements */
extern Statement *newForStatement(Scope *scope, AssignmentStatement *initial, Expression *condition,
        AssignmentStatement *change, Statement *body);
extern Statement *newWhileStatement(Scope *scope, Expression *condition, Statement *body);
extern Statement *newIfStatement(Scope *scope, Expression *condition, Statement *body);
extern Statement *newIfElseStatement(Scope *scope, Expression *condition, Statement *satisfied,
        Statement *unsatisfied);
extern Statement *newFunctionCallStatement(Scope *scope, Expression *functionCall);
extern Statement *newReturnStatement(Scope *scope, Expression *returnValue);
extern Statement *newReturnStatement(Scope *scope, Expression *returnValue);
extern Statement *newAssignmentStatement(Scope *scope, char *identifier, Expression *arrayIndex, Expression *expression);

/* Constructor functions for Declarations */
extern FunctionParameter *newFunctionParameter(Type type, char *identifier);
extern VariableDeclaration *newVariable(Type type, char *identifier);
extern FunctionDeclaration *newFunction(Type returnType, char *functionName, List *argumentNames,
        List *argumentTypes, VariableDeclaration *declarations, Statement *body);

/* Utility functions */
extern char *expressionTypeName(Expression *expression);
extern char *typeName(Type type);

#endif

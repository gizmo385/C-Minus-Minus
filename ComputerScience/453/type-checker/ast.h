#ifndef AST_H
#define AST_H
#include <stdbool.h>

/* Defining types */
typedef enum {
    INT_TYPE,
    CHAR_TYPE,
    CHAR_ARRAY_TYPE,
    INT_ARRAY_TYPE,
    VOID_TYPE
} Type;

/* Various types of values */
typedef union {
    int integer_value;
    bool boolean_value;
    char char_value;
    char *char_array_value;
    int *int_array_value;
} Value;

/* Different kinds of operations */
typedef enum { NOT_OP, NEG_OP } UnaryOperation;
typedef enum { ADD_OP, SUB_OP, MUL_OP, DIV_OP, AND_OP, OR_OP, EQ_OP, NEQ_OP, LTE_OP, GTE_OP,
    GT_OP, LT_OP } BinaryOperation;

/* Different kinds of expressions */
typedef enum { CONST, VARIABLE, UNARY, BINARY } ExpressionType;
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

struct Expression {
    ExpressionType type;
    union {
        VariableExpression *variableExpression;
        ConstExpression *constantExpression;
        UnaryExpression *unaryExpression;
        BinaryExpression *binaryExpression;
    };
};

/* Different kinds of statements */
typedef struct Statement Statement;

typedef struct {
    char *identifier;
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

/* Statement list and statements */
typedef struct StatementList {
    Statement *statement;
    struct StatementList *next;
} StatementList;

typedef enum { ST_FOR, ST_WHILE, ST_IF, ST_IF_ELSE, ST_RETURN, ST_LIST, ST_ASSIGN } StatementType;
struct Statement {
    StatementType type;
    union {
        ForStatement *stmt_for;
        WhileStatement *stmt_while;
        IfStatement *stmt_if;
        IfElseStatement *stmt_if_else;
        ReturnStatement *stmt_return;
        AssignmentStatement *stmt_assign;
        StatementList *stmt_list;
    };
};

typedef struct VariableDeclaration {
    Type type;
    char *identifier;
} VariableDeclaration;

typedef struct FunctionParameter {
    Type type;
    char *identifier;
} FunctionParameter;

typedef struct FunctionDeclaration {
    Type returnType;
    char *functionName;
    FunctionParameter *parameters;
    Statement *body;
} FunctionDeclaration;

/* Constructor functions for Expressions */
extern Expression *newBinaryExpression(BinaryOperation op, Expression *left, Expression *right);
extern Expression *newUnaryExpression(UnaryOperation op, Expression *operand);
extern Expression *newVariableExpression(Type type, char *identifier, Expression *arrayIndex);
extern Expression *newConstExpression(Type type, Value value);
extern Expression *newIntConstExpression(int val);
extern Expression *newCharConstExpression(char val);
extern Expression *newCharArrayConstExpression(char val[]);
extern Expression *newIntArrayConstExpression(int val[]);

/* Constructor functions for Statements */
extern Statement *newForStatement(AssignmentStatement *initial, Expression *condition,
        AssignmentStatement *change, Statement *body);
extern Statement *newWhileStatement(Expression *condition, Statement *body);
extern Statement *newIfStatement(Expression *condition, Statement *body);
extern Statement *newIfElseStatement(Expression *condition, Statement *satisfied,
        Statement *unsatisfied);
extern Statement *newReturnStatement(Expression *returnValue);
extern Statement *newAssignmentStatement(char *identifier, Expression *expression);
extern StatementList *newStatementList(Statement *statement, StatementList *rest);

/* Constructor functions for Declarations */
extern VariableDeclaration *newVariable(Type type, char *identifier);
extern FunctionDeclaration *newFunction(Type type, char *identifier, FunctionParameter *parameters,
        Statement *body);

/* Utility functions */
extern char *expressionTypeName(Expression *expression);

#endif

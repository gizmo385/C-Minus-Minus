#include <stdbool.h>
#include "list.h"

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
typedef enum { CONST, UNARY, BINARY } ExpressionType;
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

typedef struct Expression {
    ExpressionType type;
    union {
        ConstExpression *constantExpression;
        BinaryExpression *binaryExpression;
        UnaryExpression *unaryExpression;
    };
} Expression;

/* Different kinds of statements */
typedef union {} Statement;
typedef enum { ST_FOR, ST_WHILE, ST_IF, ST_IF_ELSE, ST_RETURN, ST_LIST, ST_ASSIGN } StatementType;

typedef struct {
    StatementType type;
    char *identifier;
    Expression *expression;
} AssignmentStatement;

typedef struct {
    StatementType type;
    AssignmentStatement *initial;
    Expression *condition;
    AssignmentStatement *change;
    Statement *body;
} ForStatement;

typedef struct {
    StatementType type;
    Expression *condition;
    Statement *body;
} WhileStatement;

typedef struct {
    StatementType type;
    Expression *condition;
    Statement *satisfied;
} IfStatement;

typedef struct {
    StatementType type;
    Expression *condition;
    Statement *satisfied;
    Statement *unsatisfied;
} IfElseStatement;

typedef struct {
    StatementType type;
    Expression *returnValue;
} ReturnStatement;

/* Statement list and statements */
typedef struct StatementList {
    Statement *statement;
    StatementType type;
    struct StatementList *next;
} StatementList;

union Statement {
    ForStatement *stmt_for;
    WhileStatement *stmt_while;
    IfStatement *stmt_if;
    IfElseStatement *stmt_if_else;
    ReturnStatement *stmt_return;
    AssignmentStatement stmt_assign;
    StatementList *stmt_list;
};

typedef struct VariableDeclaration {
    Type type;
    char *identifier;
    Expression *expression;
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
extern Statement *newReturnStatment(Expression *returnValue);
extern Statement *newAssignmentStatement(char *identifier, Expression *expression);
extern Statement *newStatementList(Statement *statement);

/* Constructor functions for Declarations */
extern VariableDeclaration *newVariable(Type type, char *identifier, Expression *expression);
extern FunctionDeclaration *newFunction(Type type, char *identifier, FunctionParameter *parameters,
        Statement *body);

/* Utility functions */
char *expressionTypeName(ExpressionType et);

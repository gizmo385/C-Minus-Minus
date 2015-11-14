#ifndef TYPES_H
#define TYPES_H
#include <stdbool.h>
#include "vector.h"

/* Defining types */
typedef enum {
    INT_TYPE,
    CHAR_TYPE,
    CHAR_ARRAY_TYPE,
    INT_ARRAY_TYPE,
    VOID_TYPE,
    BOOL_TYPE,
    ERROR_TYPE,
    UNKNOWN
} Type;

/* Various types of values */
typedef union {
    int integer_value;
    bool boolean_value;
    char char_value;
    char *char_array_value;
    int *int_array_value;
} Value;

/* Function Parameters */
typedef struct FunctionParameter {
    Type type;
    char *identifier;
    struct FunctionParameter *next;
} FunctionParameter;

/* Defining the structure of the Scope */
typedef enum { SCOPE_VAR, SCOPE_FUNC, SCOPE_LABEL} ScopeElementType;

typedef struct {
    Type type;
    Value *value;
    int size; // Only used for array variables
    int offset; // Offset from the frame pointer
    bool global;
} ScopeVariable;

typedef struct {
    Type returnType;
    FunctionParameter *parameters;
    bool implemented;
    bool declaredExtern;
} ScopeFunction;

typedef struct ScopeElement {
    char *identifier;
    ScopeElementType elementType;
    union {
        ScopeVariable *variable;
        ScopeFunction *function;
    };
} ScopeElement;

typedef struct Scope {
    Vector *variables;
    struct Scope *enclosingScope;
} Scope;

/* Three Address Code */
typedef enum {
    /* Assignment */
    ASSG_ADD, ASSG_SUB, ASSG_MUL, ASSG_DIV, ASSG_UNARY_MINUS, ASSG_VAR, ASSG_CONST, ASSG_TO_INDEX,

    /* Jumps and Labels */
    IF_GTE, IF_LTE, IF_GT, IF_LT, IF_EQ, IF_NEQ, GOTO, LABEL,

    /* Procedures */
    ENTER, LEAVE, RETURN_FROM, CALL, RETRIEVE, PARAM,

    /* Other */
    NO_OP
} ThreeAddressOperation;

typedef struct TACInstruction TACInstruction;
struct TACInstruction {
    ThreeAddressOperation op;
    ScopeElement *dest;
    ScopeElement *src1;
    ScopeElement *src2;

    TACInstruction *next;
};

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
    Value *value;
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
    Type inferredType;

    // Code generation information
    ScopeElement *place;
    Vector *code;
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

    Vector *code;
};

/* Variable and function declarations */
typedef struct VariableDeclaration {
    Type type;
    char *identifier;
    struct VariableDeclaration *next;
} VariableDeclaration;

typedef struct FunctionDeclaration {
    Type returnType;
    char *functionName;
    FunctionParameter *parameters;
    VariableDeclaration *declarations;
    Statement *body;
    Scope *functionScope;

    Vector *code;

    struct FunctionDeclaration *next;
} FunctionDeclaration;

#endif

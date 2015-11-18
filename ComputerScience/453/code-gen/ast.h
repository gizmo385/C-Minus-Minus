#ifndef AST_H
#define AST_H
#include <stdbool.h>
#include "types.h"

/* Constructor functions for Expressions */
extern Expression *newBinaryExpression(BinaryOperation op, Expression *left, Expression *right);
extern Expression *newUnaryExpression(UnaryOperation op, Expression *operand);
extern Expression *newVariableExpression(Scope *scope, char *identifier, Expression *arrayIndex);
extern Expression *newFunctionExpression(Scope *scope, char *identifier, Expression *arguments);
extern Expression *newConstExpression(Type type, Value *value);
extern Expression *newIntConstExpression(int val);
extern Expression *newCharConstExpression(char *val);
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
extern FunctionDeclaration *newFunction(Type returnType, char *functionName, Vector *parameters,
        VariableDeclaration *declarations, Statement *body);

/* Utility functions */
extern char *expressionTypeName(Expression *expression);
extern char *typeName(Type type);

#endif

#include <stdbool.h>
#include <stdio.h>
#include "ast.h"
#include "utils.h"
#include "typecheck.h"
#include "globals.h"

static inline char *binopString(BinaryOperation op) {
    char *string = "error";
    switch(op) {
        case ADD_OP:    string = "+";   break;
        case SUB_OP:    string = "-";   break;
        case MUL_OP:    string = "*";   break;
        case DIV_OP:    string = "/";   break;
        case AND_OP:    string = "&&";  break;
        case OR_OP:     string = "||";  break;
        case EQ_OP:     string = "==";  break;
        case NEQ_OP:    string = "!=";  break;
        case LTE_OP:    string = "<=";  break;
        case GTE_OP:    string = ">=";  break;
        case GT_OP:     string = ">";   break;
        case LT_OP:     string = "<";   break;
    }
    return string;
}

static inline Type typeCheckUnaryExpression(UnaryExpression *expression) {
    UnaryOperation op = expression->operation;
    Type operandType = typeCheckExpression(expression->operand);

    if(op == NOT_OP) {
        if(typesCompatible(BOOL_TYPE, operandType)) {
            return BOOL_TYPE;
        } else {
            fprintf(stderr, "Type error, line %d: Operand for ! has type %s, should be BOOL.\n",
                    mylineno, typeName(operandType));
            foundError = true;
            return ERROR_TYPE;
        }
    } else if(op == NEG_OP) {
        if(typesCompatible(INT_TYPE, operandType)) {
            return INT_TYPE;
        } else {
            fprintf(stderr, "Type error, line %d: Operand for - has type %s, should be INT.\n",
                    mylineno, typeName(operandType));
            foundError = true;
            return ERROR_TYPE;
        }
    } else {
        return ERROR_TYPE;
    }
}

static inline Type binaryTypeCheck(BinaryOperation op, Type shouldBe, Type left, Type right) {
    if(typesCompatible(shouldBe, left)) {
        if(typesCompatible(shouldBe, right)) {
            return shouldBe;
        } else {
            fprintf(stderr, "Type error, line %d: Right operand for %s has type %s, should be %s\n",
                    mylineno, binopString(op), typeName(right), typeName(shouldBe));
        }
    } else {
            fprintf(stderr, "Type error, line %d: Left operand for %s has type %s, should be %s\n",
                    mylineno, binopString(op), typeName(left), typeName(shouldBe));
    }

    foundError = true;
    return ERROR_TYPE;
}

static inline Type typeCheckBinaryExpression(BinaryExpression *expression) {
    Type finalType = ERROR_TYPE;

    if(expression) {
        // Destructure expression
        Type leftType = typeCheckExpression(expression->leftOperand);
        Type rightType = typeCheckExpression(expression->rightOperand);
        BinaryOperation op = expression->operation;

        switch(op) {
            case ADD_OP:
            case SUB_OP:
            case MUL_OP:
            case DIV_OP:
                return binaryTypeCheck(op, INT_TYPE, leftType, rightType);
                break;
            case AND_OP:
            case OR_OP:
                return binaryTypeCheck(op, INT_TYPE, leftType, rightType);
                break;
            case EQ_OP:
            case NEQ_OP:
                // TODO: Determine valid operand types
                return BOOL_TYPE;
                break;
            case LTE_OP:
            case GTE_OP:
            case GT_OP:
            case LT_OP:
                if(binaryTypeCheck(op, INT_TYPE, leftType, rightType) != ERROR_TYPE) {
                    return BOOL_TYPE;
                } else {
                    return ERROR_TYPE;
                }
                break;
            default:
                break;
        }
    }

    return finalType;
}

static inline Type typeCheckVariableExpression(VariableExpression *expression) {
    Type finalType = ERROR_TYPE;
    if(expression) {
        Type varType = expression->type;
        if(varType == CHAR_ARRAY_TYPE || varType == INT_ARRAY_TYPE) {
            Type indexType = typeCheckExpression(expression->arrayIndex);

            if(typesCompatible(INT_TYPE, indexType)) {
                finalType = (varType == CHAR_ARRAY_TYPE) ? CHAR_TYPE : INT_TYPE;
            } else {
                fprintf(stderr, "Type error, line %d: Array index has type %s, should be INT\n",
                        mylineno, typeName(indexType));
                foundError = true;
            }
        } else {
            finalType = varType;
        }
    }
    return finalType;
}

Type typeCheckExpression(Expression *expression) {
    Type finalType = ERROR_TYPE;
    if(expression) {
        ExpressionType type = expression->type;
        switch(type) {
            case CONST:
                finalType =  expression->constantExpression->type;
                break;
            case VARIABLE:
                finalType = typeCheckVariableExpression(expression->variableExpression);
                break;
            case UNARY:
                finalType = typeCheckUnaryExpression(expression->unaryExpression);
                break;
            case BINARY:
                finalType = typeCheckBinaryExpression(expression->binaryExpression);
                break;
        }
    }

    return finalType;
}

bool typesCompatible(Type t1, Type t2) {
    return (t1 == t2 ||
            t1 == ERROR_TYPE ||
            t2 == ERROR_TYPE ||
            (t1 == CHAR_TYPE && t2 == INT_TYPE) ||
            (t2 == CHAR_TYPE && t1 == INT_TYPE));
}


Type typeCheckStatement(Statement *statement) {
    return ERROR_TYPE;
}


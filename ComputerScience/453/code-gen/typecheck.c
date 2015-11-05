#include <stdbool.h>
#include <stdio.h>
#include "ast.h"
#include "utils.h"
#include "typecheck.h"
#include "globals.h"
#include "errors.h"
#include "types.h"
#include "symtab.h"

/***************************************************************************************************
 * Utility type checking functions
 **************************************************************************************************/

/* Defining which types are compatible with each other */
bool typesCompatible(Type t1, Type t2) {
    return (t1 == t2 ||
            t1 == ERROR_TYPE ||
            t2 == ERROR_TYPE ||
            (t1 == CHAR_TYPE && t2 == INT_TYPE) ||
            (t2 == CHAR_TYPE && t1 == INT_TYPE));
}

/* Type checking binary expressions */
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

static inline Type getBinaryOperandType(BinaryOperation op) {
    switch(op) {
        case AND_OP:
        case OR_OP:
            return BOOL_TYPE;
            break;
        default:
            return INT_TYPE;
            break;
    }
}

static inline Type getBinaryReturnType(BinaryOperation op) {
    switch(op) {
        case ADD_OP:
        case SUB_OP:
        case MUL_OP:
        case DIV_OP:
            return INT_TYPE;
            break;
        default:
            return BOOL_TYPE;
            break;
    }
}

static inline Type binaryTypeCheck(BinaryOperation op, Type shouldBe, Type left, Type right) {
    if(typesCompatible(shouldBe, left)) {
        if(typesCompatible(shouldBe, right)) {
            return getBinaryReturnType(op);
        } else {
            error(BIN_OPERAND_TYPE, "Right", binopString(op), typeName(right), typeName(shouldBe));
        }
    } else {
            error(BIN_OPERAND_TYPE, "Left", binopString(op), typeName(right), typeName(shouldBe));
    }

    return ERROR_TYPE;
}

/***************************************************************************************************
 * Expression type checking
 **************************************************************************************************/

static inline Type typeCheckBinaryExpression(BinaryExpression *expression) {
    Type finalType = ERROR_TYPE;

    if(expression) {
        // Destructure expression
        Type leftType = typeCheckExpression(expression->leftOperand);
        Type rightType = typeCheckExpression(expression->rightOperand);
        BinaryOperation op = expression->operation;

        finalType = binaryTypeCheck(op, getBinaryOperandType(op), leftType, rightType);
    }

    return finalType;
}

/* Type checking unary expressions */
static inline char *unopString(UnaryOperation op) {
    char *string = "error";
    switch(op) {
        case NOT_OP: string = "!"; break;
        case NEG_OP: string = "-"; break;
    }
    return string;
}

static inline Type getUnaryType(UnaryOperation op) {
    switch(op) {
        case NOT_OP: return BOOL_TYPE; break;
        case NEG_OP: return INT_TYPE; break;
    }
}

static inline Type typeCheckUnaryExpression(UnaryExpression *expression) {
    UnaryOperation op = expression->operation;
    Type unaryOpType = getUnaryType(op);
    Type operandType = typeCheckExpression(expression->operand);

    if(typesCompatible(unaryOpType, operandType)) {
        return unaryOpType;
    } else {
        error(UNARY_OPERAND_TYPE, unopString(op), typeName(unaryOpType), typeName(operandType));
        return ERROR_TYPE;
    }
}

static inline Type typeCheckVariableExpression(VariableExpression *expression) {
    Type finalType = ERROR_TYPE;
    if(expression) {
        Type varType = expression->type;

        // Check that arrays have a valid INT expression for the index
        if(varType == CHAR_ARRAY_TYPE || varType == INT_ARRAY_TYPE) {
            if(expression->arrayIndex) {
                Type indexType = typeCheckExpression(expression->arrayIndex);

                if(typesCompatible(INT_TYPE, indexType)) {
                    // If it does, the type of the expression is the type contained in the array
                    finalType = (varType == CHAR_ARRAY_TYPE) ? CHAR_TYPE : INT_TYPE;
                } else {
                    // A non-int expression constitutes a type error
                    error(ARRAY_INDEX_TYPE, typeName(indexType));
                }
            } else {
                finalType = varType;
            }
        } else {
            if(expression->arrayIndex) {
                error(VAR_AS_ARRAY, expression->identifier);
                finalType = ERROR_TYPE;
            } else {
                finalType = varType;
            }
        }
    }
    return finalType;
}

static inline void compareArgumentTypes(char *id, FunctionParameter *parameters, Expression *suppliedArguments) {
    // Check that arguments aren't being supplied to a void function
    if(!parameters) {
        if(suppliedArguments) {
            error(ARGS_TO_VOID);
        } else {
            return;
        }
    }

    int numSupplied = 0, numExpected = 0;

    while(parameters) {
        numExpected += 1;
        Type expected = parameters->type;

        if(suppliedArguments) {
            numSupplied += 1;
            Type supplied = typeCheckExpression(suppliedArguments);

            // Check that argument types are compatible with those expected
            if(! typesCompatible(supplied, expected)) {
                error(ARG_TYPE_MISMATCH, numExpected, id, typeName(expected), typeName(supplied));
            }

            suppliedArguments = suppliedArguments->next;
        }

        parameters = parameters->next;
    }

    // Tally up remaining arguments
    while(parameters) { numExpected += 1; parameters = parameters->next; }
    while(suppliedArguments) { numSupplied += 1; suppliedArguments = suppliedArguments->next; }

    // Ensure these match
    if(numSupplied != numExpected) {
        error(NUM_ARGS_DIFFER, id, numExpected, numSupplied);
    }
}

static inline Type typeCheckFunctionCall(FunctionExpression *function) {
    Type finalType = ERROR_TYPE;
    char *identifier = function->identifier;

    ScopeElement *elem = findScopeElement(globalScope, identifier);
    if(elem) {
        if(elem->elementType == SCOPE_FUNC) {
            ScopeFunction *defn = elem->function;
            FunctionParameter *parameters = defn->parameters;
            finalType = defn->returnType;
            Expression *suppliedArguments = function->arguments;

            compareArgumentTypes(identifier, parameters, suppliedArguments);
        } else {
            error(VAR_AS_FUNCTION, identifier);
            finalType = ERROR_TYPE;
        }
    } else {
        error(CALL_UNDEF_FUNCTION, identifier);
        finalType = ERROR_TYPE;
    }

    return finalType;
}

Type typeCheckExpression(Expression *expression) {
    Type finalType = ERROR_TYPE;

    if(expression) {
        // Check if we've already determined the type
        if(expression->inferredType != UNKNOWN) {
            return expression->inferredType;
        }

        ExpressionType type = expression->type;
        switch(type) {
            case CONST:
                finalType = expression->constantExpression ?  expression->constantExpression->type : ERROR_TYPE;
                break;
            case VARIABLE:
                finalType = typeCheckVariableExpression(expression->variableExpression);
                break;
            case FUNCTION:
                finalType = typeCheckFunctionCall(expression->functionExpression);
                break;
            case UNARY:
                finalType = typeCheckUnaryExpression(expression->unaryExpression);
                break;
            case BINARY:
                finalType = typeCheckBinaryExpression(expression->binaryExpression);
                break;
        }

        expression->inferredType = finalType;
    }

    return finalType;
}

/***************************************************************************************************
 * Statement type checking
 **************************************************************************************************/

static inline void typeCheckReturnStatement(Scope *scope, ReturnStatement *stmt) {
    if(stmt) {
        Expression *returnValue = stmt->returnValue;

        if(currentFunctionReturnType == VOID_TYPE) {
            if(returnValue) {
                Type returnValueType = typeCheckExpression(returnValue);
                error(RETURN_FROM_VOID, typeName(returnValueType));
            }
        } else {
            if(returnValue) {
                Type returnValueType = typeCheckExpression(returnValue);
                if(! typesCompatible(currentFunctionReturnType, returnValueType)) {
                    error(RETURN_MISMATCH, typeName(returnValueType), typeName(currentFunctionReturnType));
                }
            }
        }
    }
}

static inline void typeCheckAssignmentStatement(Scope *scope, AssignmentStatement *stmt) {
    if(stmt) {
        char *identifier = stmt->identifier;
        Expression *expression = stmt->expression;

        ScopeElement *elem = findScopeElement(scope, identifier);
        if(elem) {
            if(elem->elementType == SCOPE_VAR) {
                ScopeVariable *var = elem->variable;
                Type varType = var->type;
                Type exprType = typeCheckExpression(expression);

                // Type checking for array assignment
                if(varType == CHAR_ARRAY_TYPE || varType == INT_ARRAY_TYPE) {
                    Expression *arrayIndex = stmt->arrayIndex;
                    if(arrayIndex) {
                        Type arrayIndexType = typeCheckExpression(arrayIndex);

                        // The array index must be an INT
                        if(! typesCompatible(INT_TYPE, arrayIndexType) ) {
                            error(ARRAY_INDEX_TYPE, typeName(arrayIndexType));
                        }

                        // The type contained in the array must be compatible with the type of the
                        // expression being assigned to the location in the array
                        Type typeContained = (varType == CHAR_ARRAY_TYPE) ? CHAR_TYPE : INT_TYPE;
                        if(! typesCompatible(exprType, typeContained)) {
                            error(VAR_TYPE_MISMATCH, typeName(exprType), typeName(typeContained));
                        }

                    } else {
                        error(ARRAY_AS_VAR, identifier);
                    }
                } else {
                    // Type checking for non-array assignment
                    if(! typesCompatible(varType, exprType)) {
                        error(VAR_TYPE_MISMATCH, typeName(exprType), typeName(varType));
                    }
                }
            } else {
                error(ASSIGN_TO_FUNC);
            }
        }
    }
}

static inline void typeCheckForStatement(Scope *scope, ForStatement *stmt) {
    if(stmt) {
        Expression *condition = stmt->condition;

        // Type check the statement if it's there
        if(stmt->condition) {
            Type conditionType = typeCheckExpression(condition);
            bool compatible = typesCompatible(BOOL_TYPE, conditionType);

            if(!compatible) {
                error(INVALID_COND, typeName(conditionType));
            }
        }

    }
}

static inline void typeCheckWhileStatement(Scope *scope, WhileStatement *stmt) {
    if(stmt) {
        Type conditionType = typeCheckExpression(stmt->condition);
        bool compatible = typesCompatible(BOOL_TYPE, conditionType);

        if(!compatible) {
            error(INVALID_COND, typeName(conditionType));
        }
    }
}

static inline void typeCheckIfStatement(Scope *scope, IfStatement *stmt) {
    if(stmt) {
        Type conditionType = typeCheckExpression(stmt->condition);
        bool compatible = typesCompatible(BOOL_TYPE, conditionType);

        if(!compatible) {
            error(INVALID_COND, typeName(conditionType));
        }
    }
}

static inline void typeCheckIfElseStatement(Scope *scope, IfElseStatement *stmt) {
    if(stmt) {
        Type conditionType = typeCheckExpression(stmt->condition);
        bool compatible = typesCompatible(BOOL_TYPE, conditionType);

        if(!compatible) {
            error(INVALID_COND, typeName(conditionType));
        }
    }
}

static inline void typeCheckFunctionCallStatement(Scope *scope, FunctionCallStatement *stmt) {
    if(stmt) {
        Expression *functionCall = stmt->functionCall;
        Type returnType = typeCheckExpression(functionCall);

        if(returnType != VOID_TYPE) {
            error(NON_VOID_STMT_FUNC, typeName(returnType));
        }
    }
}

void typeCheckStatement(Scope *scope, Statement *statement) {
    switch(statement->type) {
        case ST_FOR:
            typeCheckForStatement(scope, statement->stmt_for);
            break;
        case ST_WHILE:
            typeCheckWhileStatement(scope, statement->stmt_while);
            break;
        case ST_FUNC:
            typeCheckFunctionCallStatement(scope, statement->stmt_func);
            break;
        case ST_IF:
            typeCheckIfStatement(scope, statement->stmt_if);
            break;
        case ST_IF_ELSE:
            typeCheckIfElseStatement(scope, statement->stmt_if_else);
            break;
        case ST_RETURN:
            typeCheckReturnStatement(scope, statement->stmt_return);
            break;
        case ST_ASSIGN:
            typeCheckAssignmentStatement(scope, statement->stmt_assign);
            break;
    }
}

#include <stdbool.h>
#include <stdio.h>
#include "ast.h"
#include "utils.h"
#include "typecheck.h"
#include "globals.h"

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
        fprintf(stderr, "Type Error: On line %d, Unary operation %s expects type %s, found type %s\n",
                mylineno, unopString(op), typeName(unaryOpType), typeName(operandType));
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
                    fprintf(stderr, "Type error, line %d: Attempting to index array with %s, should be INT\n",
                            mylineno, typeName(indexType));
                    foundError = true;
                }
            } else {
                finalType = varType;
            }
        } else {
            finalType = varType;
        }
    }
    return finalType;
}

static inline bool compareArgumentTypes(char *id, List *expectedTypes, Expression *suppliedArguments) {
    bool valid = true;
    ListNode *current = expectedTypes->head;
    int numSupplied = 0, numExpected = 0;

    while(current) {
        if(current->data) {
            numExpected += 1;
            Type *typeP = current->data;
            Type expected = *typeP;

            if(suppliedArguments) {
                numSupplied += 1;
                Type supplied = typeCheckExpression(suppliedArguments);

                debug(E_DEBUG, "Comparing types %s and %s for argument %d of call to %s on line %d.\n",
                        typeName(expected), typeName(supplied), numSupplied, id, mylineno);
                if(! typesCompatible(supplied, expected)) {
                    fprintf(stderr, "Type error: On line %d, argument %d to %s expected %s, got %s.\n",
                            mylineno, numExpected, id, typeName(expected), typeName(supplied));
                    valid = false;
                }
                suppliedArguments = suppliedArguments->next;
            }
        }

        current = current->next;
    }

    // Tally up remaining arguments
    while(current) { numExpected += 1; current = current->next; }
    while(suppliedArguments) { numSupplied += 1; suppliedArguments = suppliedArguments->next; }

    if(numSupplied != numExpected) {
        fprintf(stderr, "Error: On line %d, %s expected %d arguments, was given %d.\n", mylineno,
                id, numExpected, numSupplied);
        valid = false;
    }

    return valid;
}

static inline Type typeCheckFunctionCall(FunctionExpression *function) {
    Type finalType = ERROR_TYPE;
    char *identifier = function->identifier;

    ScopeElement *elem = findScopeElement(globalScope, identifier);
    if(elem) {
        if(elem->elementType == SCOPE_FUNC) {
            ScopeFunction *defn = elem->function;
            List *argumentTypes = defn->argumentTypes;
            finalType = defn->returnType;
            Expression *suppliedArguments = function->arguments;

            if(! compareArgumentTypes(identifier, argumentTypes, suppliedArguments)) {
                foundError = true;
            }
        } else {
            fprintf(stderr, "Attempting to call variable %s as function on line %d.\n", identifier,
                    mylineno);
            foundError = true;
            finalType = ERROR_TYPE;
        }
    } else {
        fprintf(stderr, "Attempting to call undefined function %s on line %d.\n", identifier,
                mylineno);
        foundError = true;
        finalType = ERROR_TYPE;
    }

    return finalType;
}

Type typeCheckExpression(Expression *expression) {
    Type finalType = ERROR_TYPE;
    if(expression) {
        ExpressionType type = expression->type;
        switch(type) {
            case CONST:
                finalType = expression->constantExpression ?
                    expression->constantExpression->type :
                    ERROR_TYPE;
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
    }

    if(finalType == ERROR_TYPE) {
        foundError = true;
    }

    return finalType;
}

static inline bool typeCheckReturnStatement(Scope *scope, ReturnStatement *stmt) {
    bool typeChecks = true;
    if(stmt) {
        Expression *returnValue = stmt->returnValue;

        if(currentFunctionReturnType == VOID_TYPE) {
            if(returnValue) {
                Type returnValueType = typeCheckExpression(returnValue);
                fprintf(stderr, "Type error, line %d: Attempting to return %s from function declared to return VOID\n",
                        mylineno, typeName(returnValueType));
                typeChecks = false;
            }
        } else {
            if(returnValue) {
                Type returnValueType = typeCheckExpression(returnValue);
                if(! typesCompatible(currentFunctionReturnType, returnValueType)) {
                    fprintf(stderr, "Type error, line %d: Attempting to return %s from function declared to return %s\n",
                            mylineno, typeName(returnValueType),
                            typeName(currentFunctionReturnType));
                }
            }
        }
    }
    return typeChecks;
}

static inline bool typeCheckAssignmentStatement(Scope *scope, AssignmentStatement *stmt) {
    bool typeChecks = true;
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
                            fprintf(stderr, "Type error, line %d: Attempting to index into array with %s\n",
                                    mylineno, typeName(arrayIndexType));
                            typeChecks = false;
                        }

                        // The type contained in the array must be compatible with the type of the
                        // expression being assigned to the location in the array
                        Type typeContained = (varType == CHAR_ARRAY_TYPE) ? CHAR_TYPE : INT_TYPE;
                        if(! typesCompatible(exprType, typeContained)) {
                            fprintf(stderr, "Type error, line %d: Attempting to assign %s to field of type %s\n",
                                    mylineno, typeName(exprType), typeName(typeContained));
                            typeChecks = false;
                        }

                    } else {
                        fprintf(stderr, "Type error, line %d: %s is an array, requires index\n",
                                mylineno, identifier);
                        typeChecks = false;
                    }
                } else {
                    // Type checking for non-array assignment
                    if(! typesCompatible(varType, exprType)) {
                        fprintf(stderr, "Type error, line %d: Attempting to assign %s to %s.\n", mylineno,
                                typeName(exprType), typeName(varType));
                        typeChecks = false;
                    }
                }
            } else {
                fprintf(stderr, "Type error, line %d: Attempting to assign to function.\n", mylineno);
                typeChecks = false;
            }
        }
    }
    return typeChecks;
}

static inline bool typeCheckForStatement(Scope *scope, ForStatement *stmt) {
    bool typeChecks = true;
    if(stmt) {
        Expression *condition = stmt->condition;

        // Type check the statement if it's there
        if(stmt->condition) {
            Type conditionType = typeCheckExpression(condition);
            bool compatible = typesCompatible(BOOL_TYPE, conditionType);

            if(!compatible) {
                fprintf(stderr, "ERROR: On line %d, the condition in a for loop must be a boolean, not %s\n",
                        mylineno, typeName(conditionType));
                typeChecks = false;
            }
        }

    }
    return typeChecks;
}

static inline bool typeCheckWhileStatement(Scope *scope, WhileStatement *stmt) {
    bool typeChecks = true;
    if(stmt) {
        Type conditionType = typeCheckExpression(stmt->condition);
        bool compatible = typesCompatible(BOOL_TYPE, conditionType);

        if(!compatible) {
            fprintf(stderr, "ERROR: On line %d, the condition in a while loop must be a boolean, not %s\n",
                    mylineno, typeName(conditionType));
            typeChecks = false;
        }
    }
    return typeChecks;
}

static inline bool typeCheckIfStatement(Scope *scope, IfStatement *stmt) {
    bool typeChecks = true;
    if(stmt) {
        Type conditionType = typeCheckExpression(stmt->condition);
        bool compatible = typesCompatible(BOOL_TYPE, conditionType);

        if(!compatible) {
            fprintf(stderr, "ERROR: On line %d, the condition in an if statement must be a boolean, not %s\n",
                    mylineno, typeName(conditionType));
            typeChecks = false;
        }
    }
    return typeChecks;
}

static inline bool typeCheckIfElseStatement(Scope *scope, IfElseStatement *stmt) {
    bool typeChecks = true;
    if(stmt) {
        Type conditionType = typeCheckExpression(stmt->condition);
        bool compatible = typesCompatible(BOOL_TYPE, conditionType);

        if(!compatible) {
            fprintf(stderr, "ERROR: On line %d, the condition in an if/else statement must be a boolean, not %s\n",
                    mylineno, typeName(conditionType));
            typeChecks = false;
        }
    }
    return typeChecks;
}

static inline bool typeCheckFunctionCallStatement(Scope *scope, FunctionCallStatement *stmt) {
    bool typeChecks = true;
    if(stmt) {
        Expression *functionCall = stmt->functionCall;

        if(functionCall->type == FUNCTION) {
            typeChecks = typeCheckExpression(functionCall) != ERROR_TYPE;
        } else {
            fprintf(stderr, "SEVERE ERROR: FunctionCallStatement doesn't contain a function, contains %s\n",
                    expressionTypeName(functionCall));
            typeChecks = false;
        }
    }
    return typeChecks;
}

bool typeCheckStatement(Scope *scope, Statement *statement) {
    bool typeChecks = true;
    switch(statement->type) {
        case ST_FOR:
            typeChecks = typeCheckForStatement(scope, statement->stmt_for);
            break;
        case ST_WHILE:
            typeChecks = typeCheckWhileStatement(scope, statement->stmt_while);
            break;
        case ST_FUNC:
            typeCheckFunctionCallStatement(scope, statement->stmt_func);
            break;
        case ST_IF:
            typeChecks = typeCheckIfStatement(scope, statement->stmt_if);
            break;
        case ST_IF_ELSE:
            typeChecks = typeCheckIfElseStatement(scope, statement->stmt_if_else);
            break;
        case ST_RETURN:
            typeChecks = typeCheckReturnStatement(scope, statement->stmt_return);
            break;
        case ST_ASSIGN:
            typeChecks = typeCheckAssignmentStatement(scope, statement->stmt_assign);
            break;
    }

    if(!typeChecks) {
        foundError = true;
    }

    return typeChecks;
}

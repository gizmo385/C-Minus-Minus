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

        // Check that arrays have a valid INT expression for the index
        if(varType == CHAR_ARRAY_TYPE || varType == INT_ARRAY_TYPE) {
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
    }
    return finalType;
}

static inline Type typeCheckFunctionCall(FunctionExpression *function) {
    Type finalType = ERROR_TYPE;
    char *identifier = function->identifier;

    ScopeElement *elem = findScopeElement(globalScope, identifier);
    if(elem) {
        if(elem->elementType == SCOPE_FUNC) {
            ScopeFunction *defn = elem->function;
            List *argumentTypes = defn->argumentTypes;
            ListNode *current = argumentTypes->head;
            Expression *suppliedArguments = function->arguments;
            finalType = defn->returnType;

            // Check that all arguments are present and the correct type
            int expected = 0;
            int supplied = 0;

            while(current) {
                Type *typeP = current->data;
                if(!typeP) {
                    current = current->next;
                    continue;
                }
                expected += 1;
                Type expectedType = *typeP;

                if(suppliedArguments) {
                    supplied += 1;
                    Type suppliedType = typeCheckExpression(suppliedArguments);

                    if(! typesCompatible(suppliedType, expectedType)) {
                        // HOLY PERCENT FORMATTING BATMAN
                        if(supplied == expected) {
                            fprintf(stderr, "Type error, line %d: Argument %d of call to function %s expected %s, got %s\n",
                                    mylineno, supplied, identifier, typeName(expectedType),
                                    typeName(suppliedType));
                        }
                        foundError = true;
                    }
                    suppliedArguments = suppliedArguments->next;
                }
                current = current->next;
            }

            // Handle the case where too many arguments are supplied to a function
            while(suppliedArguments) {
                supplied += 1;
                suppliedArguments = suppliedArguments->next;
            }

            if(supplied != expected) {
                fprintf(stderr, "ERROR: On line %d, the function %s expected %d arguments, was given %d.\n",
                        mylineno, identifier, expected, supplied);
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

bool typesCompatible(Type t1, Type t2) {
    return (t1 == t2 ||
            t1 == ERROR_TYPE ||
            t2 == ERROR_TYPE ||
            (t1 == CHAR_TYPE && t2 == INT_TYPE) ||
            (t2 == CHAR_TYPE && t1 == INT_TYPE));
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

        typeChecks = typeChecks && (typesCompatible(BOOL_TYPE, typeCheckExpression(condition)));
    }
    return typeChecks;
}

static inline bool typeCheckWhileStatement(Scope *scope, WhileStatement *stmt) {
    bool typeChecks = true;
    if(stmt) {
        Expression *condition = stmt->condition;

        typeChecks = typesCompatible(BOOL_TYPE, typeCheckExpression(condition));
    }
    return typeChecks;
}

static inline bool typeCheckIfStatement(Scope *scope, IfStatement *stmt) {
    bool typeChecks = true;
    if(stmt) {
        Expression *condition = stmt->condition;

        typeChecks = typesCompatible(BOOL_TYPE, typeCheckExpression(condition));
    }
    return typeChecks;
}

static inline bool typeCheckIfElseStatement(Scope *scope, IfElseStatement *stmt) {
    bool typeChecks = true;
    if(stmt) {
        Expression *condition = stmt->condition;

        typeChecks = typesCompatible(BOOL_TYPE, typeCheckExpression(condition));
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
        case ST_LIST:
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


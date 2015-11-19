#include <stdlib.h>
#include <stdbool.h>
#include "utils.h"
#include "symtab.h"
#include "three.h"
#include "globals.h"
#include "ast.h"
#include "types.h"

TACInstruction *newTAC(ThreeAddressOperation op, ScopeElement *dest,
        ScopeElement *src1, ScopeElement *src2) {
    TACInstruction *instruction = malloc(sizeof(TACInstruction));
    instruction->op = op;
    instruction->dest = dest;
    instruction->src1 = src1;
    instruction->src2 = src2;
    instruction->next = NULL;

    return instruction;
}

char *constantValueString(Type type, Value *value) {
    const int LEN = 20;
    char *valueString = calloc(LEN, sizeof(char));

    switch(type) {
        case INT_TYPE:
            snprintf(valueString, LEN, "%d", value->integer_value);
            break;
        case CHAR_TYPE:
            {
                char val = value->char_value;
                if(val == '\n') {
                    snprintf(valueString, LEN, "\\n");
                } else if(val == '\0') {
                    snprintf(valueString, LEN, "\\0");
                } else {
                    snprintf(valueString, LEN, "%c", value->char_value);
                }
                break;
            }
        case CHAR_ARRAY_TYPE:
            valueString = value->char_array_value;
            break;
        default:
            valueString = "";
            break;
    }

    return valueString;
}

TACInstruction *newLabel(char *id) {
    // Create scope element
    ScopeElement *labelElement = malloc(sizeof(ScopeElement));
    labelElement->identifier = id;
    labelElement->variable = NULL;
    labelElement->elementType = SCOPE_LABEL;

    // Create the instruction
    TACInstruction *instruction = malloc(sizeof(TACInstruction));
    instruction->op = LABEL;
    instruction->dest = labelElement;
    instruction->src1 = NULL;
    instruction->src2 = NULL;

    debug(E_INFO, "%s:\n", id);

    return instruction;
}

TACInstruction *newRandomLabel() {
    // String identifier
    const int LEN = 20;
    char *id = calloc(LEN, sizeof(char));
    snprintf(id, LEN, "_L%d", labelId);
    labelId += 1;

    return newLabel(id);
}

ScopeElement *newTemporaryVariable(Scope *functionScope, Type type) {
    Value *empty = malloc(sizeof(Value));
    empty->integer_value = 0;

    ScopeVariable *scopeVariable = malloc(sizeof(ScopeVariable));
    scopeVariable->type = type;
    scopeVariable->value = empty;

    // Create its ID
    const int LEN = 20;
    char *id = calloc(LEN, sizeof(char));
    snprintf(id, LEN, "_tmp%d", temporaryId);
    temporaryId += 1;

    ScopeElement *elem = malloc(sizeof(ScopeElement));
    elem->identifier = id;
    elem->protectedIdentifier = id;
    elem->elementType = SCOPE_VAR;
    elem->variable = scopeVariable;

    // Add the temporary variable to function scope
    vectorAdd(functionScope->variables, elem);

    return elem;
}

void expressionTAC(Scope *functionScope, Expression *expression, Vector *code) {
    if(expression) {
        switch(expression->type) {
            case CONST:
                {
                    // Create a new location for the constant
                    ScopeElement *newTemp = newTemporaryVariable(functionScope, expression->inferredType);
                    expression->place = newTemp;

                    // Store the value of the constant
                    ConstExpression *e = expression->constantExpression;
                    newTemp->variable->value = e->value;

                    // Create an assignment instruction for the constant
                    TACInstruction *instruction = newTAC(ASSG_VAR, newTemp, NULL, NULL);
                    vectorAdd(code, instruction);
                    debug(E_INFO, "%s = %s\n", newTemp->identifier,
                            constantValueString(e->type, e->value));
                    break;
                }
            case VARIABLE:
                {
                    // Find the location of the expression's referenced variable
                    VariableExpression *var = expression->variableExpression;
                    ScopeElement *varLocation = findScopeElement(functionScope, var->identifier);

                    if(var->arrayIndex) {
                        Expression *arrayIndex = var->arrayIndex;
                        expressionTAC(functionScope, arrayIndex, code);
                        ScopeElement *arrayIndexLocation = arrayIndex->place;

                        // Create an area to hold the result value
                        Type elementType = var->type == INT_ARRAY_TYPE ? INT_TYPE : CHAR_TYPE;
                        ScopeElement *dest = newTemporaryVariable(functionScope, elementType);

                        // Create the instruction
                        TACInstruction *assign;
                        assign = newTAC(ASSG_TO_INDEX, dest, varLocation, arrayIndexLocation);
                        vectorAdd(code, assign);
                        expression->place = dest;

                        debug(E_INFO, "%s = %s[%s]\n", dest->identifier, varLocation->identifier,
                                arrayIndexLocation->identifier);
                    } else {
                        expression->place = varLocation;
                    }
                    break;
                }
            case FUNCTION:
                {
                    FunctionExpression *function = expression->functionExpression;
                    Expression *arguments = function->arguments;

                    ScopeElement *f = findScopeElement(globalScope, function->identifier);

                    // Handle the function parameters
                    while(arguments) {
                        expressionTAC(functionScope, arguments, code);

                        TACInstruction *param = newTAC(PARAM, arguments->place, f, NULL);
                        vectorAdd(code, param);

                        debug(E_INFO, "param %s\n", arguments->place->identifier);

                        arguments = arguments->next;
                    }

                    // Call the function
                    TACInstruction *call = newTAC(CALL, f, NULL, NULL);
                    vectorAdd(code, call);
                    debug(E_INFO, "call %s\n", f->identifier);

                    // Retrieve the return value
                    ScopeElement *result = newTemporaryVariable(functionScope, function->returnType);
                    TACInstruction *retrieveResult = newTAC(RETRIEVE, result, NULL, NULL);
                    vectorAdd(code, retrieveResult);
                    expression->place = result;
                    debug(E_INFO, "retrieve %s\n", result->identifier);
                    break;
                }
            case UNARY:
                debug(E_WARNING, "Unary operations have not yet been implemented.\n");
                break;
            case BINARY:
                {
                    BinaryExpression *binary = expression->binaryExpression;
                    Expression *leftOperand = binary->leftOperand;
                    Expression *rightOperand = binary->rightOperand;
                    expressionTAC(functionScope, leftOperand, code);
                    expressionTAC(functionScope, rightOperand, code);

                    // Determine which operation is taking place
                    ThreeAddressOperation op;
                    char *opString = "";
                    switch(binary->operation) {
                        case ADD_OP: { op = ASSG_ADD; opString = "+"; break; }
                        case SUB_OP: { op = ASSG_SUB; opString = "-"; break; }
                        case MUL_OP: { op = ASSG_MUL; opString = "*"; break; }
                        case DIV_OP: { op = ASSG_DIV; opString = "/"; break; }
                        default: { op = NO_OP; opString = "?"; break; }
                    }

                    ScopeElement *result = newTemporaryVariable(functionScope, expression->inferredType);
                    TACInstruction *instr = newTAC(op, result, leftOperand->place, rightOperand->place);
                    vectorAdd(code, instr);
                    expression->place = result;
                    debug(E_DEBUG, "%s = %s %s %s\n", result->identifier, leftOperand->place->identifier, opString,
                            rightOperand->place->identifier);
                    break;
                }
        }
    }
}

void statementTAC(Scope *functionScope, Statement *statement, Vector *code) {
    if(statement) {
        switch(statement->type) {
            case ST_FOR:
                {
                    debug(E_WARNING, "For loops: not implemented.\n");
                    break;
                }
            case ST_WHILE:
                {
                    debug(E_WARNING, "While loops: not implemented.\n");
                    break;
                }
            case ST_IF:
                {
                    debug(E_WARNING, "If: not implemented.\n");
                    break;
                }
            case ST_IF_ELSE:
                {
                    debug(E_WARNING, "If/else: not implemented.\n");
                    break;
                }
            case ST_RETURN:
                {
                    debug(E_WARNING, "Returns: not implemented.\n");
                    break;
                }
            case ST_FUNC:
                {
                    // Extract the function call
                    FunctionCallStatement *functionCall = statement->stmt_func;
                    Expression *expression = functionCall->functionCall;
                    FunctionExpression *function = expression->functionExpression;
                    Expression *arguments = function->arguments;

                    ScopeElement *f = findScopeElement(functionScope, function->identifier);

                    // Handle the function parameters
                    while(arguments) {
                        expressionTAC(functionScope, arguments, code);
                        TACInstruction *param = newTAC(PARAM, arguments->place, f, NULL);
                        vectorAdd(code, param);

                        debug(E_INFO, "param %s\n", arguments->place->identifier);

                        arguments = arguments->next;
                    }

                    // Call the function
                    TACInstruction *call = newTAC(CALL, f, NULL, NULL);
                    vectorAdd(code, call);
                    debug(E_INFO, "call %s\n", f->identifier);

                    break;
                }
            case ST_ASSIGN:
                {
                    AssignmentStatement *assignment = statement->stmt_assign;
                    ScopeElement *dest = findScopeElement(functionScope, assignment->identifier);

                    if(assignment->arrayIndex) {
                        // TODO: Handle array indices
                        debug(E_WARNING, "Array index assignment: not implemented.\n");
                    } else {
                        // Get the location for the value being assigned
                        Expression *value = assignment->expression;
                        expressionTAC(functionScope, value, code);

                        // Create a new TAC instruction that represents this assignment.
                        if(value->type == CONST) {
                            TACInstruction *instruction = newTAC(ASSG_CONST, dest, value->place, NULL);
                            vectorAdd(code, instruction);
                        } else {
                            TACInstruction *instruction = newTAC(ASSG_VAR, dest, value->place, NULL);
                            vectorAdd(code, instruction);
                        }
                        debug(E_INFO, "%s = %s\n", dest->identifier, value->place->identifier);
                    }
                    break;
                }
        }
    }
}

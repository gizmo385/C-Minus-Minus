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

static char *constantValueString(ConstExpression *expr) {
    const int LEN = 20;
    char *valueString = calloc(LEN, sizeof(char));

    Value value = expr->value;
    switch(expr->type) {
        case INT_TYPE:
            snprintf(valueString, LEN, "%d", value.integer_value);
            break;
        case CHAR_TYPE:
            snprintf(valueString, LEN, "%c", value.char_value);
            break;
        case CHAR_ARRAY_TYPE:
            valueString = value.char_array_value;
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
    Value empty;
    empty.integer_value = 0;

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
    elem->elementType = SCOPE_VAR;
    elem->variable = scopeVariable;

    // Add the temporary variable to function scope
    vectorAdd(functionScope->variables, elem);

    return elem;
}

void expressionTAC(Scope *functionScope, Expression *expression) {
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
                    vectorAdd(expression->code, instruction);
                    debug(E_INFO, "%s = %s\n", newTemp->identifier, constantValueString(e));
                    break;
                }
            case VARIABLE:
                {
                    // Find the location of the expression's referenced variable
                    VariableExpression *var = expression->variableExpression;
                    ScopeElement *varLocation = findScopeElement(functionScope, var->identifier);

                    if(var->arrayIndex) {
                        expressionTAC(functionScope, var->arrayIndex);
                        ScopeElement *arrayIndexLocation = var->arrayIndex->place;

                        // Create an area to hold the result value
                        Type elementType = var->type == INT_ARRAY_TYPE ? INT_TYPE : CHAR_TYPE;
                        ScopeElement *dest = newTemporaryVariable(functionScope, elementType);

                        // Create the instruction
                        TACInstruction *assign;
                        assign = newTAC(ASSG_TO_INDEX, dest, varLocation, arrayIndexLocation);
                        vectorAdd(expression->code, assign);
                        expression->place = dest;

                        debug(E_INFO, "%s = %s[%s]\n", dest->identifier, varLocation->identifier,
                                arrayIndexLocation->identifier);
                    } else {
                        expression->place = varLocation;
                    }
                    break;
                }
            case FUNCTION:
                debug(E_WARNING, "Function calls have not yet been implemented.\n");
                break;
            case UNARY:
                debug(E_WARNING, "Unary operations have not yet been implemented.\n");
                break;
            case BINARY:
                debug(E_WARNING, "Binary operations have not yet been implemented.\n");
                break;
        }
    }
}

void statementTAC(Scope *functionScope, Statement *statement) {
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
                    debug(E_WARNING, "Function call statement: not implemented.\n");
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
                        expressionTAC(functionScope, value);

                        // Create a new TAC instruction that represents this assignment.
                        TACInstruction *instruction = newTAC(ASSG_VAR, dest, value->place, NULL);
                        vectorAdd(statement->code, instruction);
                        debug(E_INFO, "%s = %s\n", dest->identifier, value->place->identifier);
                    }
                    break;
                }
        }
    }
}

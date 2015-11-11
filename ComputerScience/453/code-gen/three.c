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

static void consExpressionCode(Expression *left, Expression *right) {
    TACInstruction *tail = left->codeEnd;

    if(tail) {
        tail->next = right->codeStart;
    } else {
        tail = right->codeStart;
    }

    left->codeEnd = right->codeEnd;
}

static void consInstruction(Expression *expression, TACInstruction *newInstruction) {
    TACInstruction *tail = expression->codeEnd;
    if(tail) {
        tail->next = newInstruction;
    } else {
        tail = newInstruction;
    }

    expression->codeEnd = newInstruction;
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

    debug(E_DEBUG, "%s:\n", id);

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
                    consInstruction(expression, instruction);
                    debug(E_DEBUG, "Declaring constant in %s.\n", newTemp->identifier);
                    break;
                }
            case VARIABLE:
                {
                    // Find the location of the expression's referenced variable
                    VariableExpression *var = expression->variableExpression;
                    ScopeElement *varLocation = findScopeElement(functionScope, var->identifier);
                    expression->place = varLocation;
                    ScopeElement *arrayIndexLocation = NULL;

                    // Generate code for the array index if one is present
                    if(var->arrayIndex) {
                        expressionTAC(functionScope, var->arrayIndex);
                        arrayIndexLocation = var->arrayIndex->place;

                        // Create an area to hold the result value
                        Type elementType = var->type == INT_ARRAY_TYPE ? INT_TYPE : CHAR_TYPE;
                        ScopeElement *dest = newTemporaryVariable(functionScope, elementType);

                        // Create the instruction
                        TACInstruction *assign;
                        assign = newTAC(ASSG_TO_INDEX, dest, expression->place, arrayIndexLocation);
                        expression->codeStart = assign;
                        expression->codeEnd = assign;
                    } else {

                    }

                    break;
                }
            case FUNCTION:
                debug(E_WARNING, "This has not yet been implemented.\n");
                break;
            case UNARY:
                debug(E_WARNING, "This has not yet been implemented.\n");
                break;
            case BINARY:
                debug(E_WARNING, "This has not yet been implemented.\n");
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
                        statement->codeStart = instruction;
                        statement->codeEnd = instruction;
                        debug(E_DEBUG, "%s = %s\n", dest->identifier, value->place->identifier);
                    }
                    break;
                }
        }
    }
}

#include <stdlib.h>
#include <stdbool.h>
#include "utils.h"
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

    return instruction;
}

ScopeElement *newTemporaryVariable(Type type) {
    Value empty;
    empty.integer_value = 0;

    ScopeVariable *scopeVariable = malloc(sizeof(ScopeVariable));
    scopeVariable->type = type;

    // Create its ID
    const int LEN = 20;
    char id[LEN];
    snprintf(id, LEN, "_tmp%d", temporaryId);

    ScopeElement *elem = malloc(sizeof(ScopeElement));
    elem->identifier = id;
    elem->elementType = SCOPE_VAR;
    elem->variable = scopeVariable;

    return elem;
}

void expressionTAC(Vector *code, Expression *expression) {
    if(expression) {
        switch(expression->type) {
            case CONST:
                {
                    // Declare a value to place the constant in
                    ScopeElement *newTemp = newTemporaryVariable(expression->inferredType);
                    expression->place = newTemp;
                    TACInstruction *instruction = newTAC(ASSG_VAR, newTemp, NULL, NULL);
                    vectorAdd(code, instruction);
                    break;
                }
            case VARIABLE:
                debug(E_WARNING, "This has not yet been implemented.\n");
                break;
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

void statementTAC(Vector *code, Statement *statement) {
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
                    if(assignment->arrayIndex) {
                        // TODO: Handle array indices
                        debug(E_WARNING, "Array index assignment: not implemented.\n");
                    } else {
                        // Get the location for the value being assigned
                        Expression *value = assignment->expression;
                        expressionTAC(code, value);

                        // Create a new temporary variable that we can assign it to
                        ScopeElement *newTemp = newTemporaryVariable(value->inferredType);

                        // Create a new TAC instruction that represents this assignment.
                        TACInstruction *instruction = newTAC(ASSG_VAR, newTemp, value->place, NULL);
                        vectorAdd(code, instruction);
                    }
                    break;
                }
        }
    }
}

#include <stdlib.h>
#include <stdbool.h>
#include "utils.h"
#include "three.h"
#include "globals.h"
#include "ast.h"
#include "types.h"

ThreeAddressInstruction *newTAC(ThreeAddressOperation op, char *dest, TACValue src1,
        TACValueType src1Type, TACValue src2, TACValueType src2Type) {
    ThreeAddressInstruction *instruction = malloc(sizeof(ThreeAddressInstruction));
    instruction->op = op;
    instruction->dest = dest;
    instruction->src1 = src1;
    instruction->src2 = src2;
    instruction->src1Type = src1Type;
    instruction->src2Type = src2Type;

    return instruction;
}

ScopeElement *newTemporaryVariable(Type type) {
    Value empty;
    empty.integer_value = 0;

    ScopeVariable *scopeVariable = malloc(sizeof(ScopeVariable));
    scopeVariable->type = type;
    scopeVariable->value = empty;

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
                // TODO: Create a field in the expressions to mark its location
                break;
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
                        expressionTAC(code, assignment->expression);
                        // TODO: How do I know where the expression was stored?
                        debug(E_WARNING, "Storing assignment: not implemented.\n");
                    }
                    break;
                }
        }
    }
}

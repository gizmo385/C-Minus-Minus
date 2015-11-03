#include <stdlib.h>
#include <stdbool.h>
#include "utils.h"
#include "three.h"
#include "ast.h"

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

void expressionTAC(Vector *code, Expression *expression) {
    if(expression) {
        switch(expression->type) {
            case CONST:
                debug(E_WARNING, "This has not yet been implemented.\n");
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
                debug(E_WARNING,  "This has not yet been implemented.\n");
                break;
            case ST_WHILE:
                debug(E_WARNING, "This has not yet been implemented.\n");
                break;
            case ST_IF:
                debug(E_WARNING, "This has not yet been implemented.\n");
                break;
            case ST_IF_ELSE:
                debug(E_WARNING, "This has not yet been implemented.\n");
                break;
            case ST_RETURN:
                debug(E_WARNING, "This has not yet been implemented.\n");
                break;
            case ST_FUNC:
                debug(E_WARNING, "This has not yet been implemented.\n");
                break;
            case ST_ASSIGN:
                debug(E_WARNING, "This has not yet been implemented.\n");
                break;
        }
    }
}

#ifndef THREE_H
#define THREE_H
#include <stdbool.h>
#include "vector.h"
#include "ast.h"

static int temporaryId = 0;

typedef enum {
    /* Assignment */
    ASSG_ADD, ASSG_SUB, ASSG_MUL, ASSG_DIV, ASSG_UNARY_MINUS, ASSG_ADDR, ASSG_DEREF, ASSG_VAR,
    ASSG_FROM_INDEX, ASSG_TO_INDEX,

    /* Jumps and Labels */
    IF_GTE, IF_LTE, IF_GT, IF_LT, IF_EQ, IF_NEQ, GOTO, LABEL,

    /* Procedures */
    ENTER, LEAVE, RETVAL, RETURN, CALL, PARAM,

    /* Other */
    NO_OP
} ThreeAddressOperation;

typedef union {
    char *tac_id;
    int tac_int;
    char tac_char;
} TACValue;

typedef enum {
    TAC_IDENTIFIER, TAC_INTEGER, TAC_CHARACTER
} TACValueType;

typedef struct {
    ThreeAddressOperation op;
    char *dest;
    TACValue src1;
    TACValue src2;
    TACValueType src1Type;
    TACValueType src2Type;
} ThreeAddressInstruction;

extern ScopeElement *newTemporaryVariable(Type t);
extern void expressionTAC(Vector *code, Expression *expression);
extern void statementTAC(Vector *code, Statement *statement);
extern ThreeAddressInstruction *newTAC(ThreeAddressOperation op, char *dest, TACValue src1,
        TACValueType src1Type, TACValue src2, TACValueType src2Type);

#endif

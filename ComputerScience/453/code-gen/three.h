#ifndef THREE_H
#define THREE_H
#include <stdbool.h>
#include "vector.h"
#include "ast.h"
#include "types.h"

static int temporaryId = 0;

extern ScopeElement *newTemporaryVariable(Type t);
extern void expressionTAC(Vector *code, Expression *expression);
extern void statementTAC(Vector *code, Statement *statement);
extern ThreeAddressInstruction *newTAC(ThreeAddressOperation op, char *dest, TACValue src1,
        TACValueType src1Type, TACValue src2, TACValueType src2Type);

#endif

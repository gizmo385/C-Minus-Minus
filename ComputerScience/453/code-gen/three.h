#ifndef THREE_H
#define THREE_H
#include <stdbool.h>
#include "vector.h"
#include "ast.h"
#include "types.h"

static int temporaryId = 0;

extern ScopeElement *newTemporaryVariable(Type type);
extern void expressionTAC(Vector *code, Expression *expression);
extern void statementTAC(Vector *code, Statement *statement);
TACInstruction *newTAC(ThreeAddressOperation op, ScopeElement *dest, ScopeElement *src1,
        ScopeElement *src2);

#endif

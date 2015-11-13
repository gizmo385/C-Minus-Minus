#ifndef THREE_H
#define THREE_H
#include <stdbool.h>
#include "ast.h"
#include "types.h"

static int temporaryId = 0;
static int labelId = 0;

extern char *constantValueString(Type type, Value *value);
extern ScopeElement *newTemporaryVariable(Scope *functionScope, Type type);
extern TACInstruction *newLabel(char *id);
extern TACInstruction *newRandomLabel();
extern void expressionTAC(Scope *functionScope, Expression *expression);
extern void statementTAC(Scope *functionScope, Statement *statement);
extern TACInstruction *newTAC(ThreeAddressOperation op, ScopeElement *dest, ScopeElement *src1,
        ScopeElement *src2);

#endif

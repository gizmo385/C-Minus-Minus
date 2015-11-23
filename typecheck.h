#ifndef TYPECHECK_H
#define TYPECHECK_H
#include <stdbool.h>
#include "types.h"

/* Type checking functions */
extern Type typeCheckExpression(Expression *expression);
extern void typeCheckStatement(Scope *scope, Statement *statement);
extern bool typesCompatible(Type t1, Type t2);

#endif

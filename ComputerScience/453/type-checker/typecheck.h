#ifndef TYPECHECK_H
#define TYPECHECK_H
#include <stdbool.h>
#include "ast.h"

/* Type checking functions */
extern Type typeCheckExpression(Expression *expression);
extern Type typeCheckStatement(Statement *statement);
extern bool typesCompatible(Type t1, Type t2);

#endif

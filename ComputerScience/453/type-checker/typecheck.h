#ifndef TYPECHECK_H
#define TYPECHECK_H
#include <stdbool.h>
#include "ast.h"

/* Type checking functions */
extern Type typeCheckExpression(Expression *expression);
extern Type typeCheckStatement(Statement *statement);

#endif

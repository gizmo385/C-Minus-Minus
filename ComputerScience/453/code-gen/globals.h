#ifndef CMM_ERRORS_H
#define CMM_ERRORS_H
#include <stdbool.h>
#include "ast.h"
#include "symtab.h"

extern int mylineno;
extern int mycolno;
extern bool foundError;

extern Type currentFunctionReturnType;
extern Scope *globalScope;
extern Scope *scope;

#endif

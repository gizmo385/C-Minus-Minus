#ifndef SYMTAB_H
#define SYMTAB_H
#include <stdbool.h>
#include "types.h"

/* Creating and moving between scopes */
extern Scope *newScope(Scope *enclosingScope);
extern Scope *flattenScope(Scope *scope);
extern Scope *stripScope(Scope *scope);
extern ScopeElement *findScopeElement(Scope *scope, char *identifier);

/* Declaring new variables and functions inside of a scope */
extern ScopeElement* declareVar(Scope *scope, Type type, char *identifier, bool parameter);
extern bool declareFunction(Scope *scope, Type returnType, char *identifier, Vector *parameters,
        bool declaredExtern, bool isPrototype);

#endif

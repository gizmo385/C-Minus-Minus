#ifndef SYMTAB_H
#define SYMTAB_H
#include <stdbool.h>
#include "list.h"

typedef struct ScopeVariable ScopeVariable;
typedef struct Scope Scope;

#include "ast.h"

struct ScopeVariable {
    char *identifier;
    Type type;
    Value value;
};

struct Scope {
    List *variables;
    Scope *enclosingScope;
};

/* Creating and moving between scopes */
extern Scope *newScope(Scope *enclosingScope);
extern Scope *stripScope(Scope *scope);
extern ScopeVariable *findScopeVariable(Scope *scope, char *identifier);

/* Declaring new variables inside of a scope */
extern bool declareVar(Scope *scope, Type type, char *identifier);
extern void declareIntVariable(Scope *scope, char* identifier, int val);
extern void declareCharVariable(Scope *scope, char* identifier, char val);
extern void declareCharArrayVariable(Scope *scope, char* identifier, char val[]);
extern void declareIntArrayVariable(Scope *scope, char* identifier, int val[]);

#endif

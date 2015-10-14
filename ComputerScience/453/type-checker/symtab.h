#ifndef SYMTAB_H
#define SYMTAB_H

#include "ast.h"
#include "list.h"

typedef struct {
    char *identifier;
    Type type;
    Value value;
} ScopeVariable;

typedef struct Scope Scope;
struct Scope {
    List *variables;
    Scope *enclosingScope;
};

/* Creating and moving between scopes */
extern Scope *newScope(Scope *enclosingScope);
extern Scope *stripScope(Scope *scope);
extern ScopeVariable *findScopeVariable(Scope *scope, char *identifier);
extern Value *findScopeValue(Scope *scope, char *identifier);

/* Declaring new variables inside of a scope */
extern void declareIntVariable(Scope *scope, char* identifier, int val);
extern void declareCharVariable(Scope *scope, char* identifier, char val);
extern void declareCharArrayVariable(Scope *scope, char* identifier, char val[]);
extern void declareIntArrayVariable(Scope *scope, char* identifier, int val[]);

#endif
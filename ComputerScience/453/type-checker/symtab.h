#ifndef SYMTAB_H
#define SYMTAB_H
#include <stdbool.h>
#include "list.h"

typedef struct ScopeElement ScopeElement;
typedef struct Scope Scope;
typedef enum { SCOPE_VAR, SCOPE_FUNC } ScopeElementType;

#include "ast.h"

typedef struct {
    Type type;
    Value value;
} ScopeVariable;

typedef struct {
    Type returnType;
    List *argumentNames;
    List *argumentTypes;
    bool implemented;
    bool declaredExtern;
} ScopeFunction;

struct ScopeElement {
    char *identifier;
    ScopeElementType elementType;
    union {
        ScopeVariable *variable;
        ScopeFunction *function;
    };
};

struct Scope {
    List *variables;
    Scope *enclosingScope;
};

/* Creating and moving between scopes */
extern Scope *newScope(Scope *enclosingScope);
extern Scope *flattenScope(Scope *scope);
extern Scope *stripScope(Scope *scope);
extern ScopeElement *findScopeElement(Scope *scope, char *identifier);

/* Declaring new variables and functions inside of a scope */
extern void declareVar(Scope *scope, Type type, char *identifier);
extern bool declareFunction(Scope *scope, Type returnType, char *identifier, List *argumentNames,
        List *argumentTypes, bool declaredExtern, bool isPrototype);

#endif

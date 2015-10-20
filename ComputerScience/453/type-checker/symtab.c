#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "symtab.h"
#include "utils.h"

static inline int compareScopeVariables(ScopeVariable *a, ScopeVariable *b) {
    if(a && b) {
        return strcmp(a->identifier, b->identifier);
    } else {
        return -1;
    }
}

Scope *newScope(Scope *enclosingScope) {
    Scope *scope = malloc(sizeof(Scope));
    scope->enclosingScope = enclosingScope;
    scope->variables = newList( (ComparisonFunction) compareScopeVariables );

    return scope;
}

Scope *stripScope(Scope *scope) {
    if(scope) {
        return scope->enclosingScope;
    } else {
        return NULL;
    }
}

ScopeVariable *findScopeVariable(Scope *scope, char *identifier) {
    if(scope) {
        List *variables = scope->variables;
        ListNode *current = variables->head;

        // Check all the variables in this level of scope
        while( current->next != NULL ) {
            ScopeVariable *variable = current->data;

            if(variable && strcmp(variable->identifier, identifier) == 0) {
                return variable;
            }

            current = current->next;
        }

        // Check the last variable in the scope list
        ScopeVariable *variable = current->data;
        if(variable && strcmp(variable->identifier, identifier) == 0) {
            return variable;
        } else {
            // If it wasn't found, check the next highest scope
            return findScopeVariable(scope->enclosingScope, identifier);
        }
    } else {
        return NULL;
    }
}

static inline ScopeVariable *newScopeVariable(Type type, char *identifier, Value value) {
    ScopeVariable *scopeVariable = malloc(sizeof(ScopeVariable));
    scopeVariable->type = type;
    scopeVariable->identifier = identifier;
    scopeVariable->value = value;

    return scopeVariable;
}

static inline void addVarToScope(Scope *scope, ScopeVariable *var) {
    ListNode *node = listFind(scope->variables, var);

    if(node) {
        node->data = var;
    } else {
        listInsert(scope->variables, var);
    }
}

bool declareVar(Scope *scope, Type type, char *identifier) {
    Value empty;
    empty.integer_value = 0;
    ScopeVariable *foundVar = findScopeVariable(scope, identifier);

    if(foundVar) {
        return false;
    } else {
        debug(E_DEBUG, "Declaring undeclared variable \"%s\"\n", identifier);
        ScopeVariable *var = newScopeVariable(type, identifier, empty);
        listInsert(scope->variables, var);
        return true;
    }
}

void declareIntVariable(Scope *scope, char *identifier, int val) {
    Value value;
    value.integer_value = val;
    ScopeVariable *var = newScopeVariable(INT_TYPE, identifier, value);

    addVarToScope(scope, var);
}

void declareCharVariable(Scope *scope, char *identifier, char val) {
    Value value;
    value.char_value = val;
    ScopeVariable *var = newScopeVariable(CHAR_TYPE, identifier, value);

    addVarToScope(scope, var);
}

void declareCharArrayVariable(Scope *scope, char *identifier, char val[]) {
    Value value;
    value.char_array_value = val;
    ScopeVariable *var = newScopeVariable(CHAR_ARRAY_TYPE, identifier, value);

    addVarToScope(scope, var);
}

void declareIntArrayVariable(Scope *scope, char *identifier, int val[]) {
    Value value;
    value.int_array_value = val;
    ScopeVariable *var = newScopeVariable(INT_ARRAY_TYPE, identifier, value);

    addVarToScope(scope, var);
}

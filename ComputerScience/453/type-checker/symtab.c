#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "symtab.h"
#include "globals.h"
#include "utils.h"

static inline int compareScopeElements(ScopeElement *a, ScopeElement *b) {
    if(a && b) {
        return strcmp(a->identifier, b->identifier);
    } else {
        return -1;
    }
}

Scope *newScope(Scope *enclosingScope) {
    Scope *scope = malloc(sizeof(Scope));
    scope->enclosingScope = enclosingScope;
    scope->variables = newList( (ComparisonFunction) compareScopeElements );

    return scope;
}

Scope *stripScope(Scope *scope) {
    if(scope) {
        return scope->enclosingScope;
    } else {
        return NULL;
    }
}

ScopeElement *findScopeElement(Scope *scope, char *identifier) {
    if(scope) {
        List *variables = scope->variables;
        ListNode *current = variables->head;

        // Check all the variables in this level of scope
        while( current->next != NULL ) {
            ScopeElement *element = current->data;

            if(element && strcmp(element->identifier, identifier) == 0) {
                return element;
            }

            current = current->next;
        }

        // Check the last element in the scope list
        ScopeElement *element = current->data;
        if(element && strcmp(element->identifier, identifier) == 0) {
            return element;
        } else {
            // If it wasn't found, check the next highest scope
            return findScopeElement(scope->enclosingScope, identifier);
        }
    } else {
        return NULL;
    }
}

void declareVar(Scope *scope, Type type, char *identifier) {
    Value empty;
    empty.integer_value = 0;
    ScopeElement *foundVar = findScopeElement(scope, identifier);

    if(foundVar) {
        fprintf(stderr, "ERROR: On line %d, %s has already been declared!\n", mylineno, identifier);
        foundError = true;
    } else {
        debug(E_DEBUG, "Declaring undeclared variable \"%s\"\n", identifier);
        ScopeVariable *scopeVariable = malloc(sizeof(ScopeVariable));
        scopeVariable->type = type;
        scopeVariable->value = empty;

        ScopeElement *elem = malloc(sizeof(ScopeElement));
        elem->identifier = identifier;
        elem->elementType = SCOPE_VAR;
        elem->variable = scopeVariable;
        listInsert(scope->variables, elem);
    }
}

bool declareFunction(Scope *scope, Type returnType, char *identifier, List *argumentNames,
        List *argumentTypes) {
    Value empty;
    empty.integer_value = 0;
    ScopeElement *foundVar = findScopeElement(scope, identifier);

    if(foundVar) {
        return false;
    } else {
        debug(E_DEBUG, "Declaring undeclared function \"%s\"\n", identifier);
        ScopeFunction *scopeFunction = malloc(sizeof(ScopeFunction));
        scopeFunction->returnType = returnType;
        scopeFunction->argumentNames = argumentNames;
        scopeFunction->argumentTypes = argumentTypes;

        ScopeElement *elem = malloc(sizeof(ScopeElement));
        elem->identifier = identifier;
        elem->elementType = SCOPE_FUNC;
        elem->function = scopeFunction;
        listInsert(scope->variables, elem);

        return true;
    }
}

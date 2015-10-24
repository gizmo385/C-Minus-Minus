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

static inline ScopeElement *inLocalScope(Scope *scope, char *identifier) {
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
        }
    }

    return NULL;
}


Scope *newScope(Scope *enclosingScope) {
    debug(E_DEBUG, "Creating new scope\n");
    Scope *scope = malloc(sizeof(Scope));
    scope->enclosingScope = enclosingScope;
    scope->variables = newList( (ComparisonFunction) compareScopeElements );

    return scope;
}

Scope *flattenScope(Scope *scope) {
    Scope *destinationScope = newScope(NULL);

    // Traverse every level of scope
    while(scope) {
        List *vars = scope->variables;
        ListNode *current = vars->head;

        // Visit every element in scope
        while(current->data) {
            ScopeElement *elem = current->data;
            char *identifier = elem->identifier;

            // If it hasn't already been declared in the flattened scope, "declare it"
            if(inLocalScope(destinationScope, identifier)) {
                fprintf(stderr, "ERROR: Redeclaration of global variable %s on line %d\n",
                        identifier, mylineno);
                foundError = true;
            } else {
                listInsert(destinationScope->variables, elem);
            }
            current = current->next;
        }

        scope = scope->enclosingScope;
    }

    return destinationScope;
}

Scope *stripScope(Scope *scope) {
    if(scope) {
        return scope->enclosingScope;
    } else {
        return NULL;
    }
}

ScopeElement *findScopeElement(Scope *scope, char *identifier) {
    ScopeElement *elem = NULL;

    while(scope) {
        elem = inLocalScope(scope, identifier);

        if(elem) {
            return elem;
        } else {
            scope = scope->enclosingScope;
        }
    }

    return elem;
}

void declareVar(Scope *scope, Type type, char *identifier) {
    Value empty;
    empty.integer_value = 0;
    ScopeElement *foundVar = inLocalScope(scope, identifier);

    if(foundVar) {
        fprintf(stderr, "ERROR: On line %d, %s has already been declared!\n", mylineno, identifier);
        foundError = true;
    } else {
        debug(E_DEBUG, "Declaring undeclared variable \"%s\" with type %s\n", identifier, typeName(type));
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
        List *argumentTypes, bool declaredExtern) {
    Value empty;
    empty.integer_value = 0;
    ScopeElement *foundVar = findScopeElement(scope, identifier);

    // Determine if something with that name already exists
    if(foundVar) {
        // If that thing is a function, check some properties
        if(foundVar->elementType == SCOPE_FUNC) {
            ScopeFunction *func = foundVar->function;

            // Determine if the function has been implemented or declared as extern
            if(func->implemented) {
                // An already implemented function cannot be reimplemented
                fprintf(stderr, "ERROR: Attempting to redefine function %s on line %d.\n",
                        identifier, mylineno);
                foundError = true;
                return false;
            } else if(func->declaredExtern) {
                // An extern function cannot be declared in the same file
                fprintf(stderr, "ERROR: Attempting to define function %s declared as extern on line %d.\n",
                        identifier, mylineno);
                foundError = true;
                return false;
            } else {
                func->implemented = true;
                return true;
            }
        } else {
            // If it's a variable, then a variable can't be defined as a scope
            fprintf(stderr, "ERROR: Attempting to redefine variable %s as function on line %d.\n",
                    identifier, mylineno);
            foundError = true;
            return false;
        }
    } else {
        debug(E_DEBUG, "Declaring function %s with arguments: ", identifier);
        if(argumentTypes) {
            ListNode *current = argumentTypes->head;
            while(current) {
                if(current->data) {
                    Type *typeP = current->data;
                    debug(E_DEBUG, "%s ", typeName(*typeP));
                }
                current = current->next;
            }
        }
        debug(E_DEBUG, "\n");
        ScopeFunction *scopeFunction = malloc(sizeof(ScopeFunction));
        scopeFunction->returnType = returnType;
        scopeFunction->argumentNames = argumentNames;
        scopeFunction->argumentTypes = argumentTypes;
        scopeFunction->implemented = false;
        scopeFunction->declaredExtern = declaredExtern;

        ScopeElement *elem = malloc(sizeof(ScopeElement));
        elem->identifier = identifier;
        elem->elementType = SCOPE_FUNC;
        elem->function = scopeFunction;
        listInsert(scope->variables, elem);

        return true;
    }
}

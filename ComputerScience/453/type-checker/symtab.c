#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "symtab.h"
#include "globals.h"
#include "utils.h"
#include "typecheck.h"

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
    ScopeElement *foundVar = findScopeElement(scope, identifier);
    bool validDeclaration = true;

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
                validDeclaration = false;
            } else if(func->declaredExtern) {
                // An extern function cannot be declared in the same file
                fprintf(stderr, "ERROR: Attempting to define function %s declared as extern on line %d.\n",
                        identifier, mylineno);
                validDeclaration = false;
            } else {
                // Ensure that the prototype and declaration match
                if(argumentTypes) {
                    if(func->argumentTypes) {
                        // Compare the types
                        ListNode *expectedTypeNode = func->argumentTypes->head;
                        ListNode *suppliedTypeNode = argumentTypes->head;
                        int numSupplied = 0, numExpected = 0;

                        while(expectedTypeNode && suppliedTypeNode) {
                            if(!expectedTypeNode->data) {
                                expectedTypeNode = expectedTypeNode->next;
                                continue;
                            }

                            if (!suppliedTypeNode->data) {
                                suppliedTypeNode = suppliedTypeNode->next;
                                continue;
                            }
                            Type expected = *((Type *) expectedTypeNode->data);
                            Type supplied = *((Type *) suppliedTypeNode->data);

                            numSupplied += 1;
                            numExpected += 1;

                            if(!typesCompatible(expected, supplied)) {
                                fprintf(stderr, "Error: Attempting to redeclare function %s to expect %s instead of %s on line %d.\n",
                                        identifier, typeName(supplied), typeName(expected),
                                        mylineno);
                                validDeclaration = false;
                            }

                            expectedTypeNode = expectedTypeNode->next;
                            suppliedTypeNode = suppliedTypeNode->next;
                        }

                        // Consume other expected types
                        while(expectedTypeNode) {
                            if(expectedTypeNode->data) numExpected += 1;
                            expectedTypeNode = expectedTypeNode->next;
                        }

                        // Consume other supplied types
                        while(suppliedTypeNode) {
                            if(suppliedTypeNode->data) numSupplied += 1;
                            suppliedTypeNode = suppliedTypeNode->next;
                        }

                        // Ensure the same number of arguments were supplied as were expected
                        if(numExpected != numSupplied) {
                            fprintf(stderr, "Error: On line %d, Attempting to change declared number of arguments for %s from %d to %d.\n",
                                    mylineno, identifier, numExpected, numSupplied);
                            validDeclaration = false;

                        }
                    } else {
                        // Declaration provides arguments, prototype does not
                        fprintf(stderr, "Error: Attempting to redeclare %s on line %d to take arguments.\n",
                                identifier, mylineno);
                        validDeclaration = false;
                    }
                } else {
                    if(func->argumentTypes) {
                        // Declaration provides no arguments, prototype does
                        fprintf(stderr, "Error: Attempting to redeclare %s on line %d to take no arguments.\n",
                                identifier, mylineno);
                        validDeclaration = false;
                    } else {
                        // Neither the prototype nor the declaration expect arguments
                        func->implemented = true;
                    }
                }
            }
        } else {
            // If it's a variable, then a variable can't be defined as a scope
            fprintf(stderr, "ERROR: Attempting to redefine variable %s as function on line %d.\n",
                    identifier, mylineno);
            validDeclaration = false;
        }
    } else {
        // Add the function to scope
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

    if(!validDeclaration) {
        foundError = true;
    }
    return validDeclaration;
}

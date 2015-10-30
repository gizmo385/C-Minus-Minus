#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "symtab.h"
#include "globals.h"
#include "utils.h"
#include "typecheck.h"
#include "errors.h"

static inline int compareScopeElements(ScopeElement *a, ScopeElement *b) {
    if(a && b) {
        return strcmp(a->identifier, b->identifier);
    } else {
        return -1;
    }
}

static inline int insertAtRear(void *a, void *b) {
    return -1;
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
                error(REDECL_GLOBAL_VAR, identifier);
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
        error(VAR_ALREADY_DECLARED, identifier);
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

bool declareFunction(Scope *scope, Type returnType, char *identifier, FunctionParameter *parameters,
        bool declaredExtern, bool isPrototype) {
    ScopeElement *foundVar = findScopeElement(scope, identifier);
    bool validDeclaration = true;

    // Parse out the names and types of the declared argument types
    List *argumentNames = NULL;
    List *argumentTypes = NULL;

    if(parameters) {
        argumentNames = newList(insertAtRear);
        argumentTypes = newList(insertAtRear);

        while(parameters) {
            listInsert(argumentNames, parameters->identifier);
            Type *typeP = malloc(sizeof(Type));
            *typeP = parameters->type;
            listInsert(argumentTypes, typeP);
            parameters = parameters->next;
        }
    }

    // Determine if something with that name already exists
    if(foundVar) {
        // If that thing is a function, check some properties
        if(foundVar->elementType == SCOPE_FUNC) {
            ScopeFunction *func = foundVar->function;
            // Determine whether or not a function prototype has been duplicated
            if(isPrototype) {
                error(REDEF_PROTOTYPE, identifier);
                validDeclaration = false;
            }

            // Check if the function's return type has been changed
            if(returnType != func->returnType) {
                error(CHANGE_RET_TYPE, identifier, typeName(func->returnType), typeName(returnType));
                validDeclaration = false;
            }

            // Determine if the function has been implemented or declared as extern
            if(func->implemented) {
                // An already implemented function cannot be reimplemented
                error(REDEF_FUNCTION, identifier);
                validDeclaration = false;
            } else if(func->declaredExtern) {
                // An extern function cannot be declared in the same file
                error(REDEF_EXTERN, identifier);
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
                                error(ARG_TYPE_CHANGE, numSupplied, identifier, typeName(supplied), typeName(expected));
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
                            error(ARG_NUM_CHANGE, identifier, numExpected, numSupplied);
                            validDeclaration = false;

                        }
                    } else {
                        // Declaration provides arguments, prototype does not
                        error(REDEF_WITH_ARGS, identifier);
                        validDeclaration = false;
                    }
                } else {
                    if(func->argumentTypes) {
                        // Declaration provides no arguments, prototype does
                        error(REDEF_WITHOUT_ARGS, identifier);
                        validDeclaration = false;
                    } else {
                        // Neither the prototype nor the declaration expect arguments
                        func->implemented = true;
                    }
                }
            }
        } else {
            // If it's a variable, then a variable can't be defined as a scope
            error(REDEF_VAR_AS_FUNC, identifier);
            validDeclaration = false;
        }
    } else {
        // Add the function to scope
        debug(E_DEBUG, "Declaring function %s on line %d.\n", identifier, mylineno);
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
    return validDeclaration;
}

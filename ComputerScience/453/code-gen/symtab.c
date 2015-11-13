#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "vector.h"
#include "symtab.h"
#include "globals.h"
#include "ast.h"
#include "utils.h"
#include "typecheck.h"
#include "errors.h"

static inline ScopeElement *inLocalScope(Scope *scope, char *identifier) {
    if(scope) {
        Vector *variables = scope->variables;
        for(int i = 0; i < variables->size; i++) {
            ScopeElement *element = vectorGet(variables, i);

            if(element && strcmp(element->identifier, identifier) == 0) {
                return element;
            }
        }
    }

    return NULL;
}

Scope *newScope(Scope *enclosingScope) {
    debug(E_DEBUG, "Creating new scope\n");
    Scope *scope = malloc(sizeof(Scope));
    scope->enclosingScope = enclosingScope;
    scope->variables = newVector(25);

    return scope;
}

Scope *flattenScope(Scope *scope) {
    Scope *destinationScope = newScope(NULL);

    // Traverse every level of scope
    while(scope) {
        Vector *vars = scope->variables;

        for(int i = 0; i < vars->size; i++) {
            ScopeElement *elem = vectorGet(vars, i);
            char *identifier = elem->identifier;

            // If it hasn't already been declared in the flattened scope, "declare it"
            if(inLocalScope(destinationScope, identifier)) {
                error(REDECL_GLOBAL_VAR, identifier);
            } else {
                vectorAdd(destinationScope->variables, elem);
            }
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

ScopeElement* declareVar(Scope *scope, Type type, char *identifier) {
    ScopeElement *foundVar = inLocalScope(scope, identifier);

    if(foundVar) {
        error(VAR_ALREADY_DECLARED, identifier);
    } else {
        debug(E_DEBUG, "Declaring undeclared variable \"%s\" with type %s\n", identifier, typeName(type));
        Value *empty = malloc(sizeof(Value));
        empty->integer_value = 0;

        ScopeVariable *scopeVariable = malloc(sizeof(ScopeVariable));
        scopeVariable->type = type;
        scopeVariable->value = empty;
        scopeVariable->size = -1;
        scopeVariable->offset = 0;

        ScopeElement *elem = malloc(sizeof(ScopeElement));
        elem->identifier = identifier;
        elem->elementType = SCOPE_VAR;
        elem->variable = scopeVariable;
        vectorAdd(scope->variables, elem);

        return elem;
    }

    return NULL;
}

bool declareFunction(Scope *scope, Type returnType, char *identifier, FunctionParameter *parameters,
        bool declaredExtern, bool isPrototype) {
    ScopeElement *foundVar = findScopeElement(scope, identifier);
    bool validDeclaration = true;

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
                if(parameters) {
                    if(func->parameters) {
                        // Compare the types
                        FunctionParameter *declaredParameters = func->parameters;
                        int numSupplied = 0, numExpected = 0;

                        while(parameters && declaredParameters) {
                            Type supplied = parameters->type;
                            Type expected = declaredParameters->type;

                            if(! typesCompatible(supplied, expected)) {
                                error(ARG_TYPE_CHANGE, numSupplied, identifier, typeName(supplied), typeName(expected));
                                validDeclaration = false;
                            }

                            numSupplied += 1;
                            numExpected += 1;
                            parameters = parameters->next;
                            declaredParameters = declaredParameters->next;
                        }

                        // Consume other expected types
                        while(declaredParameters) {
                            numExpected += 1;
                            declaredParameters = declaredParameters->next;
                        }

                        // Consume other supplied types
                        while(parameters) {
                            numExpected += 1;
                            parameters = parameters->next;
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
                    if(func->parameters) {
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
        debug(E_DEBUG, "Declaring function %s of type %s on line %d.\n", identifier,
                typeName(returnType), mylineno);
        ScopeFunction *scopeFunction = malloc(sizeof(ScopeFunction));
        scopeFunction->returnType = returnType;
        scopeFunction->parameters = parameters;
        scopeFunction->implemented = false;
        scopeFunction->declaredExtern = declaredExtern;

        ScopeElement *elem = malloc(sizeof(ScopeElement));
        elem->identifier = identifier;
        elem->elementType = SCOPE_FUNC;
        elem->function = scopeFunction;
        vectorAdd(scope->variables, elem);

        return true;
    }
    return validDeclaration;
}

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

static int variableId = 0;

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

StructField *getField(ScopeElement *element, char *field) {
    if(element && element->elementType == SCOPE_VAR) {
        ScopeVariable *var = element->variable;

        if(var->type == STRUCT_TYPE) {
            StructField *fields = var->structure->structure->structure->fields;

            while(fields) {
                if(strcmp(fields->fieldName, field)) {
                    return fields;
                }
                fields = fields->next;
            }
        }
    }

    return NULL;
}

void declareStruct(Scope *scope, char *identifier, StructDeclaration *structDeclaration) {
    ScopeStruct *structure = calloc(1, sizeof(ScopeStruct));
    structure->structure = structDeclaration;

    ScopeElement *scopeElement = calloc(1, sizeof(ScopeElement));
    scopeElement->identifier = identifier;
    scopeElement->protectedIdentifier = identifier;
    scopeElement->elementType = SCOPE_STRUCT;
    scopeElement->structure = structure;

    debug(E_DEBUG, "Adding the struct \"%s\" to scope.\n", identifier);
    vectorAdd(scope->variables, scopeElement);
}

ScopeElement* declareVar(Scope *scope, Type type, char *identifier, bool parameter) {
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
        scopeVariable->global = false;
        scopeVariable->parameter = parameter;

        int newIdentifierLength = strlen(identifier) + 20;
        char *protectedIdentifier = malloc(newIdentifierLength);
        snprintf(protectedIdentifier, newIdentifierLength, "_%s%d", identifier, variableId);
        variableId += 1;

        ScopeElement *elem = malloc(sizeof(ScopeElement));
        elem->identifier = identifier;
        elem->protectedIdentifier = protectedIdentifier;
        elem->elementType = SCOPE_VAR;
        elem->variable = scopeVariable;
        vectorAdd(scope->variables, elem);

        return elem;
    }

    return NULL;
}

bool declareFunction(Scope *scope, Type returnType, char *identifier, Vector *parameters,
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
                Vector *expectedParams = func->parameters;
                int declared = parameters->size;
                int expected = expectedParams->size;
                int min = declared < expected ? declared : expected;

                // Check the types of each declared and expected parameter
                for(int i = 0; i < min; i++) {
                    FunctionParameter *declaredParam = vectorGet(parameters, i);
                    FunctionParameter *expectedParam = vectorGet(expectedParams, i);

                    Type declaredType = declaredParam->type;
                    Type expectedType = expectedParam->type;
                    if(! typesCompatible(declaredType, expectedType)) {
                        error(ARG_TYPE_CHANGE, i, identifier, typeName(declaredType),
                                typeName(expectedType));
                        validDeclaration = false;
                    }
                }

                if(declared == 0 && expected != 0) {
                    error(REDEF_WITHOUT_ARGS, identifier);
                    validDeclaration = false;
                } else if(declared != 0 && expected == 0) {
                    error(REDEF_WITH_ARGS, identifier);
                    validDeclaration = false;
                } else if(declared != expected) {
                    error(ARG_NUM_CHANGE, identifier, expected, declared);
                    validDeclaration = false;
                }

                func->implemented = true;
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

        // Don't override the function name "main" since it is necessary for execution entry
        if(strcmp(identifier, "main") == 0) {
            elem->protectedIdentifier = identifier;
        } else {
            int newIdentifierLength = strlen(identifier) + 2;
            char *protectedIdentifier = malloc(newIdentifierLength);
            snprintf(protectedIdentifier, newIdentifierLength, "_%s", identifier);
            elem->protectedIdentifier = protectedIdentifier;
        }

        elem->elementType = SCOPE_FUNC;
        elem->function = scopeFunction;
        vectorAdd(scope->variables, elem);

        return true;
    }

    return validDeclaration;
}

#include "types.h"
#include "vector.h"
#include "utils.h"
#include "three.h"
#include "globals.h"
#include "symtab.h"

static void functionTAC(FunctionDeclaration *declaration, Vector *code) {
    // Find the function scope entry
    char *functionName = declaration->functionName;
    ScopeElement *func = findScopeElement(scope, functionName);
    Scope *functionScope = declaration->functionScope;

    // Create the function header (label and enter statement)
    TACInstruction *label = newLabel(functionName);
    vectorAdd(code, label);

    TACInstruction *enter = newTAC(ENTER, func, NULL, NULL);
    vectorAdd(code, enter);
    debug(E_DEBUG, "enter %s\n", functionName);
    // TODO: Pull arguments off the stack?

    // Generate code for the body of the function
    Statement *body = declaration->body;
    while(body) {
        statementTAC(functionScope, body);
        body = body->next;
    }

    // Leave the function
    TACInstruction *leave = newTAC(LEAVE, func, NULL, NULL);
    vectorAdd(code, leave);
    debug(E_DEBUG, "leave %s\n\n", functionName);
}

Vector *generateTAC(FunctionDeclaration *declarations) {
    Vector *code = newVector(25);

    while(declarations) {
        functionTAC(declarations, code);
        declarations = declarations->next;
    }

    return code;
}

void generateCode(FunctionDeclaration *declarations) {
    Vector *tac = generateTAC(declarations);
    debug(E_DEBUG, "Generated %d TACs.\n", tac->size);
}

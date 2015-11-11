#include "types.h"
#include "vector.h"
#include "utils.h"
#include "three.h"
#include "globals.h"
#include "symtab.h"

static void functionTAC(FunctionDeclaration *declaration) {
    // Find the function scope entry
    char *functionName = declaration->functionName;
    ScopeElement *func = findScopeElement(scope, functionName);
    Scope *functionScope = declaration->functionScope;

    // Create the function header (label and enter statement)
    TACInstruction *label = newLabel(functionName);
    TACInstruction *enter = newTAC(ENTER, func, NULL, NULL);
    label->next = enter;
    declaration->codeStart = label;
    declaration->codeEnd = enter;
    debug(E_DEBUG, "enter %s\n", functionName);

    // Generate code for the body of the function
    Statement *body = declaration->body;
    while(body) {
        statementTAC(functionScope, body);

        TACInstruction *tail = declaration->codeEnd;
        if(tail) {
            tail->next = body->codeStart;
            declaration->codeEnd = body->codeEnd;
        } else {
            declaration->codeEnd = body->codeEnd;
        }

        body = body->next;
    }

    // Leave the function
    TACInstruction *leave = newTAC(LEAVE, func, NULL, NULL);
    declaration->codeEnd->next = leave;
    declaration->codeEnd = leave;
    debug(E_DEBUG, "leave %s\n\n", functionName);
}

void generateTAC(FunctionDeclaration *declarations) {
    while(declarations) {
        functionTAC(declarations);
        declarations = declarations->next;
    }
}

void generateCode(FunctionDeclaration *declarations) {
    generateTAC(declarations);
}

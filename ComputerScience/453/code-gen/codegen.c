#include "types.h"
#include "vector.h"
#include "utils.h"
#include "three.h"
#include "globals.h"
#include "symtab.h"
#include "mips.h"

static void functionTAC(FunctionDeclaration *declaration) {
    // Find the function scope entry
    char *functionName = declaration->functionName;
    ScopeElement *func = findScopeElement(scope, functionName);
    Scope *functionScope = declaration->functionScope;

    // Create the function header (label and enter statement)
    TACInstruction *label = newLabel(functionName);
    TACInstruction *enter = newTAC(ENTER, func, NULL, NULL);
    label->next = enter;
    vectorAdd(declaration->code, label);
    vectorAdd(declaration->code, enter);
    debug(E_INFO, "enter %s\n", functionName);

    // Generate code for the body of the function
    Statement *body = declaration->body;
    while(body) {
        statementTAC(functionScope, body);

        Vector *bodyCode = body->code;
        for(int i = 0; i < bodyCode->size; i++ ) {
            vectorAdd(declaration->code, vectorGet(bodyCode, i));
        }

        body = body->next;
    }

    // Leave the function
    TACInstruction *leave = newTAC(LEAVE, func, NULL, NULL);
    vectorAdd(declaration->code, leave);
    debug(E_INFO, "leave %s\n\n", functionName);
}

void generateTAC(FunctionDeclaration *declarations) {
    while(declarations) {
        functionTAC(declarations);
        declarations = declarations->next;
    }
}

void generateCode(FunctionDeclaration *declarations) {
    generateTAC(declarations);
    generateMipsFunctions(declarations);
}

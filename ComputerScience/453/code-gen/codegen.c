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

    // Generate code for the body of the function
    Statement *body = declaration->body;
    statementTAC(functionScope, body, declaration->code);

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

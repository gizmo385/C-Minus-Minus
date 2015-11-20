#include "types.h"
#include "vector.h"
#include "utils.h"
#include "three.h"
#include "globals.h"
#include "symtab.h"
#include "mips.h"

static void functionTAC(FunctionDeclaration *declaration) {
    // Find the function scope entry
    Scope *functionScope = declaration->functionScope;

    // Generate code for the body of the function
    Statement *body = declaration->body;
    statementTAC(functionScope, body, declaration->code);
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

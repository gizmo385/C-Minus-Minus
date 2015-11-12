#include "mips.h"
#include "utils.h"
#include "vector.h"

static void generateMips(TACInstruction *instruction) {

}

static int sizeForType(Type type) {
    int size = 0;
    switch(type) {
        case INT_TYPE:
            size = sizeof(int);
            break;
        case CHAR_TYPE:
            size = sizeof(char);
            break;
        case CHAR_ARRAY_TYPE:
        case INT_ARRAY_TYPE:
            size = sizeof(char*);
            break;
        default: break;
    }
    return size;
}

int calculateRequiredStackSpace(FunctionDeclaration *declaration) {
    int requiredSpace = 0;

    // Check the size of things in local scope (this includes parameters and temporary variables)
    Scope *functionScope = declaration->functionScope;
    Vector *variables = functionScope->variables;

    for(int i = 0; i < variables->size; i++) {
        ScopeElement *element = vectorGet(variables, i);
        if(element->elementType == SCOPE_VAR) {
            int elementSize = sizeForType(element->variable->type);
            debug(E_INFO, "Size for %s is %d bytes.\n", element->identifier, elementSize);
            requiredSpace += elementSize;
        }
    }

    return requiredSpace;
}

void generateMipsFunctions(FunctionDeclaration *declarations) {
    while(declarations) {
        int requiredStackSpace = calculateRequiredStackSpace(declarations);

        printf("%d bytes of stack space are required for the variables in %s.\n", requiredStackSpace,
                declarations->functionName);

        Vector *declarationCode = declarations->code;
        for(int i = 0; i < declarationCode->size; i++) {
            TACInstruction *instruction = vectorGet(declarationCode, i);
            generateMips(instruction);
        }


        declarations = declarations->next;
    }
}

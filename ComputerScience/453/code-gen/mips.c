#include "mips.h"
#include "utils.h"

static void generateMips(TACInstruction *instruction) {

}

void generateMipsFunctions(FunctionDeclaration *declarations) {
    while(declarations) {
        Vector *declarationCode = declarations->code;

        for(int i = 0; i < declarationCode->size; i++) {
            TACInstruction *instruction = vectorGet(declarationCode, i);
            generateMips(instruction);
        }


        declarations = declarations->next;
    }
}

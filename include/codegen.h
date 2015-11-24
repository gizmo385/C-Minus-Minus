#ifndef CODEGEN_H
#define CODEGEN_H
#include "types.h"

extern Vector *generateTAC(FunctionDeclaration *declarations);
extern void generateCode(FunctionDeclaration *declarations);

#endif

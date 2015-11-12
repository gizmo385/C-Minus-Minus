#ifndef MIPS_H
#define MIPS_H
#include <stdio.h>
#include <stdbool.h>
#include "three.h"
#include "types.h"

extern void generateMipsFunctions(FunctionDeclaration *declarations);
extern int calculateRequiredStackSpace(FunctionDeclaration *declaration);

#endif

#include "mips.h"
#include "three.h"
#include "utils.h"
#include "vector.h"
#include "globals.h"
#include "symtab.h"

static int numberOfParameters = 0;

void varIntoRegister(ScopeElement *varElem, char *reg) {
    ScopeVariable *var = varElem->variable;
    Type type = var->type;

    if(var->global) {
        if(type == CHAR_TYPE) {
            printf("\tla %s, %s\n", reg, varElem->protectedIdentifier);
            printf("\tlb %s, 0(%s)\n", reg, reg);
        } else if(type == INT_TYPE) {
            printf("\tla %s, %s\n", reg, varElem->protectedIdentifier);
            printf("\tlw %s, 0(%s)\n", reg, reg);
        } else if(type == INT_ARRAY_TYPE || type == CHAR_ARRAY_TYPE) {
            printf("\tla %s, %s\n", reg, varElem->protectedIdentifier);
        }
    } else {
        if(type == INT_TYPE) {
            printf("\tlw %s, %d($fp)\n", reg, var->offset);
        } else if(type == CHAR_TYPE) {
            printf("\tlb %s, %d($fp)\n", reg, var->offset);
        } else if(type == INT_ARRAY_TYPE || type == CHAR_ARRAY_TYPE) {
            printf("\tla %s, %s\n", reg, varElem->protectedIdentifier);
        } else {
            printf("TYPE: %s\n", typeName(type));
        }
    }
}

void registerIntoVar(ScopeElement *varElem, char *reg) {
    ScopeVariable *var = varElem->variable;
    Type type = var->type;

    if(var->global) {
        if(var->type == CHAR_TYPE) {
            printf("\tsb %s, %s\n", reg, varElem->protectedIdentifier);
        } else if(var->type == INT_TYPE) {
            printf("\tsw %s, %s\n", reg, varElem->protectedIdentifier);
        }
    } else {
        if(type == INT_TYPE) {
            printf("\tsw %s, %d($fp)\n", reg, var->offset);
        } else if(type == CHAR_TYPE) {
            printf("\tsb %s, %d($fp)\n", reg, var->offset);
        } else if(type == INT_ARRAY_TYPE) {
            /*printf("INT ARRAY ASSIGNMENT NOT SUPPORTED\n");*/
        } else if(type == CHAR_ARRAY_TYPE) {
            /*printf("CHAR ARRAY ASSIGNMENT NOT SUPPORTED\n");*/
        } else {
            printf("TYPE: %s\n", typeName(type));
        }
    }
}

static void constantToReg(ScopeElement *destElem, char *reg) {
    // Since there is not a source, we're assigning a constant
    ScopeVariable *dest = destElem->variable;
    Type type = dest->type;
    Value *value = dest->value;

    printf("\t# Assigning constant: %s\n", constantValueString(type, value));
    if(type == CHAR_TYPE) {
        printf("\tli %s, %d\n", reg, value->char_value);
    } else if(type == INT_TYPE) {
        // Load the 32 bit integer register, then store it on the stack
        int constant = value->integer_value;
        unsigned int higher = constant & 0xFFFF0000;
        unsigned int lower = constant & 0x0000FFFF;

        printf("\tlui %s, %d\n", reg, higher);
        printf("\tori %s, %d\n", reg, lower);
    } else if(type == INT_ARRAY_TYPE) {
        debug(E_DEBUG, "INT ARRAY ASSIGNMENT NOT SUPPORTED\n");
    } else if(type == CHAR_ARRAY_TYPE) {
        debug(E_DEBUG, "CHAR ARRAY ASSIGNMENT NOT SUPPORTED\n");
    } else {
        debug(E_DEBUG, "TYPE: %s\n", typeName(type));
    }
}

static void arithmeticInstruction(TACInstruction *instruction, char *operation) {
    ScopeElement *src1 = instruction->src1;
    ScopeElement *src2 = instruction->src2;
    varIntoRegister(src1, "$t0");
    varIntoRegister(src2, "$t1");
    printf("\t%s $t2, $t0, $t1\n", operation);
    registerIntoVar(instruction->dest, "$t2");
}

static void generateMips(TACInstruction *instruction) {
    if(instruction) {
        switch(instruction->op) {
            case ASSG_ADD:
                {
                    printf("\t# Add %s to %s\n", instruction->src1->identifier, instruction->src2->identifier);
                    arithmeticInstruction(instruction, "add");
                    break;
                }
            case ASSG_SUB:
                {
                    printf("\t# Subtract %s from %s\n", instruction->src1->identifier, instruction->src2->identifier);
                    arithmeticInstruction(instruction, "sub");
                    break;
                }
            case ASSG_MUL:
                {
                    printf("\t# Multiply %s by %s\n", instruction->src1->identifier, instruction->src2->identifier);
                    arithmeticInstruction(instruction, "mul");
                    break;
                }
            case ASSG_DIV:
                {
                    printf("\t# Divide %s by %s\n", instruction->src1->identifier, instruction->src2->identifier);
                    arithmeticInstruction(instruction, "div");
                    break;
                }
            case ASSG_UNARY_MINUS:
                {
                    varIntoRegister(instruction->src1, "$t0");
                    printf("\t# Negate %s\n", instruction->src1->identifier);
                    printf("\tsub $t0, $zero, $t0\n");
                    registerIntoVar(instruction->dest, "$t0");
                    break;
                }
            case ASSG_CONST:
                {
                    ScopeElement *dest = instruction->dest;

                    if(instruction->src1) {
                        constantToReg(instruction->src1, "$t0");
                        registerIntoVar(dest, "$t0");
                    } else {
                        constantToReg(dest, "$t0");
                        registerIntoVar(dest, "$t0");
                    }
                    break;
                }
                break;
            case ASSG_VAR:
                {
                    ScopeElement *dest = instruction->dest;

                    if(instruction->src1) {
                        ScopeElement *src1 = instruction->src1;
                        printf("\t# Copying from %s to %s.\n", src1->identifier, dest->identifier);
                        varIntoRegister(src1, "$t0");
                        registerIntoVar(dest, "$t0");
                    } else {
                        constantToReg(dest, "$t0");
                        registerIntoVar(dest, "$t0");
                    }
                    break;
                }
            case ASSG_TO_INDEX:
                break;
            case IF_GTE:
                {
                    ScopeElement *trueDest = instruction->dest;
                    ScopeElement *left = instruction->src1;
                    ScopeElement *right = instruction->src2;

                    varIntoRegister(left, "$t0");
                    varIntoRegister(right, "$t1");

                    printf("\t# Goto %s if %s >= %s\n", trueDest->identifier, left->identifier, right->identifier);
                    printf("\tsub $t2, $t0, $t1\n");
                    printf("\tbgez $t2, %s\n", trueDest->protectedIdentifier);
                    break;
                }
            case IF_LTE:
                {
                    ScopeElement *trueDest = instruction->dest;
                    ScopeElement *left = instruction->src1;
                    ScopeElement *right = instruction->src2;

                    varIntoRegister(left, "$t0");
                    varIntoRegister(right, "$t1");

                    printf("\t# Goto %s if %s <= %s\n", trueDest->identifier, left->identifier, right->identifier);
                    printf("\tsub $t2, $t0, $t1\n");
                    printf("\tblez $t2, %s\n", trueDest->protectedIdentifier);
                    break;
                }
            case IF_GT:
                {
                    ScopeElement *trueDest = instruction->dest;
                    ScopeElement *left = instruction->src1;
                    ScopeElement *right = instruction->src2;

                    varIntoRegister(left, "$t0");
                    varIntoRegister(right, "$t1");

                    printf("\t# Goto %s if %s > %s\n", trueDest->identifier, left->identifier, right->identifier);
                    printf("\tsub $t2, $t0, $t1\n");
                    printf("\tbgtz $t2, %s\n", trueDest->protectedIdentifier);
                    break;
                }
            case IF_LT:
                {
                    ScopeElement *trueDest = instruction->dest;
                    ScopeElement *left = instruction->src1;
                    ScopeElement *right = instruction->src2;

                    varIntoRegister(left, "$t0");
                    varIntoRegister(right, "$t1");

                    printf("\t# Goto %s if %s < %s\n", trueDest->identifier, left->identifier, right->identifier);
                    printf("\tsub $t2, $t0, $t1\n");
                    printf("\tbltz $t2, %s\n", trueDest->protectedIdentifier);
                    break;
                }
            case IF_EQ:
                {
                    ScopeElement *trueDest = instruction->dest;
                    ScopeElement *left = instruction->src1;
                    ScopeElement *right = instruction->src2;

                    varIntoRegister(left, "$t0");
                    varIntoRegister(right, "$t1");

                    printf("\t# Goto %s if %s == %s\n", trueDest->identifier, left->identifier, right->identifier);
                    printf("\tbeq $t0, $t1, %s\n", trueDest->protectedIdentifier);
                    break;
                }
            case IF_NEQ:
                {
                    ScopeElement *trueDest = instruction->dest;
                    ScopeElement *left = instruction->src1;
                    ScopeElement *right = instruction->src2;

                    varIntoRegister(left, "$t0");
                    varIntoRegister(right, "$t1");

                    printf("\t# Goto %s if %s != %s\n", trueDest->identifier, left->identifier, right->identifier);
                    printf("\tbne $t0, $t1, %s\n", trueDest->protectedIdentifier);
                    break;
                }
            case GOTO:
                printf("\tj %s\n", instruction->dest->protectedIdentifier);
                break;
            case LABEL:
                printf("%s:\n", instruction->dest->protectedIdentifier);
                break;
            case ENTER:
            case LEAVE:
            case RETURN_FROM:
                break;
            case CALL:
                {
                    char *id = instruction->dest->protectedIdentifier;
                    printf("\t# Call function: %s\n", id);
                    printf("\tjal %s\n", id);
                    printf("\tla $sp, %d($sp)\n", 4 * numberOfParameters);
                    numberOfParameters = 0;
                    break;
                }
            case RETRIEVE:
                break;
            case PARAM:
                {
                    // Load the value into $t0
                    ScopeElement *dest = instruction->dest;
                    printf("\t# Handle the argument %s\n", dest->protectedIdentifier);
                    varIntoRegister(dest, "$t0");

                    // Adjust the stack pointer
                    printf("\tla $sp, -4($sp)\n");

                    // Load the right size element onto the stack
                    ScopeFunction *f = instruction->src1->function;
                    FunctionParameter *param = vectorGet(f->parameters, numberOfParameters);

                    if(param->type == CHAR_TYPE) {
                        printf("\tsb $t0, 0($sp)\n");
                    } else {
                        printf("\tsw $t0, 0($sp)\n");
                    }

                    numberOfParameters += 1;
                    break;
                }
            case NO_OP:
                break;
        }
    }
}

static int sizeForType(ScopeVariable *variable) {
    int size = 0;
    switch(variable->type) {
        case INT_TYPE:
            size = sizeof(int);
            break;
        case CHAR_TYPE:
            size = sizeof(char);
            break;
        case CHAR_ARRAY_TYPE:
            {
                int arraySize = variable->size;
                size = arraySize * sizeof(char);
                break;
            }
        case INT_ARRAY_TYPE:
            {
                int arraySize = variable->size;
                size = arraySize * sizeof(int);
                break;
            }
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
            /*int elementSize = sizeForType(element->variable);*/
            requiredSpace += 4;
        }
    }

    return requiredSpace;
}

void generateMipsFunctions(FunctionDeclaration *declarations) {

    // Before we handle functions, we should handle the globals
    Vector *globalVariables = globalScope->variables;

    printf(".data\n");

    for(int i = 0; i < globalVariables->size; i++) {
        ScopeElement *element = vectorGet(globalVariables, i);
        if(element->elementType == SCOPE_VAR) {
            ScopeVariable *var = element->variable;
            int size = sizeForType(var);

            if(var->type == INT_TYPE) {
                printf("\t.align 2\n");
            }

            printf("%s:\t.space %d\n", element->protectedIdentifier, size);
        }
    }

    printf(".text\n");

    // Now we need to figure out where everything on stack will go
    while(declarations) {
        int requiredStackSpace = calculateRequiredStackSpace(declarations);

        // Generate the data section for the function
        printf(".data\n");

        // Calculate stack offsets and populate the data section
        Vector *variables = declarations->functionScope->variables;
        for(int i = 0; i < variables->size; i++) {
            ScopeElement *element = vectorGet(variables, i);
            if(element->elementType == SCOPE_VAR) {
                ScopeVariable *var = element->variable;
                var->offset = (4 * i) + 4;
                var->offset *= - 1;

                // Handle string constants
                if(var->type == CHAR_ARRAY_TYPE) {
                    printf("%s:\t.asciiz %s\n", element->protectedIdentifier,
                            var->value->char_array_value);
                }
            }
        }

        // Determine offsets for the function parameters
        Vector *parameters = declarations->parameters;
        for(int i = 0; i < parameters->size; i++) {
            FunctionParameter *parameter = vectorGet(parameters, i);
            ScopeElement *element = findScopeElement(declarations->functionScope, parameter->identifier);
            ScopeVariable *var = element->variable;
            var->offset = (4 * i) + 8;
        }

        requiredStackSpace -= (4 * parameters->size);

        // Generate the function's prologue
        ScopeElement *functionElement = findScopeElement(globalScope, declarations->functionName);
        char *functionName = functionElement->protectedIdentifier;
        printf(".text\n");
        printf("%s:\n", functionName);
        printf("\t# Prologue for %s\n", functionName);
        printf("\tla $sp, -8($sp) # Allocate space for old $fp and $ra\n");
        printf("\tsw $fp, 4($sp) # Save old frame pointer\n");
        printf("\tsw $ra, 0($sp) # Save old return address\n");
        printf("\tla $fp, 0($sp) # Set up the new frame pointer\n");
        printf("\tla $sp, %d($sp) # Allocate space for local variables and temps\n\n", -requiredStackSpace);

        // Generate mips for everything in the function
        Vector *declarationCode = declarations->code;
        for(int i = 0; i < declarationCode->size; i++) {
            TACInstruction *instruction = vectorGet(declarationCode, i);
            generateMips(instruction);
        }

        // Generate the function's epilogue
        printf("\n");
        printf("\t# Epilogue for %s\n", functionName);
        printf("\tla $sp, 0($fp)\n");
        printf("\tlw $ra, 0($sp)\n");
        printf("\tlw $fp, 4($sp)\n");
        printf("\tla $sp, 8($sp)\n");
        printf("\tjr $ra\n\n\n");

        declarations = declarations->next;
    }

    // Since we overload function names, we need to jump to main
    /*printf("main:\n");*/
    /*printf("\tla $sp, -4($sp) # Allocate space for old $fp and $ra\n");*/
    /*printf("\tsw $ra, 0($sp) # Save old return address\n");*/
    /*printf("\tla $fp, 0($sp) # Set up the new frame pointer\n");*/
    /*printf("\tjal _main\n");*/
    /*printf("\tla $sp, 0($fp)\n");*/
    /*printf("\tlw $ra, 0($sp)\n");*/
    /*printf("\tla $sp, 4($sp)\n");*/
    /*printf("\tjr $ra\n\n\n");*/

    // Generate code for print_string and print_int
    printf("_print_string:\n");
    printf("\tli $v0, 4\n");
    printf("\tlw $a0, 0($sp)\n");
    printf("\tsyscall\n");
    printf("\tjr $ra\n\n\n");

    printf("_print_int:\n");
    printf("\tli $v0, 1\n");
    printf("\tlw $a0, 0($sp)\n");
    printf("\tsyscall\n");
    printf("\tjr $ra\n");
}

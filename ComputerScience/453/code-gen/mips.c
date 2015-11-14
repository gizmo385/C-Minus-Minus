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
            printf("\tla %s, %s\n", reg, varElem->identifier);
            printf("\tlb %s, 0(%s)\n", reg, reg);
        } else if(type == INT_TYPE) {
            printf("\tla %s, %s\n", reg, varElem->identifier);
            printf("\tlw %s, 0(%s)\n", reg, reg);
        } else if(type == INT_ARRAY_TYPE || type == CHAR_ARRAY_TYPE) {
            printf("\tla %s, %s\n", reg, varElem->identifier);
        }
    } else {
        if(type == CHAR_TYPE || type == INT_TYPE) {
            printf("\tlw %s, %d($fp)\n", reg, var->offset);
        } else if(type == INT_ARRAY_TYPE) {
            /*printf("INT ARRAY ASSIGNMENT NOT SUPPORTED\n");*/
        } else if(type == CHAR_ARRAY_TYPE) {
            /*printf("CHAR ARRAY ASSIGNMENT NOT SUPPORTED\n");*/
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
            printf("\tsb %s, %s\n", reg, varElem->identifier);
        } else if(var->type == INT_TYPE) {
            printf("\tsw %s, %s\n", reg, varElem->identifier);
        }
    } else {
        if(type == CHAR_TYPE || type == INT_TYPE) {
            printf("\tsw %s, %d($fp)\n", reg, var->offset);
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
        printf("\tli %s, %d\n", reg, (int) value->char_value);
    } else if(type == INT_TYPE) {
        // Load the 32 bit integer register, then store it on the stack
        int constant = value->integer_value;
        unsigned int higher = constant & 0xFFFF0000;
        unsigned int lower = constant & 0x0000FFFF;

        printf("\tlui %s, %d\n", reg, higher);
        printf("\tori %s, %d\n", reg, lower);
    } else if(type == INT_ARRAY_TYPE) {
        /*printf("INT ARRAY ASSIGNMENT NOT SUPPORTED\n");*/
    } else if(type == CHAR_ARRAY_TYPE) {
        /*printf("CHAR ARRAY ASSIGNMENT NOT SUPPORTED\n");*/
    } else {
        printf("TYPE: %s\n", typeName(type));
    }
}

static void generateMips(TACInstruction *instruction) {
    if(instruction) {
        switch(instruction->op) {
            case ASSG_ADD:
            case ASSG_SUB:
            case ASSG_MUL:
            case ASSG_DIV:
            case ASSG_UNARY_MINUS:
                break;
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
                        printf("\t# Copying from %s to %s.\n", src1->identifier,
                                dest->identifier);
                        varIntoRegister(src1, "$t0");
                        registerIntoVar(dest, "$t0");
                        /*printf("\tlw $t0, %d($fp)\n", src1->variable->offset);*/
                        /*printf("\tsw $t0, %d($fp)\n", dest->variable->offset);*/
                    } else {
                        constantToReg(dest, "$t0");
                        registerIntoVar(dest, "$t0");
                    }
                    break;
                }
            case ASSG_TO_INDEX:
            case IF_GTE:
            case IF_LTE:
            case IF_GT:
            case IF_LT:
            case IF_EQ:
            case IF_NEQ:
            case GOTO:
                printf("\tj %s\n", instruction->dest->identifier);
                break;
            case LABEL:
                printf("%s:\n", instruction->dest->identifier);
                break;
            case ENTER:
            case LEAVE:
            case RETURN_FROM:
                break;
            case CALL:
                {
                    char *id = instruction->dest->identifier;
                    printf("\tjal %s # Call %s\n", id, id);
                    printf("\tla $sp, %d($sp)\n", 4 * numberOfParameters);
                    numberOfParameters = 0;
                    break;
                }
            case RETRIEVE:
                break;
            case PARAM:
                {
                    ScopeElement *dest = instruction->dest;
                    printf("\t# Handle the argument %s\n", dest->identifier);
                    if(dest->variable->type == CHAR_TYPE || dest->variable->type == INT_TYPE) {
                        varIntoRegister(dest, "$t0");
                    } else if(dest->variable->type == CHAR_ARRAY_TYPE) {
                        printf("\tla $t0, %s\n", dest->identifier);
                    }

                    /*if(var->type == CHAR_TYPE) {*/
                        /*printf("\tlw $t0 %d($fp)\n", var->offset);*/
                    /*} else if(var->type == INT_TYPE) {*/
                        /*printf("\tlw $t0, %d($fp)\n", var->offset);*/
                    /*} else if(var->type == CHAR_ARRAY_TYPE) {*/
                    /*}*/

                    printf("\tla $sp, -4($sp)\n");
                    printf("\tsw $t0, 0($sp)\n");
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

            printf("%s:\t.space %d\n", element->identifier, size);
        }
    }

    printf(".text\n");

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
                    printf("%s:\t.asciiz %s\n", element->identifier, var->value->char_array_value);
                }
            }
        }

        FunctionParameter *parameters = declarations->parameters;
        int loadedParameters = 0;
        while(parameters) {
            ScopeElement *element = findScopeElement(declarations->functionScope, parameters->identifier);
            ScopeVariable *var = element->variable;
            var->offset = (4 * loadedParameters) + 8;

            loadedParameters += 1;
            parameters = parameters->next;
        }

        requiredStackSpace -= (4 * loadedParameters);

        // Generate the function's prologue
        printf(".text\n");
        printf("%s:\n", declarations->functionName);
        printf("\t# Prologue for %s\n", declarations->functionName);
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
        printf("\t# Epilogue for %s\n", declarations->functionName);
        printf("\tla $sp, 0($fp)\n");
        printf("\tlw $ra, 0($sp)\n");
        printf("\tlw $fp, 4($sp)\n");
        printf("\tla $sp, 8($sp)\n");
        printf("\tjr $ra\n\n\n");

        declarations = declarations->next;
    }

    // Generate code for print_string and print_int
    printf("print_string:\n");
    printf("\tli $v0, 4\n");
    printf("\tlw $a0, 0($sp)\n");
    printf("\tsyscall\n");
    printf("\tjr $ra\n\n\n");

    printf("print_int:\n");
    printf("\tli $v0, 1\n");
    printf("\tlw $a0, 0($sp)\n");
    printf("\tsyscall\n");
    printf("\tjr $ra\n\n\n");
}

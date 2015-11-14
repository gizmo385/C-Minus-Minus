#include "mips.h"
#include "three.h"
#include "utils.h"
#include "vector.h"
#include "globals.h"
#include "symtab.h"

static int numberOfParameters = 0;

static void generateMips(TACInstruction *instruction) {
    if(instruction) {
        switch(instruction->op) {
            case ASSG_ADD:
            case ASSG_SUB:
            case ASSG_MUL:
            case ASSG_DIV:
            case ASSG_UNARY_MINUS:
            case ASSG_ADDR:
            case ASSG_DEREF:
                break;
            case ASSG_CONST:
                {
                    ScopeVariable *dest = instruction->dest->variable;

                    if(instruction->src1) {
                        ScopeVariable *src1 = instruction->src1->variable;
                        Type srcType = src1->type;
                        Value *value = src1->value;

                        if(srcType == CHAR_TYPE) {
                            printf("\tlb $t0, %d($fp)\n", src1->offset);
                            printf("\tsb $t0, %d($fp)\n", dest->offset);
                        } else if(srcType == INT_TYPE) {
                            int constant = value->integer_value;
                            unsigned int higher = constant & 0xFFFF0000;
                            unsigned int lower = constant & 0x0000FFFF;

                            printf("\tlui $t0, %d\n", higher);
                            printf("\tori $t0, %d\n", lower);
                            printf("\tsw $t0, %d($fp)\n", dest->offset);
                        } else if(srcType == INT_ARRAY_TYPE) {
                            /*printf("INT ARRAY ASSIGNMENT NOT SUPPORTED\n");*/
                        } else if(srcType == CHAR_ARRAY_TYPE) {
                            /*printf("CHAR ARRAY ASSIGNMENT NOT SUPPORTED\n");*/
                        } else {
                            printf("TYPE: %s\n", typeName(srcType));
                        }
                    } else {
                        // Since there is not a source, we're assigning a constant
                        Type destType = dest->type;
                        printf("\t# Assigning a constant: %s\n",
                                constantValueString(destType, dest->value));
                        if(destType == CHAR_TYPE) {
                            printf("\tli $t0, %d\n", (int)dest->value->char_value);
                            printf("\tsw $t0, %d($fp)\n", dest->offset);
                        } else if(destType == INT_TYPE) {
                            int constant = dest->value->integer_value;
                            unsigned int higher = constant & 0xFFFF0000;
                            unsigned int lower = constant & 0x0000FFFF;

                            printf("\tlui $t0, %d\n", higher);
                            printf("\tori $t0, %d\n", lower);
                            printf("\tsw $t0, %d($fp)\n", dest->offset);
                        } else if(destType == INT_ARRAY_TYPE) {
                            /*printf("INT ARRAY ASSIGNMENT NOT SUPPORTED\n");*/
                        } else if(destType == CHAR_ARRAY_TYPE) {
                            /*printf("CHAR ARRAY ASSIGNMENT NOT SUPPORTED\n");*/
                        } else {
                            printf("TYPE: %s\n", typeName(destType));
                        }
                    }
                    break;
                }
                break;
            case ASSG_VAR:
                {
                    ScopeVariable *dest = instruction->dest->variable;

                    if(instruction->src1) {
                        ScopeVariable *src1 = instruction->src1->variable;
                        printf("\t# Copying from %s to %s.\n", instruction->src1->identifier,
                                instruction->dest->identifier);
                        printf("\tlw $t0, %d($fp)\n", src1->offset);
                        printf("\tsw $t0, %d($fp)\n", dest->offset);
                    } else {
                        // Since there is not a source, we're assigning a constant
                        Type destType = dest->type;
                        printf("\t# Assigning a constant: %s\n",
                                constantValueString(destType, dest->value));
                        if(destType == CHAR_TYPE) {
                            printf("\tli $t0, %d\n", (int)dest->value->char_value);
                            printf("\tsw $t0, %d($fp)\n", dest->offset);
                        } else if(destType == INT_TYPE) {
                            int constant = dest->value->integer_value;
                            unsigned int higher = constant & 0xFFFF0000;
                            unsigned int lower = constant & 0x0000FFFF;

                            printf("\tlui $t0, %d\n", higher);
                            printf("\tori $t0, %d\n", lower);
                            printf("\tsw $t0, %d($fp)\n", dest->offset);
                        } else if(destType == INT_ARRAY_TYPE) {
                            /*printf("INT ARRAY ASSIGNMENT NOT SUPPORTED\n");*/
                        } else if(destType == CHAR_ARRAY_TYPE) {
                            /*printf("CHAR ARRAY ASSIGNMENT NOT SUPPORTED\n");*/
                        } else {
                            printf("TYPE: %s\n", typeName(destType));
                        }
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
                    ScopeVariable *var = dest->variable;
                    printf("\t# Handle the argument %s\n", dest->identifier);

                    if(var->type == CHAR_TYPE) {
                        printf("\tlb $t0 %d($fp)\n", var->offset);
                    } else if(var->type == INT_TYPE) {
                        printf("\tlw $t0, %d($fp)\n", var->offset);
                    } else if(var->type == CHAR_ARRAY_TYPE) {
                        printf("\tla $t0, %s\n", dest->identifier);
                    }

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

    for(int i = 0; i < globalVariables->size; i++) {
        ScopeElement *element = vectorGet(globalVariables, i);
        if(element->elementType == SCOPE_VAR) {
            ScopeVariable *var = element->variable;
            int size = sizeForType(var);

            printf("%s:\t.space %d\n", element->identifier, sizeForType(var));
            if(size % 4 != 0) {
                printf(".align %d\n", size);
            }
        }
    }

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

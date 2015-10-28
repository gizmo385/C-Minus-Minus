#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include "errors.h"
#include "globals.h"

const char *FUNC_AS_VAR             = "Attempting to use function %s as a variable.\n";
const char *UNDECLARED_INDENTIFIER  = "Undeclared identifier \"%s\".\n";
const char *BIN_OPERAND_TYPE        = "%s operand for %s has type %s, should be %s.\n";
const char *UNARY_OPERAND_TYPE      = "Unary operation %s expects type %s, found type %s.\n";
const char *ARRAY_INDEX_TYPE        = "Attempting to index array with %s, should be INT.\n";
const char *ARGS_TO_VOID            = "Attempting to supply arguments to a void function.\n";
const char *ARG_TYPE_MISMATCH       = "Argument %d to %s expected %s, found %s.\n";
const char *NUM_ARGS_DIFFER         = "%s expected %d arguments, found %d arguments.\n";
const char *VAR_AS_FUNCTION         = "Attempting to call variable %s as function.\n";
const char *CALL_UNDEF_FUNCTION     = "Attempting to call undefined function %s.\n";
const char *RETURN_FROM_VOID        = "Attempting to return %s from function declared as VOID.\n";
const char *RETURN_MISMATCH         = "Attempting to return %s from function declared to return %s.\n";
const char *VAR_TYPE_MISMATCH       = "Attempting to assign %s to variable of type %s.\n";
const char *ARRAY_AS_VAR            = "%s is an array, requires an index.\n";
const char *ASSIGN_TO_FUNC          = "Attempting to assign to function.\n";
const char *INVALID_COND            = "Expected condition with type BOOL, found type %s.\n";


void error(const char *format, ...) {
    fprintf(stderr, "Error on line %d: ", mylineno);

    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);

    foundError = true;
}

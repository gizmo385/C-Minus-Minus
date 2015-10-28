#ifndef CMM_ERRORS
#define CMM_ERRORS

extern const char *FUNC_AS_VAR;
extern const char *UNDECLARED_INDENTIFIER;
extern const char *BIN_OPERAND_TYPE;
extern const char *UNARY_OPERAND_TYPE;
extern const char *ARRAY_INDEX_TYPE;
extern const char *ARGS_TO_VOID;
extern const char *ARG_TYPE_MISMATCH;
extern const char *NUM_ARGS_DIFFER;
extern const char *VAR_AS_FUNCTION;
extern const char *CALL_UNDEF_FUNCTION;
extern const char *RETURN_FROM_VOID;
extern const char *RETURN_MISMATCH;
extern const char *VAR_TYPE_MISMATCH;
extern const char *ARRAY_AS_VAR;
extern const char *ASSIGN_TO_FUNC;
extern const char *INVALID_COND;

extern void error(const char *format, ...);

#endif

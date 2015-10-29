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

/* SYMBOL TABLE ERRORS */
extern const char *REDECL_GLOBAL_VAR;
extern const char *VAR_ALREADY_DECLARED;
extern const char *REDEF_PROTOTYPE;
extern const char *REDEF_FUNCTION;
extern const char *REDEF_EXTERN;
extern const char *CHANGE_RET_TYPE;
extern const char *ARG_TYPE_CHANGE;
extern const char *ARG_NUM_CHANGE;
extern const char *REDEF_WITH_ARGS;
extern const char *REDEF_WITHOUT_ARGS;
extern const char *REDEF_VAR_AS_FUNC;

extern void error(const char *format, ...);

#endif

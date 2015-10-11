#include <stdbool.h>

/* Symbols in the symbol table */
typedef enum {
    INT_TYPE,
    FLOAT_TYPE,
    BOOLEAN_TYPE,
    CHAR_TYPE,
    STRING_TYPE,
    VOID_TYPE
} Type;

typedef union {
    int iVal;
    float fVal;
    bool bVal;
    char cVal;
    char *sVal;
} Value;

typedef struct SymbolNode {
    char *idName;
    Type type;
    Value value;
} SymbolNode;

extern SymbolNode createSymbol(char *idName, Type type, Value value);
extern int compareSymbolNodes(SymbolNode *node1, SymbolNode *node2);
extern Value floatVal(float value);
extern Value boolVal(bool value);
extern Value charVal(char value);
extern Value stringVal(char *value);

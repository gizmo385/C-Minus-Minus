#include "symtab.h"
#include "list.h"
#include "utils.h"
#include <string.h>

SymbolNode createSymbol(char *idName, Type type, Value value) {
    SymbolNode node;
    node.idName = idName;
    node.type = type;
    node.value = value;

    return node;
}

int compareSymbolNodes(SymbolNode *node1, SymbolNode *node2) {
    return strcmp(node1->idName, node2->idName);
}

Value floatVal(float value) {
    Value val;
    val.fVal = value;
    return val;
}

Value boolVal(bool value) {
    Value val;
    val.bVal = value;
    return val;
}

Value charVal(char value) {
    Value val;
    val.cVal = value;
    return val;
}

Value stringVal(char *value) {
    Value val;
    val.sVal = value;
    return val;
}

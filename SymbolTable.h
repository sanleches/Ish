#ifndef SYMBOL_TABLE_H_
#define SYMBOL_TABLE_H_

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef SCANNER_H_
#include "Scanner.h"
#endif

#define SYMBOL_TABLE_MAX 256

typedef enum SymbolKind {
    SYMBOL_VARIABLE,
    SYMBOL_FUNCTION
} SymbolKind;

typedef enum SymbolType {
    SYMBOL_TYPE_UNKNOWN,
    SYMBOL_TYPE_INT,
    SYMBOL_TYPE_FLOAT,
    SYMBOL_TYPE_STRING,
    SYMBOL_TYPE_LONG,
    SYMBOL_TYPE_VOID
} SymbolType;

typedef struct Symbol {
    ish_cha name[VID_LEN + 1];
    SymbolKind kind;
    SymbolType type;
    ish_intg scopeDepth;
    ish_intg lineDeclared;
} Symbol;

typedef struct SymbolTable {
    Symbol symbols[SYMBOL_TABLE_MAX];
    ish_intg count;
    ish_intg scopeDepth;
} SymbolTable;

ish_void symbolTableInit(SymbolTable* table);
ish_bool symbolTableEnterScope(SymbolTable* table);
ish_bool symbolTableLeaveScope(SymbolTable* table);
ish_bool symbolTableDeclare(SymbolTable* table, ish_thread name, SymbolKind kind, SymbolType type, ish_intg lineDeclared);
Symbol* symbolTableLookup(SymbolTable* table, ish_thread name, SymbolKind kind);

#endif

#define _CRT_SECURE_NO_WARNINGS

#include <string.h>

#ifndef SYMBOL_TABLE_H_
#include "SymbolTable.h"
#endif

ish_void symbolTableInit(SymbolTable* table) {
    if (!table)
        return;
    table->count = 0;
    table->scopeDepth = 0;
}

ish_bool symbolTableEnterScope(SymbolTable* table) {
    if (!table)
        return ISH_FALSE;
    table->scopeDepth++;
    return ISH_TRUE;
}

ish_bool symbolTableLeaveScope(SymbolTable* table) {
    if (!table || table->scopeDepth <= 0)
        return ISH_FALSE;

    while (table->count > 0 && table->symbols[table->count - 1].scopeDepth == table->scopeDepth)
        table->count--;

    table->scopeDepth--;
    return ISH_TRUE;
}

ish_bool symbolTableDeclare(SymbolTable* table, ish_thread name, SymbolKind kind, SymbolType type, ish_intg lineDeclared) {
    if (!table || !name || name[0] == '\0' || table->count >= SYMBOL_TABLE_MAX)
        return ISH_FALSE;

    for (ish_intg i = table->count - 1; i >= 0; i--) {
        Symbol* symbol = &table->symbols[i];
        if (symbol->scopeDepth != table->scopeDepth)
            continue;
        if (symbol->kind == kind && strcmp(symbol->name, name) == 0)
            return ISH_FALSE;
    }

    Symbol* symbol = &table->symbols[table->count++];
    strncpy(symbol->name, name, VID_LEN);
    symbol->name[VID_LEN] = '\0';
    symbol->kind = kind;
    symbol->type = type;
    symbol->scopeDepth = table->scopeDepth;
    symbol->lineDeclared = lineDeclared;
    return ISH_TRUE;
}

Symbol* symbolTableLookup(SymbolTable* table, ish_thread name, SymbolKind kind) {
    if (!table || !name)
        return NULL;

    for (ish_intg i = table->count - 1; i >= 0; i--) {
        Symbol* symbol = &table->symbols[i];
        if (symbol->kind == kind && strcmp(symbol->name, name) == 0)
            return symbol;
    }

    return NULL;
}

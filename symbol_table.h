#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdbool.h>

// Max depth of nested scopes, adjust as needed
#define MAX_SCOPES 100

// Symbol struct (adjust fields as needed)
typedef struct Symbol {
    char* name;
    // add type, other info if needed
    struct Symbol* next;
} Symbol;

// Scope: linked list of symbols
typedef struct Scope {
    Symbol* symbols;
} Scope;

void push_scope();
void pop_scope();
bool insert_symbol(const char* name);
bool lookup_symbol(const char* name);
int get_symbol_count();

#endif


#include "symbol_table.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static Scope scopes[MAX_SCOPES];
static int current_scope = -1;

// Create new symbol node
static Symbol* create_symbol(const char* name) {
    Symbol* sym = malloc(sizeof(Symbol));
    sym->name = strdup(name);
    sym->next = NULL;
    return sym;
}

void push_scope() {
    if (current_scope + 1 >= MAX_SCOPES) {
        fprintf(stderr, "Error: scope stack overflow\n");
        exit(1);
    }
    current_scope++;
    scopes[current_scope].symbols = NULL;
    // printf("Push scope %d\n", current_scope);
}

void pop_scope() {
    if (current_scope < 0) {
        fprintf(stderr, "Error: scope stack underflow\n");
        exit(1);
    }
    // Free symbols in current scope
    Symbol* sym = scopes[current_scope].symbols;
    while (sym) {
        Symbol* next = sym->next;
        free(sym->name);
        free(sym);
        sym = next;
    }
    scopes[current_scope].symbols = NULL;
    // printf("Pop scope %d\n", current_scope);
    current_scope--;
}

bool insert_symbol(const char* name) {
    if (current_scope < 0) {
        fprintf(stderr, "Error: no active scope to insert symbol '%s'\n", name);
        return false;
    }
    // Check if symbol already exists in current scope
    Symbol* sym = scopes[current_scope].symbols;
    while (sym) {
        if (strcmp(sym->name, name) == 0) {
            fprintf(stderr, "Warning: symbol '%s' already declared in current scope\n", name);
            return false;
        }
        sym = sym->next;
    }
    // Insert at head
    Symbol* new_sym = create_symbol(name);
    new_sym->next = scopes[current_scope].symbols;
    scopes[current_scope].symbols = new_sym;
    return true;
}

bool lookup_symbol(const char* name) {
    // Search from innermost scope outward
    for (int i = current_scope; i >= 0; i--) {
        Symbol* sym = scopes[i].symbols;
        while (sym) {
            if (strcmp(sym->name, name) == 0) return true;
            sym = sym->next;
        }
    }
    return false;
}

int get_symbol_count() {
    int count = 0;
    for (int i = 0; i <= current_scope; i++) {
        Symbol* sym = scopes[i].symbols;
        while (sym) {
            count++;
            sym = sym->next;
        }
    }
    return count;
}



#ifndef CFG_H
#define CFG_H

#include "ir_generator.h"

typedef struct CFGNode {
    int id;
    IR* instruction;
    struct CFGNode* true_branch;
    struct CFGNode* false_branch;
    struct CFGNode* next;
} CFGNode;

CFGNode* build_linear_cfg(IR ir_list[], int count);
void export_cfg_dot(CFGNode* head, const char* filename);

#endif


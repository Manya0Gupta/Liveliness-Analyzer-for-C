#include <stdio.h>
#include <stdlib.h>
#include "cfg.h"

CFGNode* build_linear_cfg(IR ir_list[], int count) {
    CFGNode* head = NULL;
    CFGNode* prev = NULL;

    for (int i = 0; i < count; ++i) {
        CFGNode* node = malloc(sizeof(CFGNode));
        node->id = i;
        node->instruction = &ir_list[i];
        node->true_branch = NULL;
        node->false_branch = NULL;
        node->next = NULL;

        if (!head) head = node;
        if (prev) prev->next = node;
        prev = node;
    }

    // For now, true_branch and false_branch remain NULL
    // You can extend here to set them based on jump targets if you implement those

    return head;
}

void export_cfg_dot(CFGNode* head, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        fprintf(stderr, "Error opening file %s\n", filename);
        return;
    }

    fprintf(file, "digraph CFG {\n");

    CFGNode* current = head;
    while (current) {
        char label[256];
        IR* ir = current->instruction;
        switch (ir->op) {
            case IR_ASSIGN:
                snprintf(label, sizeof(label), "%s = %s", ir->lhs, ir->rhs1);
                break;
            case IR_RETURN:
                snprintf(label, sizeof(label), "RETURN %s", ir->rhs1);
                break;
            case IR_IF:
                snprintf(label, sizeof(label), "IF %s", ir->rhs1);
                break;
            case IR_IF_ELSE:
                snprintf(label, sizeof(label), "IF-ELSE %s", ir->rhs1);
                break;
            case IR_WHILE:
                snprintf(label, sizeof(label), "WHILE %s", ir->rhs1);
                break;
            default:
                snprintf(label, sizeof(label), "UNKNOWN");
                break;
        }

        fprintf(file, "Node%d [label=\"%s\"];\n", current->id, label);

        if (current->next)
            fprintf(file, "Node%d -> Node%d;\n", current->id, current->id + 1);

        current = current->next;
    }

    fprintf(file, "}\n");
    fclose(file);
}


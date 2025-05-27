#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "ir_generator.h"
#include "symbol_table.h"

extern int line_num;

void perform_live_variable_analysis() {
    bool live[MAX_IR] = {false};

    // Live set to track used variables
    char live_set[MAX_IR][MAX_NAME_LEN];
    int live_count = 0;

    // Backward pass
    for (int i = ir_count - 1; i >= 0; --i) {
        bool is_used_later = false;

        // Mark return operand as live
        if (ir_list[i].op == IR_RETURN) {
            const char* ret_var = ir_list[i].rhs1;
            bool already_live = false;

            for (int k = 0; k < live_count; ++k) {
                if (strcmp(live_set[k], ret_var) == 0) {
                    already_live = true;
                    break;
                }
            }

            if (!already_live) {
                strcpy(live_set[live_count++], ret_var);
            }
        }

        // Check if lhs is used in future rhs1
        for (int j = i + 1; j < ir_count; ++j) {
            if (strcmp(ir_list[i].lhs, ir_list[j].rhs1) == 0) {
                is_used_later = true;
                break;
            }
        }

        if (!is_used_later && ir_list[i].op == IR_ASSIGN) {
            printf("Redundant assignment at line %d: '%s = %s' is never used.\n",
                   ir_list[i].line, ir_list[i].lhs, ir_list[i].rhs1);
        }
    }

    FILE* report = fopen("report.txt", "w");
    if (!report) {
        printf("Could not open report.txt for writing.\n");
        return;
    }

    int redundant_count = 0;

    fprintf(report, "Redundant Assignments:\n");

    for (int i = ir_count - 1; i >= 0; --i) {
        bool is_used_later = false;

        // Handle return usage
        if (ir_list[i].op == IR_RETURN) continue;

        for (int j = i + 1; j < ir_count; ++j) {
            if (strcmp(ir_list[i].lhs, ir_list[j].rhs1) == 0 ||
                strcmp(ir_list[i].lhs, ir_list[j].rhs2) == 0) {
                is_used_later = true;
                break;
            }
        }

        if (!is_used_later && ir_list[i].op == IR_ASSIGN) {
            fprintf(report, "Line %d: '%s = %s' is never used.\n",
                    ir_list[i].line, ir_list[i].lhs, ir_list[i].rhs1);
            redundant_count++;
        }
    }

    fprintf(report, "\nSummary:\n");
    fprintf(report, "Total lines of code: %d\n", line_num);
    fprintf(report, "Total variables declared: %d\n", get_symbol_count());
    fprintf(report, "Total assignments: %d\n", assignment_count);
    fprintf(report, "Redundant assignments: %d\n", redundant_count);

    fclose(report);
    printf("Report written to report.txt\n");
}


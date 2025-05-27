#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ir_generator.h"

extern int line_num;

IR ir_list[MAX_IR];
int ir_count = 0;
int assignment_count = 0;

void generate_assignment(const char* var, const char* expr) {
    strncpy(ir_list[ir_count].lhs, var, MAX_NAME_LEN);
    strncpy(ir_list[ir_count].rhs1, expr, MAX_NAME_LEN);
    ir_list[ir_count].rhs2[0] = '\0';
    ir_list[ir_count].line = line_num;
    ir_list[ir_count].op = IR_ASSIGN;
    ir_count++;
    assignment_count++;
}

void generate_return(const char* var) {
    ir_list[ir_count].lhs[0] = '\0';
    strncpy(ir_list[ir_count].rhs1, var, MAX_NAME_LEN);
    ir_list[ir_count].rhs2[0] = '\0';
    ir_list[ir_count].line = line_num;
    ir_list[ir_count].op = IR_RETURN;
    ir_count++;
}

void generate_if(const char* cond, const char* then_stmt, const char* else_stmt) {
    ir_list[ir_count].lhs[0] = '\0';
    strncpy(ir_list[ir_count].rhs1, cond, MAX_NAME_LEN);
    if (else_stmt)
        strncpy(ir_list[ir_count].rhs2, "if-else", MAX_NAME_LEN);
    else
        strncpy(ir_list[ir_count].rhs2, "if", MAX_NAME_LEN);

    ir_list[ir_count].line = line_num;
    ir_list[ir_count].op = else_stmt ? IR_IF_ELSE : IR_IF;
    ir_count++;
}

void generate_while(const char* cond, const char* body) {
    ir_list[ir_count].lhs[0] = '\0';
    strncpy(ir_list[ir_count].rhs1, cond, MAX_NAME_LEN);
    strncpy(ir_list[ir_count].rhs2, "while", MAX_NAME_LEN);
    ir_list[ir_count].line = line_num;
    ir_list[ir_count].op = IR_WHILE;
    ir_count++;
}

void print_ir() {
    printf("\nIntermediate Representation:\n");
    for (int i = 0; i < ir_count; i++) {
        IR* ir = &ir_list[i];
        switch (ir->op) {
            case IR_ASSIGN:
                printf("Line %d: %s = %s\n", ir->line, ir->lhs, ir->rhs1);
                break;
            case IR_RETURN:
                printf("Line %d: RETURN %s\n", ir->line, ir->rhs1);
                break;
            case IR_IF:
                printf("Line %d: IF %s\n", ir->line, ir->rhs1);
                break;
            case IR_IF_ELSE:
                printf("Line %d: IF-ELSE %s\n", ir->line, ir->rhs1);
                break;
            case IR_WHILE:
                printf("Line %d: WHILE %s\n", ir->line, ir->rhs1);
                break;
            default:
                printf("Line %d: UNKNOWN IR op\n", ir->line);
        }
    }
}


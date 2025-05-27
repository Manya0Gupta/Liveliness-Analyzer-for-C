#ifndef IR_GENERATOR_H
#define IR_GENERATOR_H

#define MAX_IR 1000
#define MAX_NAME_LEN 64

typedef enum {
    IR_ASSIGN,
    IR_RETURN,
    IR_IF,       // if condition without else
    IR_IF_ELSE,  // if with else
    IR_WHILE,
    // Add more IR ops here if needed
} IROp;

typedef struct {
    IROp op;
    char lhs[MAX_NAME_LEN];   // left hand side variable (empty for some ops)
    char rhs1[MAX_NAME_LEN];  // first operand or condition
    char rhs2[MAX_NAME_LEN];  // second operand or info (optional)
    int line;                 // source code line number
} IR;

extern IR ir_list[MAX_IR];
extern int ir_count;
extern int assignment_count;

void generate_assignment(const char* var, const char* expr);
void generate_return(const char* var);
void generate_if(const char* cond, const char* then_stmt, const char* else_stmt);
void generate_while(const char* cond, const char* body);
void print_ir();

#endif


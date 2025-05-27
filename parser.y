%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"
#include "ir_generator.h"
#include "cfg.h"

char* create_expr_string(const char* lhs, const char* op, const char* rhs) {
    char* result = malloc(strlen(lhs) + strlen(op) + strlen(rhs) + 3);
    sprintf(result, "%s %s %s", lhs, op, rhs);
    return result;
}

extern int yylex();
extern int line_num;
extern void perform_live_variable_analysis();

extern void push_scope(void);
extern void pop_scope(void);

void yyerror(const char* s) {
    fprintf(stderr, "Error: %s at line %d\n", s, line_num);
}
%}

%union {
    char* str;
}

%token INT RETURN IF ELSE WHILE
%token <str> IDENTIFIER NUMBER
%token PLUS MINUS ASSIGN SEMICOLON COMMA LPAREN RPAREN LBRACE RBRACE

/* Declare relational operators tokens */
%token GE LE EQ NEQ GT LT

/* Precedence and associativity */
%nonassoc ELSE           /* Makes ELSE lowest precedence (helps dangling else) */
%nonassoc IFX            /* Special token for dangling else resolution */

%left PLUS MINUS
%right ASSIGN

%type <str> expr

%%

program:
    function_list
    ;

function_list:
    function_list function
  | function
    ;

function:
    INT IDENTIFIER LPAREN RPAREN compound_stmt
    ;

compound_stmt:
    LBRACE { push_scope(); } stmt_list RBRACE { pop_scope(); }
    ;

stmt_list:
    stmt_list stmt
  | /* empty */
    ;

stmt:
    declaration
  | assignment
  | RETURN expr SEMICOLON { generate_return($2); }
  | IF LPAREN expr RPAREN stmt %prec IFX
  | IF LPAREN expr RPAREN stmt ELSE stmt
  | WHILE LPAREN expr RPAREN stmt
  | compound_stmt
  | error SEMICOLON { yyerror("Invalid statement"); yyerrok; }
    ;

declaration:
    INT IDENTIFIER SEMICOLON {
        if (!insert_symbol($2)) {
            fprintf(stderr, "Error: redeclaration of variable %s\n", $2);
        }
        /* no semantic value */
    }
   | INT IDENTIFIER ASSIGN expr SEMICOLON{
	if (!insert_symbol($2)){
		fprintf(stderr,"Error: redeclaration of variable %s\n", $2);
        } else {
            generate_assignment($2, $4);
        }
    }
    ;

assignment:
    IDENTIFIER ASSIGN expr SEMICOLON { generate_assignment($1, $3); }
    ;

expr:
    expr GT expr   { $$ = create_expr_string($1, ">", $3); }
  | expr LT expr   { $$ = create_expr_string($1, "<", $3); }
  | expr GE expr   { $$ = create_expr_string($1, ">=", $3); }
  | expr LE expr   { $$ = create_expr_string($1, "<=", $3); }
  | expr EQ expr   { $$ = create_expr_string($1, "==", $3); }
  | expr NEQ expr  { $$ = create_expr_string($1, "!=", $3); }
  | expr PLUS expr { $$ = create_expr_string($1, "+", $3); }
  | expr MINUS expr{ $$ = create_expr_string($1, "-", $3); }
  | IDENTIFIER     { $$ = $1; }
  | NUMBER         { $$ = $1; }
  | LPAREN expr RPAREN { $$ = $2; }
  ;

%%

int main() {
    yyparse();
    print_ir();
    perform_live_variable_analysis();

    // Build and export the CFG
    CFGNode* cfg = build_linear_cfg(ir_list, ir_count);
    export_cfg_dot(cfg, "cfg.dot");
    printf("CFG written to cfg.dot\n");

    return 0;
}


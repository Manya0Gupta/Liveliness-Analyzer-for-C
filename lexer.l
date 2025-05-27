%{
#include "parser.tab.h"
#include <stdio.h>
#include <stdlib.h>

int line_num = 1;
%}

%x COMMENT

digit   [0-9]
letter  [a-zA-Z_]
id      {letter}({letter}|{digit})*
number  {digit}+

%%
">="   { return GE; }
"<="   { return LE; }
"=="   { return EQ; }
"!="   { return NEQ; }
">"    { return GT; }
"<"    { return LT; }

"//".*                                  { line_num++; }
"/*"                                     { BEGIN(COMMENT); }
<COMMENT>\n                              { line_num++; }
<COMMENT>[^*]*                           { }
<COMMENT>"*"+[^*/]*                      { }
<COMMENT>"*"+"/"                         { BEGIN(INITIAL); }

"int"                                    { return INT; }
"return"                                 { return RETURN; }
"if"                                     { return IF; }
"else"                                   { return ELSE; }
"while"                                  { return WHILE; }

{number}                                 { yylval.str = strdup(yytext); return NUMBER; }
{id}                                     { yylval.str = strdup(yytext); return IDENTIFIER; }

"+"                                      { return PLUS; }
"-"                                      { return MINUS; }
"="                                      { return ASSIGN; }
";"                                      { return SEMICOLON; }
","                                      { return COMMA; }
"("                                      { return LPAREN; }
")"                                      { return RPAREN; }
"{"                                      { return LBRACE; }
"}"                                      { return RBRACE; }

[\t ]+                                   { }
\n                                       { line_num++; }

.                                        { printf("Unknown character: %s\n", yytext); }

%%
int yywrap() { return 1; }

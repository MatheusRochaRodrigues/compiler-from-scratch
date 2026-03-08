/** 
 * @brief Modulo do analisado sintatico  
 */

#ifndef _SYNT_H_
#define _SYNT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "struct_compiler.h"
#include "lex.h"
#include "gen.h"
#include "symbols.h"

/*
 * Gramatica: 
 *      program -> declarations  statements
 *      declarations-> declaration declarations | [vazio]
 *      declaration -> int id;
 *      statements --> statement statements | [vazio]
 *      statement  --> read_stmt | write_stmt | if_stmt | if_else_stmt | while_stmt | assign_stmt
 *      read_stmt  --> read id;
 *      write_stmt --> write id;
 *      if_stmt    --> if '(' rel_expr ')' begin statements end [ else begin statements end  ]  (ideal, ainda por fazer)
 *      if_stmt    --> if '(' E ')' begin statements end [ else begin statements end ] (implementado)
 *      assign_stmt -> id '=' E;
 *      rel_expr   --> id rel_operator E
 *      rel_operator -->  '<' | '>' | '<=' | '>=' | '==' | '!='
 *      E          --> expressões com numeros inteiros, cujo resultado final fica na pilha.
 */


// Prototipos
int program (void);
int declarations(void);
int statements (type_symbol_table_entry* parametros, int quant);
int declaration(void);
int statement(type_symbol_table_variables*localVariables);

int B(void);
int boolOperator(int*);

int E();
int ER();
int T();
int TR();
int F();

int DeclaracaoV(char var_name[MAX_CHAR],  int var_type, type_symbol_table_variables*TableVariables);
int DeclaracaoF(char var_name[MAX_CHAR],  int var_type);
type_symbol_table_entry* DeclaracaoFunc(type_symbol_function_entry* func);

int assignment(char var_name[MAX_CHAR], type_symbol_table_variables* localVariables);

int fun_call_cmd(char var_name[MAX_CHAR], type_symbol_table_variables*TableVariables);

int func_main();
int Func_code();

int main();

#endif //_SYNT_H_
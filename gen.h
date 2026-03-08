/** 
 * @brief Modulo do gerador de codigo  
 */

#ifndef _GEN_H_
#define _GEN_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "struct_compiler.h"
#include "symbols.h"

// Prototipos
void genAdd();
void genSub();
void genMult();
void genDiv();
void genNum(char num_string[MAX_TOKEN]);
void gen_preambule(void);
void gen_data_section(void);
void gen_preambule_code(void);
void gen_epilog_code(void);
void gen_read(char *, int);
void gen_write(char *, int);
void gen_label_name(char *);
void gen_label(char *);
void gen_cond_jump(char *);
void gen_incond_jump(char *);
void gen_bool(int);
void gen_bool_label_name(char *);
void gen_id(char *);
void gen_assignment(char *, int);

void gen_label_FuncName( char *name );
void gen_label_JAL_Function(char *label);
void gen_label_JAL_Function_RA();

#endif //_GEN_H_
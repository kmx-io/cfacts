#ifndef BLOCK_H
#define BLOCK_H

#include <setjmp.h>
#include "form.h"

typedef struct block {
        s_symbol *name;
        u_form *return_value;
        jmp_buf buf;
        struct block *next;
} s_block;

typedef struct env s_env;

s_block *  push_block (s_symbol *name, s_env *env);
s_block ** find_block (s_symbol *name, s_env *env);
u_form *        block (s_symbol *name, s_env *env);
u_form *        block_pop (s_symbol *name, s_env *env);

void return_from (s_symbol *name, u_form *value, s_env *env);

#endif

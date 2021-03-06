#ifndef FRAME_H
#define FRAME_H

#include "typedefs.h"

struct frame
{
        unsigned long type;
        s_skiplist *variables;
        s_skiplist *functions;
        s_skiplist *macros;
        struct frame *parent;
};

int compare_frame_bindings (void *a, void *b);

s_frame * new_frame (s_frame *parent);
void          frame_new_variable (s_symbol *sym, u_form *value,
                                  s_frame *frame);
void          frame_new_function (s_symbol *sym, u_form *value,
                                  s_frame *frame);
void          frame_new_macro (s_symbol *sym, s_lambda *value,
                               s_frame *frame);
u_form **     frame_variable (s_symbol *sym, s_frame *frame);
u_form **     frame_function (s_symbol *sym, s_frame *frame);
u_form **     frame_macro (s_symbol *sym, s_frame *frame);

void          frame_destructuring_bind (u_form *lambda_list,
                                        u_form *args,
                                        s_frame *frame, s_env *env);

#endif

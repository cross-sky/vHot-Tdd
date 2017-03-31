#ifndef __STACK_A_H
#define __STACK_A_H
#include <stdint.h>

//#define T Stack_T
//typedef struct T *T;
typedef struct Stack_struct *Stack_T;
#define T Stack_T

T Stack_new(void);
int16_t Stack_empty(T stk);
void Stack_push(T stk, void *x);
void *Stack_pop(T stk);
void Stack_free(T *stk);

#undef T
#endif
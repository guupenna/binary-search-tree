#ifndef _STACK_H_
#define _STACK_H_

#include "vector.h"

typedef struct {
    Vector *v;
    int size;
} Stack;

Stack *stack_construct();

void stack_push(Stack *stack, data_type val);

data_type stack_pop(Stack *stack);

bool stack_empty(Stack *stack);

void stack_destroy(Stack *stack);

#endif
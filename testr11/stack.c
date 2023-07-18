#include <stdlib.h>

#include "stack.h"

Stack *stack_construct()
{
    Stack *stack = (Stack *)malloc(sizeof(Stack));

    stack->v = vector_construct();
    stack->size = 0;

    return stack;
}

void stack_push(Stack *stack, data_type val)
{
    vector_push_back(stack->v, val);

    stack->size++;
}

data_type stack_pop(Stack *stack)
{
    data_type val = vector_pop_back(stack->v);

    stack->size--;

    return val;
}

bool stack_empty(Stack *stack)
{
    if (stack->size == 0)
        return 1;

    return 0;
}

void stack_destroy(Stack *stack)
{
    vector_destroy(stack->v);
    free(stack);
}
#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "vector.h"

typedef struct {
    Vector *v;
    int size;
} Queue;

Queue *queue_construct();

void queue_push(Queue *queue, data_type val);

data_type queue_pop(Queue *queue);

bool queue_empty(Queue *queue);

void queue_destroy(Queue *queue);

#endif